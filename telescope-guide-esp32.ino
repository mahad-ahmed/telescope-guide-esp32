#include <Bluepad32.h>


#define AZM_1 5
#define AZM_2 18
#define AZM_PWM 25

#define ELV_1 19
#define ELV_2 21
#define ELV_PWM 26

#define MIN_PWM 55

ControllerPtr controller;

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

void setMotorSpeed(int pwmPin, int speed) {
    analogWrite(pwmPin, speed);
}


void onConnectedController(ControllerPtr ctl) {
    if (controller == nullptr) {
        Serial.printf("CALLBACK: Controller connected\n");
        // Additionally, you can get certain gamepad properties like:
        // Model, VID, PID, BTAddr, flags, etc.
        ControllerProperties properties = ctl->getProperties();
        Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id, properties.product_id);
        controller = ctl;
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    if (controller == ctl) {
        Serial.printf("CALLBACK: Controller disconnected\n");
        controller = nullptr;
    }
}

void processGamepad(ControllerPtr ctl) {
    int rx = map(ctl->axisX(), -511, 512, -255, 255);
    int ry = map(ctl->axisY(), -511, 512, -255, 255);

    if(rx > MIN_PWM) {
        rotate(AZM_1, AZM_2, AZM_PWM, true, abs(rx));
    }
    else if(rx < -MIN_PWM) {
        rotate(AZM_1, AZM_2, AZM_PWM, false, abs(rx));
    }
    else {
        stopRotation(AZM_1, AZM_2, AZM_PWM);
    }

    if(ry > MIN_PWM) {
        rotate(ELV_1, ELV_2, ELV_PWM, true, abs(ry));
    }
    else if(ry < -MIN_PWM) {
        rotate(ELV_1, ELV_2, ELV_PWM, false, abs(ry));
    }
    else {
        stopRotation(ELV_1, ELV_2, ELV_PWM);
    }
}


void setup() {
    Serial.begin(115200);

    pinMode(AZM_1, OUTPUT);
    pinMode(AZM_2, OUTPUT);
    pinMode(AZM_PWM, OUTPUT);

    pinMode(ELV_1, OUTPUT);
    pinMode(ELV_2, OUTPUT);
    pinMode(ELV_PWM, OUTPUT);

    BP32.setup(&onConnectedController, &onDisconnectedController);

    // "forgetBluetoothKeys()" should be called when the user performs
    // a "device factory reset", or similar.
    // Calling "forgetBluetoothKeys" in setup() just as an example.
    // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
    // But it might also fix some connection / re-connection issues.
    BP32.forgetBluetoothKeys();
}

void loop() {
    bool dataUpdated = BP32.update();
    // TODO: Ensure only one controller is ever connected and that it is a gamepad
    if(dataUpdated && controller && controller->isConnected() && controller->hasData() && controller->isGamepad()) {
        processGamepad(controller);
    }
}
