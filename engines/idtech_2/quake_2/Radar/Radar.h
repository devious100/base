// 6/20/2003 - Created by Vertigo Software, Inc.
// 11/17/2007 - Modified by Greg Dolley.

#include <vector>

// different radar item types
enum RadarType
{ 
	RadarTypeMonster,
	RadarTypeDeadMonsters,
	RadarTypePlayer,
	RadarTypeHealth,
	RadarTypeItems,
	RadarTypeUnknown
};


// each item in the radar list
struct RadarItem // ***GREGS_VC9_PORT_MOD*** -- took out "__nogc"
{
	int x;			// x position relative to player
	int y;			// y position relative to player
	RadarType type;	// item type
};


// pass vector list to managed assembly
class ItemVector : public std::vector<RadarItem> // ***GREGS_VC9_PORT_MOD*** -- took out "__nogc"
{
};