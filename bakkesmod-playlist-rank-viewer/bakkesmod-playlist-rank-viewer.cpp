#include "bakkesmod-playlist-rank-viewer.h"

BAKKESMOD_PLUGIN(PlaylistRankViewer, "Playlist Rank Viewer", "1.0", 0)

string prefix = "playlist_rank_viewer_";
string enabledName = prefix + "enabled";
string nextPlayerName = prefix + "next_player";


void PlaylistRankViewer::log(std::string str) {
	cvarManager->log("******************* " + str);
}

void PlaylistRankViewer::onLoad() {
	log("PlaylistRankViewer loaded");
	cvarManager->registerCvar(enabledName, "1", "Show ranks from competitive playlists", true, true, 0, true, 1, true);
	gameWrapper->RegisterDrawable(std::bind(&PlaylistRankViewer::render, this, std::placeholders::_1));
	cvarManager->registerNotifier(nextPlayerName, [self = this](vector<string>) {
		ServerWrapper server = self->gameWrapper->GetOnlineGame();
		if (!server.IsNull()) {
			self->log("player is " + to_string(self->currentPlayer) + ", total players: " + to_string(server.GetPRIs().Count()));
			if (self->currentPlayer + 1 >= server.GetPRIs().Count()) {
				self->currentPlayer = 0;
			}
			else {
				self->currentPlayer++;
			}
			self->log("player is now " + to_string(self->currentPlayer));
		}
	}, "View next player in the match", PERMISSION_ONLINE);
}

void PlaylistRankViewer::onUnload() {
	currentPlayer = 0;
	playerMmrs.clear();
}

void PlaylistRankViewer::updatePlayerMmr(SteamID id) {
	MMRWrapper mmr = gameWrapper->GetMMRWrapper();
	long long steamId = id.ID;
	if (playerMmrs.count(steamId) == 0 || playerMmrs[steamId].size() == 0) {
		playerMmrs[steamId] = {};
		for (auto playlist : playlistsToCheck) {
			playerMmrs[steamId][playlist] = mmr.GetPlayerMMR(id, playlist);
		}
	}
}

void PlaylistRankViewer::writeStats(CanvasWrapper& canvas, long long uniqueId, string playerName) {
	Vector2 screen = canvas.GetSize();
	
	float nameX = screen.X * .85;
	float nameY = screen.Y * .1;
	float tableX = nameX;
	float tableY = nameY + 50.0;

	canvas.SetColor(100, 100, 100, 150);
	canvas.SetPosition(Vector2{int(nameX - 10), int(nameY - 10)});
	canvas.FillBox(Vector2{int(210), int(70 + (20 * playlistsToCheck.size()))});

	canvas.SetColor(230, 230, 230, 255);
	canvas.SetPosition(Vector2{int(nameX), int(nameY)});
	canvas.DrawString(playerName);

	for (int i = 0; i < playlistsToCheck.size(); i++) {
		PLAYLIST playlist = playlistsToCheck[i];
		canvas.SetPosition(Vector2{int(tableX) , int(tableY + (20 * i))});
		canvas.DrawString(playlistNames[playlist] + " - " + to_string(playerMmrs[uniqueId][playlist]));
	}
}

void PlaylistRankViewer::render(CanvasWrapper canvas) {
	if (!cvarManager->getCvar(enabledName).getBoolValue()) {
		return;
	}

	if (gameWrapper->IsInOnlineGame()) {
		ServerWrapper server = gameWrapper->GetOnlineGame();

		if (server.GetPRIs().Count() != 0) {
			for (int i = 0; i < server.GetPRIs().Count(); i++) {
				updatePlayerMmr(server.GetPRIs().Get(i).GetUniqueId());
			}
			if (currentPlayer >= 0 && currentPlayer < server.GetPRIs().Count()) {
				PriWrapper pri = server.GetPRIs().Get(currentPlayer);
				writeStats(canvas, pri.GetUniqueId().ID, pri.GetPlayerName().ToString());
			}
		}
	}
	else {
		map<PLAYLIST, float> oldMmr = playerMmrs[gameWrapper->GetSteamID()];
		playerMmrs.clear();
		playerMmrs[gameWrapper->GetSteamID()] = oldMmr;
		updatePlayerMmr({gameWrapper->GetSteamID()});
		writeStats(canvas, gameWrapper->GetSteamID(), "You");
	}
}