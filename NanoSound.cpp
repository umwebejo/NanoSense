/*



\author    Stephen Barrass
\version   0.0

*/
#include <math.h>
#include "NanoSound.h"

// root resource path to find sound files
string resourceRootAudio;
// convert to resource path
#define RESOURCE_PATH(p)    (char*)((resourceRootAudio+string(p)).c_str())


NanoSound::NanoSound()
{
	audioFlag = true;	// audio default
	aMapping = 0;		// select audio mapping
	audioSetup();
}


NanoSound::~NanoSound()
{
	// clean up
}


/*------------------------------------
audio mapping
--------------------------------------*/

void NanoSound::audioOn()
{
	audioFlag = true;
	audioSourceP->play();
}

void NanoSound::audioOff()
{
	audioFlag = false;
	audioSourceP->stop();
}

void NanoSound::audioToggle()
{
	audioFlag = !audioFlag;
	if (audioFlag == true)
		audioSourceP->play();
	else
		audioSourceP->stop();
}

void NanoSound::setAudioMapping(int m)
{
	if (m > 1) m = 0;// currently 2 mappings
	aMapping = m;
}

void NanoSound::changeAudioMapping()
{
	setAudioMapping(++aMapping);
}

/*------------------------------------
z force to sound mappings
--------------------------------------*/
double NanoSound::fLinear(double f)
{
	if (f < 0) return 0;

	return f;
}

double NanoSound::fExp(double f)
{
	if (f < 0) return 0;
	return(exp(f) - 1);
}

double NanoSound::audioMapping(double z)
{
	switch (aMapping)
	{
	case 0:	return fLinear(z);
	case 1:	return fExp(z);
	default: return fLinear(z);
	}
}

cAudioDevice* NanoSound::getAudioDevice()
{
	return audioDeviceP;
}

int NanoSound::audioSetup()
{
	audioDeviceP = new cAudioDevice();
	audioBufferP = new cAudioBuffer();

	bool fileload = audioBufferP->loadFromFile(RESOURCE_PATH("drill.wav"));
/*
	bool fileload = audioBufferP->loadFromFile(RESOURCE_PATH("../resources/sounds/drill.wav"));

	if (!fileload)
	{
#if defined(_MSVC)
		fileload = audioBufferP->loadFromFile("../../../bin/resources/sounds/drill.wav");
#endif
	}
*/
	if (!fileload)
	{
		cout << "Error - Sound file failed to load or initialize correctly." << endl;
		return (-1);
	}

	// create audio source
	audioSourceP = new cAudioSource();

	// assign audio buffer to audio source
	audioSourceP->setAudioBuffer(audioBufferP);

	// loop playing of sound
	audioSourceP->setLoop(true);

	// turn off sound for now
	audioSourceP->setGain(0.0);

	// set pitch
	audioSourceP->setPitch(0.0);

	// play sound
	audioSourceP->play();
	return 0;
}


void NanoSound::sonifyForce(double f)
{
	if (audioFlag == false) return;

	double audioForceMap = audioMapping(f);
	if (audioForceMap < 0) audioForceMap = 0;
	audioSourceP->setGain(audioForceMap);
	audioSourceP->setPitch(audioForceMap*0.1);
}
