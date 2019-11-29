#include <Arduino.h>
#include "Debugger.h"
#include "Config.h"
#include "SerialTransport.h"
#include "Gateway.h"
#include "ServoJoint.h"
#include "StepperJoint.h"
#include "Robot.h"
#include "Joystick.h"

SerialTransport trans(MAIN_SERIAL, BAUDRATE);
Gateway gateway(&trans);
Robot robot;
Joystick ps2;

StepperJoint joint0 = StepperJoint(JOINT0_ENA_PIN, JOINT0_DIR_PIN, JOINT0_STP_PIN, JOINT0_LOCK_PIN);
StepperJoint joint1 = StepperJoint(JOINT1_ENA_PIN, JOINT1_DIR_PIN, JOINT1_STP_PIN, JOINT1_LOCK_PIN);

void setup() {
    gateway.init();

    robot.add_joint(0, &joint0);
    robot.add_joint(1, &joint1);

    robot.init();

#if USE_PS2
    // init ps2
    ps2.init();
#endif
}

void loop() {
    gateway.run();
#if USE_PS2
    ps2.check_command();
#endif
    robot.run();
}
