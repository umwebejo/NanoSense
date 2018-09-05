#pragma once
/*



\author    Stephen Barrass
\version   0.0

*/
#include "chai3d.h"

using namespace chai3d;
using namespace std;

class NanoSound
{
public:
	NanoSound();// constructor
	~NanoSound();// destructor

	void audioOn();
	void audioOff();
	void audioToggle();

	// audio mappings
	int audioSetup();
	void setAudioMapping(int m);
	void changeAudioMapping();
	double audioMapping(double z);
	void sonifyForce(double f);

	cAudioDevice* getAudioDevice();

private:
	cAudioDevice* audioDeviceP;	// create Audio Device
	cAudioBuffer* audioBufferP;	// audio buffer to store sound
	cAudioSource* audioSourceP;	// audio source

	bool audioFlag;	// turn audio on and off
	int aMapping;	// select audio mapping

	double fLinear(double z); // linear
	double fExp(double z);	// exponential
};

