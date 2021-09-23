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

// |---------- Library Imports ----------|
#include "vex.h"
using namespace vex;

// |---------- Initializing Global Variables ----------|
int centerFieldX = 158;         // this is half the number of pixels of the vision sensor
int bottomFieldY = 190;         // bottom of vision sensor field of view
double speedMultiplier = 0.35;  // multiplier used when calculating speed based on distance from object

int32_t ObjectCount = 0;
bool LinedUp = false;

// |---------- Functions ----------|
bool find(signature sig)
{
  int objects = Vision5.takeSnapshot(sig);
  if (objects != 0) return true;
  else return false;
}

void focus(signature sig)
{
  LinedUp = false;
  while (!LinedUp)
  {
    // Takes snapshot with vision camera and returns number of signature objects in frame
    int objects = Vision5.takeSnapshot(sig);

    if (objects != 0)
    {
      if (Vision5.largestObject.centerX > CenterFieldX+10)
      {
        // turn right
        Controller1.Screen.clearLine();
        Controller1.Screen.print("right");
        double speed = speedMultiplier*(Vision5.largestObject.centerX-CenterFieldX);
        leftMotor.setVelocity(speed, velocityUnits::pct);
        rightMotor.setVelocity(0, velocityUnits::pct);
      }
      else if (Vision5.largestObject.centerX < CenterFieldX-10)
      {
        // turn left
        Controller1.Screen.clearLine();
        Controller1.Screen.print("left");
        double speed = speedMultiplier*(CenterFieldX-Vision5.largestObject.centerX);
        leftMotor.setVelocity(0, velocityUnits::pct);
        rightMotor.setVelocity(speed, velocityUnits::pct);
      }
      else
      {
        leftMotor.setVelocity(0, velocityUnits::pct);
        rightMotor.setVelocity(0, velocityUnits::pct);
        LinedUp = true;
        return;
      }
      // brief delay to keep the loop from looping too fast
      rightMotor.spin(forward);
      leftMotor.spin(forward);
    }
    else
    {
      Controller1.Screen.clearLine();
      Controller1.Screen.print("no object");
      rightMotor.setVelocity(0, velocityUnits::pct);
      leftMotor.setVelocity(0, velocityUnits::pct);
    }
    wait(50, msec);
  }
}

void approach(signature sig)
{
  LinedUp = false;
  while (!LinedUp)
  {
    int objects = Vision5.takeSnapshot(sig);

    if (objects != 0)
    {
      if (Vision5.largestObject.centerY < BottomFieldY)
      {
        // turn right
        Controller1.Screen.clearLine();
        Controller1.Screen.print("approaching");
        double speed = 0.75*(BottomFieldY-Vision5.largestObject.centerY);
        leftMotor.setVelocity(speed, velocityUnits::pct);
        rightMotor.setVelocity(speed, velocityUnits::pct);
      }
      else
      {
        Controller1.Screen.clearLine();
        Controller1.Screen.print("no object");
        leftMotor.setVelocity(0, velocityUnits::pct);
        rightMotor.setVelocity(0, velocityUnits::pct);
        LinedUp = true;
        return;
      }
      // brief delay to keep the loop from looping too fast
      rightMotor.spin(forward);
      leftMotor.spin(forward);
    }
    else
    {
      Controller1.Screen.clearLine();
      Controller1.Screen.print("no object");
      rightMotor.setVelocity(0, velocityUnits::pct);
      leftMotor.setVelocity(0, velocityUnits::pct);
    }
    wait(50, msec);
  }
}

// |---------- Main ----------|
int main() {
  // Initializing Robot Configuration
  vexcodeInit();
  
  // setting initial velocity of right motor
  rightMotor.setVelocity(25, velocityUnits::pct);

  // while loop spinning right motor until object appears in frame
  while (!find(Vision5__PINKDICE)) rightMotor.spin(forward);

  // once object in frame, the focus function is used to rotate the robot until the object's x-coordinate is centered in its vision
  focus(Vision5__PINKDICE);
  
  // approach function is used to move towards the object until the object's y-coordinate is at the bottom of its vision (close to robot)
  approach(Vision5__PINKDICE);
  
  // for accuracy, the focus function is used again to center the object on the x-axis
  focus(Vision5__PINKDICE);
}
