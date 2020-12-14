#pragma once
#pragma comment( lib, "pluginsdk.lib" )

using namespace std;

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "Enums.h"
#include "RankAndPlaylistData.h"

class PlaylistRankViewer : public BakkesMod::Plugin::BakkesModPlugin {
public:
	virtual void onLoad();
	virtual void onUnload();

	bool isEnabled();

	void setEnabled(bool enabled);
	void log(string str);
	void render(CanvasWrapper canvas);
	void updatePlayerStats(UniqueIDWrapper uniqueId);
	void writeStats(CanvasWrapper& canvas, UniqueIDWrapper uniqueId, string playerName, int teamNum);
	void resetMmrCache();
	void resetMmrCacheTimeoutCallback(GameWrapper* gw);
	void autoCycleTimeoutCallback(GameWrapper* gw);
	void startCycleTimeout();
	void nextPlayer();
	void applySettings();

	map<long long, map<PLAYLIST, PlayerStatStruct>> playerStats = {};
	int currentPlayer = 0;
	shared_ptr<bool> enabled = make_shared<bool>(true);
};
