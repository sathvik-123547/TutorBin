# Extended C++ Coursework - Primary Task Execution Plan

This plan translates the assignment brief into an implementation and submission checklist.

## 1) Non-negotiable deliverables

You must submit:

1. Packaged source code.
2. Screencast video showing required functionality.
3. One plain-text file describing your custom feature:
   - What the feature is
   - What it does
   - Where it is implemented in source code

Important: only one custom feature is marked. If you build multiple, only the largest (by LOC) is assessed for the custom feature criterion.

## 2) Core implementation requirements (primary technical task)

## A. Implement looper with template + atomics

### A1. Create `Interval` template class (`Interval.h`)

Required behavior:

- Members:
  - `mStart`
  - `mEnd`
- Public API:
  - `const T& start() const`
  - `const T& end() const`
  - `Interval& start(const T& v)` (setter + validate)
  - `Interval& end(const T& v)` (setter + validate)
  - `T length() const`
  - `bool proper() const`
- Private:
  - `Interval& validate()`:
    - if start > end, swap values

Design intent:

- `proper()` returns true only when interval has non-zero length (start < end).
- Setter chaining should work: `obj.start(a).end(b)`.

### A2. Add loop region to `DJAudioPlayer`

- Add member for loop state.
- Final version must be atomic to avoid GUI/audio thread races.

Suggested form:

- `using LoopRegion = Interval<float>;`
- `std::atomic<LoopRegion> loopRegion;`

Also include:

- `#include <atomic>`
- `#include "Interval.h"`

### A3. Loop logic in `DJAudioPlayer::getNextAudioBlock`

At top of callback:

1. Load loop region atomically (`auto l = loopRegion.load();`)
2. If `l.proper()`:
   - read current relative position
   - clamp to loop start if position < start
   - jump to loop start if position >= end

This creates forward looping over selected region.

### A4. Interactive loop visualization in `DeckGUI`

- Override `paintOverChildren(juce::Graphics&)`.
- Read loop region atomically from player.
- If loop region is proper:
  - draw translucent rectangle over waveform display bounds.

Notes:

- Use waveform bounds in parent coordinates.
- Rectangle x/width should map normalized loop [0..1] to waveform width.

### A5. Mouse-based loop editing from `DeckGUI`

Required:

- Override:
  - `mouseDown(const juce::MouseEvent&)`
  - `mouseDrag(const juce::MouseEvent&)`
- In constructor:
  - `waveformDisplay.addMouseListener(this, false);`

Behavior:

- If event component is `waveformDisplay`:
  - convert pixel x to normalized [0, 1]
  - start = mouseDownX * invWidth
  - end = currentX * invWidth
  - update loop region atomically:
    - load current
    - mutate copy with `.start(start).end(end)`
    - store updated copy
  - repaint waveform
- `mouseDown` should call `mouseDrag(event)` for click-to-set behavior.

### A6. Atomic refactor completion

Ensure every read/write of loop region uses `load()/store()`:

- `DJAudioPlayer::getNextAudioBlock`
- `DeckGUI::paintOverChildren`
- `DeckGUI::mouseDrag`

Optional debug:

- print `loopRegion.is_lock_free()` in constructor.

## B. Existing project architecture requirements from rubric

Confirm these are true in your code:

1. `DeckGUI` inherits `Button::Listener` and `Slider::Listener`, with proper overrides.
2. `MainComponent` and/or `DJAudioPlayer` inherits `AudioAppComponent` and required virtuals are correctly implemented.
3. `MainComponent` contains `DJAudioPlayer` and `DeckGUI` members.
4. `DJAudioPlayer::loadURL` uses a `std::unique_ptr` when loading files.
5. Crossfader in `MainComponent.cpp` is implemented using a lambda instead of a listener.

## C. Custom feature requirement (major marks)

Must satisfy all:

- At least 80 LOC
- Demonstrates clear programming effort/competency
- Uses custom class(es), not just wiring existing JUCE components
- Well commented
- Demonstrated clearly in screencast

Suggested high-scoring options:

1. Vinyl/scratch mode (matches brief suggestion):
   - custom `ScratchEngine` class
   - speed modulation + inertia + drag gesture response
2. Beat jump + quantized loop manager:
   - custom `GridQuantizer` and `LoopNavigator` classes
3. Cue point bank with persistence:
   - custom `CuePointManager` class with save/load

## 3) Test checklist before recording

Playback/control:

- Start/stop works on deck
- Gain/volume changes continuously
- Load from disk works via drag and drop

Waveform:

- Timecode visible, properly formatted, updates in real time
- Playhead moves with playback

Mixing:

- Two decks active
- Crossfader audibly mixes two tracks

Looper:

- Loop overlay appears over waveform
- Loop region can be adjusted interactively by mouse
- Audio loops exactly within selected region
- No crashes/glitches while dragging loop during playback

Custom feature:

- Works reliably
- Clearly visible/audible effect
- Controls are understandable

## 4) Submission checklist

Before submitting:

1. Build passes in Release config.
2. Remove debug-only prints if not needed.
3. Verify packaged source includes all needed files.
4. Add plain-text custom feature explanation file.
5. Record screencast covering every rubric item.

## 5) Common failure points to avoid

- Missing atomic `load/store` in one code path.
- Loop drawing in wrong coordinate space.
- Mouse events not attached to waveform display.
- Crossfader not implemented with lambda.
- Custom feature too small or not custom enough.
- Screencast skips one rubric item.

## 6) What to do next (immediate)

1. Place your JUCE project source in this workspace.
2. I will implement/verify each required item file by file.
3. After primary implementation is complete, I will produce:
   - final screencast shot list
   - full narration script
   - plain-text custom feature description file
