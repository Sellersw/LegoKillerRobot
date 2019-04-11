#pragma config(Sensor, S1,     LightSensor1,   sensorEV3_Color, modeEV3Color_Ambient)
#pragma config(Sensor, S2,     DistanceSensor, sensorSONAR)
#pragma config(Sensor, S3,     Bump,           sensorEV3_Touch)
#pragma config(Motor,  motorA,          RightMotor,    tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,          LeftMotor,     tmotorEV3_Large, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//


// Roachy is a program written for compatibility with the Lego EV3 robot.
//
// It is designed to follow wall on its right side using a provided distance
// sensor and adjust its route by remaining within a range of distance from
// the wall. When it is in darkness, it will halt its movement.
//
// Written by Scott Harrington and Patrick Sellers

bool LightSensorBool= false;
bool TouchSensorBool= false;
bool DistanceSensorBool=false;
bool SoundSensorBool=false;

// This is a task that will continuously check the light sensor to see if
// it is below a certain value (< 10). When it is below that threshold it
// returns true.
task LightSensorTask()
{
	while(true){
		if(SensorValue[LightSensor1] < 10)
		{
			LightSensorBool = true;
		}
		else{
			LightSensorBool =false;

		}
		releaseCPU();
	}
}

// This task checks to see if the touch sensor is activated, resulting
// in a value of zero. This indicates to the robot that the front bumper
// has been triggered, and it returns true.
task TouchSensorTask()
{
	while(true){
		if(SensorValue[Bump] < 1)
		{
			TouchSensorBool=true;
		}
		else{
			TouchSensorBool=false;
		}
		releaseCPU();
	}
}

// This task checks to see if the distance from an object is either
// over 20 or less than 10, which gives us range where it returns false.
task DistanceSensorTask()
{
	while(true){
		if(SensorValue[DistanceSensor] > 20 || SensorValue[DistanceSensor] < 10)
		{
			DistanceSensorBool=true;
		}
		else{
			DistanceSensorBool=false;

		}
		releaseCPU();
	}
}

// This is a method that syncs the motors to move in a clock-wise
// rotation to reverse the movement of the robot and go for a given
// time.
void reverse (int duration, int motorPower)
{
	setMotorSyncTime(RightMotor, LeftMotor , 0, duration, motorPower);
	wait1Msec(duration);
}

// This method adjusts the robot to the left approximately 90 degrees.
void rotateLeft()
{
	setMotorSyncEncoder(RightMotor, LeftMotor, -100, 500, 25);
	waitUntilMotorStop(LeftMotor);

}

// This method adjusts the robot to the right approximately 90 degrees.
void rotateRight()
{
	setMotorSyncEncoder(RightMotor, LeftMotor, 100, 500, 25);
	waitUntilMotorStop(RightMotor);
}

// This is the method that takes an action based on an input string.
void TheMove(string action)
{

	// If the string is forward then the robot will move forward for a
	// period of time (100 milliseconds).
	if(action== "Foward")
	{
		setMotorSyncTime(RightMotor, LeftMotor, 0, 100, 100);
		wait1Msec(100);
	}

	// If the touch sensor is activated, the robot will perform a
	// reverse and a 90 degree right rotation.
	if(action== "Wall")
	{
		reverse(700, -50);
		rotateRight();
	}

	// if the robot cannot "see" anything with the distance sensor,
	// it reads it as the wall continuing to the right. Therefore,
	// this sequence arcs the robot around to where the wall continues
	// on the right side.
	if(action== "EmptyRightTurn")
	{
		setMotorSyncTime(RightMotor, LeftMotor, 0, 1500, 25);
		waitUntilMotorStop(RightMotor);
		setMotorSyncEncoder(RightMotor, LeftMotor, -40 , 1300, 25);
		waitUntilMotorStop(RightMotor);
		setMotorSyncTime(RightMotor, LeftMotor, 0, 1500, 25);
		waitUntilMotorStop(RightMotor);
	}

	// If the robot is too far away from the wall, it will perform a
	// correction to the right so that is in within the distance range.
	if(action== "RightTurn")
	{
		setMotorSync(RightMotor, LeftMotor, -100, 25);
		wait1Msec(100);
		setMotorSyncEncoder(RightMotor, LeftMotor, 0, 5, 100);
		waitUntilMotorStop(RightMotor);
	}

	// If the robot is too close to the wall, it will perform a correction
	// to the left so that it will not run into the wall.
	if(action== "LeftTurn")
	{
		setMotorSync(RightMotor, LeftMotor, 100, 25);
		wait1Msec(100);
		setMotorSyncEncoder(RightMotor, LeftMotor, 0, 5, 100);
		waitUntilMotorStop(RightMotor);
	}

	// If the string is reverserightturn, the robot will reverse for
	// 700 milliseconds and turn right 90 degrees.
	if(action== "ReverseRightTurn")
	{
		reverse(700, -50);
		rotateRight();
	}

	// If the robot is told to do nothing, then this method executes no code.
	if(action=="Nothing")
	{
	}
}

// This is the method that actually runs the robot by starting task loops
// and making an appropriate decision based on the state of the sensors.
task main()
{
	string WhatToDo = "Forward";
	startTask (LightSensorTask);
	startTask (TouchSensorTask);
	startTask (DistanceSensorTask);

	while(true)
	{
		// The robot will go forward if the distance sensor detects
		// that it is within a given range, else it will check distances
		// to see if it should correct itself to the right or left.
		WhatToDo= "Foward";
		if(DistanceSensorBool)
		{
			if(SensorValue[DistanceSensor] <10)
			{
				WhatToDo= "LeftTurn";
			}
			else if (SensorValue[DistanceSensor] <30)
			{
				WhatToDo= "RightTurn";
			}
			else
			{
			// If the distance sensor is outside the typical range, it will assume
			// that the wall turns right and it will move appropriately.
				WhatToDo= "EmptyRightTurn";
			}
		}

		// If the bumper is engaged, it will overwrite the instruction
		// so that the robot will execute a reverse.
		if(TouchSensorBool)
		{
			WhatToDo="Wall";
		}
		if(TouchSensorBool & DistanceSensorBool)
		{
			WhatToDo="ReverseRightTurn";
		}

		// If the light sensor is triggered, it will not do anything.
		if(LightSensorBool)
		{
			WhatToDo="Nothing";
		}

		// This call executes the move.
		TheMove(WhatToDo);

	}



}
