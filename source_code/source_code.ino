/*
 * Robotic Arm Control System
 * 
 * This program controls a 5-servo robotic arm that performs an automated
 * pick-and-place sequence. The arm moves through predefined positions to
 * simulate grabbing an object, lifting it, and placing it in a new location.
 * 
 * Hardware Configuration:
 * - Base servo (pin 9): Rotates the entire arm horizontally
 * - Link servo (pin 10): Controls the main arm segment (shoulder joint)
 * - Stamp servo (pin 11): Controls the forearm/wrist positioning
 * - Gripper servo (pin 5): Opens/closes the gripper mechanism
 * - Gripper_2 servo (pin 6): Secondary gripper control for fine manipulation
 */

#include <Servo.h>

// Servo motor objects for each joint of the robotic arm
Servo base;      // Base rotation servo (horizontal movement)
Servo link;      // Main arm link servo (vertical movement)
Servo stamp;     // Stamp/wrist servo (end effector positioning)
Servo gripper;   // Primary gripper servo (main grab mechanism)
Servo gripper_2; // Secondary gripper servo (fine grip control)

// Position variables to track current servo angles (in degrees)
int pos_base = 0;        // Base starting position (0° = centered)
int pos_link = 90;       // Link starting position (90° = horizontal)
int pos_stamp = 0;       // Stamp starting position (0° = neutral)
int pos_gripper = 90;    // Gripper starting position (90° = partially open)
int pos_gripper_2 = 10;  // Secondary gripper starting position (10° = nearly closed)

void setup() {
    // Attach each servo to its corresponding digital pin
    base.attach(9);      // Base rotation servo on pin 9
    link.attach(10);     // Main arm servo on pin 10
    stamp.attach(11);    // Stamp/wrist servo on pin 11
    gripper.attach(5);   // Primary gripper on pin 5
    gripper_2.attach(6); // Secondary gripper on pin 6
    
    // Initialize all servos to their starting positions
    base.write(pos_base);           // Set base to center position
    link.write(pos_link);           // Set link to horizontal position
    stamp.write(pos_stamp);         // Set stamp to neutral position
    gripper.write(pos_gripper);     // Set gripper to partially open
    gripper_2.write(pos_gripper_2); // Set secondary gripper to nearly closed
    
    // Initialize serial communication for debugging output
    Serial.begin(9600);
}

void loop() {
    // === PHASE 1: APPROACH TARGET ===
    // Rotate base from 0° to 90° (quarter turn to face target)
    for (pos_base; pos_base <= 90; pos_base += 1) {
        base.write(pos_base);
        delay(15); // Smooth movement with 15ms delay between steps
    }
    
    // Extend main arm link from 90° to 120° (reach toward target)
    for (pos_link; pos_link <= 120; pos_link += 1) {
        link.write(pos_link);
        delay(15); // Gradual extension for precise positioning
    }
    
    // Lower stamp/wrist from 0° to 80° (position for grabbing)
    for (pos_stamp; pos_stamp <= 80; pos_stamp += 1) {
        stamp.write(pos_stamp);
        delay(15);
        Serial.println(pos_stamp); // Debug output to monitor stamp position
    }
    
    // === PHASE 2: GRAB OBJECT ===
    // Open primary gripper from 90° to 115° (prepare to grab)
    for (pos_gripper; pos_gripper <= 115; pos_gripper += 1) {
        gripper.write(pos_gripper);
        delay(15);
    }
    
    // Adjust secondary gripper from 10° to 40° (secure grip positioning)
    for (pos_gripper_2; pos_gripper_2 <= 40; pos_gripper_2 += 1) {
        gripper_2.write(pos_gripper_2);
        delay(40); // Slower movement for precise grip adjustment
    }
    
    // Close primary gripper from 115° to 70° (secure the object)
    for (pos_gripper; pos_gripper >= 70; pos_gripper -= 1) {
        gripper.write(pos_gripper);
        delay(15);
        // Serial.println(pos_link); // Commented debug line
    }
    
    // Adjust secondary gripper back to 10° (finalize grip)
    for (pos_gripper_2; pos_gripper_2 >= 10; pos_gripper_2 -= 1) {
        gripper_2.write(pos_gripper_2);
        delay(40); // Slow, controlled movement for secure grip
    }
    
    // === PHASE 3: LIFT AND TRANSPORT ===
    // Raise stamp/wrist from 80° to 45° (lift object)
    for (pos_stamp; pos_stamp >= 45; pos_stamp -= 1) {
        stamp.write(pos_stamp);
        delay(15);
        Serial.println(pos_stamp); // Monitor lifting progress
    }
    
    // Retract main arm from 120° to 90° (pull object closer)
    for (pos_link; pos_link >= 90; pos_link -= 1) {
        link.write(pos_link);
        delay(15);
    }
    
    // Rotate base back from 90° to 0° (return to starting position)
    for (pos_base; pos_base >= 0; pos_base -= 1) {
        base.write(pos_base);
        delay(15);
    }
    
    // === PHASE 4: RELEASE OBJECT ===
    // Open secondary gripper from 10° to 40° (prepare to release)
    for (pos_gripper_2; pos_gripper_2 <= 40; pos_gripper_2 += 1) {
        gripper_2.write(pos_gripper_2);
        delay(40); // Controlled opening for gentle release
    }
    
    // Note: Primary gripper remains closed at 70° - object should be released
    // by secondary gripper movement. The cycle then repeats automatically.
}