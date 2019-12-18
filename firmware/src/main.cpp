#include <Arduino.h>
#include "Debugger.h"
#include "Config.h"
#include "SerialTransport.h"
#include "Gateway.h"
#include "ServoJoint.h"
#include "StepperJoint.h"
#include "Robot.h"
#include "ServoEndEffector.h"
#include "SuctionEndEffector.h"

SerialTransport trans(MAIN_SERIAL, BAUDRATE);
Gateway gateway(&trans);
Robot robot;

StepperJoint joint0 = StepperJoint(JOINT0_ENA_PIN, JOINT0_DIR_PIN, JOINT0_STP_PIN, JOINT0_LOCK_PIN, JOINT0_MAX);
StepperJoint joint1 = StepperJoint(JOINT1_ENA_PIN, JOINT1_DIR_PIN, JOINT1_STP_PIN, JOINT1_LOCK_PIN, JOINT1_MAX);
StepperJoint joint2 = StepperJoint(JOINT2_ENA_PIN, JOINT2_DIR_PIN, JOINT2_STP_PIN, JOINT2_LOCK_PIN, JOINT2_MAX);

#if END_EFFECTOR_TYPE == SERVO
ServoEndEffector effector = ServoEndEffector(SERVO_PIN, 180);
#elif END_EFFECTOR_TYPE == SUNCTION
SuctionEndEffector effector = SuctionEndEffector(RELAY_PIN);
#endif

void setup() {
    gateway.init();

    robot.add_joint(0, &joint0);
    robot.add_joint(1, &joint1);
    robot.add_joint(2, &joint2);

#if JOINT0_REVERSE
    joint0.set_reverse(1);
#endif
#if JOINT1_REVERSE
    joint1.set_reverse(1);
#endif
#if JOINT2_REVERSE
    joint2.set_reverse(1);
#endif

    robot.add_end_effector(&effector);

    robot.init();
}

void loop() {
    gateway.run();
    robot.run();
}
