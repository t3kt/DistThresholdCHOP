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

bool DistThresholdCHOP::getOutputInfo(CHOP_OutputInfo *info)
{


	int maxLines = (int)info->inputArrays->floatInputs[1].values[0];

	info->numChannels = 7;


	linepos = (float**)malloc(info->numChannels*sizeof(float*));
	for (int i = 0; i < info->numChannels; i++)
	{
		linepos[i] = (float*)malloc(maxLines*sizeof(float));
	}

	l = 0;

	if (info->inputArrays->numCHOPInputs > 0)
	{
		for (int i = 0; i < info->inputArrays->CHOPInputs[0].length; i++)
		{
			if(l<maxLines)
			{
				float p1[] = {info->inputArrays->CHOPInputs[0].channels[0][i],
					info->inputArrays->CHOPInputs[0].channels[1][i],
					info->inputArrays->CHOPInputs[0].channels[2][i]};

				for (int j = i+1; j < info->inputArrays->CHOPInputs[0].length; j++)
				{
					if(l<maxLines)
					{
						float p2[] = {info->inputArrays->CHOPInputs[0].channels[0][j],
							info->inputArrays->CHOPInputs[0].channels[1][j],
							info->inputArrays->CHOPInputs[0].channels[2][j]};

						float sqrdist = pow(p2[0]-p1[0],2) + pow(p2[1]-p1[1],2) + pow(p2[2]-p1[2],2);
						float distMax = info->inputArrays->floatInputs[0].values[0];
						//float fade = info->inputArrays->floatInputs[0].values[1];
						if (sqrdist<distMax)
						{

							linepos[0][l] = p1[0];
							linepos[1][l] = p1[1];
							linepos[2][l] = p1[2];
							linepos[3][l] = p2[0];
							linepos[4][l] = p2[1];
							linepos[5][l] = p2[2];
							//linepos[5][l] = sqrdist;
							linepos[6][l] = sqrdist;
							l++;
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

	if(l<1) l=1;
	info->length = l;
	return true;
}

const char*
	DistThresholdCHOP::getChannelName(int index, void* reserved)
{
	const char* name = "";

	switch(index)
	{
	case 0:
		name = "tx1";
		break;
	case 1:
		name = "ty1";
		break;
	case 2:
		name = "tz1";
		break;
	case 3:
		name = "tx2";
		break;
	case 4:
		name = "ty2";
		break;
	case 5:
		name = "tz2";
		break;
	case 6:
		name = "sqrdist";
		break;
	}
	return name;
}

void DistThresholdCHOP::execute(const CHOP_Output* output, const CHOP_InputArrays* inputs, void* reserved)
{
	if (inputs->numCHOPInputs > 0)
	{
		for (int i = 0 ; i < output->numChannels; i++)
		{
			memcpy(output->channels[i], linepos[i], l*sizeof(float));

			free(linepos[i]);
		}
	}
}
