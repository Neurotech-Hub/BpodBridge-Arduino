#include <TMCStepper.h>

// BOARD
#define VIO_CTRL 33

// GPIO
#define GPIO_1A 16
#define GPIO_2A 17
#define GPIO_3A 18
#define GPIO_4A 19
#define GPIO_5A 20
#define GPIO_6A 21
#define GPIO_7A 22
#define GPIO_8A 23

// MOTOR0
#define EN_MOTOR0 5
#define DIR_MOTOR0 2
#define STP_MOTOR0 3
#define CS_MOTOR0 10

// OTHER
#define R_SENSE 0.075f  // Watterott TMC5160 uses 0.075

TMC5160Stepper driver(CS_MOTOR0, R_SENSE);

void setup() {
  pinMode(GPIO_1A, INPUT_PULLUP);
  pinMode(GPIO_2A, INPUT_PULLUP);
  pinMode(GPIO_3A, OUTPUT);
  pinMode(GPIO_4A, OUTPUT);
  pinMode(GPIO_5A, INPUT_PULLUP);
  pinMode(GPIO_6A, INPUT_PULLUP);

  pinMode(EN_MOTOR0, OUTPUT);
  pinMode(STP_MOTOR0, OUTPUT);
  pinMode(DIR_MOTOR0, OUTPUT);
  digitalWrite(EN_MOTOR0, LOW);  // Enable driver in hardware

  pinMode(VIO_CTRL, OUTPUT);  // give TMC VIO/3.3V
  digitalWrite(VIO_CTRL, HIGH);
  delay(10);

  Serial.begin(9600);
  delay(1000);

  SPI.begin();  // SPI drivers

  driver.begin();              // SPI: Init CS pins and possible SW SPI pins
  driver.toff(5);              // Enables driver in software
  driver.rms_current(600);     // Set motor RMS current
  driver.microsteps(2);        // Set microsteps to 1/16th
  driver.en_pwm_mode(true);    // Toggle stealthChop on TMC2130/2160/5130/5160
  driver.pwm_autoscale(true);  // Needed for stealthChop
}

void loop() {
  // Check GPIO_1A and set GPIO_3A high for 3 seconds if GPIO_1A goes LOW
  if (digitalRead(GPIO_1A) == LOW) {
    digitalWrite(GPIO_3A, HIGH);
    delay(3000);
    digitalWrite(GPIO_3A, LOW);
  }

  // Check GPIO_2A and set GPIO_4A high for 3 seconds if GPIO_2A goes LOW
  if (digitalRead(GPIO_2A) == LOW) {
    digitalWrite(GPIO_4A, HIGH);
    delay(3000);
    digitalWrite(GPIO_4A, LOW);
  }

  // Set motor direction and run while GPIO_5A or GPIO_6A are LOW
  if (digitalRead(GPIO_5A) == LOW) {
    digitalWrite(DIR_MOTOR0, HIGH);  // Set direction
    digitalWrite(STP_MOTOR0, HIGH);
    delayMicroseconds(160);
    digitalWrite(STP_MOTOR0, LOW);
    delayMicroseconds(160);
  } else if (digitalRead(GPIO_6A) == LOW) {
    digitalWrite(DIR_MOTOR0, LOW);  // Set opposite direction
    digitalWrite(STP_MOTOR0, HIGH);
    delayMicroseconds(160);
    digitalWrite(STP_MOTOR0, LOW);
    delayMicroseconds(160);
  }
}
