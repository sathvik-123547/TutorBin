#pragma once

#include <JuceHeader.h>
#include <atomic>

#include "Interval.h"

class DJAudioPlayer : public juce::AudioSource
{
public:
    using LoopRegion = Interval<float>;

    explicit DJAudioPlayer(juce::AudioFormatManager& formatManager);
    ~DJAudioPlayer() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(const juce::URL& audioURL);

    void setUserGain(double gain);
    void setCrossfadeGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSeconds);
    void setPositionRelative(double pos);
    void nudgePositionRelative(double deltaNormalized);

    void start();
    void stop();

    double getPositionRelative() const;
    double getLengthInSeconds() const;
    bool isLoaded() const;
    bool isPlaying() const;

    std::atomic<LoopRegion> loopRegion;

private:
    void updateOutputGain();

    juce::AudioFormatManager& mFormatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> mReaderSource;
    juce::AudioTransportSource mTransportSource;
    juce::ResamplingAudioSource mResampleSource;

    std::atomic<bool> mFileLoaded { false };

    float mUserGain = 1.0f;
    float mCrossfadeGain = 1.0f;
    float mCurrentSpeedRatio = 1.0f;
};
