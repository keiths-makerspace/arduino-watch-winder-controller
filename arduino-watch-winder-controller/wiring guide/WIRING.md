# Detailed Wiring Guide

## Component Overview

### Arduino Nano R4 Pinout
```
         USB-C
           |
    +------+------+
    |  NANO R4   |
    |            |
GND |●          ●| VIN (5V input)
RST |●          ●| GND
D2  |●          ●| RST
D3  |●          ●| 5V (output)
D4  |●          ●| A7
D5  |●          ●| A6
    |            |
    +------------+
```

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
```
USB-C Breakout → Arduino Nano R4
  5V pin       →   VIN pin
  GND pin      →   GND pin
```

**Arduino to DRV8833:**
```
Arduino Nano R4 → DRV8833
  5V pin        →   VCC (motor power)
  GND pin       →   GND
```

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

### Step 4: Leave Unconnected

These pins are optional and not needed for basic operation:
- DRV8833 FAULT pin (leave floating)
- DRV8833 SENSE pins (both - leave floating)

## Complete Wiring Diagram (Text Version)

```
                         USB-C Power (5V 2A)
                               |
                    +----------+----------+
                    |                     |
                   VIN                   GND
                    |                     |
              +-----+-----+               |
              | NANO R4   |               |
              |           |               |
          5V  |●         ●| GND ----------+
              |           |               |
          D2  |●         ●|               |
              |  ↓↓↓↓     |               |
          D3  |● ↓AIN1    |               |
              |  ↓AIN2    |               |
          D4  |● ↓BIN1    |               |
              |  ↓BIN2    |               |
          D5  |● ↓        |               |
              |   ↓       |               |
              +---↓-------+               |
                  ↓                       |
                  ↓ (control signals)     |
                  ↓                       |
            +-----↓---------+             |
            |   DRV8833     |             |
    +-------|               |-------------+
    |       |  AIN1 ← D2    |
    |   VCC |● AIN2 ← D3    |
    |       |  BIN1 ← D4    |
    +--nSLP |● BIN2 ← D5    |
    |       |               |
    +--5V   |               | GND ---------+
            |               |
            |  AOUT1  BOUT1 |
            |    ↓      ↓   |
            +----↓------↓---+
                 ↓      ↓
                 ↓      ↓
            Switch1  Switch2
                 ↓      ↓
              Motor1  Motor2
                 ↓      ↓
            AOUT2 ← → BOUT2
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
3. ❌ **Swapping VIN and 5V** on Arduino - won't power correctly
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
- Verify all ground connections
- Check sketch uploaded successfully

**One motor doesn't run:**
- Check control pin connections (AIN1/2 or BIN1/2)
- Test switch is closed (ON position)
- Swap motor to other channel to isolate issue

**Motors run constantly:**
- Check sketch is running (LED should blink during operation)
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