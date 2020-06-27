#pragma once
#pragma comment( lib, "pluginsdk.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include <string>
#include <array>

using namespace std;

enum PLAYLIST {
	ONES = 10,
	TWOS = 11,
	SOLO_STANDARD = 12,
	THREES = 13,
	HOOPS = 27,
	RUMBLE = 28,
	DROPSHOT = 29,
	SNOWDAY = 30
};
map<PLAYLIST, string> playlistNames = {
	{ONES, "Solo"},
	{TWOS, "Doubles"},
	{SOLO_STANDARD, "Solo Standard"},
	{THREES, "Standard"},
	{HOOPS, "Hoops"},
	{RUMBLE, "Rumble"},
	{DROPSHOT, "Dropshot"},
	{SNOWDAY, "Snowday"}
};
array<PLAYLIST, 8> playlistsToCheck = { ONES, TWOS, THREES, SOLO_STANDARD, HOOPS, RUMBLE, DROPSHOT, SNOWDAY };

class PlaylistRankViewer : public BakkesMod::Plugin::BakkesModPlugin {
public:
	virtual void onLoad();
	virtual void onUnload();

	bool isEnabled();
	void setEnabled(bool enabled);

	void log(string str);
	void render(CanvasWrapper canvas);
	void updatePlayerMmr(SteamID id);
	void writeStats(CanvasWrapper& canvas, long long uniqueId, string playerName);

	map<long long, map<PLAYLIST, float>> playerMmrs = {};
	int currentPlayer = 0;
	shared_ptr<bool> enabled = make_shared<bool>(true);
};