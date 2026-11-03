# Code examples

Four small, runnable components for audio software, physical controls and music diagrams. Each includes source and tests.

**Built with:** C++ and TypeScript · **Run locally:** Python 3, g++ and Node.js 24+

## Pass a message between two threads

A control changes in the interface; the audio thread needs the new value. The [SPSC queue](include/SpscQueue.h) passes messages in order using storage reserved in advance. One thread adds messages and one removes them.

A full queue returns `false` instead of growing. An empty queue also returns `false`, leaving the caller to decide what to do next.

## Move a value gradually

The [parameter ramp](include/SmoothedValue.h) spreads a change over several samples. A move from 0 to 1 over four steps produces:

**0.25 → 0.5 → 0.75 → 1.0**

This is useful for smoothing a level change. A new target starts from the current value, while repeating the same target lets the existing ramp continue.

## Read a physical button

A switch can briefly flicker between pressed and released as its contacts settle. The [debouncer](include/Debouncer.h) accepts a new state only after it stays unchanged for a chosen interval, producing one accepted change rather than reacting to every flicker.

## Draw a playable set of notes

The [keyboard renderer](diagram/render.ts) takes a label and explicit pitches, then highlights their keys. Here, MIDI note numbers 60, 64 and 67 represent C, E and G:

![C major keyboard diagram](diagram/example.svg)

The renderer validates the pitches and safely includes the label in the image. Interpreting chord names belongs in a separate music-theory layer.

## Try them

From the repository root:

```sh
python3 check.py
```

This compiles and runs the C++ checks, then runs the keyboard-renderer tests. Tests cover queue order and capacity, threaded messages, ramps, button bounce, clock wraparound and image output. GitHub Actions also runs the build and memory checks.

## Further reading

[Usage examples, component limits and individual test commands](docs/implementation.md)

These are educational implementations developed with AI assistance, separate from the private product code.
