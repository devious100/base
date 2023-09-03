#ifndef safedisc_h
#define safedisc_h

#if TARGET_VERSION == BF2_VERSION_RETAIL141


#include "../cheats/cheat_base.h.h"
#include "../bf2/bf2.h"

class SafediscFix
{
	vector<CodePatch> patches;

	BOOL applied;

public:
	SafediscFix();
	~SafediscFix();

	void Exterminate(void);


};



#endif // target version


#endif //safedisc_h