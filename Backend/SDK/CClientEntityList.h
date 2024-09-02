#pragma once
#include "vfunc.h"

class CBaseEntity;

class CClientEntityList
{
	public:

		CBaseEntity* GetClientEntity( int index )
		{
			typedef CBaseEntity*( __thiscall * Fn )( void*, int );

			if ( this != NULL)
				return vfunc<Fn>( this, 3 )( this, index );
     	}

		int NumberOfEntities( bool bIncludeNonNetworkable )
		{
			typedef int( __thiscall * Fn )( void*, bool );

			if ( this )
				return vfunc<Fn>( this, 7 )( this, bIncludeNonNetworkable );
		}

		int GetHighestEntityIndex()
		{
			typedef int( __thiscall * Fn )( void* );

			if ( this )
				return vfunc<Fn>( this, 8 )( this );
		}

		CBaseEntity* GetClientEntityFromHandle( void* handle )
		{
			typedef CBaseEntity*( __thiscall * Fn )( void*, void* );

			if ( this )
				return vfunc<Fn>( this, 7 )( this, handle );
		}
		CBaseEntity* 	GetClientEntityFromHandle2( ULONG hEnt )
		{
			typedef CBaseEntity*( __thiscall * Fn )( void*, ULONG );

			if ( this )
				return vfunc<Fn>( this, 4 )( this, hEnt );
		}
};
