#include "SpscQueue.h"
#include "SmoothedValue.h"
#include "Debouncer.h"
#include <atomic>
#include <cstdlib>
#define CHECK(condition) do { if (!(condition)) { std::cerr << "Check failed: " << #condition << " (line " << __LINE__ << ")\n"; return 1; } } while (false)
#include <cmath>
#include <cstdint>
#include <iostream>
#include <thread>

struct Message { std::uint32_t index; std::uint32_t check; };

int main() {
    {
        SpscQueue<int, 4> queue;
        int out = -7;
        CHECK(!queue.pop(out) && out == -7);
        for (int i = 0; i < 4; ++i) CHECK(queue.push(i));
        CHECK(!queue.push(123));
        for (int i = 0; i < 4; ++i) { CHECK(queue.pop(out)); CHECK(out == i); }
        CHECK(!queue.pop(out));
        for (int i = 0; i < 10000; ++i) {
            CHECK(queue.push(i));
            CHECK(queue.pop(out));
            CHECK(out == i);
        }
    }
    {
        SpscQueue<Message, 64> queue;
        constexpr std::uint32_t count = 100000;
        std::atomic<bool> good{true};
        std::thread producer([&] {
            for (std::uint32_t i = 0; i < count; ++i) {
                Message m{i, i ^ 0xA7B71C93U};
                while (!queue.push(m)) std::this_thread::yield();
            }
        });
        for (std::uint32_t i = 0; i < count; ++i) {
            Message m{};
            while (!queue.pop(m)) std::this_thread::yield();
            if (m.index != i || m.check != (i ^ 0xA7B71C93U)) good = false;
        }
        producer.join();
        CHECK(good.load());
    }
    {
        SmoothedValue value;
        value.reset(0.0f, 4);
        value.setTarget(1.0f);
        CHECK(value.isRamping());
        for (int i = 0; i < 3; ++i) CHECK(value.next() > 0.0f);
        CHECK(std::fabs(value.next() - 1.0f) < 1e-6f);
        CHECK(!value.isRamping());
        CHECK(std::fabs(value.next() - 1.0f) < 1e-6f);
    }
    {
        SmoothedValue v;
        v.reset(1.0f, 4); v.setTarget(0.0f);
        CHECK(v.next() == 0.75f); CHECK(v.next() == 0.5f);
        v.setTarget(1.0f); CHECK(v.next() == 0.625f);
        v.setTarget(1.0f); // repeated target must not restart the ramp
        CHECK(v.next() == 0.75f); CHECK(v.next() == 0.875f);
        CHECK(v.next() == 1.0f && !v.isRamping());
        v.reset(0.0f, 0); v.setTarget(-1.0f); CHECK(v.next() == -1.0f);
        v.reset(0.0f, -8); v.setTarget(1.0f); CHECK(v.next() == 1.0f);
    }
    {
        Debouncer d(10);
        CHECK(!d.update(true, 100)); CHECK(!d.update(false, 102));
        CHECK(!d.update(true, 104)); CHECK(!d.update(true, 113));
        CHECK(d.update(true, 114) && d.pressed());
        CHECK(!d.update(true, 200)); CHECK(!d.update(false, 201));
        CHECK(d.update(false, 211) && !d.pressed());
        Debouncer wrap(10);
        CHECK(!wrap.update(true, UINT32_MAX - 5));
        CHECK(!wrap.update(true, 2)); CHECK(wrap.update(true, 4));
        CHECK(wrap.pressed());
    }
    std::cout << "All standalone example checks passed.\n";
}
