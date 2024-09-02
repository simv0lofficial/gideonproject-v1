#pragma once
#include "vector3d.h"
#include "CUserCmd.h"

class CInput
{
public:
		char pad_0x00[0x0C]; // 0x00
		bool m_trackir_available; //0x0C
		bool m_mouse_initiated; //0x0D
		bool m_mouse_active; //0x0E
		char pad_0x0F[0xFBE];
};
