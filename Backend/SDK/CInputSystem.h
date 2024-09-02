#pragma once
#include "vfunc.h"

class CInputSystem
{
	public:
		void EnableInput( bool enabled )
		{
			typedef void( __thiscall * Fn )( void*, bool );

			if ( this )
				return ( ( Fn )vfunc<Fn>( this, 11 ) )( this, enabled );
		}
};
