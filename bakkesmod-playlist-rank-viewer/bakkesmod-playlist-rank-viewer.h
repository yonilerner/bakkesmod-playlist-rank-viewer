#pragma once
#pragma comment( lib, "pluginsdk.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include <string>
#include <array>

using namespace std;

enum PLAYLIST {
	UNRANKED = 1, //doesn't always work but will when in game, might take a player swap for it to update to the correct mmr
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
	{UNRANKED, "Unranked"},
	{ONES, "Solo"},
	{TWOS, "Doubles"},
	{SOLO_STANDARD, "Solo Standard"},
	{THREES, "Standard"},
	{HOOPS, "Hoops"},
	{RUMBLE, "Rumble"},
	{DROPSHOT, "Dropshot"},
	{SNOWDAY, "Snowday"}
};
array<PLAYLIST, 9> playlistsToCheck = { UNRANKED, ONES, TWOS, THREES, SOLO_STANDARD, HOOPS, RUMBLE, DROPSHOT, SNOWDAY };

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
	void resetMmrCache();
	void timeoutCallback(GameWrapper* gw);

	map<long long, map<PLAYLIST, string>> playerMmrs = {};
	int currentPlayer = 0;
	shared_ptr<bool> enabled = make_shared<bool>(true);
};
