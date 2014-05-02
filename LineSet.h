#pragma once

#include "shared.h"
#include "InputPoint.h"

class LineSet
{
public:
	LineSet() :
		maxLines(-1),
		numLines(0)
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

	float *partBuffers[NUM_OUTS];
};
