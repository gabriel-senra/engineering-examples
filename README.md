# Engineering Examples

Small examples at the boundary between sound, input and software. Each has a narrow contract, explicit failure behavior and tests that run without audio hardware or product source.

**C++17 · TypeScript · Standard library · Node.js**

These examples were prepared for this portfolio review. Queue and ramp examples were revised from the supplied portfolio starter; debounce and rendering examples were added in this execution. They are not historical product commits or extracted proprietary implementations. Gabriel should review and be able to explain each example before publishing it as selected work.

| Example | Contract | Checks |
| --- | --- | --- |
| [Bounded SPSC queue](include/SpscQueue.h) | Exactly one producer and one consumer; fixed power-of-two capacity; failed push changes nothing; failed pop preserves output | Empty/full, FIFO, slot reuse and 100,000 two-field messages across two threads |
| [Parameter ramp](include/SmoothedValue.h) | Finite float targets; single-thread use; fixed sample count; exact final target | Endpoint, holding, mid-ramp retarget, repeated target and nonpositive length |
| [Button debounce](include/Debouncer.h) | One control task; monotonic uint32 milliseconds; stable input for the chosen interval | Bounce, press/release, no repeated transition and clock rollover |
| [Diagram renderer](diagram/render.ts) | 1–12 distinct explicit MIDI pitches, 60–71; text label at most 80 characters | Rejected inputs, escaped text, 12-key geometry, selected-note count and input isolation |

Run both suites with `python3 check.py` (requires g++ and Node 24+).

## Run C++ checks

```sh
g++ -std=c++17 -pthread -Wall -Wextra -Wpedantic -Werror -O2 -Iinclude tests/test_samples.cpp -o /tmp/engineering-tests
/tmp/engineering-tests
```

Or with CMake 3.16+:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

The direct compiler route was run here. CMake is supplied for portability but was unavailable in the review environment. The workflow is prepared; no hosted CI result is claimed.

## Run TypeScript checks

Use Node.js 24+, whose native type stripping executes this erasable TypeScript without downloads:

```sh
node --test diagram/render.test.mjs
node --input-type=module -e "import {render} from './diagram/render.ts'; console.log(render({label:'C major',notes:[60,64,67]}))" > /tmp/chord.svg
```

Native execution checks runtime behavior; it is not a TypeScript compiler type check. The example does not implement MCP transport or PNG export.

## Why these choices matter

A queue publishes slot writes with release/acquire ordering; relaxed access is sufficient for an index owned by only one thread. The queue rejects overload instead of allocating. Lock-free atomics are required at compile time on the target. No guarantee of application-level deadline compliance follows from that alone.

A ramp lands exactly on its target after the chosen number of samples, avoiding accumulated endpoint drift. A retarget starts from the current value. NaN/infinite targets are outside this small example's contract.

The debouncer waits for a stable candidate instead of accepting the first change after a timeout. Unsigned subtraction tolerates one clock rollover within the documented polling contract. Raw encoder quadrature requires a separate decoder.

The renderer accepts notes rather than generating voicings, which keeps musical inference outside the inspectable sample. XML escaping applies to visible and accessible text. It intentionally draws one octave only.

## Validation scope

Local C++ checks and four Node tests pass. AddressSanitizer and UndefinedBehaviorSanitizer pass with leak detection disabled because LeakSanitizer cannot inspect processes in this environment. No ThreadSanitizer, real-time deadline, device or full-product test result is asserted.

