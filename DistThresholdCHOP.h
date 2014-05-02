#pragma once

#include "shared.h"

#include "LineSet.h"

class DistThresholdCHOP : public CHOP_CPlusPlusBase
{
public:
	DistThresholdCHOP(const CHOP_NodeInfo *info) : lines() { }
	virtual ~DistThresholdCHOP() { }

	virtual void		getGeneralInfo(CHOP_GeneralInfo *);
	virtual bool		getOutputInfo(CHOP_OutputInfo*);
	virtual const char*	getChannelName(int index, void* reserved)
	{
		return getOutputPartName(index);
	}

	virtual void		execute(const CHOP_Output*,
								const CHOP_InputArrays*,
								void* reserved);

	virtual int			getNumInfoCHOPChans();
	virtual void		getInfoCHOPChan(int index,
										CHOP_InfoCHOPChan *chan);
private:

	void loadSettings(const CHOP_FloatInput *inputs);

	LineSet lines;

	float settings[NUM_SETTINGS];
};

