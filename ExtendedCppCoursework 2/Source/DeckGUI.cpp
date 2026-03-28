#include "DeckGUI.h"

#include <cmath>

DeckGUI::DeckGUI(DJAudioPlayer& player, juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& cache)
    : djAudioPlayer(player),
      waveformDisplay(formatManager, cache)
{
    addAndMakeVisible(waveformDisplay);
    waveformDisplay.addMouseListener(this, false);

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(scratchButton);

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    scratchButton.addListener(this);

    auto initSlider = [this](juce::Slider& slider, double min, double max, double value)
    {
        slider.setRange(min, max);
        slider.setValue(value);
        slider.setSliderStyle(juce::Slider::LinearHorizontal);
        slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 64, 20);
        slider.addListener(this);
        addAndMakeVisible(slider);
    };

    initSlider(gainSlider, 0.0, 1.0, 0.8);
    initSlider(speedSlider, 0.05, 4.0, 1.0);
    initSlider(posSlider, 0.0, 1.0, 0.0);
    initSlider(scratchSensitivitySlider, 0.2, 3.0, 1.2);
    initSlider(scratchInertiaSlider, 0.75, 0.99, 0.92);

    auto initLabel = [this](juce::Label& label)
    {
        label.setJustificationType(juce::Justification::centredLeft);
        label.setColour(juce::Label::textColourId, juce::Colours::white.withAlpha(0.9f));
        addAndMakeVisible(label);
    };

    initLabel(gainLabel);
    initLabel(speedLabel);
    initLabel(posLabel);
    initLabel(scratchSensitivityLabel);
    initLabel(scratchInertiaLabel);

    auto initHeaderLabel = [this](juce::Label& label)
    {
        label.setJustificationType(juce::Justification::centredLeft);
        label.setColour(juce::Label::textColourId, juce::Colours::white.withAlpha(0.95f));
        label.setFont(juce::Font(juce::FontOptions(13.5f, juce::Font::bold)));
        addAndMakeVisible(label);
    };

    initHeaderLabel(coreControlsHeader);
    initHeaderLabel(scratchControlsHeader);

    scratchEngine.setSensitivity(static_cast<float>(scratchSensitivitySlider.getValue()));
    scratchEngine.setInertia(static_cast<float>(scratchInertiaSlider.getValue()));

    djAudioPlayer.setUserGain(gainSlider.getValue());
    djAudioPlayer.setSpeed(speedSlider.getValue());

    startTimerHz(60);
}

DeckGUI::~DeckGUI()
{
    stopTimer();

    playButton.removeListener(this);
    stopButton.removeListener(this);
    loadButton.removeListener(this);
    scratchButton.removeListener(this);

    gainSlider.removeListener(this);
    speedSlider.removeListener(this);
    posSlider.removeListener(this);
    scratchSensitivitySlider.removeListener(this);
    scratchInertiaSlider.removeListener(this);

    waveformDisplay.removeMouseListener(this);
}

void DeckGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkslategrey.darker(0.5f));
    g.setColour(juce::Colours::white.withAlpha(0.8f));
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(1.0f), 8.0f, 1.5f);
}

void DeckGUI::paintOverChildren(juce::Graphics& g)
{
    const auto region = djAudioPlayer.loopRegion.load();
    if (! (region.proper() && djAudioPlayer.isLoaded()))
    {
        return;
    }

    g.setColour(juce::Colours::red);
    g.setOpacity(0.25f);

    auto b = waveformDisplay.getBoundsInParent();
    const auto startPx = b.getX() + static_cast<int>(std::round(region.start() * static_cast<float>(b.getWidth())));
    const auto widthPx = static_cast<int>(std::round(region.length() * static_cast<float>(b.getWidth())));
    const auto clampedWidth = juce::jlimit(1, b.getWidth(), widthPx);
    const auto maxWidthFromStart = juce::jmax(1, b.getRight() - startPx);

    juce::Rectangle<int> loopRect(startPx, b.getY(), juce::jmin(clampedWidth, maxWidthFromStart), b.getHeight());
    g.fillRect(loopRect);
}

void DeckGUI::resized()
{
    auto area = getLocalBounds().reduced(8);
    auto waveformArea = area.removeFromTop(static_cast<int>(area.getHeight() * 0.46f));
    waveformDisplay.setBounds(waveformArea);
    area.removeFromTop(8);

    auto buttonRow = area.removeFromTop(30);
    const auto buttonW = buttonRow.getWidth() / 4;
    playButton.setBounds(buttonRow.removeFromLeft(buttonW).reduced(2));
    stopButton.setBounds(buttonRow.removeFromLeft(buttonW).reduced(2));
    loadButton.setBounds(buttonRow.removeFromLeft(buttonW).reduced(2));
    scratchButton.setBounds(buttonRow.reduced(2));

    area.removeFromTop(6);

    auto layoutLabeledSlider = [&area](juce::Label& label, juce::Slider& slider)
    {
        auto row = area.removeFromTop(28);
        const auto labelWidth = juce::jmax(96, static_cast<int>(row.getWidth() * 0.25f));
        label.setBounds(row.removeFromLeft(labelWidth));
        slider.setBounds(row);
        area.removeFromTop(4);
    };

    coreControlsHeader.setBounds(area.removeFromTop(18));
    area.removeFromTop(2);
    layoutLabeledSlider(gainLabel, gainSlider);
    layoutLabeledSlider(speedLabel, speedSlider);
    layoutLabeledSlider(posLabel, posSlider);
    area.removeFromTop(2);
    scratchControlsHeader.setBounds(area.removeFromTop(18));
    area.removeFromTop(2);
    layoutLabeledSlider(scratchSensitivityLabel, scratchSensitivitySlider);
    layoutLabeledSlider(scratchInertiaLabel, scratchInertiaSlider);
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        djAudioPlayer.start();
    }
    else if (button == &stopButton)
    {
        djAudioPlayer.stop();
    }
    else if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>("Select an audio file...");
        const auto flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        fileChooser->launchAsync(flags, [this](const juce::FileChooser& chooser)
        {
            const auto selected = chooser.getResult();
            if (selected.existsAsFile())
            {
                loadAudioFile(selected);
            }
        });
    }
    else if (button == &scratchButton)
    {
        const auto enableScratch = ! scratchEngine.isEnabled();
        scratchEngine.setEnabled(enableScratch);
        scratchButton.setButtonText(enableScratch ? "SCRATCH ON" : "SCRATCH OFF");
    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &gainSlider)
    {
        djAudioPlayer.setUserGain(gainSlider.getValue());
    }
    else if (slider == &speedSlider)
    {
        djAudioPlayer.setSpeed(speedSlider.getValue());
    }
    else if (slider == &posSlider)
    {
        djAudioPlayer.setPositionRelative(posSlider.getValue());
    }
    else if (slider == &scratchSensitivitySlider)
    {
        scratchEngine.setSensitivity(static_cast<float>(scratchSensitivitySlider.getValue()));
    }
    else if (slider == &scratchInertiaSlider)
    {
        scratchEngine.setInertia(static_cast<float>(scratchInertiaSlider.getValue()));
    }
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (const auto& path : files)
    {
        const auto ext = juce::File(path).getFileExtension().toLowerCase();
        if (ext == ".wav" || ext == ".mp3" || ext == ".aiff" || ext == ".flac")
        {
            return true;
        }
    }
    return false;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int, int)
{
    if (files.isEmpty())
    {
        return;
    }

    loadAudioFile(juce::File(files[0]));
}

void DeckGUI::timerCallback()
{
    const auto pos = djAudioPlayer.getPositionRelative();
    waveformDisplay.setPositionRelative(pos);

    if (! posSlider.isMouseButtonDown())
    {
        posSlider.setValue(pos, juce::dontSendNotification);
    }

    scratchEngine.process(djAudioPlayer, 1.0f / 60.0f);
}

void DeckGUI::mouseDown(const juce::MouseEvent& event)
{
    if (! isWaveformEvent(event))
    {
        return;
    }

    if (scratchEngine.isEnabled() && event.mods.isRightButtonDown())
    {
        scratchGestureActive = true;
        loopGestureActive = false;
        scratchEngine.beginGesture(normalisedXForEvent(event));
        return;
    }

    loopGestureActive = true;
    scratchGestureActive = false;
    mouseDrag(event);
}

void DeckGUI::mouseDrag(const juce::MouseEvent& event)
{
    if (! isWaveformEvent(event))
    {
        return;
    }

    if (scratchGestureActive)
    {
        scratchEngine.updateGesture(normalisedXForEvent(event), 1.0f / 60.0f);
        return;
    }

    if (loopGestureActive)
    {
        applyLoopFromMouse(event);
    }
}

void DeckGUI::mouseUp(const juce::MouseEvent&)
{
    if (scratchGestureActive)
    {
        scratchEngine.endGesture();
    }

    loopGestureActive = false;
    scratchGestureActive = false;
}

float DeckGUI::normalisedXForEvent(const juce::MouseEvent& event) const
{
    const auto width = juce::jmax(1, waveformDisplay.getWidth());
    return juce::jlimit(0.0f, 1.0f, static_cast<float>(event.getPosition().getX()) / static_cast<float>(width));
}

bool DeckGUI::isWaveformEvent(const juce::MouseEvent& event) const
{
    return (&waveformDisplay == event.eventComponent);
}

void DeckGUI::applyLoopFromMouse(const juce::MouseEvent& event)
{
    const auto width = juce::jmax(1, waveformDisplay.getWidth());
    const auto invW = 1.0f / static_cast<float>(width);

    const auto start = juce::jlimit(0.0f, 1.0f, static_cast<float>(event.getMouseDownX()) * invW);
    const auto end = juce::jlimit(0.0f, 1.0f, static_cast<float>(event.getPosition().getX()) * invW);

    auto region = djAudioPlayer.loopRegion.load();
    region.start(start).end(end);
    djAudioPlayer.loopRegion.store(region);

    repaint();
    waveformDisplay.repaint();
}

void DeckGUI::loadAudioFile(const juce::File& file)
{
    if (! file.existsAsFile())
    {
        return;
    }

    const juce::URL fileURL(file);
    djAudioPlayer.loadURL(fileURL);
    waveformDisplay.loadURL(fileURL);

    DJAudioPlayer::LoopRegion emptyRegion;
    djAudioPlayer.loopRegion.store(emptyRegion);
}
