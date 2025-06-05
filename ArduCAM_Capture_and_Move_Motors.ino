#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include <Stepper.h>
#include "memorysaver.h"

// Ensure only OV2640 is enabled
#if !(defined(OV2640_MINI_2MP))
#error Please enable OV2640_MINI_2MP in memorysaver.h
#endif

// Camera setup
#define CS_PIN 53
ArduCAM myCAM(OV2640, CS_PIN);

// Motor setup
#define STEPS 2048  // Full step for 360°
Stepper motor1(STEPS, 4, 5, 6, 7);   // IN1-IN4 for M1
Stepper motor2(STEPS, 8, 9, 10, 11); // IN1-IN4 for M2

void setup() {
  uint8_t vid, pid;
  uint8_t temp;

  Serial.begin(115200);
  Wire.begin();
  SPI.begin();
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);

  // Initialize motors
  motor1.setSpeed(10); // RPM
  motor2.setSpeed(10); // RPM

  // Initialize camera
  myCAM.write_reg(0x07, 0x80); delay(100);
  myCAM.write_reg(0x07, 0x00); delay(100);
  while (true) {
    myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
    if ((vid == 0x26) && (pid == 0x42)) break;
    delay(100);
  }

  Serial.println("CAM OK");
  myCAM.set_format(JPEG);
  myCAM.InitCAM();
  myCAM.OV2640_set_JPEG_size(OV2640_320x240);
  delay(1000);
}

void loop() {
  // Wait for trigger from Python
  Serial.println("Waiting for 'START' signal...");
  while (!Serial.available());
  String cmd = Serial.readStringUntil('\n');
  if (cmd != "START") return;

  // Capture and send image
  myCAM.flush_fifo();
  myCAM.clear_fifo_flag();
  myCAM.start_capture();

  Serial.println("Capturing...");
  while (!myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));

  Serial.println("Sending image...");
  uint32_t len = myCAM.read_fifo_length();
  if (len >= 100000 || len == 0) return;

  myCAM.CS_LOW();
  myCAM.set_fifo_burst();
  Serial.write("START\n"); // Notify Python to start reading image

  for (uint32_t i = 0; i < len; i++) {
    uint8_t val = SPI.transfer(0x00);
    Serial.write(val);
  }
  myCAM.CS_HIGH();
  Serial.write("END\n"); // End of image

  // Wait for ROTATE command
  Serial.println("Waiting for 'ROTATE' command...");
  unsigned long t_start = millis();
  while ((millis() - t_start) < 5000) { // 5 second timeout
    if (Serial.available()) {
      String signal = Serial.readStringUntil('\n');
      if (signal == "ROTATE") {
        Serial.println("Gesture detected. Rotating...");
        rotateMotors();
        break;
      }
    }
  }

  delay(500); // Small pause before next loop
}

void rotateMotors() {
  motor1.step(STEPS / 2);  // 180°
  motor2.step(STEPS / 2);  // 180°
}

