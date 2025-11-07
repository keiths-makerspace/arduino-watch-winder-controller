/*
 * Dual Watch Winder Controller
 * 
 * Controls two watch winder motors with configurable duty cycle.
 * Default: 3 minutes run, 60 minutes rest (~800 TPD)
 * Alternates rotation direction each cycle for even wear.
 * 
 * Hardware:
 * - Arduino Nano R4 (or compatible)
 * - DRV8833 dual motor driver
 * - 2x 3V DC gear motors (300mA each)
 * - USB-C 5V power supply
 * 
 * Author: [Your Name]
 * License: MIT
 * Date: October 2025
 */

// ============================================
// CONFIGURATION - Adjust these for your needs
// ============================================

// Motor run and rest times (in milliseconds)
const unsigned long RUN_TIME = 180000;      // 3 minutes (180,000 ms)
const unsigned long REST_TIME = 3600000;    // 60 minutes (3,600,000 ms)

// Motor 1 control pins (connected to DRV8833 channel A)
const int MOTOR1_PIN1 = 2;  // AIN1
const int MOTOR1_PIN2 = 3;  // AIN2

// Motor 2 control pins (connected to DRV8833 channel B)
const int MOTOR2_PIN1 = 4;  // BIN1
const int MOTOR2_PIN2 = 5;  // BIN2

// ============================================
// GLOBAL VARIABLES
// ============================================

bool clockwise = true;  // Track rotation direction

// ============================================
// SETUP - Runs once at startup
// ============================================

void setup() {
  // Initialize motor control pins as outputs
  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);
  
  // Ensure motors start in stopped state
  stopMotors();
  
  // Optional: Initialize serial for debugging
  // Serial.begin(9600);
  // Serial.println("Watch Winder Controller Started");
}

// ============================================
// MAIN LOOP - Runs continuously
// ============================================

void loop() {
  // Run motors in current direction
  runMotors(clockwise);
  delay(RUN_TIME);
  
  // Stop motors for rest period
  stopMotors();
  delay(REST_TIME);
  
  // Alternate direction for next cycle
  clockwise = !clockwise;
}

// ============================================
// MOTOR CONTROL FUNCTIONS
// ============================================

/**
 * Run both motors in specified direction
 * @param direction - true for clockwise, false for counter-clockwise
 */
void runMotors(bool direction) {
  if (direction) {
    // Clockwise rotation
    digitalWrite(MOTOR1_PIN1, HIGH);
    digitalWrite(MOTOR1_PIN2, LOW);
    digitalWrite(MOTOR2_PIN1, HIGH);
    digitalWrite(MOTOR2_PIN2, LOW);
  } else {
    // Counter-clockwise rotation
    digitalWrite(MOTOR1_PIN1, LOW);
    digitalWrite(MOTOR1_PIN2, HIGH);
    digitalWrite(MOTOR2_PIN1, LOW);
    digitalWrite(MOTOR2_PIN2, HIGH);
  }
}

/**
 * Stop both motors completely
 */
void stopMotors() {
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, LOW);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, LOW);
}

// ============================================
// NOTES & TPD CALCULATION
// ============================================

/*
 * TPD (Turns Per Day) Calculation:
 * 
 * With 3 min run / 60 min rest:
 * - Cycles per day: 1440 min / 63 min = ~23 cycles
 * - Rotations per 3-min cycle: ~40 rotations (at ~13 RPM)
 * - Total TPD: 23 × 40 = ~800-900 TPD
 * 
 * To adjust TPD:
 * - More TPD: Decrease REST_TIME or increase RUN_TIME
 * - Less TPD: Increase REST_TIME or decrease RUN_TIME
 * 
 * Recommended TPD by brand:
 * - Seiko: 700-900 TPD (bidirectional)
 * - Rolex: 650-800 TPD (bidirectional)
 * - Omega: 650-800 TPD (bidirectional)
 * - Breitling: 650-800 TPD (bidirectional)
 */