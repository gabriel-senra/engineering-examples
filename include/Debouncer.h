#pragma once
#include <cstdint>

// Call from one control task, with a monotonic uint32 millisecond clock.
// Poll at intervals below 2^31 ms. No GPIO, synthesis, or product logic.
class Debouncer {
public:
    explicit Debouncer(std::uint32_t intervalMs) : interval_(intervalMs) {}
    bool update(bool raw, std::uint32_t now) noexcept {
        if (raw != candidate_) { candidate_ = raw; since_ = now; }
        if (candidate_ != stable_ && std::uint32_t(now - since_) >= interval_) {
            stable_ = candidate_;
            return true;
        }
        return false;
    }
    bool pressed() const noexcept { return stable_; }
private:
    std::uint32_t interval_, since_ = 0;
    bool candidate_ = false, stable_ = false;
};
