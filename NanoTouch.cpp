/*



\author    Stephen Barrass
\version   0.0

*/
#include <math.h>
#include "NanoTouch.h"
#include "NanoSound.h"


NanoTouch::NanoTouch()
{
	hapticFlag = true;
	hMapping = 0;
}

NanoTouch::~NanoTouch()
{

}

/*------------------------------------
haptic mapping
--------------------------------------*/

void NanoTouch::hapticOn()
{
	hapticFlag = true;
}

void NanoTouch::hapticOff()
{
	hapticFlag = false;
}
/*------------------------------------
z force to sound mappings
--------------------------------------*/
double NanoTouch::fLinear(double f)
{
	if (f < 0) return 0;

	return f;
}

double NanoTouch::fExp(double f)
{
	if (f < 0) return 0;
	return(exp(f) - 1);
}


void NanoTouch::setHapticMapping(int m)
{
	if (m > 1) m = 0;
	hMapping = m;
}

void NanoTouch::changeHapticMapping()
{
	setHapticMapping(++hMapping);
}

double NanoTouch::hapticMapping(double z)
{
	switch (hMapping)
	{
	case 0:	return fLinear(z);
	case 1:	return fExp(z);
	default: return fLinear(z);
	}
}

