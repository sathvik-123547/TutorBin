# 1:1 Screencast Recording Checklist

Use this as a literal runbook while recording. Follow in order.

## 0) Preflight (2-3 minutes before recording)

- Prepare two music files on Desktop:
  - `trackA.mp3` (or `.wav`)
  - `trackB.mp3` (or `.wav`)
- Close noisy apps/notifications.
- Set system output volume around 50-70%.
- Open app binary:
  - `build/ExtendedCppCoursework_artefacts/ExtendedCppCoursework.app`
- Resize app so both decks and crossfader are visible.
- Open this checklist + `VIDEO_NARRATION_SCRIPT.md` on side screen if possible.
- Start screen recording with system audio + microphone.

## 1) Recording timeline (target 4:30 to 6:00)

## 00:00 - 00:15 Intro

Clicks:
- Keep app centered and idle.

Say:
- "This is my Extended C++ coursework project: a two-deck JUCE DJ app with an interactive looper and a custom Scratch/Vinyl mode."

Pass check:
- App UI is clearly visible and readable.

## 00:15 - 00:55 One deck controls (5+ widgets)

Clicks:
- On left deck:
  - show `PLAY`, `STOP`, `LOAD`, gain slider, speed slider, position slider.
- Click `PLAY`, then `STOP`.
- Move gain slider continuously up/down.
- Move speed slider slightly and return near `1.0`.

Say:
- "On one deck I have play, stop, load, gain, speed, and position controls. Playback can be started and stopped, and gain is continuously adjustable."

Pass check:
- At least five widgets are clearly shown.

## 00:55 - 01:25 Drag-and-drop loading

Clicks:
- Drag `trackA` onto left waveform area.
- Drag `trackB` onto right waveform area.

Say:
- "I load files directly from disk via drag and drop onto each deck."

Pass check:
- Viewer can see file drag gesture onto both decks.

## 01:25 - 01:55 Waveform, timecode, playhead

Clicks:
- Press `PLAY` on left deck.
- Point mouse near waveform timecode text.
- Let it play for a few seconds.

Say:
- "The waveform display is shown with updating timecode, and the playhead moves with playback."

Pass check:
- Timecode visibly changes.
- Playhead visibly moves.

## 01:55 - 02:35 Two decks + crossfader

Clicks:
- Press `PLAY` on right deck.
- Move crossfader fully left, center, fully right, then center.

Say:
- "Both repeated decks are active, and the crossfader mixes audio between deck A and deck B."

Pass check:
- Crossfader movement is clearly visible and slow enough to follow.

## 02:35 - 03:35 Looper demo (core requirement)

Clicks:
- On left deck waveform, left-click and drag to create a loop region.
- Keep playback running; wait long enough to hear repeat.
- Drag again to resize/move selected loop area.

Say:
- "I can interactively select a loop region on top of the waveform. The loop area is visualized by a translucent red overlay, and playback loops correctly inside that region."
- "This looper is implemented across `Interval`, `DJAudioPlayer`, and `DeckGUI` using a template class and atomics for thread-safe GUI/audio access."

Pass check:
- Red loop overlay is clearly shown.
- Repeating loop is audible.
- Region is adjusted interactively during playback.

## 03:35 - 04:55 Custom feature: Scratch/Vinyl mode

Clicks:
- On left deck, click `SCRATCH OFF` to turn it on (`SCRATCH ON`).
- Right-click drag horizontally on waveform (slow + fast gestures).
- Adjust scratch sensitivity slider (higher, then lower).
- Adjust inertia slider (higher, release mouse and let decay happen).

Say:
- "My custom feature is Scratch/Vinyl mode with a custom `ScratchEngine` class."
- "Right-click drag simulates turntable movement, with gesture tracking, velocity smoothing, and inertia decay."
- "Sensitivity and inertia are user-adjustable in real time."

Pass check:
- `SCRATCH ON` is visible.
- Right-click drag gesture is visible.
- Effect difference before/after slider changes is noticeable.

## 04:55 - 05:20 Wrap-up

Clicks:
- Keep both decks visible and pause mouse.

Say:
- "This demonstrates the required two-deck controls, drag-and-drop loading, waveform with timecode and playhead, crossfader mixing, interactive looper, and my custom Scratch/Vinyl feature."
- "The source package and custom feature explanation text file are included."

Pass check:
- End cleanly without abrupt cutoff.

## 2) One-take quality gates (quick self-check)

- No dead air > 5 seconds.
- Mouse movement is deliberate (not frantic).
- Every rubric item is visibly demonstrated at least once.
- Audio is audible but not clipping.
- Total duration between 4 and 6 minutes.

## 3) Fast retake guide (if needed)

If any item is missed, retake only from nearest section:

- Missed drag/drop -> restart from `00:55`.
- Missed timecode/playhead -> restart from `01:25`.
- Looper not obvious -> restart from `02:35`.
- Scratch feature unclear -> restart from `03:35`.

## 4) Final export checklist

- Filename example: `ExtendedCppCoursework_Screencast.mp4`
- Confirm video includes:
  - app window
  - your voice narration
  - system audio
- Play first 20s and last 20s before submission.
