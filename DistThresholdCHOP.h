#include "CHOP_CPlusPlusBase.h"

enum
{
	OUT_TX1,
	OUT_TY1,
	OUT_TZ1,
	OUT_TX2,
	OUT_TY2,
	OUT_TZ2,
	OUT_SQRDIST,
	NUM_OUTS
};

enum
{
	IN_X,
	IN_Y,
	IN_Z
};
enum
{
	PT_X,
	PT_Y,
	PT_Z
};

enum
{
	SETTING_DISTMAX,
	SETTING_MAXLINES,
	SETTING_MAXLINESPERSOURCE,
	SETTING_DISTMIN,
	SETTING_SEPARATESOURCE
};

class DistThresholdCHOP : public CHOP_CPlusPlusBase
{
public:
	DistThresholdCHOP(const CHOP_NodeInfo *info) { }
	virtual ~DistThresholdCHOP() { }

	virtual void		getGeneralInfo(CHOP_GeneralInfo *);
	virtual bool		getOutputInfo(CHOP_OutputInfo*);
	virtual const char*	getChannelName(int index, void* reserved);

	virtual void		execute(const CHOP_Output*,
								const CHOP_InputArrays*,
								void* reserved);
private:

	float** linepos;

	int numlines;
};
