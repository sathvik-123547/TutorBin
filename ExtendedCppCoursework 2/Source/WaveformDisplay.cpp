#include "WaveformDisplay.h"

#include <cmath>

WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& cacheToUse)
    : mThumbnail(1024, formatManager, cacheToUse)
{
    mThumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
    mThumbnail.removeChangeListener(this);
}

void WaveformDisplay::loadURL(const juce::URL& audioURL)
{
    mFileLoaded = false;
    mThumbnail.clear();

    if (audioURL.isLocalFile())
    {
        mThumbnail.setSource(new juce::FileInputSource(audioURL.getLocalFile()));
    }
    else
    {
        mThumbnail.setSource(new juce::URLInputSource(audioURL));
    }
}

void WaveformDisplay::setPositionRelative(double pos)
{
    mPositionRelative = juce::jlimit(0.0, 1.0, pos);
    repaint();
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    auto area = getLocalBounds();
    g.setColour(juce::Colours::grey);
    g.drawRect(area, 1);

    if (mFileLoaded)
    {
        g.setColour(juce::Colours::lightblue);
        mThumbnail.drawChannel(g, area, 0.0, mThumbnail.getTotalLength(), 0, 1.0f);

        g.setColour(juce::Colours::lime);
        const auto areaF = area.toFloat();
        const auto playheadX = static_cast<float>(mPositionRelative) * areaF.getWidth();
        g.drawLine(playheadX, areaF.getY(), playheadX, areaF.getBottom(), 2.0f);

        const auto totalLength = mThumbnail.getTotalLength();
        const auto currentPos = mPositionRelative * totalLength;
        const auto label = formatTimecode(currentPos) + " / " + formatTimecode(totalLength);

        g.setColour(juce::Colours::white);
        g.drawFittedText(label, getLocalBounds().reduced(6), juce::Justification::topRight, 1);
    }
    else
    {
        g.setColour(juce::Colours::white);
        g.drawFittedText("Drop audio file here", getLocalBounds(), juce::Justification::centred, 1);
    }
}

void WaveformDisplay::resized()
{
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &mThumbnail)
    {
        mFileLoaded = (mThumbnail.getTotalLength() > 0.0);
        repaint();
    }
}

juce::String WaveformDisplay::formatTimecode(double seconds)
{
    const auto totalSeconds = juce::jmax(0, static_cast<int>(std::round(seconds)));
    const auto mins = totalSeconds / 60;
    const auto secs = totalSeconds % 60;

    return juce::String(mins).paddedLeft('0', 2) + ":" + juce::String(secs).paddedLeft('0', 2);
}
