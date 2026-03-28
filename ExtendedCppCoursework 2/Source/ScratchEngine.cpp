#include "ScratchEngine.h"

#include <cmath>

#include "DJAudioPlayer.h"

void ScratchEngine::setEnabled(bool enabled)
{
    mEnabled = enabled;

    if (! mEnabled)
    {
        mGestureActive = false;
        mTurntableVelocity = 0.0f;
        mTargetSpeedRatio = 1.0f;
        mPendingPositionNudge = 0.0f;
    }
}

bool ScratchEngine::isEnabled() const
{
    return mEnabled;
}

void ScratchEngine::setSensitivity(float sensitivity)
{
    mSensitivity = clamp(sensitivity, 0.2f, 3.0f);
}

void ScratchEngine::setInertia(float inertia)
{
    mInertia = clamp(inertia, 0.75f, 0.99f);
}

void ScratchEngine::beginGesture(float normalizedX)
{
    if (! mEnabled)
    {
        return;
    }

    mGestureActive = true;
    mLastNormalizedX = clamp01(normalizedX);
    mTurntableVelocity = 0.0f;
    mTargetSpeedRatio = 1.0f;
    mPendingPositionNudge = 0.0f;
}

void ScratchEngine::updateGesture(float normalizedX, float deltaTimeSeconds)
{
    if (! (mEnabled && mGestureActive))
    {
        return;
    }

    const auto clampedX = clamp01(normalizedX);
    const auto dt = clamp(deltaTimeSeconds, 0.001f, 0.2f);
    const auto deltaX = clampedX - mLastNormalizedX;
    mLastNormalizedX = clampedX;

    // Convert horizontal drag into "record angular velocity", then smooth it.
    const auto instantaneousVelocity = (deltaX / dt) * mSensitivity;
    mTurntableVelocity = 0.82f * mTurntableVelocity + 0.18f * instantaneousVelocity;

    // We bias around 1.0 to retain continuity with regular playback speed.
    mTargetSpeedRatio = clamp(1.0f + (mTurntableVelocity * 0.35f), 0.05f, 4.0f);

    // A direct position nudge gives a more tactile vinyl feel than speed-only modulation.
    mPendingPositionNudge += deltaX * 0.75f * mSensitivity;
}

void ScratchEngine::endGesture()
{
    mGestureActive = false;
}

void ScratchEngine::process(DJAudioPlayer& player, float deltaTimeSeconds)
{
    if (! mEnabled)
    {
        return;
    }

    const auto dt = clamp(deltaTimeSeconds, 0.001f, 0.2f);

    if (mGestureActive)
    {
        player.setSpeed(mTargetSpeedRatio);
    }
    else
    {
        // Exponential decay gives inertia after releasing the virtual platter.
        const auto frames = dt * 60.0f;
        const auto decay = std::pow(mInertia, frames);
        mTurntableVelocity *= decay;
        mTargetSpeedRatio = clamp(1.0f + (mTurntableVelocity * 0.35f), 0.25f, 2.5f);
        player.setSpeed(mTargetSpeedRatio);

        // Snap to normal speed when close enough to avoid endless drift.
        if (std::abs(mTurntableVelocity) < 0.0005f)
        {
            mTurntableVelocity = 0.0f;
            mTargetSpeedRatio = 1.0f;
            player.setSpeed(1.0);
        }
    }

    if (std::abs(mPendingPositionNudge) > 0.00001f)
    {
        player.nudgePositionRelative(mPendingPositionNudge);
        mPendingPositionNudge = 0.0f;
    }
}

float ScratchEngine::clamp01(float v) const
{
    return clamp(v, 0.0f, 1.0f);
}

float ScratchEngine::clamp(float v, float lo, float hi) const
{
    if (v < lo)
    {
        return lo;
    }
    if (v > hi)
    {
        return hi;
    }
    return v;
}
