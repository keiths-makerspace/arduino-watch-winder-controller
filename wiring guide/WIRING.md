# Detailed Wiring Guide

## Component Overview

### Arduino Nano R4 Pinout

Header order is from the Nano R4 datasheet, and arrows mark the signal pins this project uses.

```
           USB-C
             |
      +------+------+
D13   |●           ●| D12
3V3   |●           ●| D11
AREF  |●           ●| D10
A0    |●           ●| D9
A1    |●           ●| D8
A2    |●           ●| D7
A3    |●           ●| D6
A4    |●           ●| D5   → BIN2
A5    |●           ●| D4   → BIN1
A6    |●           ●| D3   → AIN2
A7    |●           ●| D2   → AIN1
5V    |●           ●| GND  → common ground
BOOT  |●           ●| RST
GND   |●           ●| RX
VIN   |●           ●| TX
      +-------------+
```

Don't use VIN or the 5V pin to power this project. VIN is rated for 6-21V, so it's out of spec at 5V.

### DRV8833 Motor Driver Pinout
```
     +----------------+
     |   DRV8833     |
     |               |
VCC  |●             ●| GND
nSLP |●             ●| FAULT (ignore)
AIN1 |●             ●| BIN1
AIN2 |●             ●| BIN2
     |               |
AOUT1|●             ●| BOUT1
AOUT2|●             ●| BOUT2
     |               |
     +----------------+
```

## Step-by-Step Wiring

### Step 1: Power Connections

**USB-C to Arduino:**

Wire a USB-C plug pigtail (5V and GND only) to the breakout and plug it into the Arduino's USB-C port. Then connect the grounds directly:
```
USB-C Breakout → Arduino Nano R4
  5V pin       →   USB-C port (through pigtail)
  GND pin      →   USB-C port (through pigtail)
  GND pin      →   GND pin
```

**USB-C to DRV8833:**

Motor current comes straight from the breakout, not through the Arduino:
```
USB-C Breakout → DRV8833
  5V pin       →   VCC (motor power)
  GND pin      →   GND
```

If you power this from a USB-C to USB-C adapter, the breakout needs 5.1k pull-downs on CC1 and CC2. Many boards include them, but check yours.

**Keep Driver Awake:**
```
DRV8833
  nSLEEP/nSLP  →   VCC (tie high to keep driver enabled)
```

### Step 2: Control Signal Connections

**Arduino to DRV8833 (Motor A - Channel A):**
```
Arduino Pin 2  →  DRV8833 AIN1
Arduino Pin 3  →  DRV8833 AIN2
```

**Arduino to DRV8833 (Motor B - Channel B):**
```
Arduino Pin 4  →  DRV8833 BIN1
Arduino Pin 5  →  DRV8833 BIN2
```

### Step 3: Motor Connections

**Motor 1 (with manual switch):**
```
DRV8833 AOUT1 → Switch 1 (terminal 1)
Switch 1 (terminal 2) → Motor 1 positive wire
Motor 1 negative wire → DRV8833 AOUT2
```

**Motor 2 (with manual switch):**
```
DRV8833 BOUT1 → Switch 2 (terminal 1)
Switch 2 (terminal 2) → Motor 2 positive wire
Motor 2 negative wire → DRV8833 BOUT2
```

### Step 4: Remaining Pins

- DRV8833 FAULT pin: leave floating
- DRV8833 SENSE pins (both): connect to GND if your board brings them out, unless the board has its own sense resistors
- Arduino A0: leave floating, because the sketch reads its noise to seed the random rest times

## Complete Wiring Diagram (Text Version)

For the full drawing, see the KiCad schematic in [`kicad/`](../kicad/watch_winder_controller.kicad_pro).

```
              USB-C Power (5V 2A)
               |               |
               5V             GND
               |               |
   +-----------+               +----------+
   |           |               |          |
   |     USB-C pigtail (5V + GND)         |
   |           |               |          |
   |     +-----+-----+         |          |
   |     |  NANO R4  |         |          |
   |     |       GND |---------+          |
   |     |           |                    |
   |     | D2 ──────────→ AIN1            |
   |     | D3 ──────────→ AIN2            |
   |     | D4 ──────────→ BIN1            |
   |     | D5 ──────────→ BIN2            |
   |     +-----------+                    |
   |                                      |
   |     +---------------+                |
   +---→ | VCC, nSLEEP   |                |
         |    DRV8833    |                |
         |           GND |----------------+
         | AOUT1   BOUT1 |
         +---↓-------↓---+
             ↓       ↓
          Switch1  Switch2
             ↓       ↓
          Motor1   Motor2
             ↓       ↓
          AOUT2    BOUT2
```

## Wire Color Recommendations

While not mandatory, using consistent colors helps debugging:

- **Red**: 5V power
- **Black**: Ground (GND)
- **Yellow/Orange**: Control signals (AIN1, AIN2, BIN1, BIN2)
- **Blue/Green**: Motor connections

## Common Wiring Mistakes to Avoid

1. ❌ **Forgetting to tie nSLEEP HIGH** - motors won't run
2. ❌ **Not connecting all grounds** - erratic behavior
3. ❌ **Feeding 5V into VIN** on the Arduino - VIN is rated for 6-21V
4. ❌ **Using 3.3V instead of 5V** for DRV8833 VCC - insufficient motor power
5. ❌ **Reversed motor polarity** - motors run backwards (easy fix: swap wires or swap AIN1/AIN2)

## Testing Sequence

After wiring, test in this order:

1. **Power test**: Connect USB-C, check Arduino LED turns on
2. **Voltage test**: With multimeter, verify 5V at DRV8833 VCC
3. **Upload sketch**: Program Arduino before connecting motors
4. **Motor test**: Connect one motor, verify it runs/pauses correctly
5. **Full test**: Connect both motors, verify switches work

## Troubleshooting

**No motors run:**
- Check nSLEEP is tied to VCC (5V)
- Check the SENSE pins are grounded, if your board brings them out
- Verify all ground connections
- Check sketch uploaded successfully

**One motor doesn't run:**
- Check control pin connections (AIN1/2 or BIN1/2)
- Test switch is closed (ON position)
- Swap motor to other channel to isolate issue

**Motors run constantly:**
- Check the sketch uploaded successfully (it doesn't blink an LED, so watch for the 3-minute stop)
- Verify control pins aren't shorted

**Motors run wrong direction:**
- Swap motor wire polarity, OR
- Swap AIN1/AIN2 (or BIN1/BIN2) in code

## Safety Notes

- Use appropriate wire gauge (22-24 AWG recommended)
- Keep wires away from moving parts
- Secure all connections to prevent shorts
- Use heat shrink or electrical tape on exposed connections
- Don't exceed 2A total current draw