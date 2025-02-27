#pragma once
#include <Windows.h>

struct surfacephysicsparams_t
{
	float    friction;
	float    elasticity;
	float    density;
	float    thickness;
	float    dampening;
};

struct surfaceaudioparams_t
{
	float    reflectivity;             // like elasticity, but how much sound should be reflected by this surface
	float    hardnessFactor;           // like elasticity, but only affects impact sound choices
	float    roughnessFactor;          // like friction, but only affects scrape sound choices
	float    roughThreshold;           // surface roughness > this causes xorstr_("rough") scrapes, < this causes xorstr_("smooth") scrapes
	float    hardThreshold;            // surface hardness > this causes xorstr_("hard") impacts, < this causes xorstr_("soft") impacts
	float    hardVelocityThreshold;    // collision velocity > this causes xorstr_("hard") impacts, < this causes xorstr_("soft") impacts
	float    highPitchOcclusion;       //a value betweeen 0 and 100 where 0 is not occluded at all and 100 is silent (except for any additional reflected sound)
	float    midPitchOcclusion;
	float    lowPitchOcclusion;
};

struct surfacesoundnames_t
{
	unsigned short    walkStepLeft;
	unsigned short    walkStepRight;
	unsigned short	  runStepLeft;
	unsigned short	  runStepRight;
	unsigned short    impactSoft;
	unsigned short    impactHard;
	unsigned short    scrapeSmooth;
	unsigned short    scrapeRough;
	unsigned short    bulletImpact;
	unsigned short    rolling;
	unsigned short    breakSound;
	unsigned short    strainSound;
};

struct surfacegameprops_t
{
	public:
		float maxSpeedFactor;
		float jumpFactor;
		float flPenetrationModifier;
		float flDamageModifier;
		unsigned short material;
		byte climbable;
		char pad00[0x4];

};

struct surfacedata_t
{
	surfacephysicsparams_t    physics;
	surfaceaudioparams_t    audio;
	surfacesoundnames_t        sounds;
	surfacegameprops_t        game;
};

#include "vfunc.h"

class IPhysicsProps
{
	public:

		surfacedata_t* GetSurfaceData( int surfaceDataIndex )
		{
			typedef surfacedata_t* ( __thiscall * fnGetSurfaceData )( void*, int );
			return vfunc<fnGetSurfaceData>( this, 5 )( this, surfaceDataIndex );
		}
};
