#pragma once
#include <array>
#include <atomic>
#include <cstddef>
#include <type_traits>

// Isolated example for one producer and one consumer only.
// A full queue drops/rejects a push; callers must explicitly handle false.
template <typename T, std::size_t Capacity>
class SpscQueue {
    static_assert(std::atomic<std::size_t>::is_always_lock_free,
                  "Target must support lock-free size_t atomics");
    static_assert(Capacity >= 2 && (Capacity & (Capacity - 1)) == 0,
                  "Capacity must be a power of two");
    static_assert(std::is_trivially_copyable<T>::value,
                  "Messages must be trivially copyable");
public:
    bool push(const T& value) noexcept {
        const auto head = head_.load(std::memory_order_relaxed);
        const auto tail = tail_.load(std::memory_order_acquire);
        if (head - tail == Capacity) return false;
        items_[head & (Capacity - 1)] = value;
        head_.store(head + 1, std::memory_order_release);
        return true;
    }
    bool pop(T& output) noexcept {
        const auto tail = tail_.load(std::memory_order_relaxed);
        const auto head = head_.load(std::memory_order_acquire);
        if (tail == head) return false;
        output = items_[tail & (Capacity - 1)];
        tail_.store(tail + 1, std::memory_order_release);
        return true;
    }
private:
    std::array<T, Capacity> items_{};
    std::atomic<std::size_t> head_{0};
    std::atomic<std::size_t> tail_{0};
};
