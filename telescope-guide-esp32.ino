#define AZM_1 5
#define AZM_2 18
#define AZM_PWM 25

#define ELV_1 19
#define ELV_2 21
#define ELV_PWM 26

#define JOYSTICK_RX 14
#define JOYSTICK_RY 27

#define THRESHOLD 30

void setup() {
  pinMode(AZM_1, OUTPUT);
  pinMode(AZM_2, OUTPUT);
  pinMode(AZM_PWM, OUTPUT);

  pinMode(ELV_1, OUTPUT);
  pinMode(ELV_2, OUTPUT);
  pinMode(ELV_PWM, OUTPUT);

  pinMode(JOYSTICK_RX, INPUT_PULLUP);
  pinMode(JOYSTICK_RY, INPUT_PULLUP);
}

int rx, ry;
void loop() {
  rx = analogRead(JOYSTICK_RX);
  ry = analogRead(JOYSTICK_RY);
  rx = map(rx, 0, 4095, -255, 255);
  ry = map(ry, 0, 4095, -255, 255);

  if(rx > THRESHOLD) {
    rotate(AZM_1, AZM_2, AZM_PWM, true, abs(rx));
  }
  else if(rx < -THRESHOLD) {
    rotate(AZM_1, AZM_2, AZM_PWM, false, abs(rx));
  }
  else {
    stopRotation(AZM_1, AZM_2, AZM_PWM);
  }

  if(ry > THRESHOLD) {
    rotate(ELV_1, ELV_2, ELV_PWM, true, abs(ry));
  }
  else if(ry < -THRESHOLD) {
    rotate(ELV_1, ELV_2, ELV_PWM, false, abs(ry));
  }
  else {
    stopRotation(ELV_1, ELV_2, ELV_PWM);
  }
}

void rotate(int pin1, int pin2, int pwmPin, bool direction, int speed) {
  digitalWrite(pin1, direction);
  digitalWrite(pin2, !direction);
  analogWrite(pwmPin, speed);
}

void stopRotation(int pin1, int pin2, int pwmPin) {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  analogWrite(pwmPin, 0);
}

// void rotate(int pin1, int pin2, int pin_pwm, double degrees) {
//   long counts = degreesToCounts(degrees);
//   myEnc.write(0);
//   // Serial.println(String(counts) + "   " + String(myEnc.read()) + "   " + String(abs(myEnc.read()) < counts));
//   setMotor(pin1, pin2, pin_pwm, degrees > 0, 85);
//   while(abs(myEnc.read()) < abs(counts)) {}
//   stopMotor(pin1, pin2, pin_pwm);
//   // delay(50);
// }

// void setMotor(int pin1, int pin2, int pin_pwm, bool direction, int speed) {
//   digitalWrite(pin1, direction);
//   digitalWrite(pin2, !direction);
//   analogWrite(pin_pwm, speed);
// }

// void stopMotor(int pin1, int pin2, int pwmPin) {
//   digitalWrite(pin1, LOW);
//   digitalWrite(pin2, LOW);
//   analogWrite(pwmPin, 0);
// }

// long degreesToCounts(double degrees) {
//   return degrees * 256 * 40;
// }
