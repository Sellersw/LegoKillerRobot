#pragma config(Sensor, S1,     Wall,           sensorEV3_Ultrasonic)
#pragma config(Sensor, S2,     Bumper,         sensorEV3_Touch)
#pragma config(Sensor, S3,     Light,          sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4,     Ball,           sensorSONAR)
#pragma config(Motor,  motorA,          RightMotor,    tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          Grabber,       tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,          LeftMotor,     tmotorEV3_Large, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// wall sensor globals
bool wallBool= false;
bool distanceBool= false;

bool celebrate= false;

//bumper sensor globals
bool bumpberBool= false;

//color sensor globals
bool colorBool= false;
bool colorRight= false;

bool FOUNDBALL = false;
bool empty = true;


const int wallConst= 10;
const int distanceConst= 50;

string goDo = "foward";

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

task wallSensor()
{
	while(true){
		if(SensorValue[Wall] < wallConst)
		{
			wallBool = true;

		}
		else
			{
			wallBool = false;


		}

	}
	releaseCPU();
}
task BallSensor()
{
	while(true){

		if(SensorValue[Ball] < distanceConst)
		{
			FOUNDBALL= true;
		}
		else
		{
			FOUNDBALL = false;
		}
		releaseCPU();

	}


		releaseCPU();
}
task bumperSensor()
{
		while(true){
			if(SensorValue[Bumper] < 1)
		{
			bumpberBool=true;
		}
		else{
			bumpberBool=false;
		}
		releaseCPU();
	}
}
task lightSensor()
{
	while(true)
	{
		if(getColorName(Light)== colorRed)
		{
			colorBool = false;
			colorRight= true;
			;

		}
		else if(getColorName(Light)== colorBlue)
		{
			colorBool=true;
			colorRight= false;
		}
		else
	  {
			colorBool=false;
			colorRight= false;
	  }
		releaseCPU();
	}
}
void undoGrabber()
{
		Motor[Grabber]=10;

		wait1Msec(5000);
		Motor[Grabber]=0;
}
void gogogo(string action)
{
	if(action == "turn" )
	{
		rotateRight();
		waitUntilMotorStop(RightMotor);
	}
	if(action == "walk")
	{
		setMotorSyncTime(RightMotor, LeftMotor, 0, 100, 100)
		waitUntilMotorStop(LeftMotor);
	}
	if(action == "followfar")
	{

		setMotorSyncEncoder(RightMotor, LeftMotor, -80, 30, 100);
		waitUntilMotorStop(LeftMotor);
		setMotorSyncTime(RightMotor,LeftMotor, 0, 100, 100);
		wait1Msec(100);
	}
	if(action == "follownear")
	{
		setMotorSyncEncoder(RightMotor, LeftMotor, 80, 30, 100);
		waitUntilMotorStop(RightMotor);
		setMotorSyncTime(RightMotor,LeftMotor, 0, 100, 100);
		wait1Msec(100);
	}
	if(action == "reverse")
	{
		reverse(700, -50);
		rotateRight();
	}
	if(action == "wrongball" )
	{

		setMotorSyncTime(RightMotor, LeftMotor, 0, 2000, -100)
		waitUntilMotorStop(RightMotor);
	}
	if(action == "grab" )
	{

		Motor[Grabber]=-10;

		wait1Msec(5000);
		motor[Grabber]=0;
		empty = false;
		celebrate= true;

	}
	if(action == "celebrate" )
	{
		setMotorSyncTime(RightMotor, LeftMotor, 0, 3000, 100);
		playTone(soundBlip);
		waitUntilMotorStop(RightMotor);
	}
}
task main()
{
	startTask (BallSensor);
	startTask (wallSensor);
	startTask (bumperSensor);
	startTask (lightSensor );


		undoGrabber();
		while(true)
		{
			while(true)
				{
					goDo = "walk";
					if(wallBool)
					{
						goDo = "follownear";
					}
					if(bumpberBool)
					{
						goDo = "reverse";

					}
					if(colorBool & empty)
					{
						goDo = "wrongball";
					}
					if(colorRight & empty)
					{
						goDo = "grab";
					}
					if(FOUNDBALL)
					{
						goDo= "turn";
					}
					if(celebrate)
					{
						goDo="celebrate";
					}
					gogogo(goDo);
				}




		}


}
