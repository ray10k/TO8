#ifndef __COMMON_H
#define __COMMON_H 

#define forever while(1==1)

//*****************
//
//! Written in a misguided attempt at increasing portability.
//! Just don't think too hard about it.
//
//*****************

namespace display
{
	const unsigned int width = 16;
	const unsigned int height = 2;
	const unsigned int characters = width * height;
}

#endif
