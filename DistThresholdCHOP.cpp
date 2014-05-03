#include "DistThresholdCHOP.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// These functions are basic C function, which the DLL loader can find
// much easier than finding a C++ Class.
// The DLLEXPORT prefix is needed so the compile exports these functions from the .dll
// you are creating
extern "C"
{

	DLLEXPORT int GetCHOPAPIVersion(void)
	{
		// Always return CHOP_CPLUSPLUS_API_VERSION in this function.
		return CHOP_CPLUSPLUS_API_VERSION;
	}

	DLLEXPORT CHOP_CPlusPlusBase* CreateCHOPInstance(const CHOP_NodeInfo *info)
	{
		// Return a new instance of your class every time this is called.
		// It will be called once per CHOP that is using the .dll
		return new DistThresholdCHOP(info);
	}

	DLLEXPORT void DestroyCHOPInstance(CHOP_CPlusPlusBase *instance)
	{
		// Delete the instance here, this will be called when
		// Touch is shutting down, when the CHOP using that instance is deleted, or
		// if the CHOP loads a different DLL
		delete (DistThresholdCHOP*)instance;
	}

};


void DistThresholdCHOP::getGeneralInfo(CHOP_GeneralInfo *ginfo)
{
	// This will cause the node to cook every frame
	ginfo->cookEveryFrameIfAsked = true;
	//ginfo->timeslice = true;
	ginfo->timeslice = false;
	ginfo->inputMatchIndex = 0;
}

void DistThresholdCHOP::loadSettings( const CHOP_FloatInput *inputs )
{
	for( int setting = 0; setting < NUM_SETTINGS; setting++ )
	{
		settings[setting] = inputs[setting].values[0];
	}
}

bool DistThresholdCHOP::getOutputInfo(CHOP_OutputInfo *info)
{
	if( info->numChannels == 0 || info->length == 0 )
	{
		return false;
	}
	info->numChannels = NUM_OUTS;
	loadSettings(info->inputArrays->floatInputs);
	const int maxLines = (int)settings[SETTING_MAXLINES];
	const float distMin = settings[SETTING_DISTMIN];
	const float distMax = settings[SETTING_DISTMAX];
	const bool useSeparateSource = settings[SETTING_SEPARATESOURCE] != 0;
	//float fade = info->inputArrays->floatInputs[0].values[1];

	lines.reset( maxLines );

	const CHOP_CHOPInput input0 = info->inputArrays->CHOPInputs[0];
	const int input0len = input0.length;
	if (useSeparateSource && info->inputArrays->numCHOPInputs >= 2)
	{
		const int maxLinesPerSource = (int)settings[SETTING_MAXLINESPERSOURCE];
		const CHOP_CHOPInput input1 = info->inputArrays->CHOPInputs[1];
		const int input1len = input1.length;
		for(int i = 0; i < input0len && lines.canFitMore(); i++)
		{
			InputPoint p1(input0.channels, i);
			int linesForSource = 0;
			for(int j = 0; j < input1len; j++)
			{
				InputPoint p2(input1.channels, j);
				float sqrdist = p1.squareDist(p2);
				if (sqrdist<distMax && sqrdist >=distMin)
				{
					if(!lines.addLine( p1, p2, sqrdist ))
						break;
					linesForSource++;
					if(linesForSource >= maxLinesPerSource)
						break;
				}
			}
		}
	}
	else if (info->inputArrays->numCHOPInputs >= 1)
	{
		for (int i = 0; i < input0len && lines.canFitMore(); i++)
		{
			const InputPoint p1(input0.channels, i);

			for (int j = i+1; j < input0len; j++)
			{
				InputPoint p2(input0.channels, j);
				float sqrdist = p1.squareDist(p2);
				if (sqrdist<distMax && sqrdist >=distMin)
				{
					if(!lines.addLine(p1, p2, sqrdist))
						break;
				}
			}
		}
	}

	info->length = max(1, lines.getNumLines());

	return true;
}

void DistThresholdCHOP::execute(const CHOP_Output* output, const CHOP_InputArrays* inputs, void* reserved)
{
	if (inputs->numCHOPInputs > 0 && inputs->CHOPInputs[0].length > 0 && inputs->CHOPInputs[0].numChannels > 0)
	{
		if( lines.getNumLines() > 0 )
			lines.copyTo(output);
		else
		{
			for( int part = 0; part < NUM_OUTS; part++ )
				output->channels[part][0] = 0;
		}
	}
}

enum
{
	INFO_NUMLINES = NUM_SETTINGS,

	NUM_INFO
};

int DistThresholdCHOP::getNumInfoCHOPChans()
{
	return NUM_INFO;
}

void DistThresholdCHOP::getInfoCHOPChan( int index, CHOP_InfoCHOPChan *chan )
{
	switch( index )
	{
	case SETTING_DISTMAX:
		chan->name = "distmax";
		break;
	case SETTING_MAXLINES:
		chan->name = "maxlines";
		break;
	case SETTING_MAXLINESPERSOURCE:
		chan->name = "maxlinespersource";
		break;
	case SETTING_DISTMIN:
		chan->name = "distmin";
		break;
	case SETTING_SEPARATESOURCE:
		chan->name = "separatesource";
		break;
	case INFO_NUMLINES:
		chan->name = "numlines";
		chan->value = (float)lines.getNumLines();
		break;
	}
	if( index < NUM_SETTINGS )
	{
		chan->value = settings[index];
	}
}

