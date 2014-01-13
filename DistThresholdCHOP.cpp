#include "DistThresholdCHOP.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

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

const char* DistThresholdCHOP::getChannelName(int index, void* reserved)
{
	const char* name = "";

	switch(index)
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
	}
	return name;
}

inline float squareDist( float p1[3], float p2[3] )
{
	return pow(p2[0]-p1[0],2) + pow(p2[1]-p1[1],2) + pow(p2[2]-p1[2],2);
}

bool DistThresholdCHOP::getOutputInfo(CHOP_OutputInfo *info)
{
	int maxLines = (int)info->inputArrays->floatInputs[SETTING_MAXLINES].values[0];
	float distMax = info->inputArrays->floatInputs[SETTING_DISTMAX].values[0];
	//float fade = info->inputArrays->floatInputs[0].values[1];

	info->numChannels = NUM_OUTS;

	linepos = (float**)malloc(NUM_OUTS*sizeof(float*));
	for (int i = 0; i < NUM_OUTS; i++)
	{
		linepos[i] = (float*)malloc(maxLines*sizeof(float));
	}

	numlines = 0;

	const CHOP_CHOPInput input0 = info->inputArrays->CHOPInputs[0];
	int input0len = input0.length;
	if (info->inputArrays->numCHOPInputs >= 2)
	{
		int maxLinesPerSource = (int)info->inputArrays->floatInputs[SETTING_MAXLINESPERSOURCE].values[0];
		const CHOP_CHOPInput input1 = info->inputArrays->CHOPInputs[1];
		int input1len = input1.length;
		for(int i = 0; i < input0len && numlines < maxLines; i++)
		{
			float p1[] = {
				input0.channels[IN_X][i],
				input0.channels[IN_Y][i],
				input0.channels[IN_Z][i]
			};
			int linesForSource = 0;
			for(int j = 0; j < input1len; j++)
			{
				float p2[] = {
					input1.channels[IN_X][j],
					input1.channels[IN_Y][j],
					input1.channels[IN_Z][j]
				};
				float sqrdist = squareDist(p1, p2);
				if (sqrdist<distMax)
				{
					linepos[OUT_TX1][numlines] = p1[PT_X];
					linepos[OUT_TY1][numlines] = p1[PT_Y];
					linepos[OUT_TZ1][numlines] = p1[PT_Z];
					linepos[OUT_TX2][numlines] = p2[PT_X];
					linepos[OUT_TY2][numlines] = p2[PT_Y];
					linepos[OUT_TZ2][numlines] = p2[PT_Z];
					linepos[OUT_SQRDIST][numlines] = sqrdist;
					numlines++;
					linesForSource++;
					if(linesForSource >= maxLinesPerSource || numlines >= maxLines)
						break;
				}
			}
		}
	}
	else if (info->inputArrays->numCHOPInputs >0)
	{
		for (int i = 0; i < input0len; i++)
		{
			if(numlines<maxLines)
			{
				float p1[] = {
					input0.channels[IN_X][i],
					input0.channels[IN_Y][i],
					input0.channels[IN_Z][i]
				};

				for (int j = i+1; j < input0len; j++)
				{
					if(numlines<maxLines)
					{
						float p2[] = {
							input0.channels[IN_X][j],
							input0.channels[IN_Y][j],
							input0.channels[IN_Z][j]
						};

						float sqrdist = squareDist(p1, p2);
						if (sqrdist<distMax)
						{
							linepos[OUT_TX1][numlines] = p1[0];
							linepos[OUT_TY1][numlines] = p1[1];
							linepos[OUT_TZ1][numlines] = p1[2];
							linepos[OUT_TX2][numlines] = p2[0];
							linepos[OUT_TY2][numlines] = p2[1];
							linepos[OUT_TZ2][numlines] = p2[2];
							linepos[OUT_SQRDIST][numlines] = sqrdist;
							numlines++;
						}
					}
				}
			}
		}
	} else {
		for (int i = 0 ; i < info->numChannels; i++)
		{
			free(linepos[i]);
		}
	}

	if(numlines<1) numlines=1;
	info->length = numlines;
	return true;
}

void DistThresholdCHOP::execute(const CHOP_Output* output, const CHOP_InputArrays* inputs, void* reserved)
{
	if (inputs->numCHOPInputs > 0)
	{
		for (int i = 0 ; i < output->numChannels; i++)
		{
			memcpy(output->channels[i], linepos[i], numlines*sizeof(float));

			free(linepos[i]);
		}
	}
}
