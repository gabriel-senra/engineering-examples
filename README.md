# Engineering Examples

This repository contains small, independently runnable C++ and TypeScript components related to audio control, physical input and music diagrams. They are educational implementations developed with AI assistance, separate from the private product source.

**C++17 · TypeScript · Node.js**

## What each example demonstrates

| Component | Problem it addresses | Implementation and checks |
| --- | --- | --- |
| [SPSC queue](include/SpscQueue.h) | Passing messages between one producer and one consumer without growing a container | Fixed capacity, explicit full/empty results and release/acquire publication; tests check FIFO order, overflow and two-thread message integrity |
| [Parameter ramp](include/SmoothedValue.h) | Moving a control value toward a target over a known number of samples | A sample-stepped linear ramp with an exact endpoint; tests cover retargeting, repeated targets and zero-length input |
| [Button debounce](include/Debouncer.h) | Preventing short switch changes from becoming repeated button actions | Accepts a candidate state after a stable interval; tests cover bounce, release and clock rollover |
| [SVG keyboard renderer](diagram/render.ts) | Producing a consistent diagram from explicit pitches | Validates input and escapes label text; tests check ranges, duplicates, text escaping and selected keys |

The components share a small scope and explicit contracts. They are not extra products or substitutes for the complete recorder, synthesizer or MCP server.

## Run the tests

With g++ and Node.js 24+ installed:

```sh
python3 check.py
```

The C++ checks can also run through CMake 3.16+:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

To run only the TypeScript renderer tests:

```sh
node --test diagram/render.test.mjs
```

GitHub Actions runs the CMake/CTest build, sanitizer checks and renderer tests.

## Contracts and limitations

The queue requires exactly one producer and one consumer and a target with lock-free index atomics. A failed push must be handled by its caller; queue behavior alone does not guarantee an application's audio deadline.

The ramp is single-threaded and accepts finite values. A new target starts from the current value, while repeating the same target does not restart the ramp.

The debouncer uses a monotonic unsigned millisecond clock and requires the documented polling interval. It handles switch states, not encoder quadrature.

The renderer accepts explicit pitches within one octave. It does not resolve chords, choose voicings or implement MCP. Node's native TypeScript execution checks runtime behavior rather than compiler type checking.

Local sanitizer checks passed with leak detection disabled because LeakSanitizer could not inspect process state in that environment. Device behavior and full-product performance require their own tests.
