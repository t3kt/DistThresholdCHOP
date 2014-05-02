#pragma once

#include "shared.h"

class LineSet;

class InputPoint
{
	friend class LineSet;
public:
	InputPoint( const float **channels, const int i );

	float squareDist( const InputPoint &p2 ) const;

private:
	const float x;
	const float y;
	const float z;
#ifdef INCLUDE_INDEX_OUTPUTS
	const int index;
#endif
};

