#pragma once

#include "CHOP_CPlusPlusBase.h"

#ifdef _DEBUG

#define INCLUDE_INDEX_OUTPUTS

#endif

enum
{
	SETTING_DISTMAX,
	SETTING_MAXLINES,
	SETTING_MAXLINESPERSOURCE,
	SETTING_DISTMIN,
	SETTING_SEPARATESOURCE,
	NUM_SETTINGS
};

enum
{
	IN_X,
	IN_Y,
	IN_Z
};

enum
{
	OUT_TX1,
	OUT_TY1,
	OUT_TZ1,
	OUT_TX2,
	OUT_TY2,
	OUT_TZ2,
	OUT_SQRDIST,
#ifdef INCLUDE_INDEX_OUTPUTS
	OUT_INDEX1,
	OUT_INDEX2,
#endif
	NUM_OUTS
};

inline const char* getOutputPartName( const int part )
{
	const char* name = "";

	switch(part)
	{
	case OUT_TX1:
		name = "tx1";
		break;
	case OUT_TY1:
		name = "ty1";
		break;
	case OUT_TZ1:
		name = "tz1";
		break;
	case OUT_TX2:
		name = "tx2";
		break;
	case OUT_TY2:
		name = "ty2";
		break;
	case OUT_TZ2:
		name = "tz2";
		break;
	case OUT_SQRDIST:
		name = "sqrdist";
		break;
#ifdef INCLUDE_INDEX_OUTPUTS
	case OUT_INDEX1:
		name = "index1";
		break;
	case OUT_INDEX2:
		name = "index2";
		break;
#endif
	}
	return name;
}
