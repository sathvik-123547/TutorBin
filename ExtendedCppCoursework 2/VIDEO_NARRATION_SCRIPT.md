# Video Narration Script

Use this as spoken narration while recording.

---

"This is my Extended C++ coursework project. It is a two-deck DJ application built in JUCE, and it includes the required interactive looper plus my custom feature, Scratch or Vinyl mode."

"I will first show one deck control with multiple widgets. On this deck, I have Play, Stop, Load, Gain, Speed, Position, and Scratch controls. I can start playback, stop playback, and continuously change the gain while audio is playing."

"Now I will demonstrate file loading from disk using drag and drop. I drag one file onto Deck A and another file onto Deck B. Both files are loaded and ready for playback."

"On the waveform display, you can see the timecode text updating in real time, and the playhead moves in sync with playback."

"Next, I will show both repeated decks running together, then use the crossfader in the center. As I move the crossfader left and right, the output mix transitions smoothly between Deck A and Deck B."

"Now I will demonstrate the interactive looper. I click and drag on the waveform to set a loop interval. The selected region is visualized as a red translucent overlay above the waveform. While playback continues, the audio loops correctly inside this selected region. I can also adjust the loop region interactively while the track is playing."

"The looper is implemented with a custom template Interval class and uses atomics to avoid data races between GUI and audio threads."

"I will now demonstrate my custom feature: Scratch or Vinyl mode. I enable Scratch mode on this deck, then right-click drag across the waveform to simulate turntable-style control. The playback responds immediately to drag direction and speed. I can increase sensitivity for stronger scratch response and adjust inertia so the motion decays naturally after releasing the mouse."

"Technically, this feature is implemented with a custom ScratchEngine class that handles gesture tracking, velocity smoothing, inertia decay, and real-time speed and position modulation. This is a custom implementation, not just a minor UI tweak."

"That completes the demonstration of the two-deck interface, drag-and-drop loading, waveform and timecode, playhead tracking, crossfader mixing, interactive looping, and the custom Scratch/Vinyl feature."

---

Tip while recording:
- Keep system audio level moderate to avoid clipping.
- Pause briefly between sections to make editing easier.
