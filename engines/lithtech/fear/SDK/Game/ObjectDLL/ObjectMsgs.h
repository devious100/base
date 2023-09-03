// ----------------------------------------------------------------------- //
//
// MODULE  : ObjectMsgs.h
//
// PURPOSE : Object to object messages (replaces generic_msg_de.h)
//
// CREATED : 4/22/99
//
// (c) 1999-2000 Monolith Productions, Inc.  All Rights Reserved
//
// ----------------------------------------------------------------------- //

#ifndef __OBJECT_MSGS_H__
#define __OBJECT_MSGS_H__

// Defines...

// All of the following messages are sent between objects:
// (i.e., Object <-> Object)

// For MID_DAMAGE, see DamageStruct in Destructible.h for message info.
//
#define MID_DAMAGE					1001

// For MID_REPAIR, the message should contain 1 float that represents the
// repair amount
//
#define MID_REPAIR					1002

// For MID_HEAL, the message should contain 1 float that represents the
// heal amount
//
#define MID_HEAL					1003

// For MID_AMMOBOX, the message should contain 1 uint8 indicating the number
// of ammo types in the box. For each type the message should contain (in order),
// 1 uint8 that represents an ammo id (this id will be game specific), and
// 1 float that represents the amount of ammo of that type

#define MID_AMMOBOX					1004

// For MID_ADDWEAPON, the message should contain (in order), 1 uint8 that
// represents the weapon id (this id will be game specific), 1 uint8 that
// contains an ammo id (this id will be game specific), and 1 float that
// represents the amount of ammo the weapon contains
//
#define	MID_ADDWEAPON				1005

// For MID_PICKEDUP
#define MID_PICKEDUP				1006

// For MID_ANIMCALLBACK, the message should contain (in order),
//
#define MID_ANIMCALLBACK			1006

// For MID_ADDGEAR, the message should contain 1 uint8 that
// represents the gear id (this id will be game specific)
//
#define	MID_ADDGEAR					1007

// For MID_ADDMOD, the message should contain 1 uint8 that
// represents the mod id (this id will be game specific)
//
#define	MID_ADDMOD					1008

#endif // __OBJECT_MSGS_H__
