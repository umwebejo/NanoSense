#pragma once
/*



\author    Stephen Barrass
\version   0.0

*/
#include "chai3d.h"
#include "NanoSound.h"
#include "NanoTouch.h"

using namespace chai3d;
using namespace std;

class NanoSense
{
public:
	NanoSense();// constructor
	~NanoSense();
	// has a
	NanoSound nanoSound;
	NanoTouch nanoTouch;

	void zOn();
	void zOff();
	void zToggle();

	cHapticDeviceInfo info;	// haptic device info
	bool button0, button1; // buttons

	void setForceFunction(int f);// set the force function 0,,1,2....
	void changeForceFunction();
	double forceFunction(double z);
	double getForceNow();
	cVector3d getDiffPosition();

	void update(void);	// the main haptics audio loop

private:
	bool zFlag;
	double fNow;	// force from z function
	int zFunction;	// select force function

	// various z functions
	double zLinear(double z); // linear
	double zExp(double z);	// exponential
	double zModelAFM(double z);// simple model of AFM data profile

							   // haptic thread
	cVector3d position;
	cVector3d diffPosition;
	cVector3d desiredPosition;

	// a haptic device handler
	cHapticDeviceHandler* handler;

	// a pointer to the current haptic device
	cGenericHapticDevicePtr hapticDevice;
};

