#include "LineSet.h"

LineSet::~LineSet()
{
	for( int part = 0; part < NUM_OUTS; part++ )
		free( partBuffers[part] );
}

void LineSet::reset( int max )
{
	numLines = 0;
	if( max <= 0 )
		max = 1;
	// if the new max is different than the old max, realloc()
	// the first time this is called, buffer == NULL, so this is just like malloc()
	if( max != maxLines )
	{
		maxLines = max;

		for( int part = 0; part < NUM_OUTS; part++ )
		{
			partBuffers[part] = (float*)realloc(partBuffers[part], max * NUM_OUTS * sizeof(float));
		}
	}
}

bool LineSet::addLine( const InputPoint p1, const InputPoint p2, const float sqrDist )
{
	if( numLines >= maxLines )
		return false;
	partBuffers[OUT_TX1][numLines] = p1.x;
	partBuffers[OUT_TY1][numLines] = p1.y;
	partBuffers[OUT_TZ1][numLines] = p1.z;
	partBuffers[OUT_TX2][numLines] = p2.x;
	partBuffers[OUT_TY2][numLines] = p2.y;
	partBuffers[OUT_TZ2][numLines] = p2.z;
	partBuffers[OUT_SQRDIST][numLines] = sqrDist;
#ifdef INCLUDE_INDEX_OUTPUTS
	partBuffers[OUT_INDEX1][numLines] = (float)p1.index;
	partBuffers[OUT_INDEX2][numLines] = (float)p2.index;
#endif
	numLines++;
	return true;
}

void LineSet::copyTo( const CHOP_Output* output ) const
{
	for( int part = 0; part < NUM_OUTS; part++ )
	{
		memcpy( output->channels[part], partBuffers[part], numLines * sizeof(float) );
	}
}
