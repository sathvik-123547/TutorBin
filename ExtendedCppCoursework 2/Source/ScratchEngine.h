#pragma once

class DJAudioPlayer;

class ScratchEngine
{
public:
    void setEnabled(bool enabled);
    bool isEnabled() const;

    void setSensitivity(float sensitivity);
    void setInertia(float inertia);

    void beginGesture(float normalizedX);
    void updateGesture(float normalizedX, float deltaTimeSeconds);
    void endGesture();

    void process(DJAudioPlayer& player, float deltaTimeSeconds);

private:
    float clamp01(float v) const;
    float clamp(float v, float lo, float hi) const;

    bool mEnabled = false;
    bool mGestureActive = false;

    float mSensitivity = 1.0f;
    float mInertia = 0.92f;

    float mLastNormalizedX = 0.0f;
    float mTurntableVelocity = 0.0f;
    float mTargetSpeedRatio = 1.0f;

    float mPendingPositionNudge = 0.0f;
};
