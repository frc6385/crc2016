#include <iostream>
#include <thread>
#include "WPILib.h"
#include <math.h>
using namespace std;

const double __SPEEDCONSTANT=1.00;

/*
MOTORS DEFINITION

      FORWARD
==================
|3              0|
|2              1|
==================
     BACKWARD
*/

class Robot : public IterativeRobot {
	Joystick stick;
	Talon motor0,motor1,motor2,motor3;
	Talon roller;
	int counter;
	double x,y,buttonLT,buttonRT;
	bool buttonLB,buttonRB;
	bool buttonA,buttonB,buttonX,buttonY;

public:
	Robot():
			stick(1),
			motor0(0),
			motor1(1),
			motor2(2),
			motor3(3),
			roller(4)
	{
	}

	//general functions in the class.
	void resetMotors(){
		motor0.Set(0);
		motor1.Set(0);
		motor2.Set(0);
		motor3.Set(0);
	}

	void controlMainMotors(double val){
		double speed=val*__SPEEDCONSTANT;
		motor0.Set(speed);
		motor2.Set(-speed);
	}

	void controlSlaveMotors(double val){
		double speed=val*__SPEEDCONSTANT;
		if(val==0){
			motor1.Set(0);
			motor3.Set(0);
		}else if(val<0){
			motor1.Set(speed);
		}else{
			motor3.Set(speed);
		}
	}

	void turnRounds(double val){
		double speed=val*__SPEEDCONSTANT;
		motor0.Set(speed);
		motor2.Set(speed);
	}

	//multi threads functions
	void TeleopPeriodic_Move(){
		if(!(buttonB)){
			if((buttonLT>0.02) || (buttonRT>0.02)){
				printf("turn rounds\n");
				if(buttonLT>=buttonRT){
					turnRounds(-buttonLT);
				}else{
					turnRounds(buttonRT);
				}
			}else if((fabs(y)>0.02) || (fabs(x)>0.02)){
				if(fabs(y)>0.02){
					controlMainMotors(y);
				}else{
					controlMainMotors(0);
				}
				if(fabs(x)>0.02){
					controlSlaveMotors(x);
				}else{
					controlSlaveMotors(0);
				}
			}else{
				resetMotors();
			}
		}else{
			printf("Emergency STOP!\n");
			resetMotors();
		}
	}

	void TeleopPeriodic_Roll(){
		if(buttonLB || buttonRB){
			if(buttonLB){
				roller.Set(0.8);
			}else if(buttonRB){
				roller.Set(-0.8);
			}else{
				roller.Set(0);
			}
		}else{
			roller.Set(0);
		}
	}

	//driver station functions
	void RobotInit(){
		printf("Robot Loaded!\n");
	}

	void TeleopInit(){
		counter=0;
		printf("Teleop Mode Entered!\n");
	}

	void TeleopPeriodic(){
		//counter++;
		x=stick.GetRawAxis(0);
		y=stick.GetRawAxis(1);
		buttonLT=stick.GetRawAxis(2);
		buttonRT=stick.GetRawAxis(3);
		buttonLB=stick.GetRawButton(5);
		buttonRB=stick.GetRawButton(6);
		buttonA=stick.GetRawButton(1);
		buttonB=stick.GetRawButton(2);
		buttonX=stick.GetRawButton(3);
		buttonY=stick.GetRawButton(4);

		//init threads
		Robot::TeleopPeriodic_Move();
		Robot::TeleopPeriodic_Roll();

		//call threads

		//printf("Teleop Mode Period: %d\n",counter);
	}

	void AutonomousInit(){
		counter=0;
		printf("Autonomous Mode Entered!\n");
	}

	void AutonomousPeriodic(){
		motor0.Set(0.233);
		motor1.Set(0.233);
		motor2.Set(0.3);
		motor3.Set(0.3);
	}

	void TestInit(){
		printf("Test Mode Entered!\n");
	}

	void TestPeriodic(){

	}

	void DisabledInit(){
		printf("Robot Disabled!\n");
		resetMotors();
	}

	void DisabledPeriodic(){

	}
};

START_ROBOT_CLASS(Robot)
