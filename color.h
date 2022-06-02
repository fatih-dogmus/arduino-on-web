#ifndef COLOR_H

#define COLOR_H

#include "Arduino.h"

class Color{
	private:
		int minVal, maxVal;
	public:
		Color();
        String toHex(float val);
};

#endif
