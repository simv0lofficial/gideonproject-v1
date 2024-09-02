#pragma once

struct player_info_t
{
	int64_t __pad0;
	union {
		int64_t xuid; // Steam64
		struct {
			int xuidlow;
			int xuidhigh;
		};
	};
	char name[128];	// Username
	int userid; // UserID
	char guid[33];	// SteamID
	unsigned int friendsid;
	char friendsname[128];
	bool fakeplayer;	// Is BOT
	bool ishltv;	// Is HLTV
	unsigned int customfiles[4];
	unsigned char filesdownloaded;
};