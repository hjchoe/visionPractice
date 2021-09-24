/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       jameschoe                                                 */
/*    Created:      Wed Sep 22 2021                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// leftMotor            motor         14              
// rightMotor           motor         11              
// Vision5              vision        5               
// ---- END VEXCODE CONFIGURED DEVICES ----

// |-------------------- Library Imports --------------------|

#include "vex.h"
using namespace vex;

// |-------------------- Initialize Global Variables --------------------|

int centerFieldX = 158;          // half the number of pixels of the vision sensor
int bottomFieldY = 190;          // bottom of vision sensor field of view
double speedMultiplierX = 0.35;  // multiplier used when calculating speed based on distance from object on x-axis
double speedMultiplierY = 0.75;  // multiplier used when calculating speed based on distance from object on y-axis

// |-------------------- Function Definitions --------------------|

// find(object): function that returns a boolean of whether the object is in vision
bool find(signature sig) // sig parameter is the object to detect
{
  // take snapshot with vision camera and return number of signature objects in frame
  int objects = Vision5.takeSnapshot(sig);
  if (objects == 0) return false;  // return false if no objects found
  else return true;                // return true if objects in vision
}

// focus(object): function that rotates the robot until the object in vision is centered on the x-axis
void focus(signature sig) // sig parameter is the object to detect
{
  // initialize variables
  int objects = 0;
  int x = 0;
  double speed = 0;
  bool linedUp = false;

  // while loop until robot is lined up with object in vision on the x-axis
  while (!linedUp)
  {
    // take snapshot with vision camera and return number of signature objects in frame
    objects = Vision5.takeSnapshot(sig);
    // store the largest object's x-coordinate
    x = Vision5.largestObject.centerX;

    // if no object in vision
    if (objects == 0)
    {
      // print "no object" to controller
      Controller1.Screen.clearLine();
      Controller1.Screen.print("no object");

      // set motor velocities to 0 (stop motors)
      rightMotor.setVelocity(0, velocityUnits::pct);
      leftMotor.setVelocity(0, velocityUnits::pct);

      // end function
      return;
    }
    // if objects in vision
    else
    {
      // if the object's x-coordinate is to the left of the center of vision
      if (x > centerFieldX + 10)       // 10 is added to the center x value to give a 10 pixel band to the target
      {
        // print "right" direction to controller
        Controller1.Screen.clearLine();
        Controller1.Screen.print("turning right");
        
        // calculate speed by multiplying the speedMultiplier to the distance of object from the center on the x-axis
        speed = speedMultiplierX * (x - centerFieldX);

        // turn off right motor and set left motor velocity to the calculated speed to turn right
        leftMotor.setVelocity(speed, velocityUnits::pct);
        rightMotor.setVelocity(0, velocityUnits::pct);
      }
      // if the object's x-coordinate is to the right of the center of vision
      else if (x < centerFieldX - 10)  // 10 is subtracted to the center x value to give a 10 pixel band to the target
      {
        // print "left" direction to controller
        Controller1.Screen.clearLine();
        Controller1.Screen.print("turning left");
        
        // calculate speed by multiplying the x-speedMultiplier to the distance of object from the center on the x-axis
        speed = speedMultiplierX * (centerFieldX - x);

        // turn off left motor and set right motor velocity to the calculated speed to turn left
        leftMotor.setVelocity(0, velocityUnits::pct);
        rightMotor.setVelocity(speed, velocityUnits::pct);
      }
      // if object is in vision and centered on the x-axis
      else
      {
        // set motor velocities to 0 (stop motors)
        leftMotor.setVelocity(0, velocityUnits::pct);
        rightMotor.setVelocity(0, velocityUnits::pct);

        // end while loop
        linedUp = true;
      }
    }

    // spin both motors at set velocities
    rightMotor.spin(forward);
    leftMotor.spin(forward);

    // while loop delay (50 milliseconds)
    wait(50, msec);
  }
}

// approach(object): function that approaches object in frame until the object is at the bottom of vision
void approach(signature sig) // sig parameter is the object to detect
{
  // Initialize variables
  int objects = 0;
  int y = 0;
  double speed = 0;
  bool linedUp = false;

  // while loop until robot is lined up with bottom of vision on the y-axis
  while (!linedUp)
  {
    // take snapshot with vision camera and return number of signature objects in frame
    objects = Vision5.takeSnapshot(sig);
    // store the largest object's y-coordinate
    y = Vision5.largestObject.centerY;

    // if no object in vision
    if (objects == 0)
    {
      // print "no object" to controller
      Controller1.Screen.clearLine();
      Controller1.Screen.print("no object");

      // set motor velocities to 0 (stop motors)
      rightMotor.setVelocity(0, velocityUnits::pct);
      leftMotor.setVelocity(0, velocityUnits::pct);

      // end function
      return;
    }
    // if objects in vision
    else
    {
      // if the object's y-coordinate is above the bottom of vision
      if (y < bottomFieldY)
      {
        // print "approaching" status to controller
        Controller1.Screen.clearLine();
        Controller1.Screen.print("approaching");

        // calculate speed by multiplying the y-speedMultiplier to the distance of object from the bottom of vision on the y-axis
        speed = speedMultiplierY * (bottomFieldY - y);

        // set both motors to calculated speed to move towards object
        leftMotor.setVelocity(speed, velocityUnits::pct);
        rightMotor.setVelocity(speed, velocityUnits::pct);
      }
      // if the object is at bottom of vision
      else
      {
        // set motor velocities to 0 (stop motors)
        leftMotor.setVelocity(0, velocityUnits::pct);
        rightMotor.setVelocity(0, velocityUnits::pct);

        // end while loop
        linedUp = true;
      }
    }

    // spin both motors at set velocities
    rightMotor.spin(forward);
    leftMotor.spin(forward);

    // while loop delay (50 milliseconds)
    wait(50, msec);
  }
}

// |------------------- Main --------------------|

int main()
{
  // Initializing Robot Configuration
  vexcodeInit();
  
  // setting initial velocity of right motor
  rightMotor.setVelocity(25, velocityUnits::pct);

  // while loop spinning right motor until object appears in frame
  while (!find(Vision5__PINKDICE)) rightMotor.spin(forward);
  
  // stop motor when object found
  rightMotor.stop();

  // once object in frame, the focus function is used to rotate the robot until the object's x-coordinate is centered in its vision
  focus(Vision5__PINKDICE);
  
  // approach function is used to move towards the object until the object's y-coordinate is at the bottom of its vision (close to robot)
  approach(Vision5__PINKDICE);
  
  // for accuracy, the focus function is used again to center the object on the x-axis
  focus(Vision5__PINKDICE);
}