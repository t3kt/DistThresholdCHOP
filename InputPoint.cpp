#include "InputPoint.h"

#include <math.h>

InputPoint::InputPoint( const float **channels, const int i ) 
	:
#ifdef INCLUDE_INDEX_OUTPUTS
	index(i),
#endif
	x(channels[IN_X][i]),
	y(channels[IN_Y][i]),
	z(channels[IN_Z][i])
{ }

float InputPoint::squareDist( const InputPoint &p2 ) const
{
	return pow(p2.x-x,2) + pow(p2.y-y,2) + pow(p2.z-z,2);
}