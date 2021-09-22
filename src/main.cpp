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

#include "vex.h"

using namespace vex;

int32_t ObjectCount = 0;
int CenterFieldX = 158; // this is half the number of pixels of the vision sensor
int BottomFieldY = 190; // bottom of vision sensor field of view
bool LinedUp = false;
double autonomousSpeed = 0.35;

bool find(signature sig)
{
  int objects = Vision5.takeSnapshot(sig);
  if (objects != 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void focus(signature sig)
{
  LinedUp = false;
  while (!LinedUp)
  {
    int objects = Vision5.takeSnapshot(sig);

    /*Controller1.Screen.clearLine();
    Controller1.Screen.print(Vision5.largestObject.centerX);*/

    if (objects != 0)
    {
      if (Vision5.largestObject.centerX > CenterFieldX+10)
      {
        // turn right
        Controller1.Screen.clearLine();
        Controller1.Screen.print("right");
        double speed = autonomousSpeed*(Vision5.largestObject.centerX-CenterFieldX);
        leftMotor.setVelocity(speed, velocityUnits::pct);
        rightMotor.setVelocity(0, velocityUnits::pct);
      }
      else if (Vision5.largestObject.centerX < CenterFieldX-10)
      {
        // turn left
        Controller1.Screen.clearLine();
        Controller1.Screen.print("left");
        double speed = autonomousSpeed*(CenterFieldX-Vision5.largestObject.centerX);
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

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  rightMotor.setVelocity(25, velocityUnits::pct);

  while (!find(Vision5__PINKDICE))
  {
    rightMotor.spin(forward);
  }

  focus(Vision5__PINKDICE);
  approach(Vision5__PINKDICE);
  focus(Vision5__PINKDICE);
  
}
