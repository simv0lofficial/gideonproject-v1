#include "cmenu.h"
#include "bgtexture.h"
#include "bindsfont.h"

#include "../../Backend/Configuration/Config.h"
#include "../../Backend/Interfaces/Interfaces.h"
#include "../../Backend/lazyimports.h"
#include "../../Backend/Lua/CLua.h"
#include "../../Backend/Features/Ragebot/c_playerlist.h"

void c_menu::initialize(IDirect3DDevice9* pDevice) {
	if (this->m_bInitialized)
		return;

	ui::CreateContext();
	auto io = ui::GetIO();
	auto style = &ui::GetStyle();

	style->WindowRounding = 0.f;
	style->AntiAliasedLines = true;
	style->AntiAliasedFill = true;
	style->ScrollbarRounding = 0.f;
	style->ScrollbarSize = 6.f;
	style->WindowPadding = ImVec2(0, 0);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(45 / 255.f, 45 / 255.f, 45 / 255.f, 1.f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(65 / 255.f, 65 / 255.f, 65 / 255.f, 1.f);

	this->m_bIsOpened = true;

	IDirect3DSwapChain9* pChain = nullptr;
	D3DPRESENT_PARAMETERS pp = {};
	D3DDEVICE_CREATION_PARAMETERS param = {};
	pDevice->GetCreationParameters(&param);
	pDevice->GetSwapChain(0, &pChain);

	if (pChain)
		pChain->GetPresentParameters(&pp);

	ImGui_ImplWin32_Init(param.hFocusWindow);
	ImGui_ImplDX9_Init(pDevice);

	D3DXCreateTextureFromFileInMemoryEx(
		pDevice, texture, sizeof(texture), 4096, 4096, D3DX_DEFAULT, NULL,
		pp.BackBufferFormat, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &this->m_pTexture);

	ImFontConfig cfg;
	io.Fonts->AddFontFromFileTTF("C:/windows/fonts/verdana.ttf", 12.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	io.Fonts->AddFontFromFileTTF("C:/windows/fonts/verdanab.ttf", 12.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	io.Fonts->AddFontFromMemoryTTF(keybinds_font, 25600, 10.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	io.Fonts->AddFontDefault();

	ImGuiFreeType::BuildFontAtlas(io.Fonts, 0x00);

	this->m_bInitialized = true;
}

void c_menu::draw_begin() {
	if (!this->m_bInitialized)
		return;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ui::NewFrame();
}

void dmt(std::string key) {
	if (c_config::get()->b["lua_devmode"] && ui::IsItemHovered()) {
		ui::BeginTooltip();
		ui::Text(key.c_str());
		ui::EndTooltip();
	}
}

void draw_lua_items(std::string tab, std::string container) {
	auto cfg = c_config::get();
	for (auto i : c_lua::get()->menu_items[tab][container]) {
		if (!i.is_visible)
			continue;

		auto type = i.type;
		switch (type)
		{
		case MENUITEM_CHECKBOX:
			if (ui::Checkbox(i.label.c_str(), &cfg->b[i.key])) {
				if (i.callback != sol::nil)
					i.callback(cfg->b[i.key]);
			}

			dmt(i.key);
			break;
		case MENUITEM_SLIDERINT:
			if (ui::SliderInt(i.label.c_str(), &cfg->i[i.key], i.i_min, i.i_max, i.format.c_str())) {
				if (i.callback != sol::nil)
					i.callback(cfg->i[i.key]);
			}

			dmt(i.key);
			break;
		case MENUITEM_SLIDERFLOAT:
			if (ui::SliderFloat(i.label.c_str(), &cfg->f[i.key], i.f_min, i.f_max, i.format.c_str())) {
				if (i.callback != sol::nil)
					i.callback(cfg->f[i.key]);
			}

			dmt(i.key);
			break;
		case MENUITEM_KEYBIND:
			if (ui::Keybind(i.label.c_str(), &cfg->i[i.key], i.allow_style_change ? &cfg->i[i.key + "style"] : NULL)) {
				if (i.callback != sol::nil)
					i.callback(cfg->i[i.key], cfg->i[i.key + "style"]);
			}

			dmt(i.key + (i.allow_style_change ? " | " + i.key + "style" : ""));
			break;
		case MENUITEM_TEXT:
			ui::Text(i.label.c_str());
			break;
		case MENUITEM_SINGLESELECT:
			if (ui::SingleSelect(i.label.c_str(), &cfg->i[i.key], i.items)) {
				if (i.callback != sol::nil)
					i.callback(cfg->i[i.key]);
			}

			dmt(i.key);
			break;
		case MENUITEM_MULTISELECT:
			if (ui::MultiSelect(i.label.c_str(), &cfg->m[i.key], i.items)) {
				if (i.callback != sol::nil)
					i.callback(cfg->m[i.key]);
			}

			dmt(i.key);
			break;
		case MENUITEM_COLORPICKER:
			if (ui::ColorEdit4(i.label.c_str(), cfg->c[i.key])) {
				if (i.callback != sol::nil)
					i.callback(cfg->c[i.key][0] * 255, cfg->c[i.key][1] * 255, cfg->c[i.key][2] * 255, cfg->c[i.key][3] * 255);
			}

			dmt(i.key);
			break;
		case MENUITEM_BUTTON:
			if (ui::Button(i.label.c_str())) {
				if (i.callback != sol::nil)
					i.callback();
			}
			break;
		default:
			break;
		}
	}
}

void c_menu::draw() {
	if (!this->m_bIsOpened && ui::GetStyle().Alpha > 0.f) {
		float fc = 255.f / 0.2f * ui::GetIO().DeltaTime;
		ui::GetStyle().Alpha = std::clamp(ui::GetStyle().Alpha - fc / 255.f, 0.f, 1.f);
	}

	if (this->m_bIsOpened && ui::GetStyle().Alpha < 1.f) {
		float fc = 255.f / 0.2f * ui::GetIO().DeltaTime;
		ui::GetStyle().Alpha = std::clamp(ui::GetStyle().Alpha + fc / 255.f, 0.f, 1.f);
	}

	if (!this->m_bIsOpened && ui::GetStyle().Alpha == 0.f)
		return;

	ui::SetNextWindowSizeConstraints(ImVec2(750, 450), ImVec2(4096, 4096));
	ui::Begin("gideonproject", 0, ImGuiWindowFlags_NoTitleBar);

	ui::TabButton("Ragebot", &this->m_nCurrentTab, 0, 7);
	ui::TabButton("Anti-aims", &this->m_nCurrentTab, 1, 7);
	ui::TabButton("Legitbot", &this->m_nCurrentTab, 2, 7);
	ui::TabButton("Visuals", &this->m_nCurrentTab, 3, 7);
	ui::TabButton("Misc", &this->m_nCurrentTab, 4, 7);
	ui::TabButton("Skins", &this->m_nCurrentTab, 5, 7);
	ui::TabButton("Players", &this->m_nCurrentTab, 6, 7);

	static auto calculateChildWindowPosition = [](int num) -> ImVec2 {
		return ImVec2(ui::GetWindowPos().x + 26 + (ui::GetWindowSize().x / 3 - 31) * num + 20 * num, ui::GetWindowPos().y + 52);
	};

	static auto calculateChildWindowPositionDouble = [](int num) -> ImVec2 {
		return ImVec2(ui::GetWindowPos().x + 26 + (ui::GetWindowSize().x / 2 - 36) * num + 20 * num, ui::GetWindowPos().y + 52);
	};

	auto child_size = ImVec2(ui::GetWindowSize().x / 3 - 31, ui::GetWindowSize().y - 80);
	auto child_size_d = ImVec2(ui::GetWindowSize().x / 2 - 36, ui::GetWindowSize().y - 80);
	auto cfg = c_config::get();

	if (this->m_nCurrentTab == 0) {
		ui::SetNextWindowPos(calculateChildWindowPosition(0));
		ui::BeginChild("Aimbot", child_size);
		ui::Checkbox("Enable aimbot", &cfg->b["rage_aimbot"]); dmt("rage_aimbot");
		ui::Keybind("aimbot", &cfg->i["rage_aimbot_key"], &cfg->i["rage_aimbot_keystyle"]); dmt("rage_aimbot_key | rage_aimbot_keystyle");

		ui::SingleSelect("Selection mode", &cfg->i["rage_selectionmode"], { "FOV", "Health", "Distance", "Highest damage" }); dmt("rage_selectionmode");

		ui::Checkbox("Aim lock", &cfg->b["rage_aimlock"]); dmt("rage_aimlock");
		ui::Checkbox("Friendly fire", &cfg->b["rage_friendlyfire"]); dmt("rage_friendlyfire");
		ui::Checkbox("Automatic fire", &cfg->b["rage_autofire"]); dmt("rage_autofire");
		ui::Checkbox("Automatic penetration", &cfg->b["rage_automaticpenetration"]); dmt("rage_automaticpenetration");

		if (cfg->b["rage_automaticpenetration"])
			ui::SliderInt("##penedamage", &cfg->i["rage_automaticpenetration_damage"], 0, 100, cfg->i["rage_automaticpenetration_damage"] == 0 ? "Auto" : "%dhp"); dmt("rage_automaticpenetration_damage");

		ui::Checkbox("Silent aim", &cfg->b["rage_silentaim"]); dmt("rage_silentaim");
		ui::Checkbox("Minimal hitchance", &cfg->b["rage_hitchance"]); dmt("rage_hitchance");

		if (cfg->b["rage_hitchance"])
			ui::SliderInt("##mindamage", &cfg->i["rage_hitchance_value"], 1, 100, cfg->i["rage_hitchance_value"] == 0 ? "Off" : "%d%%"); dmt("rage_hitchance_value");

		ui::SliderInt("Minimal damage", &cfg->i["rage_minimaldamage"], 0, 126, cfg->i["rage_minimaldamage"] == 0 ? "Auto" : (cfg->i["rage_minimaldamage"] > 100 ? "HP+%d" : "%dhp"), cfg->i["rage_minimaldamage"] > 100 ? 100 : 0); dmt("rage_minimaldamage");
		ui::Checkbox("Scale damage on hp", &cfg->b["rage_baimonlowhp"]); dmt("rage_baimonlowhp");
		ui::Checkbox("Override AWP", &cfg->b["rage_awpbodyaim"]); dmt("rage_awpbodyaim");
		ui::Checkbox("Automatic scope", &cfg->b["rage_autoscope"]); dmt("rage_autoscope");
		ui::Checkbox("Reduce aimstep", &cfg->b["rage_aimstep"]); dmt("rage_aimstep");
		ui::SliderInt("Maximal FOV", &cfg->i["rage_fov"], 0, 180, u8"%d°"); dmt("rage_fov");

		draw_lua_items("ragebot", "aimbot");
		ui::EndChild();

		ui::SetNextWindowPos(calculateChildWindowPosition(1));
		ui::BeginChild("Target point selection", child_size);
		ui::MultiSelect("##targetpoints", &cfg->m["rage_points"], { "Head", "Neck", "Chest", "Stomach", "Arms", "Legs" }); dmt("rage_points");

		if (cfg->m["rage_points"][0]) {
			ui::SliderInt("Head", &cfg->i["rage_headselection_scale"], 0, 100, "%d%%");
			dmt("rage_headselection_scale");
		}

		if (cfg->m["rage_points"][1]) {
			ui::SliderInt("Neck", &cfg->i["rage_neckselection_scale"], 0, 100, "%d%%");
			dmt("rage_neckselection_scale");
		}

		if (cfg->m["rage_points"][2]) {
			ui::SliderInt("Chest", &cfg->i["rage_chestselection_scale"], 0, 100, "%d%%");
			dmt("rage_chestselection_scale");
		}

		if (cfg->m["rage_points"][3]) {
			ui::SliderInt("Stomach", &cfg->i["rage_pelvisselection_scale"], 0, 100, "%d%%");
			dmt("rage_pelvisselection_scale");
		}

		if (cfg->m["rage_points"][4]) {
			ui::SliderInt("Arms", &cfg->i["rage_armsselection_scale"], 0, 100, "%d%%");
			dmt("rage_armsselection_scale");
		}

		if (cfg->m["rage_points"][5]) {
			ui::SliderInt("Legs", &cfg->i["rage_legsselection_scale"], 0, 100, "%d%%");
			dmt("rage_legsselection_scale");
		}

		ui::MultiSelect("Low FPS mitigations", &cfg->m["rage_lowfps_fix"], { "Reduce penetration", "Reduce multipoints", "Reduce hitbox count", "Force low fake-lag fix" });

		draw_lua_items("ragebot", "target point selection");
		ui::EndChild();

		ui::SetNextWindowPos(calculateChildWindowPosition(2));
		ui::BeginChild("Accuracy", child_size);
		ui::Checkbox("Remove recoil", &cfg->b["rage_removerecoil"]); dmt("rage_removerecoil");
		ui::Checkbox("Quick stop", &cfg->b["rage_quickstop"]); dmt("rage_quickstop");
		ui::Keybind("stop", &cfg->i["rage_quickstop_key"], &cfg->i["rage_quickstop_keystyle"]); dmt("rage_quickstop_key | rage_quickstop_keystyle");
		ui::Checkbox("Resolver", &cfg->b["rage_resolver"]); dmt("rage_resolver");
		ui::SingleSelect("Fake-lag correction", &cfg->i["rage_fakelagfix"], { "Off", "Low", "Medium", "High" }); dmt("rage_fakelagfix");

		if (cfg->m["rage_points"][3])
			ui::SingleSelect("Body-aim preference", &cfg->i["rage_baimpreference"], { "Off", "On moving", "Adaptive" }); dmt("rage_baimpreference");

		ui::Text("Force body-aim");
		ui::Keybind("##forcebaim", &cfg->i["rage_forcebaim_key"], &cfg->i["rage_forcebaim_keystyle"]); dmt("rage_forcebaim_key | rage_forcebaim_keystyle");

		draw_lua_items("ragebot", "accuracy");
		ui::EndChild();
	}

	if (this->m_nCurrentTab == 1) {
		ui::SetNextWindowPos(calculateChildWindowPositionDouble(0));
		ui::BeginChild("Anti-aimbot angles", child_size_d);

		ui::SingleSelect("Pitch", &cfg->i["rage_antiaim_pitch"], { "Off", "Default", "Fake Up", "Minimal" }); dmt("rage_antiaim_pitch");
		ui::SingleSelect("Yaw", &cfg->i["rage_antiaim_yaw"], { "Off", "180", "Jitter", "Static" }); dmt("rage_antiaim_yaw");
		ui::SliderInt("##yawaddy", &cfg->i["rage_antiaim_yaw_custom"], -180, 180, u8"%d°"); dmt("rage_antiaim_yaw_custom");
		ui::SliderInt("Jitter min", &cfg->i["rage_antiaim_yaw_jitter_min"], -90, 90, u8"%d°"); dmt("rage_antiaim_yaw_jitter_min");
		ui::SliderInt("Jitter max", &cfg->i["rage_antiaim_yaw_jitter_max"], -90, 90, u8"%d°"); dmt("rage_antiaim_yaw_jitter_max");

		ui::SingleSelect("Yaw move", &cfg->i["rage_antiaim_yaw_move"], { "Off", "180", "Jitter", "Static" }); dmt("rage_antiaim_yaw_move");
		ui::SliderInt("##yawaddymov", &cfg->i["rage_antiaim_yaw_move_custom"], -180, 180, u8"%d°"); dmt("rage_antiaim_yaw_move_custom");
		ui::SliderInt("Move jitter min", &cfg->i["rage_antiaim_yaw_move_jitter_min"], -90, 90, u8"%d°"); dmt("rage_antiaim_yaw_move_jitter_min");
		ui::SliderInt("Move jitter max", &cfg->i["rage_antiaim_yaw_move_jitter_max"], -90, 90, u8"%d°"); dmt("rage_antiaim_yaw_move_jitter_max");
		
		ui::Checkbox("Freestanding", &cfg->b["rage_antiaim_freestand"]);dmt("rage_antiaim_freestand");
		if (cfg->b["rage_antiaim_freestand"]) {
			ui::MultiSelect("Freestanding conditions", &cfg->m["rage_antiaim_freestand"], { "While standing", "While moving", "While jumping" });dmt("rage_antiaim_freestand");
			ui::SliderInt("##yawfreestandadd", &cfg->i["rage_antiaim_yaw_freestand_add"], -60, 60, u8"%d°");dmt("rage_antiaim_yaw_freestand_add");
		}

		ui::SingleSelect("Fake yaw", &cfg->i["rage_antiaim_desync"], { "Off", "Right", "Left" }); dmt("rage_antiaim_desync");
		ui::SliderInt("##fake shit", &cfg->i["rage_antiaim_desync_limit"], 0, 56, u8"%d°"); dmt("rage_antiaim_desync_limit");

		ui::Checkbox("Slow motion", &cfg->b["rage_slowmotion"]);dmt("rage_slowmotion");
		ui::Keybind("slowmotion", &cfg->i["rage_slowmotion_key"], &cfg->i["rage_slowmotion_keystyle"]);dmt("rage_slowmotion_key | rage_slowmotion_keystyle");

		if (cfg->b["rage_slowmotion"]) {
			ui::SliderInt("##speed", &cfg->i["rage_slowmotion_speed"], 0, 100, "%d%%");
			dmt("rage_slowmotion_speed");
		}

		ui::Checkbox("Fake-duck", &cfg->b["rage_fakeduck"]); dmt("rage_fakeduck");
		ui::Keybind("bottlesitter", &cfg->i["rage_fakeduck_key"], &cfg->i["rage_fakeduck_keystyle"]); dmt("rage_fakeduck_key | rage_fakeduck_keystyle");

		draw_lua_items("anti-aims", "anti-aimbot angles");
		ui::EndChild();

		ui::SetNextWindowPos(calculateChildWindowPositionDouble(1));
		ui::BeginChild("Fake-lag settings", child_size_d);

		ui::SingleSelect("Fake-lag style", &cfg->i["rage_fakelag"], { "Off", "Maximal", "Dynamic" });dmt("rage_fakelag");
		ui::MultiSelect("Conditions", &cfg->m["rage_fakelag_conds"], { "While standing", "While moving", "While shooting" });dmt("rage_fakelag_conds");
		ui::SliderInt("Limit", &cfg->i["rage_fakelag_limit"], 0, 17, "%d");dmt("rage_fakelag_limit"); //3
		ui::SliderInt("Variance", &cfg->i["rage_fakelag_variance"], 0, 100, "%d%%");dmt("rage_fakelag_variance");

		draw_lua_items("anti-aims", "fake-lag settings");
		ui::EndChild();
	}

	// визуалы *-*
	if (this->m_nCurrentTab == 3) {
		ui::SetNextWindowPos(calculateChildWindowPosition(0));
		ui::BeginChild("Player ESP", child_size);

		ui::Checkbox("Teammates", &cfg->b["vis_teammates"]); dmt("vis_teammates");
		ui::Checkbox("Self", &cfg->b["vis_self"]); dmt("vis_self");
		ui::Checkbox("Dormant", &cfg->b["vis_dormant"]); dmt("vis_dormant");
		ui::Checkbox("Hide ESP on distance", &cfg->b["vis_hideespdistance"]); dmt("vis_hideespdistance");
		ui::SingleSelect("Bounding box", &cfg->i["vis_boundingbox"], { "Off", "Static", "Dynamic" }); dmt("vis_boundingbox");
		ui::ColorEdit4("##bboxcolor", cfg->c["vis_boundingbox_color"]); dmt("vis_boundingbox_color");
		ui::SliderInt("ESP draw distance", &cfg->i["vis_maximalespdistance"], 10, 500, cfg->i["vis_maximalespdistance"] == 500 ? "Map" : "%dft"); dmt("vis_maximalespdistance");
		ui::Checkbox("Health bar", &cfg->b["vis_healthbar"]); dmt("vis_healthbar");
		ui::Checkbox("Armor", &cfg->b["vis_armor"]); dmt("vis_armor");
		ui::Checkbox("Latency", &cfg->b["vis_latency"]); dmt("vis_latency");
		ui::Checkbox("Name", &cfg->b["vis_name"]); dmt("vis_name");
		ui::ColorEdit4("##namecol", cfg->c["vis_name_color"]); dmt("vis_name_color");
		ui::Checkbox("Flags", &cfg->b["vis_flags"]); dmt("vis_flags"); 
		ui::Checkbox("Weapons", &cfg->b["vis_weapons"]); dmt("vis_weapons"); 
		ui::Checkbox("Ammo", &cfg->b["vis_ammo"]); dmt("vis_ammo");
		ui::ColorEdit4("##ammocol", cfg->c["vis_ammo_color"]); dmt("vis_ammo_color"); 
		ui::Checkbox("Distance", &cfg->b["vis_distance"]); dmt("vis_distance");
		ui::Checkbox("Money", &cfg->b["vis_money"]); dmt("vis_money"); 
		ui::Checkbox("Offscreen ESP", &cfg->b["vis_outoffovarrows"]); dmt("vis_outoffovarrows"); 
		ui::ColorEdit4("##offscreencol", cfg->c["vis_outoffovarrows_color"]); dmt("vis_outoffovarrows_color");

		if (cfg->b["vis_outoffovarrows"]) {
			ui::SliderInt("Offset", &cfg->i["vis_outoffovarrows_dist"], 0, 480, "%dpx"); dmt("vis_outoffovarrows_dist"); 
			ui::SliderInt("Size", &cfg->i["vis_outoffovarrows_size"], 20, 58, "%dpx"); dmt("vis_outoffovarrows_size"); 
		}

		ui::SingleSelect("Glow", &cfg->i["vis_glow"], { "Off", "Default", "Pulsating" }); dmt("vis_glow"); 
		ui::ColorEdit4("##glowcol", cfg->c["vis_glow_color"]); dmt("vis_glow_color");

		if (cfg->b["vis_self"]) {
			ui::SingleSelect("Self glow", &cfg->i["vis_glowself"], { "Off", "Default", "Pulsating" }); dmt("vis_glowself");
			ui::ColorEdit4("##glowselfcol", cfg->c["vis_glowself_color"]); dmt("vis_glowself_color");
		}

		ui::Checkbox("Hitmarker", &cfg->b["vis_hitmarker"]); dmt("vis_hitmarker"); 
		ui::SingleSelect("Hitsound", &cfg->i["vis_hitsound"], { "Off", "Default", "Metallic", "Flick", "Bubble", "Meme", "Pain" }); dmt("vis_hitsound");
		ui::Checkbox("Visualize sounds", &cfg->b["vis_soundesp"]); dmt("vis_soundesp");
		ui::ColorEdit4("##soundespcol", cfg->c["vis_soundesp_color"]); dmt("vis_soundesp_color");

		draw_lua_items("visuals", "player esp");
		ui::EndChild();

		ui::SetNextWindowPos(calculateChildWindowPosition(1));
		ui::BeginChild("Effects", child_size);

		ui::SingleSelect("Colored models type", &cfg->i["vis_chams"], {"Default", "Flat", "Shiny"}); dmt("vis_chams");
		ui::Checkbox("Enemies", &cfg->b["vis_chamenemy"]); dmt("vis_chamenemy");
		ui::ColorEdit4("##enemiescol", cfg->c["vis_chamenemy_color"]); dmt("vis_chamenemy_color");
		ui::Checkbox("Enemies XQZ", &cfg->b["vis_chamenemy_xqz"]); dmt("vis_chamenemy_xqz");
		ui::ColorEdit4("##enemies2col", cfg->c["vis_chamenemy_xqz_color"]); dmt("vis_chamenemy_xqz_color");
		ui::Checkbox("Shadow", &cfg->b["vis_chamshadow"]); dmt("vis_chamshadow"); 
		ui::ColorEdit4("##enemies3col", cfg->c["vis_chamshadow_color"]); dmt("vis_chamshadow_color");
		ui::Checkbox("Teammates", &cfg->b["vis_chamally"]); dmt("vis_chamally"); 
		ui::ColorEdit4("##enemies4col", cfg->c["vis_chamally_color"]); dmt("vis_chamally_color");
		ui::Checkbox("Remove teammates", &cfg->b["vis_dontrenderteam"]); dmt("vis_dontrenderteam");
		ui::Checkbox("Self", &cfg->b["vis_chamself"]); dmt("vis_chamself");
		ui::ColorEdit4("##enemies5col", cfg->c["vis_chamself_color"]); dmt("vis_chamself_color");
		ui::Checkbox("Self shadow", &cfg->b["vis_fakeangles"]);dmt("vis_fakeangles");
		ui::ColorEdit4("##enemies6col", cfg->c["vis_fakeangles_color"]);dmt("vis_fakeangles_color");
		ui::Checkbox("Lag shadow", &cfg->b["vis_fakeposition"]);dmt("vis_fakeposition");
		ui::ColorEdit4("##enemies9col", cfg->c["vis_fakeposition_color"]);dmt("vis_fakeposition_color");
		ui::SliderInt("Alpha in scope", &cfg->i["vis_alphainscope"], 0, 255, "%d");dmt("vis_alphainscope");

		ui::Checkbox("Remove flash effect", &cfg->b["vis_noflash"]);dmt("vis_noflash");
		ui::Checkbox("Remove smoke effect", &cfg->b["vis_nosmoke"]);dmt("vis_nosmoke");
		ui::Checkbox("Remove visual recoil", &cfg->b["vis_novisrecoil"]);dmt("vis_novisrecoil");
		ui::Checkbox("Remove zoom", &cfg->b["vis_nozoom"]);dmt("vis_nozoom");
		ui::SliderInt("Walls transparency", &cfg->i["vis_wallstransp"], 0, 100, "%d%%");dmt("vis_wallstransp");
		ui::SliderInt("Props transparency", &cfg->i["vis_propstransp"], 0, 100, "%d%%");dmt("vis_propstransp");
		ui::Checkbox("Night mode", &cfg->b["vis_nightmode"]);dmt("vis_nightmode");

		if (cfg->b["vis_nightmode"]) {
			ui::SliderInt("##brightness", &cfg->i["vis_brightness"], 0, 100, "%d%%");dmt("vis_brightness");
		}

		ui::SingleSelect("Remove scope", &cfg->i["vis_noscope"], { "Off", "Default", "Fullscreen", "Clear" }); dmt("vis_noscope");
		ui::Checkbox("Disable post processing", &cfg->b["vis_nopostprocess"]);dmt("vis_nopostprocess");
		ui::Checkbox("Thirdperson", &cfg->b["vis_thirdperson"]);dmt("vis_thirdperson");
		ui::Keybind("tpkey", &cfg->i["vis_thirdperson_key"], &cfg->i["vis_thirdperson_keystyle"]);dmt("vis_thirdperson_key | vis_thirdperson_keystyle");
		ui::Checkbox("Bullet tracers", &cfg->b["vis_bullettracer"]);dmt("vis_bullettracer");
		ui::ColorEdit4("##buletcol", cfg->c["vis_bullettracer_color"]);dmt("vis_bullettracer_color");
		ui::Checkbox("Bullet impacts", &cfg->b["vis_bulletimpacts"]);dmt("vis_bulletimpacts");

		draw_lua_items("visuals", "effects");
		ui::EndChild();

		ui::SetNextWindowPos(calculateChildWindowPosition(2));
		ui::BeginChild("Other", child_size);
		ui::MultiSelect("Dropped weapons", &cfg->m["vis_droppedwpns"], { "Text", "Glow", "Box", "Ammo" });dmt("vis_droppedwpns");
		ui::ColorEdit4("##droppedcol", cfg->c["vis_droppedwpns_color"]);dmt("vis_droppedwpns_color");
		ui::Checkbox("Projectiles", &cfg->b["vis_grenades"]);dmt("vis_grenades");
		ui::ColorEdit4("##nadescol", cfg->c["vis_grenades_color"]);dmt("vis_grenades_color");
		ui::SingleSelect("Spread crosshair", &cfg->i["vis_inaccuracyoverlay"], { "Disabled", "Gradient", "Rainbow" });dmt("vis_inaccuracyoverlay");
		ui::ColorEdit4("##spreadcol", cfg->c["vis_inaccuracyoverlay_color"]);dmt("vis_inaccuracyoverlay_color");
		ui::Checkbox("Recoil crosshair", &cfg->b["vis_recoiloverlay"]);dmt("vis_recoiloverlay");
		ui::Checkbox("Force crosshair", &cfg->b["vis_crosshair"]);dmt("vis_crosshair");
		ui::Checkbox("Bomb overlay", &cfg->b["vis_bomb"]);dmt("vis_bomb");
		ui::ColorEdit4("##bombovercol", cfg->c["vis_bomb_color"]);dmt("vis_bomb_color");
		ui::Checkbox("Grenade trajectory", &cfg->b["vis_grenadetrajectory"]);dmt("vis_grenadetrajectory");
		ui::Checkbox("Spectators", &cfg->b["vis_spectators"]);dmt("vis_spectators");
		ui::Checkbox("Penetration crosshair", &cfg->b["vis_penetrationreticle"]);dmt("vis_penetrationreticle");
		ui::Checkbox("Preserve killfeed", &cfg->b["vis_preservekillfeed"]);dmt("vis_preservekillfeed");

		draw_lua_items("visuals", "other");
		ui::EndChild();
	}

	if (this->m_nCurrentTab == 4) {
		ui::SetNextWindowPos(calculateChildWindowPosition(0));
		ui::BeginChild("Miscellaneous", child_size);

		ui::SliderInt("Override FOV", &cfg->i["misc_overridefov"], 48, 148);dmt("misc_overridefov");
		ui::Checkbox("Auto-jump", &cfg->b["misc_autojump"]);dmt("misc_autojump");

		if (cfg->b["misc_autojump"]) {
			ui::SingleSelect("Air-strafe", &cfg->i["misc_airstrafe"], { "Off", "Legit", "Rage" });
			dmt("misc_airstrafe");
		}

		ui::Checkbox("Circle-strafe", &cfg->b["misc_circlestrafe"]); dmt("misc_circlestrafe");
		ui::Keybind("circlestrafe", &cfg->i["misc_circlestrafe_key"], &cfg->i["misc_circlestrafe_keystyle"]); dmt("misc_circlestrafe_key | misc_circlestrafe_keystyle");
		ui::Checkbox("Air-duck", &cfg->b["misc_airduck"]); dmt("misc_airduck");
		ui::Checkbox("Slide-walk", &cfg->b["misc_slidewalk"]); dmt("misc_slidewalk");
		ui::Checkbox("Knife-bot", &cfg->b["misc_knifebot"]); dmt("misc_knifebot");
		ui::Checkbox("Auto-reload weapons", &cfg->b["misc_automaticwpns"]); dmt("misc_automaticwpns");
		ui::Checkbox("Reveal MM ranks", &cfg->b["misc_revealranks"]); dmt("misc_revealranks");
		ui::Checkbox("Auto-accept MM", &cfg->b["misc_autoacceptmm"]); dmt("misc_autoacceptmm");
		ui::Checkbox("Clan-tag spammer", &cfg->b["misc_clantagspammer"]); dmt("misc_clantagspammer");
		ui::Checkbox("Log events", &cfg->b["misc_logevents"]); dmt("misc_logevents");

		ui::Checkbox("Lag exploit (MM only)", &cfg->b["misc_lagexploit"]); dmt("misc_lagexploit");
		ui::Keybind("lagexploit", &cfg->i["misc_lagexploit_key"], &cfg->i["misc_lagexploit_keystyle"]); dmt("misc_lagexploit_key | misc_lagexploit_keystyle");

		if (cfg->b["misc_logevents"]) {
			ui::Checkbox("Log damage", &cfg->b["misc_showdamage"]); dmt("misc_showdamage");
			ui::Checkbox("Log buy log", &cfg->b["misc_showbuylog"]); ("misc_showbuylog");
			ui::Checkbox("Log aimbot info", &cfg->b["misc_showaiminfo"]); dmt("misc_showaiminfo");
			ui::Checkbox("Show prefix", &cfg->b["misc_showprefix"]); dmt("misc_showprefix");
			ui::ColorEdit4("##showprefixcolor", cfg->c["misc_prefixcolor"]); dmt("misc_prefixcolor");
		}

		ui::Checkbox("Infinite duck", &cfg->b["misc_infiniteduck"]); dmt("misc_infiniteduck");
		ui::Checkbox("Ping spike", &cfg->b["misc_pingspike"]); dmt("misc_pingspike");
		ui::Keybind("pingspike", &cfg->i["misc_pingspike_key"], &cfg->i["misc_pingspike_keystyle"]); dmt("misc_pingspike_key | misc_pingspike_keystyle");

		if (cfg->b["misc_pingspike"])
			ui::SliderInt("Latency", &cfg->i["misc_pingspike_val"], 0, 250);

		draw_lua_items("misc", "miscellaneous");
		ui::EndChild();

		ui::SetNextWindowPos(calculateChildWindowPosition(1));
		ui::BeginChild("Cheat settings", child_size);

		ui::Text("Menu key");
		ui::Keybind("menukey", &cfg->i["misc_menukey"]);dmt("misc_menukey");
		ui::Text("Menu color");
		ui::ColorEdit4("##menucolor", cfg->c["menu_color"]);dmt("menu_color");
		ui::Checkbox("Low FPS warning", &cfg->b["misc_lowfpswarning"]);dmt("misc_lowfpswarning");
		ui::Checkbox("Anti-untrusted", &cfg->b["misc_antiuntrusted"]);dmt("misc_antiuntrusted");

		ui::SingleSelect("Preset", &cfg->i["_preset"], c_config::get()->presets);dmt("_preset");
		ui::Keybind("##presetkey", &cfg->i["_preset_" + std::to_string(cfg->i["_preset"])]);dmt("_preset_" + std::to_string(cfg->i["_preset"]));

		if (ui::Button("Load"))
			c_config::get()->load();
		if (ui::Button("Save"))
			c_config::get()->save();
		if (ui::Button("Reset")) {
			auto res = LI_FN(MessageBoxA).get()(0, "Are you sure?", "Reset warning", MB_APPLMODAL | MB_ICONWARNING | MB_YESNO);
			
			if (res == IDYES)
				c_config::get()->load_defaults();
		}
		if (ui::Button("Open settings folder"))
			ShellExecuteA(0, "open", "C:/gideonproject", NULL, NULL, SW_NORMAL);
		if (ui::Button("Quit"))
			exit(0);

		draw_lua_items("misc", "cheat settings");
		ui::EndChild();

		ui::SetNextWindowPos(calculateChildWindowPosition(2));
		ui::BeginChild("Lua extensions", child_size);

		ui::Checkbox("Developer mode", &cfg->b["lua_devmode"]); dmt("lua_devmode");
		if (ui::Button("Refresh scripts"))
			c_lua::get()->refresh_scripts();
		if (ui::Button("Reload active scripts"))
			c_lua::get()->reload_all_scripts();
		if (ui::Button("Unload all"))
			c_lua::get()->unload_all_scripts();

		ui::ListBoxHeader("##urnn", ImVec2(0, 160));

		for (auto s : c_lua::get()->scripts)
		{
			if (ui::Selectable(s.c_str(), c_lua::get()->loaded.at(c_lua::get()->get_script_id(s)), NULL, ImVec2(0, 0))) {
				auto scriptId = c_lua::get()->get_script_id(s);

				if (c_lua::get()->loaded.at(scriptId))
					c_lua::get()->unload_script(scriptId);
				else
					c_lua::get()->load_script(scriptId);
			}
		}

		ui::ListBoxFooter();

		draw_lua_items("misc", "lua extensions");
		ui::EndChild();
	}

	if (this->m_nCurrentTab == 6) {
		auto players = c_playerlist::get()->get_players();

		ui::SetNextWindowPos(calculateChildWindowPositionDouble(0));
		ui::BeginChild("Player list", child_size_d);

		ui::Checkbox("Everyone", &cfg->b["plist_everyone"]); dmt("plist_everyone");

		static char uname[128];
		ui::InputText("Search", uname, 128);

		ui::ListBoxHeader("##players", ImVec2(0, ui::GetWindowSize().y - 80));

		for (auto p : players) {
			auto search = std::string(uname);
			auto name = std::string(p.name);

			std::transform(search.begin(), search.end(), search.begin(), ::tolower);
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);

			if (search != "" && name.find(search) == std::string::npos)
				continue;

			if (ui::Selectable(p.name, cfg->i["plist_selected"] == p.userid))
				cfg->i["plist_selected"] = p.userid;
		}

		ui::ListBoxFooter();

		draw_lua_items("players", "player list");
		ui::EndChild();

		ui::SetNextWindowPos(calculateChildWindowPositionDouble(1));
		ui::BeginChild("Override settings", child_size_d);

		if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected()) {
			auto key = "plist_" + c_playerlist::get()->get_steam_id(players, cfg->i["plist_selected"]) + "_";

			ui::Checkbox("Friend", &cfg->b[key + "friend"]); dmt(key + "friend");
			ui::Checkbox("Disable resolver", &cfg->b[key + "noresolver"]); dmt(key + "noresolver");
			ui::Checkbox("Pitch", &cfg->b[key + "pitch"]); dmt(key + "pitch");

			if (cfg->b[key + "pitch"]) {
				ui::SliderInt("##custompitch", &cfg->i[key + "pitchval"], -90, 90, u8"%d°"); dmt(key + "pitchval");
			}

			ui::Checkbox("Yaw", &cfg->b[key + "yaw"]); dmt(key + "yaw");

			if (cfg->b[key + "yaw"]) {
				ui::SliderInt("##customyaw", &cfg->i[key + "yawval"], -180, 180, u8"%d°"); dmt(key + "yawval");
				ui::SingleSelect("Type", &cfg->i[key + "yawtype"], { "Add", "Set", "Subtract" }); dmt(key + "yawtype");
			}

			ui::Checkbox("Prefer body-aim", &cfg->b[key + "baim"]); dmt(key + "baim");
			ui::Checkbox("Disable visuals", &cfg->b[key + "novis"]); dmt(key + "novis");

			if (ui::Button("Reset all")) {
				for (auto p : players) {
					key = "plist_" + c_playerlist::get()->get_steam_id(players, p.userid) + "_";

					cfg->b[key + "friend"] = false;
					cfg->b[key + "noresolver"] = false;
					cfg->b[key + "pitch"] = false;
					cfg->i[key + "pitchval"] = 0;
					cfg->b[key + "yaw"] = false;
					cfg->i[key + "yawval"] = 0;
					cfg->i[key + "yawtype"] = 0;
					cfg->b[key + "baim"] = false;
					cfg->b[key + "novis"] = false;
				}
			}

			if (ui::Button("Apply to all")) {
				for (auto p : players) {
					auto key_temp = "plist_" + c_playerlist::get()->get_steam_id(players, p.userid) + "_";

					cfg->b[key_temp + "friend"] = cfg->b[key + "friend"];
					cfg->b[key_temp + "noresolver"] = cfg->b[key + "noresolver"];
					cfg->b[key_temp + "pitch"] = cfg->b[key + "pitch"];
					cfg->i[key_temp + "pitchval"] = cfg->i[key + "pitchval"];
					cfg->b[key_temp + "yaw"] = cfg->b[key + "yaw"];
					cfg->i[key_temp + "yawval"] = cfg->i[key + "yawval"];
					cfg->i[key_temp + "yawtype"] = cfg->i[key + "yawtype"];
					cfg->b[key_temp + "baim"] = cfg->b[key + "baim"];
					cfg->b[key_temp + "novis"] = cfg->b[key + "novis"];
				}
			}

			draw_lua_items("players", "override settings");
		}
		else
			ui::Text("Not connected.");

		ui::EndChild();
	}

	ui::End();
}

void c_menu::draw_end() {
	if (!this->m_bInitialized)
		return;

	ui::EndFrame();
	ui::Render();
	ImGui_ImplDX9_RenderDrawData(ui::GetDrawData());
}

bool c_menu::is_menu_initialized() {
	return this->m_bInitialized;
}

bool c_menu::is_menu_opened() {
	return this->m_bIsOpened;
}

void c_menu::set_menu_opened(bool v) {
	this->m_bIsOpened = v;
}

IDirect3DTexture9* c_menu::get_texture_data() {
	return this->m_pTexture;
}

ImColor c_menu::get_accent_color() {
	return ImColor(
		c_config::get()->c["menu_color"][0],
		c_config::get()->c["menu_color"][1],
		c_config::get()->c["menu_color"][2],
		ui::GetStyle().Alpha
	);
}