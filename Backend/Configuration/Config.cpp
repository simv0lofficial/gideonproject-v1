#include "Config.h"
#include "../Interfaces/Interfaces.h"
#include "../Features/Visuals/EventLogger.h"

#pragma warning(disable: 4244)
#pragma warning(disable: 4800)
#pragma warning(disable: 4018)
#pragma warning(disable: 4715)

using namespace Cheat;

std::string color_to_string(float col[4]) {
	return std::to_string((int)(col[0] * 255)) + "," + std::to_string((int)(col[1] * 255)) + "," + std::to_string((int)(col[2] * 255)) + "," + std::to_string((int)(col[3] * 255));
}

float* string_to_color(std::string s) {
	static auto split = [](std::string str, const char* del) -> std::vector<std::string>
	{
		char* pTempStr = _strdup(str.c_str());
		char* pWord = strtok(pTempStr, del);
		std::vector<std::string> dest;

		while (pWord != NULL)
		{
			dest.push_back(pWord);
			pWord = strtok(NULL, del);
		}

		free(pTempStr);

		return dest;
	};

	std::vector<std::string> col = split(s, ",");
	return new float[4]{
		(float)std::stoi(col.at(0)) / 255.f,
		(float)std::stoi(col.at(1)) / 255.f,
		(float)std::stoi(col.at(2)) / 255.f,
		(float)std::stoi(col.at(3)) / 255.f
	};
}

void c_config::save() {
	char file_path[MAX_PATH] = { 0 };
	sprintf(file_path, "C:/gideonproject/cfg/csgo_%s.ini", presets.at(i["_preset"]));

	for (auto e : b) {
		if (!std::string(e.first).find("_")) continue;
		char buffer[8] = { 0 }; _itoa(e.second, buffer, 10);
		WritePrivateProfileStringA("b", e.first.c_str(), std::string(buffer).c_str(), file_path);
	}

	for (auto e : i) {
		if (!std::string(e.first).find("_")) continue;
		char buffer[32] = { 0 }; _itoa(e.second, buffer, 10);
		WritePrivateProfileStringA("i", e.first.c_str(), std::string(buffer).c_str(), file_path);
	}

	for (auto e : f) {
		if (!std::string(e.first).find("_")) continue;
		char buffer[64] = { 0 }; sprintf(buffer, "%f", e.second);
		WritePrivateProfileStringA("f", e.first.c_str(), std::string(buffer).c_str(), file_path);
	}

	for (auto e : c) {
		if (!std::string(e.first).find("_")) continue;
		WritePrivateProfileStringA("c", e.first.c_str(), color_to_string(e.second).c_str(), file_path);
	}

	for (auto e : m) {
		if (!std::string(e.first).find("_")) continue;

		std::string vs = "";
		for (auto v : e.second)
			vs += std::to_string(v.first) + ":" + std::to_string(v.second) + "|";

		WritePrivateProfileStringA("m", e.first.c_str(), vs.c_str(), file_path);
	}

	this->save_keys();
}

void c_config::load() {
	this->load_defaults();

	char file_path[MAX_PATH] = { 0 };
	sprintf(file_path, "C:/gideonproject/cfg/csgo_%s.ini", presets.at(i["_preset"]));

	char b_buffer[65536], i_buffer[65536], f_buffer[65536], c_buffer[65536], m_buffer[65536] = { 0 };

	int b_read = GetPrivateProfileSectionA("b", b_buffer, 65536, file_path);
	int i_read = GetPrivateProfileSectionA("i", i_buffer, 65536, file_path);
	int f_read = GetPrivateProfileSectionA("f", f_buffer, 65536, file_path);
	int c_read = GetPrivateProfileSectionA("c", c_buffer, 65536, file_path);
	int m_read = GetPrivateProfileSectionA("m", m_buffer, 65536, file_path);

	if ((0 < b_read) && ((65536 - 2) > b_read)) {
		const char* pSubstr = b_buffer;

		while ('\0' != *pSubstr) {
			size_t substrLen = strlen(pSubstr);

			const char* pos = strchr(pSubstr, '=');
			if (NULL != pos) {
				char name[256] = "";
				char value[256] = "";

				strncpy_s(name, _countof(name), pSubstr, pos - pSubstr);
				strncpy_s(value, _countof(value), pos + 1, substrLen - (pos - pSubstr));

				b[name] = atoi(value);
			}

			pSubstr += (substrLen + 1);
		}
	}

	if ((0 < i_read) && ((65536 - 2) > i_read)) {
		const char* pSubstr = i_buffer;

		while ('\0' != *pSubstr) {
			size_t substrLen = strlen(pSubstr);

			const char* pos = strchr(pSubstr, '=');
			if (NULL != pos) {
				char name[256] = "";
				char value[256] = "";

				strncpy_s(name, _countof(name), pSubstr, pos - pSubstr);
				strncpy_s(value, _countof(value), pos + 1, substrLen - (pos - pSubstr));

				i[name] = atoi(value);
			}

			pSubstr += (substrLen + 1);
		}
	}

	if ((0 < f_read) && ((65536 - 2) > f_read)) {
		const char* pSubstr = f_buffer;

		while ('\0' != *pSubstr) {
			size_t substrLen = strlen(pSubstr);

			const char* pos = strchr(pSubstr, '=');
			if (NULL != pos) {
				char name[256] = "";
				char value[256] = "";

				strncpy_s(name, _countof(name), pSubstr, pos - pSubstr);
				strncpy_s(value, _countof(value), pos + 1, substrLen - (pos - pSubstr));

				f[name] = atof(value);
			}

			pSubstr += (substrLen + 1);
		}
	}

	if ((0 < c_read) && ((65536 - 2) > c_read)) {
		const char* pSubstr = c_buffer;

		while ('\0' != *pSubstr) {
			size_t substrLen = strlen(pSubstr);

			const char* pos = strchr(pSubstr, '=');
			if (NULL != pos) {
				char name[256] = "";
				char value[256] = "";

				strncpy_s(name, _countof(name), pSubstr, pos - pSubstr);
				strncpy_s(value, _countof(value), pos + 1, substrLen - (pos - pSubstr));

				auto col = string_to_color(value);
				c[name][0] = col[0];
				c[name][1] = col[1];
				c[name][2] = col[2];
				c[name][3] = col[3];
			}

			pSubstr += (substrLen + 1);
		}
	}

	static auto split = [](std::string str, const char* del) -> std::vector<std::string>
	{
		char* pTempStr = _strdup(str.c_str());
		char* pWord = strtok(pTempStr, del);
		std::vector<std::string> dest;

		while (pWord != NULL)
		{
			dest.push_back(pWord);
			pWord = strtok(NULL, del);
		}

		free(pTempStr);

		return dest;
	};

	if ((0 < m_read) && ((65536 - 2) > m_read)) {
		const char* pSubstr = m_buffer;

		while ('\0' != *pSubstr) {
			size_t substrLen = strlen(pSubstr);

			const char* pos = strchr(pSubstr, '=');
			if (NULL != pos) {
				char name[256] = "";
				char value[256] = "";

				strncpy_s(name, _countof(name), pSubstr, pos - pSubstr);
				strncpy_s(value, _countof(value), pos + 1, substrLen - (pos - pSubstr));

				std::vector<std::string> kvpa = split(value, "|");
				std::unordered_map<int, bool> vl = {};
				for (auto kvp : kvpa) {
					if (kvp == "")
						continue; // ало глухой

					std::vector<std::string> kv = split(kvp, ":");
					vl[std::stoi(kv.at(0))] = std::stoi(kv.at(1));
				}

				m[name] = vl;
			}

			pSubstr += (substrLen + 1);
		}
	}

	// хуй

	this->load_keys();
}

void c_config::load_defaults() {
	int _preset = this->i["_preset"];

	b = std::unordered_map<std::string, bool>();
	i = std::unordered_map<std::string, int>();
	f = std::unordered_map<std::string, float>();
	c = std::unordered_map<std::string, float[4]>();

	i["_preset"] = _preset;

	c["menu_color"][0] = 0.937f;
	c["menu_color"][1] = 0.035f;
	c["menu_color"][2] = 0.373f;
	c["menu_color"][3] = 1.00f;

	c["misc_prefixcolor"][0] = 0.937f;
	c["misc_prefixcolor"][1] = 0.035f;
	c["misc_prefixcolor"][2] = 0.373f;
	c["misc_prefixcolor"][3] = 1.00f;

	c["vis_boundingbox_color"][0] = 1.f;
	c["vis_boundingbox_color"][1] = 1.f;
	c["vis_boundingbox_color"][2] = 1.f;
	c["vis_boundingbox_color"][3] = 0.5f;

	c["vis_name_color"][0] = 1.f;
	c["vis_name_color"][1] = 1.f;
	c["vis_name_color"][2] = 1.f;
	c["vis_name_color"][3] = 1.f;

	c["vis_ammo_color"][0] = 1.f;
	c["vis_ammo_color"][1] = 1.f;
	c["vis_ammo_color"][2] = 1.f;
	c["vis_ammo_color"][3] = 1.f;

	c["vis_outoffovarrows_color"][0] = 1.f;
	c["vis_outoffovarrows_color"][1] = 1.f;
	c["vis_outoffovarrows_color"][2] = 1.f;
	c["vis_outoffovarrows_color"][3] = 1.f;

	c["vis_glow_color"][0] = 1.f;
	c["vis_glow_color"][1] = 0.f;
	c["vis_glow_color"][2] = 170 / 255.f;
	c["vis_glow_color"][3] = 0.5f;

	c["vis_glowself_color"][0] = 1.f;
	c["vis_glowself_color"][1] = 1.f;
	c["vis_glowself_color"][2] = 1.f;
	c["vis_glowself_color"][3] = 1.f;

	c["vis_soundesp_color"][0] = 1.f;
	c["vis_soundesp_color"][1] = 1.f;
	c["vis_soundesp_color"][2] = 1.f;
	c["vis_soundesp_color"][3] = 1.f;

	c["vis_bullettracer_color"][0] = 1.f;
	c["vis_bullettracer_color"][1] = 0.f;
	c["vis_bullettracer_color"][2] = 159 / 255.f;
	c["vis_bullettracer_color"][3] = 1.f;

	c["vis_chamally_color"][0] = 255 / 255.f;
	c["vis_chamally_color"][1] = 0 / 255.f;
	c["vis_chamally_color"][2] = 179 / 255.f;
	c["vis_chamally_color"][3] = 255 / 255.f;

	c["vis_chamenemy_color"][0] = 173 / 255.f;
	c["vis_chamenemy_color"][1] = 244 / 255.f;
	c["vis_chamenemy_color"][2] = 5 / 255.f;
	c["vis_chamenemy_color"][3] = 255 / 255.f;

	c["vis_chamenemy_xqz_color"][0] = 0 / 255.f;
	c["vis_chamenemy_xqz_color"][1] = 170 / 255.f;
	c["vis_chamenemy_xqz_color"][2] = 255 / 255.f;
	c["vis_chamenemy_xqz_color"][3] = 255 / 255.f;

	c["vis_chamself_color"][0] = 0 / 255.f;
	c["vis_chamself_color"][1] = 0 / 255.f;
	c["vis_chamself_color"][2] = 0 / 255.f;
	c["vis_chamself_color"][3] = 0 / 255.f;

	c["vis_chamshadow_color"][0] = 0 / 255.f;
	c["vis_chamshadow_color"][1] = 0 / 255.f;
	c["vis_chamshadow_color"][2] = 0 / 255.f;
	c["vis_chamshadow_color"][3] = 255 / 255.f;

	c["vis_fakeangles_color"][0] = 255 / 255.f;
	c["vis_fakeangles_color"][1] = 255 / 255.f;
	c["vis_fakeangles_color"][2] = 255 / 255.f;
	c["vis_fakeangles_color"][3] = 255 / 255.f;

	c["vis_droppedwpns_color"][0] = 255 / 255.f;
	c["vis_droppedwpns_color"][1] = 255 / 255.f;
	c["vis_droppedwpns_color"][2] = 255 / 255.f;
	c["vis_droppedwpns_color"][3] = 255 / 255.f;

	c["vis_grenades_color"][0] = 255 / 255.f;
	c["vis_grenades_color"][1] = 255 / 255.f;
	c["vis_grenades_color"][2] = 255 / 255.f;
	c["vis_grenades_color"][3] = 255 / 255.f;

	c["vis_inaccuracyoverlay_color"][0] = 255 / 255.f;
	c["vis_inaccuracyoverlay_color"][1] = 255 / 255.f;
	c["vis_inaccuracyoverlay_color"][2] = 255 / 255.f;
	c["vis_inaccuracyoverlay_color"][3] = 100 / 255.f;

	c["vis_bomb_color"][0] = 255 / 255.f;
	c["vis_bomb_color"][1] = 0 / 255.f;
	c["vis_bomb_color"][2] = 0 / 255.f;
	c["vis_bomb_color"][3] = 255 / 255.f;

	i["misc_menukey"] = VK_INSERT;
	i["misc_overridefov"] = 90;
	i["vis_wallstransp"] = 100;
	i["vis_propstransp"] = 100;
	i["vis_maximalespdistance"] = 500;
	i["vis_outoffovarrows_dist"] = 200;
	i["vis_outoffovarrows_size"] = 32;
	i["rage_antiaim_desync_limit"] = 60;

	i["rage_aimbot_keystyle"] = 0;
	i["rage_quickstop_keystyle"] = 1;
	i["rage_forcebaim_keystyle"] = 1;
	i["rage_slowmotion_keystyle"] = 1;
	i["rage_fakeduck_keystyle"] = 1;
	i["vis_thirdperson_keystyle"] = 2;
	i["misc_circlestrafe_keystyle"] = 1;
	i["misc_lagexploit_keystyle"] = 1;
	i["misc_pingspike_keystyle"] = 2;

	b["misc_showprefix"] = true;
	b["misc_antiuntrusted"] = true;
	b["rage_friendlyfire"] = false;
}

void c_config::load_keys() {
	for (int k = 0; k < presets.size(); k++) {
		char buffer[32] = { 0 }; sprintf(buffer, "_preset_%i", k);
		i[buffer] = GetPrivateProfileIntA("k", buffer, 0, "C:/gideonproject/cfg/csgo_keys.ini");
	}
}

void c_config::save_keys() {
	for (int k = 0; k < presets.size(); k++) {
		char buffer[32] = { 0 }; sprintf(buffer, "_preset_%i", k);
		char value[32] = { 0 }; sprintf(value, "%i", i[buffer]);
		WritePrivateProfileStringA("k", buffer, value, "C:/gideonproject/cfg/csgo_keys.ini");
	}
}