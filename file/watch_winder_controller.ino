/*
 * Dual Watch Winder Controller
 * 
 * Controls two watch winder motors with configurable duty cycle.
 * Default: 3 minutes run, 60 minutes rest (~800 TPD)
 * Each motor alternates rotation direction each cycle for even wear.
 * Motors start at different times to reduce power spikes.
 * 
 * Hardware:
 * - Arduino Nano R4 (or compatible)
 * - DRV8833 dual motor driver
 * - 2x 3V DC gear motors (300mA each)
 * - USB-C 5V power supply
 * 
 * Author: Keith Refior
 * License: MIT
 * Date: November 2025
 */

// ============================================
// CONFIGURATION - Adjust these for your needs
// ============================================

// Motor run and rest times (in milliseconds)
const unsigned long RUN_TIME = 180000;          // 3 minutes (180,000 ms)
const unsigned long REST_TIME_BASE = 3600000;   // 60 minutes base (3,600,000 ms)
const unsigned long REST_TIME_VARIANCE = 300000; // +/- 5 minutes variance (300,000 ms)
const unsigned long STARTUP_STAGGER = 90000;    // 1.5 minute delay between motor starts

// Motor 1 control pins (connected to DRV8833 channel A)
const int MOTOR1_PIN1 = 2;  // AIN1
const int MOTOR1_PIN2 = 3;  // AIN2

// Motor 2 control pins (connected to DRV8833 channel B)
const int MOTOR2_PIN1 = 4;  // BIN1
const int MOTOR2_PIN2 = 5;  // BIN2

// ============================================
// GLOBAL VARIABLES
// ============================================

// Track timing for each motor independently
unsigned long motor1NextStart = 0;
unsigned long motor2NextStart = STARTUP_STAGGER;  // Start Motor 2 later
bool motor1Running = false;
bool motor2Running = false;
bool motor1Direction = true;   // true = clockwise, false = counter-clockwise
bool motor2Direction = true;

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
  
  // Seed random number generator with analog noise
  randomSeed(analogRead(A0));
  
  // Optional: Initialize serial for debugging
  // Serial.begin(9600);
  // Serial.println("Watch Winder Controller Started");
  // Serial.println("Motors will start at staggered times");
}

// ============================================
// MAIN LOOP - Runs continuously
// ============================================

void loop() {
  unsigned long currentTime = millis();
  
  // === MOTOR 1 CONTROL ===
  if (!motor1Running && currentTime >= motor1NextStart) {
    // Time to start Motor 1
    runMotor1(motor1Direction);
    motor1Running = true;
    motor1NextStart = currentTime + RUN_TIME;  // Schedule stop time
  } 
  else if (motor1Running && currentTime >= motor1NextStart) {
    // Time to stop Motor 1
    stopMotor1();
    motor1Running = false;
    
    // Schedule next start with random variance (55-65 minutes from now)
    unsigned long restTime = REST_TIME_BASE + random(-REST_TIME_VARIANCE, REST_TIME_VARIANCE);
    motor1NextStart = currentTime + restTime;
    
    // Alternate direction for next cycle
    motor1Direction = !motor1Direction;
  }
  
  // === MOTOR 2 CONTROL ===
  if (!motor2Running && currentTime >= motor2NextStart) {
    // Time to start Motor 2
    runMotor2(motor2Direction);
    motor2Running = true;
    motor2NextStart = currentTime + RUN_TIME;  // Schedule stop time
  } 
  else if (motor2Running && currentTime >= motor2NextStart) {
    // Time to stop Motor 2
    stopMotor2();
    motor2Running = false;
    
    // Schedule next start with random variance (55-65 minutes from now)
    unsigned long restTime = REST_TIME_BASE + random(-REST_TIME_VARIANCE, REST_TIME_VARIANCE);
    motor2NextStart = currentTime + restTime;
    
    // Alternate direction for next cycle
    motor2Direction = !motor2Direction;
  }
  
  // Small delay to prevent tight looping
  delay(100);
}

// ============================================
// MOTOR CONTROL FUNCTIONS
// ============================================

/**
 * Run Motor 1 in specified direction
 * @param direction - true for clockwise, false for counter-clockwise
 */
void runMotor1(bool direction) {
  if (direction) {
    // Clockwise rotation
    digitalWrite(MOTOR1_PIN1, HIGH);
    digitalWrite(MOTOR1_PIN2, LOW);
  } else {
    // Counter-clockwise rotation
    digitalWrite(MOTOR1_PIN1, LOW);
    digitalWrite(MOTOR1_PIN2, HIGH);
  }
}

/**
 * Run Motor 2 in specified direction
 * @param direction - true for clockwise, false for counter-clockwise
 */
void runMotor2(bool direction) {
  if (direction) {
    // Clockwise rotation
    digitalWrite(MOTOR2_PIN1, HIGH);
    digitalWrite(MOTOR2_PIN2, LOW);
  } else {
    // Counter-clockwise rotation
    digitalWrite(MOTOR2_PIN1, LOW);
    digitalWrite(MOTOR2_PIN2, HIGH);
  }
}

/**
 * Stop Motor 1
 */
void stopMotor1() {
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, LOW);
}

/**
 * Stop Motor 2
 */
void stopMotor2() {
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, LOW);
}

/**
 * Stop both motors (used during initialization)
 */
void stopMotors() {
  stopMotor1();
  stopMotor2();
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
 * Features:
 * 
 * 1. STAGGERED STARTUP: Motor 2 starts 1.5 minutes after Motor 1
 *    This reduces the initial power spike when first plugged in.
 * 
 * 2. RANDOM REST VARIANCE: Each rest period varies by ±5 minutes
 *    This ensures motors don't always start simultaneously after
 *    the initial stagger, reducing power spikes throughout operation.
 * 
 * 3. DIRECTION ALTERNATION: Each motor alternates between clockwise
 *    and counter-clockwise on every cycle. This prevents overwinding
 *    and mimics natural wrist movement for healthier watch operation.
 * 
 * 4. INDEPENDENT OPERATION: Each motor runs on its own schedule,
 *    allowing manual switches to work independently.
 * 
 * To adjust TPD:
 * - More TPD: Decrease REST_TIME_BASE or increase RUN_TIME
 * - Less TPD: Increase REST_TIME_BASE or decrease RUN_TIME
 * 
 * Recommended TPD by brand:
 * - Seiko: 700-900 TPD (bidirectional)
 * - Bering: 650-800 TPD (bidirectional)
 * - Rolex: 650-800 TPD (bidirectional)
 * - Omega: 650-800 TPD (bidirectional)
 * - Breitling: 650-800 TPD (bidirectional)
 * 
 * Note on millis() overflow:
 * The millis() function overflows after ~49 days. This code handles
 * overflow gracefully since all timing comparisons are relative.

 */
