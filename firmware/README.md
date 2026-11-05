# ESP32-C3 Tri-State Toggle

Tenstar ESP32-C3 Super Mini firmware that "presses a button" on GPIO3:
two short LOW pulses (300 ms each, 600 ms apart) every 60 s, hi-Z otherwise.

It also provides basic diagnostics:

- **Heartbeat LED:** onboard blue LED (GPIO8, active low) blinks once per
  second (2 s period, 50% duty) to show the firmware is alive.
- **UART logging:** every message is prefixed with the uptime in seconds:
  `[0] Startup complete` on boot and `[N] Toggling units button` on each
  press, over the built-in USB port at 115200 baud (USB CDC enabled via
  `ARDUINO_USB_CDC_ON_BOOT`).

## Prerequisites

- [PlatformIO CLI](https://platformio.org/install/cli) (`pio`)
- Board connected via USB (built-in USB-Serial-JTAG shows up as `/dev/ttyACM0`)

## Build

```sh
pio run -d firmware
```

## Flash

```sh
pio run -d firmware -t upload
```

Or build + flash in one go (the default target already builds first — `-t upload` is enough).

## Monitor

```sh
pio device monitor -p /dev/ttyACM0 -b 115200
```

## GPIO

The button line is driven by **GPIO3** (`TOGGLE_PIN`) as an open-drain output:

- **Pulse (press):** pin switched to `OUTPUT` and driven `LOW` for `PULSE_MS`
- **Idle (release):** pin switched back to `INPUT` — true hi-Z, no internal
  pull-up/pull-down, never driven high

Because the idle state is hi-Z, an external device can pull the line low at any
time without fighting the ESP32. Add a pull-up resistor if the line needs a
defined high level.

**Note:** the onboard BOOT button sits on GPIO9. If it is held low during reset
(e.g. a squeezed or stuck button), the chip boots into download mode and the app
never runs — the LED will not blink.

## Configuration

Edit the defines at the top of `src/main.cpp`, then reflash:

```cpp
#define TOGGLE_PIN 3                  // GPIO to drive
#define LED_PIN 8                     // onboard LED, active low
#define PERIOD_MS 60 * 1000           // overall cycle time
#define PULSE_MS 300                  // LOW duration per press
#define PULSE_SPACING_MS 600          // start-to-start spacing of the two presses
#define LED_PERIOD_MS 2000            // blink period
#define LED_ON_MS 1000                // 50% duty cycle
```
