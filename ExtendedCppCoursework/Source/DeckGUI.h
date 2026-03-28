#pragma once

#include <JuceHeader.h>

#include "DJAudioPlayer.h"
#include "ScratchEngine.h"
#include "WaveformDisplay.h"

class DeckGUI : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::FileDragAndDropTarget,
                public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer& player, juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& cache);
    ~DeckGUI() override;

    void paint(juce::Graphics& g) override;
    void paintOverChildren(juce::Graphics& g) override;
    void resized() override;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    void timerCallback() override;

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;

private:
    float normalisedXForEvent(const juce::MouseEvent& event) const;
    bool isWaveformEvent(const juce::MouseEvent& event) const;
    void applyLoopFromMouse(const juce::MouseEvent& event);
    void loadAudioFile(const juce::File& file);

    DJAudioPlayer& djAudioPlayer;
    WaveformDisplay waveformDisplay;
    ScratchEngine scratchEngine;

    juce::TextButton playButton { "PLAY" };
    juce::TextButton stopButton { "STOP" };
    juce::TextButton loadButton { "LOAD" };
    juce::TextButton scratchButton { "SCRATCH OFF" };

    juce::Slider gainSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;
    juce::Slider scratchSensitivitySlider;
    juce::Slider scratchInertiaSlider;
    juce::Label gainLabel { {}, "Gain" };
    juce::Label speedLabel { {}, "Speed" };
    juce::Label posLabel { {}, "Position" };
    juce::Label scratchSensitivityLabel { {}, "Scratch Sens" };
    juce::Label scratchInertiaLabel { {}, "Scratch Inertia" };
    juce::Label coreControlsHeader { {}, "Core Controls" };
    juce::Label scratchControlsHeader { {}, "Scratch Controls" };

    std::unique_ptr<juce::FileChooser> fileChooser;

    bool loopGestureActive = false;
    bool scratchGestureActive = false;
};
