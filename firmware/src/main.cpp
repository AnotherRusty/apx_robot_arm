#include <Arduino.h>
#include "Debugger.h"
#include "Config.h"
#include "SerialTransport.h"
#include "Gateway.h"
#include "ServoJoint.h"
#include "StepperJoint.h"
#include "Robot.h"
#include "ServoEndEffector.h"
#include "VacuumEndEffector.h"

SerialTransport trans(MAIN_SERIAL, BAUDRATE);
Gateway gateway(&trans);
Robot robot;

StepperJoint joint0 = StepperJoint(JOINT0_ENA_PIN, JOINT0_DIR_PIN, JOINT0_STP_PIN, JOINT0_LOCK_PIN);
StepperJoint joint1 = StepperJoint(JOINT1_ENA_PIN, JOINT1_DIR_PIN, JOINT1_STP_PIN, JOINT1_LOCK_PIN);
StepperJoint joint2 = StepperJoint(JOINT2_ENA_PIN, JOINT2_DIR_PIN, JOINT2_STP_PIN, JOINT2_LOCK_PIN);


void setup() {
    gateway.init();

    robot.add_joint(0, &joint0);
    robot.add_joint(1, &joint1);
    robot.add_joint(2, &joint2);

    robot.init();
}

void loop() {
    gateway.run();
    robot.run();
}
