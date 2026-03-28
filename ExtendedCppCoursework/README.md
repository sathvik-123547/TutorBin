# Extended C++ Coursework Project

This project is a complete two-deck JUCE DJ application created from the coursework PDF requirements.

## Implemented core requirements

- Two repeated decks (`DeckGUI`) controlled by `MainComponent`.
- Audio engine (`DJAudioPlayer`) with load/play/stop/gain/speed/position support.
- Drag-and-drop file loading.
- Waveform display with moving playhead and visible timecode.
- Crossfader implemented using a lambda in `MainComponent.cpp`.
- Interactive looper implemented across:
  - `Source/Interval.h` (template interval class)
  - `Source/DJAudioPlayer.*` (audio looping logic)
  - `Source/DeckGUI.*` (loop visualization + mouse interaction)
- Loop state uses `std::atomic<Interval<float>>` with `load/store` on GUI/audio paths.

## Additional custom feature

Scratch/Vinyl mode with custom class:

- `Source/ScratchEngine.h`
- `Source/ScratchEngine.cpp`

The feature includes gesture handling, velocity smoothing, inertia decay, and real-time speed/position modulation.

## Build instructions (CMake)

From project root:

```bash
cd ExtendedCppCoursework
cmake -B build -S .
cmake --build build
```

JUCE is fetched automatically via CMake `FetchContent`.

If your system does not have `cmake` installed, this repository includes a local portable copy at:

- `.tools/cmake-4.3.0-macos-universal/CMake.app/Contents/bin/cmake`

Example with local CMake:

```bash
export PATH="$(pwd)/.tools/cmake-4.3.0-macos-universal/CMake.app/Contents/bin:$PATH"
cmake -B build -S .
cmake --build build
```

## Projucer

A Projucer project file is included:

- `ExtendedCppCoursework.jucer`

Open this in Projucer and save to generate Xcode exporter files if your module workflow requires Projucer submission.

## Runtime controls (per deck)

- Buttons: PLAY, STOP, LOAD, SCRATCH ON/OFF
- Sliders:
  - gain
  - speed
  - position
  - scratch sensitivity
  - scratch inertia
- Loop selection:
  - Left-click and drag on waveform
- Scratch gesture:
  - Enable SCRATCH mode
  - Right-click drag on waveform

## Submission artifacts included

- `CUSTOM_FEATURE_EXPLANATION.txt` (required by coursework brief)
- `VIDEO_SHOT_LIST.md`
- `VIDEO_NARRATION_SCRIPT.md`
