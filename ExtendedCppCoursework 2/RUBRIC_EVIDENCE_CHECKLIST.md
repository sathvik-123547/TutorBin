# Rubric Evidence Checklist

This file maps rubric requirements to implementation locations.

## Architecture and OOP requirements

- `DeckGUI` inherits `Button::Listener` and `Slider::Listener`:
  - `Source/DeckGUI.h`
- `MainComponent` inherits `AudioAppComponent`:
  - `Source/MainComponent.h`
- `MainComponent` contains `DJAudioPlayer` and `DeckGUI` members:
  - `Source/MainComponent.h`
- `DJAudioPlayer::loadURL` uses unique pointers:
  - `Source/DJAudioPlayer.cpp`
- Crossfader implemented with lambda:
  - `Source/MainComponent.cpp`

## Looper requirements

- Template interval class:
  - `Source/Interval.h`
- Looping logic in player:
  - `Source/DJAudioPlayer.cpp`
- Interactive loop drawing over waveform:
  - `Source/DeckGUI.cpp` in `paintOverChildren`
- Mouse loop control:
  - `Source/DeckGUI.cpp` in `mouseDown` and `mouseDrag`
- Atomic loop region with `load/store`:
  - `Source/DJAudioPlayer.h` and `Source/DJAudioPlayer.cpp`
  - `Source/DeckGUI.cpp`

## GUI/audio functionality for screencast

- File load and drag-drop:
  - `Source/DeckGUI.cpp` (`load` button, `isInterestedInFileDrag`, `filesDropped`)
- Waveform + moving playhead + timecode:
  - `Source/WaveformDisplay.cpp`
- Two decks mixed with crossfader:
  - `Source/MainComponent.cpp`

## Custom feature

- Scratch/Vinyl mode custom class:
  - `Source/ScratchEngine.h`
  - `Source/ScratchEngine.cpp`
- Integration into deck controls and mouse gestures:
  - `Source/DeckGUI.cpp`
- Required plain text explanation:
  - `CUSTOM_FEATURE_EXPLANATION.txt`
