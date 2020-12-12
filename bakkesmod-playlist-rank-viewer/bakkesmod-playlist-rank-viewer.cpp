#include "bakkesmod-playlist-rank-viewer.h"

BAKKESMOD_PLUGIN(PlaylistRankViewer, "Playlist Rank Viewer", "1.2", 0)

string prefix = "playlist_rank_viewer_";

string enabledName = prefix + "enabled";
string enabledBinding = "LeftShift+N";

string nextPlayerName = prefix + "next_player";
string nextPlayerBinding = "N";


void PlaylistRankViewer::log(std::string str) {
	cvarManager->log(str);
}

void PlaylistRankViewer::onLoad() {
	cvarManager->registerCvar(enabledName, "1", "Show ranks from competitive playlists", true, true, 0, true, 1, true)
		.addOnValueChanged([self = this](string oldValue, CVarWrapper cvar) {
			self->setEnabled(oldValue == "0");
			if (self->isEnabled()) {
				// Reset MMR cache when the setting is enabled
				self->resetMmrCache();
			}
		});

	// Apparently this wont run unless it happens a bit later
	gameWrapper->SetTimeout([self = this](GameWrapper *gw) {
		self->log("enabled: " + to_string(self->cvarManager->getCvar(enabledName).getBoolValue()));
		if (self->cvarManager->getCvar(enabledName).getBoolValue()) {
			self->setEnabled(true);
		}
	}, 5);

	cvarManager->registerNotifier(enabledName, [self = this](vector<string>) {
		self->setEnabled(!self->isEnabled());
		self->cvarManager->getCvar(enabledName).setValue(self->isEnabled());
	}, "Toggle plguin", PERMISSION_ALL);

	cvarManager->setBind(enabledBinding, enabledName);
	cvarManager->setBind(nextPlayerBinding, nextPlayerName);

	gameWrapper->RegisterDrawable(std::bind(&PlaylistRankViewer::render, this, placeholders::_1));
	
	// Show the next users MMR stats
	cvarManager->registerNotifier(nextPlayerName, [self = this](vector<string>) {
		if (!self->isEnabled()) {
			return;
		}
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

	gameWrapper->SetTimeout([self = this](GameWrapper *gw) {
		self->timeoutCallback(gw);
	}, 0);
}

float callbackEvery = 30;
void PlaylistRankViewer::timeoutCallback(GameWrapper* _) {
	this->resetMmrCache();
	gameWrapper->SetTimeout([self = this](GameWrapper *gw) {
		self->timeoutCallback(gw);
	}, callbackEvery);
}

void PlaylistRankViewer::onUnload() {
	currentPlayer = 0;
	playerMmrs.clear();
}

// Update the MMR map for the user if it isnt updated yet
void PlaylistRankViewer::updatePlayerMmr(UniqueIDWrapper uniqueId) {
	MMRWrapper mmrWrapper = gameWrapper->GetMMRWrapper();

	long long uID = uniqueId.GetUID();

	// long long steamId = id.ID;
	if (playerMmrs.count(uID) == 0 || playerMmrs[uID].size() == 0) {
		playerMmrs[uID] = {};
		for (auto playlist : playlistsToCheck) {
			string mmr = to_string(mmrWrapper.GetPlayerMMR(uniqueId, playlist));
			string gamesPlayed = to_string(mmrWrapper.GetPlayerRank(uniqueId, playlist).MatchesPlayed);
			playerMmrs[uID][playlist] = mmr + " (" + gamesPlayed + ")";
		}
	}
}

void PlaylistRankViewer::writeStats(CanvasWrapper& canvas, UniqueIDWrapper uniqueId, string playerName) {
	Vector2 screen = canvas.GetSize();
	
	float nameX = screen.X * .85;
	float nameY = screen.Y * .1;
	float tableX = nameX;
	float tableY = nameY + 50.0;
	long long uID = uniqueId.GetUID();

	// Draw a box for the stats to go into
	canvas.SetColor(100, 100, 100, 150);
	canvas.SetPosition(Vector2{int(nameX - 10), int(nameY - 10)});
	// Estimating the required width and height of the box
	canvas.FillBox(Vector2{int(240), int(70 + (20 * playlistsToCheck.size()))});

	// First draw the players name at the top
	canvas.SetColor(230, 230, 230, 255);
	canvas.SetPosition(Vector2{int(nameX), int(nameY)});
	canvas.DrawString(playerName);

	// For each playlist, draw that playlists stats 20 pixels below the previous row
	for (int i = 0; i < playlistsToCheck.size(); i++) {
		PLAYLIST playlist = playlistsToCheck[i];
		canvas.SetPosition(Vector2{int(tableX) , int(tableY + (20 * i))});
		canvas.DrawString(playlistNames[playlist] + " - " + playerMmrs[uID][playlist]);
	}
}

void PlaylistRankViewer::render(CanvasWrapper canvas) {
	// Only render if the plugin is enabled
	if (!isEnabled()) {
		return;
	}

	if (gameWrapper->IsInOnlineGame()) {
		ServerWrapper server = gameWrapper->GetOnlineGame();

		if (server.GetPRIs().Count() != 0) {
			// Make sure all players have their MMR updated if they werent in the map already
			for (int i = 0; i < server.GetPRIs().Count(); i++) {
				updatePlayerMmr(server.GetPRIs().Get(i).GetUniqueIdWrapper());
			}
			// Write the stats of the current player
			if (currentPlayer >= 0 && currentPlayer < server.GetPRIs().Count()) {
				PriWrapper pri = server.GetPRIs().Get(currentPlayer);
				writeStats(canvas, pri.GetUniqueIdWrapper(), pri.GetPlayerName().ToString());
			}
		}
	}
	else {
		writeStats(canvas, gameWrapper->GetUniqueID(), "You");
	}
}

void PlaylistRankViewer::resetMmrCache() {
	playerMmrs.clear();
	updatePlayerMmr(gameWrapper->GetUniqueID());
	log("Refreshed all MMRs");
}

bool PlaylistRankViewer::isEnabled() {
	return *enabled;
}

void PlaylistRankViewer::setEnabled(bool enabled) {
	this->enabled = make_shared<bool>(enabled);
}