#pragma once

#include <JuceHeader.h>

class WaveformDisplay : public juce::Component, public juce::ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    void loadURL(const juce::URL& audioURL);
    void setPositionRelative(double pos);

    void paint(juce::Graphics& g) override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:
    static juce::String formatTimecode(double seconds);

    juce::AudioThumbnail mThumbnail;
    bool mFileLoaded = false;
    double mPositionRelative = 0.0;
};
