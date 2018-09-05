#pragma once
/*



\author    Stephen Barrass
\version   0.0

*/
#include "chai3d.h"

using namespace chai3d;
using namespace std;

class NanoTouch
{
public:
	NanoTouch();
	~NanoTouch();

	// turn haptics and audio on and off
	void hapticOn();
	void hapticOff();
	// haptic mappings
	void setHapticMapping(int m);
	void changeHapticMapping();
	double hapticMapping(double z);

private:


	bool hapticFlag;	// turn haptics on and off
	int hMapping;	// select haptic mapping

	double fLinear(double z); // linear
	double fExp(double z);	// exponential
};

