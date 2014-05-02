#pragma once

#include "CHOP_CPlusPlusBase.h"
#include <math.h>

enum
{
	OUT_TX1,
	OUT_TY1,
	OUT_TZ1,
	OUT_TX2,
	OUT_TY2,
	OUT_TZ2,
	OUT_SQRDIST,
	OUT_INDEX1,
	OUT_INDEX2,
	NUM_OUTS
};

enum
{
	IN_X,
	IN_Y,
	IN_Z
};

class LineSet;

class InputPoint
{
	friend class LineSet;
public:
	InputPoint( const float **channels, const int i ) :
		x(channels[IN_X][i]),
		y(channels[IN_Y][i]),
		z(channels[IN_Z][i]),
		index(i) { }

	float squareDist( const InputPoint &p2 ) const
	{
		return pow(p2.x-x,2) + pow(p2.y-y,2) + pow(p2.z-z,2);
	}

private:
	const float x;
	const float y;
	const float z;
	const int index;
};

class LineSet
{
public:
	LineSet() :
		maxLines(-1),
		numLines(0)//,
		//buffer(NULL)
	{
		for( int part = 0; part < NUM_OUTS; part++ )
			partBuffers[part] = NULL;
	}
	~LineSet();

	void reset( int max );

	bool addLine( InputPoint p1, InputPoint p2, float sqrDist );

	void copyTo( const CHOP_Output* output ) const;

	inline int getNumLines() const { return numLines; }

	inline bool canFitMore() const { return numLines < maxLines; }

private:

	int maxLines;
	int numLines;

	//float *buffer;
	float *partBuffers[NUM_OUTS];
};
