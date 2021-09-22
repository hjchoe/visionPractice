using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern controller Controller1;
extern motor leftMotor;
extern motor rightMotor;
extern signature Vision5__PINKDICE;
extern signature Vision5__SIG_2;
extern signature Vision5__SIG_3;
extern signature Vision5__SIG_4;
extern signature Vision5__SIG_5;
extern signature Vision5__SIG_6;
extern signature Vision5__SIG_7;
extern vision Vision5;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );