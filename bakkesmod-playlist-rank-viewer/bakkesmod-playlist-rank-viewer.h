#pragma once
#pragma comment( lib, "pluginsdk.lib" )
#include "pch.h"
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
	void writeStats(CanvasWrapper& canvas, UniqueIDWrapper uniqueId, string playerName);
	void resetMmrCache();
	void timeoutCallback(GameWrapper* gw);
	void applySettings();

	map<long long, map<PLAYLIST, PlayerStatStruct>> playerStats = {};
	int currentPlayer = 0;
	shared_ptr<bool> enabled = make_shared<bool>(true);
};
