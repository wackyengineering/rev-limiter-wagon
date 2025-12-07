# Rev Limiter Wagon

Rev Limiter Wagon is a lightweight C++ project that implements a configurable engine rev-limiter for microcontroller-based motor control systems. It is intended to be run on embedded platforms (Arduino, Teensy, STM32, RP2040, etc.) and provides a predictable, tunable method to limit engine RPM for safety, testing, or performance tuning.

This repository contains the core C++ code and configuration to detect RPM and apply limiter logic (e.g., ignition cut, fuel cut, or throttle control hooks). A wiring diagram will be uploaded to `./docs/wiring-diagram.*` — see the "Wiring diagram" section below.

Important safety note: This project directly affects engine operation. Improper wiring, configuration, or use can cause engine damage, personal injury, or property damage. Use at your own risk and always test in a safe, controlled environment.

## Features

- Simple, deterministic rev-limiter algorithm
- Configurable limit thresholds and behavior (soft/hard limit)
- Support for multiple RPM sensing methods (hall/optical/pulse-per-rev)
- Hook points for ignition/fuel/throttle intervention
- Minimal, portable C++ code suitable for cross-compilation to many MCUs

## Repository layout

- `src/` — C++ source files for limiter logic and platform glue
- `include/` — public headers and configuration definitions
- `platforms/` — example platform integration (Arduino/PlatformIO, Makefiles)
- `docs/` — documentation and (upcoming) wiring diagram
- `examples/` — example sketches / main programs for supported boards

## Supported Platforms

The code is portable C++ (C++11/C++17 compatible). Official example integration is provided for:

- Arduino (AVR/ARM) via PlatformIO or Arduino IDE
- Teensy via Teensyduino
- Generic CMake for STM32 or RP2040 ports

If you plan to use a specific board, check `platforms/` for an example or adapt the hardware abstraction layer in `src/hal/`.

## Requirements

- C++ toolchain appropriate for your microcontroller (gcc-arm-none-eabi, avr-gcc, etc.)
- PlatformIO or make/cmake (optional but recommended)
- Access to RPM input (Hall sensor, optical sensor, tach output) and a safe method to cut or retard ignition/fuel

## Building

Two common ways to build:

PlatformIO (recommended for Arduino/Teensy):
1. Install PlatformIO in VS Code or CLI.
2. Copy or adapt `platformio.ini` from `platforms/` to the repo root.
3. Run: pio run -e <environment_name>

CMake (for native or STM32/RP2040 projects):
1. Create a board-specific toolchain file or use existing SDK.
2. Add `src/` and `include/` as sources to your project.
3. Build with your usual CMake flow.

See `platforms/` for concrete examples per board.

## Configuration

Configuration is performed via the header `include/config.h` (or via platform-specific config in `platforms/<board>`). Typical configurable parameters:

- RPM_LIMIT: the maximum RPM before limiter activates (e.g., 7500)
- LIMIT_MODE: HARD (immediate cut) or SOFT (retard/controlled intervention)
- HYSTERESIS_RPM: amount below RPM_LIMIT to release limiter
- SAMPLE_WINDOW_MS: rpm/tach sampling window size
- INTERVENTION_DURATION_MS: how long to apply the cut per activation

Example (in `config.h`):
```c++
#define RPM_LIMIT 7500
#define LIMIT_MODE_SOFT 1
#define HYSTERESIS_RPM 200
#define SAMPLE_WINDOW_MS 100
#define INTERVENTION_DURATION_MS 50
```

Adjust these values carefully for your engine and testing environment.

## Usage

1. Wire RPM sensor to the configured input pin.
2. Wire the intervention output(s) (ignition/fuel cut or throttle control) to appropriate hardware (solid-state relay, ignition module input, etc.). NEVER drive ignition coils or high-voltage circuits directly from your MCU outputs.
3. Flash the compiled firmware to your board.
4. Start the engine in a safe place and observe behavior while slowly increasing RPM.
5. Tune `config.h` parameters as needed and retest.

## Wiring diagram

A wiring diagram will be added shortly to `docs/wiring-diagram.png` (and/or `.svg`). For now, assume:

- RPM sensor signal -> MCU digital input (with proper level shifting and debouncing)
- Ground common between MCU and sensor/engine electronics
- Intervention output -> appropriate driver (optocoupler, transistor, SSR) -> ignition/fuel cut device
- Power: MCU supplied independently with proper voltage regulation and noise filtering

Do not connect MCU pins directly to high-voltage ignition systems.

(You can find the placeholder at `./docs/wiring-diagram-placeholder.md` until the real diagram is uploaded.)

## Testing & Safety

- Always use an isolated test bench when possible.
- Use logging or LED indicators before wiring to actual ignition/fuel lines.
- Start with conservative RPM_LIMIT and verify sensor readings before enabling any intervention.
- Keep a hardware kill-switch accessible at all times.

## Troubleshooting

- No RPM detected: verify sensor wiring, pull-ups/downs, and sampling window. Check signal with oscilloscope.
- Erratic limiter triggering: add debouncing or increase SAMPLE_WINDOW_MS; check for EMI from ignition.
- Intervention not working: confirm output pin and driver are functioning and that the control device accepts the expected input type.

## Contributing

Contributions are welcome. Please open issues describing feature requests or bugs and submit PRs for improvements. Follow the coding conventions in `CONTRIBUTING.md` (if present) and document any hardware changes clearly.

## License

MIT License — see LICENSE file.

## Contact

Repository: https://github.com/wackyengineering/rev-limiter-wagon
Author / Maintainer: wackyengineering

Safety reminder: This project can affect engine operation. Follow all local laws and safety practices when using and modifying this code.
