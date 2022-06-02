#include "color.h"

Color::Color(){
	minVal = 34;
	maxVal = 255;
}

String Color::toHex(float val){
    val += 0.77F; // (5+0.77)*(255-34)/5.0 = 255 | 0.77*(255-34)/5.0 = 34 yani 0x22
    int color = (int)(val*(maxVal-minVal)/5.0F);
    return "#"+String(color, HEX)+String(color, HEX)+String(color, HEX);
}
