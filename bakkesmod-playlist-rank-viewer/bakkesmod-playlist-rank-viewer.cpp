#include "bakkesmod-playlist-rank-viewer.h"
#include "Enums.h"
#include "RankAndPlaylistData.h"
#include <iomanip>
#include <sstream>

BAKKESMOD_PLUGIN(PlaylistRankViewer, "Playlist Rank Viewer", "1.3", 0)

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
	}, "Toggle plugin", PERMISSION_ALL);
	
	cvarManager->registerCvar("PRV_show_rank", "1", "Show rank", true, true, 0, true, 1);
	cvarManager->registerCvar("PRV_show_mmr", "1", "Show mmr", true, true, 0, true, 1);
	cvarManager->registerCvar("PRV_show_games_played", "1", "Show games played", true, true, 0, true, 1);
	cvarManager->registerCvar("PRV_color_rows", "1", "Highlight rows", true, true, 0, true, 1);
	cvarManager->registerCvar("PRV_background_opacity", "200", "Background opacity", true, true, 0, true, 255);
	cvarManager->registerCvar("PRV_x_position", "0.800", "Overlay X position", true, true, 0, true, 1.0f);
	cvarManager->registerCvar("PRV_y_position", "0.080", "Overlay Y position", true, true, 0, true, 1.0f);
	cvarManager->registerCvar("PRV_scale", "1", "Overlay scale", true, true, 0.5, true, 3);

	cvarManager->registerNotifier(
		"PRV_apply_settings",
		[this](std::vector<std::string> params) { applySettings(); },
		"Apply",
		PERMISSION_ALL
	);

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
	playerStats.clear();
}

string to_string_with_precision(float val, int len) {
	std::ostringstream oss;

	oss << std::fixed << std::setprecision(len) << val;

	return oss.str();
}

string GetRankName(int rank, int div) {
	if (rank < 0 || rank > 22) {
		return "ERROR";
	} else {
		RANK realRank = (RANK)(rank);
		std::string rankName = RankTiersAbrv[realRank].name;

		if (rank != RANK::Unranked && rank != RANK::SupersonicLegend)
			rankName += ".D" + to_string(div + 1);

		return rankName;
	}
}

Color GetRankColor(int rank) {
	if (rank < 0 || rank > 22) {
		return RankTiersAbrv[RANK::Unranked].color;
	} else {
		RANK realRank = (RANK)(rank);

		return RankTiersAbrv[realRank].color;
	}
}

// Update the MMR/Rank data and map for the user if it isnt updated yet
void PlaylistRankViewer::updatePlayerStats(UniqueIDWrapper uniqueId) {
	MMRWrapper mmrWrapper = gameWrapper->GetMMRWrapper();

	long long uID = uniqueId.GetUID();

	if (playerStats.count(uID) == 0 || playerStats[uID].size() == 0) {
		playerStats[uID] = {};
		bool showRank = cvarManager->getCvar("PRV_show_rank").getBoolValue();
		bool showMmr = cvarManager->getCvar("PRV_show_mmr").getBoolValue();
		bool showGamesPlayed = cvarManager->getCvar("PRV_show_games_played").getBoolValue();

		for (auto playlist : playlistsToCheck) {
			playerStats[uID][playlist] = {};

			SkillRank userRank = mmrWrapper.GetPlayerRank(uniqueId, playlist);
			string mmr = to_string_with_precision(mmrWrapper.GetPlayerMMR(uniqueId, playlist), 1);
			string gamesPlayed = to_string(userRank.MatchesPlayed);
			string currentRankName = GetRankName(userRank.Tier, userRank.Division);
			Color rankColor = GetRankColor(userRank.Tier);

			playerStats[uID][playlist].color = rankColor;

			if (playlist != PLAYLIST::UNRANKED && showRank) {
				playerStats[uID][playlist].text = currentRankName + " ";
			}

			if (showMmr) {
				playerStats[uID][playlist].text += mmr + " ";
			}

			if (showGamesPlayed) {
				playerStats[uID][playlist].text += "(" + gamesPlayed + ")";
			}
		}
	}
}

void PlaylistRankViewer::writeStats(CanvasWrapper& canvas, UniqueIDWrapper uniqueId, string playerName) {
	float PRV_x_position = cvarManager->getCvar("PRV_x_position").getFloatValue();
	float PRV_y_position = cvarManager->getCvar("PRV_y_position").getFloatValue();
	float PRV_scale = cvarManager->getCvar("PRV_scale").getFloatValue();
	int PRV_background_opacity = cvarManager->getCvar("PRV_background_opacity").getIntValue();

	long long uID = uniqueId.GetUID();

	// Draw box here
	Vector2 drawLoc = {
		int(canvas.GetSize().X * PRV_x_position),
		int(canvas.GetSize().Y * PRV_y_position)
	};
	Vector2 sizeBox = {
		int(255 * PRV_scale),
		int(int(40 + (20 * playlistsToCheck.size())) * PRV_scale)
	};
	Vector2 textPos = { int(drawLoc.X + 15), int(drawLoc.Y + 10) };

	canvas.SetPosition(drawLoc);

	// Set background color
	canvas.SetColor(0, 0, 0, PRV_background_opacity);
	canvas.FillBox(sizeBox);

	// First draw the players name at the top
	canvas.SetColor(255, 255, 255, 255);
	canvas.SetPosition(textPos);
	canvas.DrawString(playerName, PRV_scale, PRV_scale);

	// For each playlist, draw that playlists stats 20 pixels below the previous row
	textPos.Y += int(5 * PRV_scale);
	for (int i = 0; i < playlistsToCheck.size(); i++) {
		PLAYLIST playlist = playlistsToCheck[i];

		textPos.Y += int(20 * PRV_scale);
		Color color = playerStats[uID][playlist].color;
		
		if (cvarManager->getCvar("PRV_color_rows").getBoolValue()) {
			canvas.SetColor(color.r, color.g, color.b, 255);
		}

		canvas.SetPosition(textPos);
		canvas.DrawString(playlistNames[playlist] + " - " + playerStats[uID][playlist].text, PRV_scale, PRV_scale);
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
				updatePlayerStats(server.GetPRIs().Get(i).GetUniqueIdWrapper());
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
	playerStats.clear();
	updatePlayerStats({ gameWrapper->GetUniqueID() });
	log("Refreshed all MMRs");
}

bool PlaylistRankViewer::isEnabled() {
	return *enabled;
}

void PlaylistRankViewer::setEnabled(bool enabled) {
	this->enabled = make_shared<bool>(enabled);
}

void PlaylistRankViewer::applySettings() {
	this->resetMmrCache();
}
