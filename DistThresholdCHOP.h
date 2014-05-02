#include "CHOP_CPlusPlusBase.h"

#include "LineSet.h"

enum
{
	SETTING_DISTMAX,
	SETTING_MAXLINES,
	SETTING_MAXLINESPERSOURCE,
	SETTING_DISTMIN,
	SETTING_SEPARATESOURCE,
	NUM_SETTINGS
};

class DistThresholdCHOP : public CHOP_CPlusPlusBase
{
public:
	DistThresholdCHOP(const CHOP_NodeInfo *info) : lines() { }
	virtual ~DistThresholdCHOP() { }

	virtual void		getGeneralInfo(CHOP_GeneralInfo *);
	virtual bool		getOutputInfo(CHOP_OutputInfo*);
	virtual const char*	getChannelName(int index, void* reserved);

	virtual void		execute(const CHOP_Output*,
								const CHOP_InputArrays*,
								void* reserved);

	virtual int			getNumInfoCHOPChans();
	virtual void		getInfoCHOPChan(int index,
										CHOP_InfoCHOPChan *chan);
private:

	void loadSettings(const CHOP_FloatInput *inputs)
	{
		for( int setting = 0; setting < NUM_SETTINGS; setting++ )
		{
			settings[setting] = inputs[setting].values[0];
		}
	}

	LineSet lines;

	float settings[NUM_SETTINGS];
};

