#pragma once

#include <JuceHeader.h>

#include "DJAudioPlayer.h"
#include "DeckGUI.h"

class MainComponent : public juce::AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache waveformCache { 1024 };

    DJAudioPlayer player1;
    DJAudioPlayer player2;

    DeckGUI deckGUI1;
    DeckGUI deckGUI2;

    juce::MixerAudioSource mixerSource;

    juce::Slider crossfader;
    juce::Label crossfaderLabel;
};
