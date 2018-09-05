/*



\author    Stephen Barrass
\version   0.0

*/
#include <math.h>
#include "chai3d.h"
#include "NanoSense.h"

NanoSense::NanoSense()
{
	zFlag = true;
	zFunction = 0;		// select z to force function
	fNow = 0;			// current force from z function mapping
	button0 = false;
	button1 = false;
	diffPosition.set(0.0, 0.0, 0.0);
	desiredPosition.set(0.0, 0.0, 0.1);

	// create a haptic device handler
	handler = new cHapticDeviceHandler();

	// get a handle to the first haptic device
	handler->getDevice(hapticDevice, 0);

	// open a connection to haptic device
	hapticDevice->open();

	// calibrate device (if necessary)
	hapticDevice->calibrate();

	// retrieve information about the current haptic device
	cHapticDeviceInfo info = hapticDevice->getSpecifications();
}


NanoSense::~NanoSense()
{
	// clean up
	// close haptic device
	hapticDevice->close();
	delete handler;
}

void NanoSense::zOn()
{
	zFlag = true;
}

void NanoSense::zOff()
{
	zFlag = false;
}

void NanoSense::zToggle()
{
	zFlag = !zFlag;
}

/*------------------------------------
z axis force functions
--------------------------------------*/
double NanoSense::zLinear(double z)
{
	if (z < 0) return 0;

	return 10*z;
}

double NanoSense::zExp(double z)
{
	if (z < 0) return 0;
	return(10*(exp(z) - 1));
}

double NanoSense::zModelAFM(double z)
{
	if (z < 0) return 0;
	else if (z < 0.01) return 50 * -z;
	else if (z < 0.02) return 25 * -z;
	else if (z < 0.025) return 5 * -z;
	else if (z < 0.03) return 0;
	else if (z < 0.07) return 10 * z;
	else if (z < 0.11) return 15 * z;
	return 20 * z;
}



void NanoSense::setForceFunction(int f)
{
	if (f > 2) f = 0;
	zFunction = f;
}

void NanoSense::changeForceFunction()
{
	setForceFunction(++zFunction);
}

double NanoSense::forceFunction(double z)
{
	switch (zFunction)
	{
		case 0:	fNow = zLinear(z); break;
		case 1:	fNow = zExp(z); break;
		case 2:	fNow = zModelAFM(z); break;
		default: fNow = zLinear(z); break;
	}
	return fNow;
}

double NanoSense::getForceNow()
{
	return fNow;
}

cVector3d NanoSense::getDiffPosition()
{
	return diffPosition;
}

void NanoSense::update(void)
{
	/////////////////////////////////////////////////////////////////////
	// READ HAPTIC DEVICE
	/////////////////////////////////////////////////////////////////////

	// read position 
	hapticDevice->getPosition(position);

	// read user-switch status (button 0)
	hapticDevice->getUserSwitch(0, button0);
	hapticDevice->getUserSwitch(1, button1);


	/////////////////////////////////////////////////////////////////////
	// COMPUTE AND APPLY FORCES
	/////////////////////////////////////////////////////////////////////

	cVector3d force(0, 0, 0);

	diffPosition = desiredPosition - position;
	forceFunction(diffPosition.z()); // map the z position through a Force Function

	if (zFlag)
	{
		double antiG = 0.65; // anti gravity on stylus
		double zForce = getForceNow();
		// constrain force feedback to the z axis
		double Kp = 300;
		cVector3d forceField(Kp*diffPosition.x(), Kp*diffPosition.y(), zForce + antiG);

		force.add(forceField);
	}

	nanoSound.sonifyForce(getForceNow());	// sonify the force throiugh the Force to Audio mapping 

	// send computed force, torque, and gripper force to haptic device
	//      hapticDevice->setForceAndTorqueAndGripperForce(force, torque, gripperForce);
	hapticDevice->setForce(force);

}


