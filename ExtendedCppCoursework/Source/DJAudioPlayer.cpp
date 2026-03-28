#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& formatManager)
    : loopRegion(LoopRegion {}),
      mFormatManager(formatManager),
      mResampleSource(&mTransportSource, false, 2)
{
    DBG("loopRegion is lock-free: " << static_cast<int>(loopRegion.is_lock_free()));
}

DJAudioPlayer::~DJAudioPlayer()
{
    mTransportSource.setSource(nullptr);
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    mTransportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mResampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto region = loopRegion.load();
    if (region.proper() && isLoaded())
    {
        const auto pos = getPositionRelative();
        if (pos < region.start())
        {
            setPositionRelative(region.start());
        }
        else if (pos >= region.end())
        {
            setPositionRelative(region.start());
        }
    }

    if (! isLoaded())
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    mResampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    mResampleSource.releaseResources();
    mTransportSource.releaseResources();
}

void DJAudioPlayer::loadURL(const juce::URL& audioURL)
{
    auto inputStream = audioURL.createInputStream(juce::URL::InputStreamOptions(juce::URL::ParameterHandling::inAddress));
    if (inputStream == nullptr)
    {
        mFileLoaded.store(false);
        return;
    }

    std::unique_ptr<juce::AudioFormatReader> reader(mFormatManager.createReaderFor(std::move(inputStream)));
    if (reader == nullptr)
    {
        mFileLoaded.store(false);
        return;
    }

    const auto sourceRate = reader->sampleRate;
    auto newReaderSource = std::make_unique<juce::AudioFormatReaderSource>(reader.release(), true);

    mTransportSource.stop();
    mTransportSource.setSource(nullptr);
    mReaderSource = std::move(newReaderSource);
    mTransportSource.setSource(mReaderSource.get(), 0, nullptr, sourceRate);

    mFileLoaded.store(true);
    setPosition(0.0);
}

void DJAudioPlayer::setUserGain(double gain)
{
    mUserGain = juce::jlimit(0.0f, 1.0f, static_cast<float>(gain));
    updateOutputGain();
}

void DJAudioPlayer::setCrossfadeGain(double gain)
{
    mCrossfadeGain = juce::jlimit(0.0f, 1.0f, static_cast<float>(gain));
    updateOutputGain();
}

void DJAudioPlayer::setSpeed(double ratio)
{
    mCurrentSpeedRatio = juce::jlimit(0.05f, 4.0f, static_cast<float>(ratio));
    mResampleSource.setResamplingRatio(1.0 / static_cast<double>(mCurrentSpeedRatio));
}

void DJAudioPlayer::setPosition(double posInSeconds)
{
    mTransportSource.setPosition(juce::jmax(0.0, posInSeconds));
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    const auto safePos = juce::jlimit(0.0, 1.0, pos);
    const auto length = getLengthInSeconds();

    if (length > 0.0)
    {
        mTransportSource.setPosition(safePos * length);
    }
}

void DJAudioPlayer::nudgePositionRelative(double deltaNormalized)
{
    if (! isLoaded())
    {
        return;
    }

    const auto current = getPositionRelative();
    setPositionRelative(current + deltaNormalized);
}

void DJAudioPlayer::start()
{
    if (isLoaded())
    {
        mTransportSource.start();
    }
}

void DJAudioPlayer::stop()
{
    mTransportSource.stop();
}

double DJAudioPlayer::getPositionRelative() const
{
    const auto length = getLengthInSeconds();
    if (length <= 0.0)
    {
        return 0.0;
    }

    return juce::jlimit(0.0, 1.0, mTransportSource.getCurrentPosition() / length);
}

double DJAudioPlayer::getLengthInSeconds() const
{
    return mTransportSource.getLengthInSeconds();
}

bool DJAudioPlayer::isLoaded() const
{
    return mFileLoaded.load();
}

bool DJAudioPlayer::isPlaying() const
{
    return mTransportSource.isPlaying();
}

void DJAudioPlayer::updateOutputGain()
{
    mTransportSource.setGain(mUserGain * mCrossfadeGain);
}
