#include "KeyHandler.h"
#include "../Configuration/Config.h"

void c_keyhandler::wnd_handler(UINT msg, WPARAM wParam) {
	
}

bool c_keyhandler::is_key_down(int key) {
	return HIWORD(GetKeyState(key));
}

bool c_keyhandler::is_key_up(int key) {
	return !HIWORD(GetKeyState(key));
}

bool c_keyhandler::is_key_pressed(int key) {
	return false;
}

bool c_keyhandler::auto_check(std::string key) {
	switch (c_config::get()->i[key + "style"]) {
	case 0:
		return true;
	case 1:
		return this->is_key_down(c_config::get()->i[key]);
	case 2:
		return LOWORD(GetKeyState(c_config::get()->i[key]));
	case 3:
		return this->is_key_up(c_config::get()->i[key]);
	default:
		return true;
	}
}