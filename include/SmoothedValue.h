#pragma once

// A simple sample-stepped linear parameter ramp, useful for avoiding
// instantaneous level changes that can create audible discontinuities.
class SmoothedValue {
public:
    void reset(float current, int rampLengthSamples) noexcept {
        current_ = target_ = current;
        ramp_ = rampLengthSamples > 0 ? rampLengthSamples : 1;
        remaining_ = 0;
        step_ = 0.0f;
    }
    void setTarget(float newTarget) noexcept {
        if (newTarget == target_) return;
        target_ = newTarget;
        remaining_ = ramp_;
        step_ = (target_ - current_) / static_cast<float>(ramp_);
    }
    float next() noexcept {
        if (remaining_ > 0)
            current_ = (--remaining_ == 0) ? target_ : current_ + step_;
        return current_;
    }
    float current() const noexcept { return current_; }
    bool isRamping() const noexcept { return remaining_ > 0; }
private:
    float current_ = 0.0f;
    float target_ = 0.0f;
    float step_ = 0.0f;
    int ramp_ = 1;
    int remaining_ = 0;
};
