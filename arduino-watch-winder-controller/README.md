# Arduino Watch Winder Controller

A simple and reliable dual watch winder controller using Arduino Nano R4 and DRV8833 motor driver. This project replaces failed commercial watch winder PCBs with an easy-to-build, customizable solution.

## Features

- Controls two independent watch winder motors
- Configurable duty cycle (default: 3 min run / 60 min rest)
- Bidirectional rotation (alternates each cycle)
- ~720-800 turns per day (TPD) - suitable for most automatic watches
- Manual on/off switches for each motor
- USB-C powered (5V)

## Suitable for

This duty cycle works well for:
- **Seiko** automatic watches (700-900 TPD)
- **Bering** automatic watches (650-800 TPD)
- Most **Rolex, Omega, Tag Heuer** (650-800 TPD)
- Other brands with similar requirements

## Hardware Requirements

### Components
- Arduino Nano R4 (or compatible)
- DRV8833 dual motor driver board
- 2x 3V DC gear motors (tested with 300mA motors)
- USB-C panel mount connector or breakout board
- USB-C power adapter (5V, 2A recommended)
- 2x SPST switches (for manual motor control)
- Jumper wires
- Existing watch winder enclosure (optional)

### Estimated Cost
Approximately $15-25 depending on parts sourcing.

## Wiring Diagram

```
USB-C 5V Power → Arduino Nano R4 VIN
USB-C GND ────→ Arduino GND ──→ DRV8833 GND

Arduino 5V ───→ DRV8833 VCC (motor power)
Arduino 5V ───→ DRV8833 nSLEEP (keep awake)

Arduino Pin 2 → DRV8833 AIN1
Arduino Pin 3 → DRV8833 AIN2
Arduino Pin 4 → DRV8833 BIN1
Arduino Pin 5 → DRV8833 BIN2

DRV8833 AOUT1 → Switch 1 → Motor 1 positive
DRV8833 AOUT2 ───────────→ Motor 1 negative

DRV8833 BOUT1 → Switch 2 → Motor 2 positive
DRV8833 BOUT2 ───────────→ Motor 2 negative
```

**Note:** DRV8833 SENSE pins can be left unconnected.

## Installation

1. **Clone this repository:**
   ```bash
   git clone https://github.com/yourusername/arduino-watch-winder-controller.git
   cd arduino-watch-winder-controller
   ```

2. **Open the sketch:**
   - Open `watch_winder_controller.ino` in Arduino IDE

3. **Install board support (if needed):**
   - Go to Tools → Board → Boards Manager
   - Search for "Arduino Nano R4" and install

4. **Upload to Arduino:**
   - Connect Arduino via USB
   - Select Board: Arduino Nano R4
   - Select correct Port
   - Click Upload

5. **Wire according to diagram above**

6. **Test:**
   - Power on via USB-C
   - Both motors should run for 3 minutes, then pause for 60 minutes
   - Direction alternates each cycle
   - Switches provide manual on/off control

## Configuration

Edit these values in `watch_winder_controller.ino` to customize:

```cpp
const int RUN_TIME = 180000;      // Run time in milliseconds (default: 3 min)
const int REST_TIME = 3600000;    // Rest time in milliseconds (default: 60 min)
```

### TPD Adjustment Guide

| Rest Time | Approximate TPD |
|-----------|----------------|
| 45 min    | ~1000 TPD      |
| 60 min    | ~800 TPD       |
| 90 min    | ~550 TPD       |

## Troubleshooting

**Motors don't run:**
- Check all connections, especially ground
- Verify 5V power is reaching DRV8833 VCC
- Ensure nSLEEP is tied HIGH (to VCC)
- Test switches are in ON position

**Motors run too fast/slow:**
- Adjust RUN_TIME to compensate
- Consider PWM speed control (see advanced branch)

**Only one motor works:**
- Check switch position
- Verify both sets of motor driver pins are connected
- Test motor directly with power supply

**Watch stops/overwinding:**
- Increase REST_TIME (try 90 minutes)
- Some watches need less TPD than others

## License

MIT License - see LICENSE file for details

## Contributing

Contributions welcome! Please open an issue or submit a pull request.

## Acknowledgments

Built to replace a failed commercial watch winder PCB. Suitable for Seiko, Bering, and other automatic watch brands.

---

**Disclaimer:** Use at your own risk. Monitor your watches initially to ensure proper winding. Different watch movements have different requirements.