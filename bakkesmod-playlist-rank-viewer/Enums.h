#pragma once

enum PLAYLIST {
	UNRANKED = 1, //doesn't always work but will when in game, might take a player swap for it to update to the correct mmr
	SOLO = 10,
	TWOS = 11,
	THREES = 13,
	HOOPS = 27,
	RUMBLE = 28,
	DROPSHOT = 29,
	SNOWDAY = 30,
	TOURNAMENT = 34,
};

enum RANK {
	SupersonicLegend = 22,
	GrandChamp3 = 21,
	GrandChamp2 = 20,
	GrandChamp1 = 19,
	Champ3 = 18,
	Champ2 = 17,
	Champ1 = 16,
	Diamond3 = 15,
	Diamond2 = 14,
	Diamond1 = 13,
	Platinum3 = 12,
	Platinum2 = 11,
	Platinum1 = 10,
	Gold3 = 9,
	Gold2 = 8,
	Gold1 = 7,
	Silver3 = 6,
	Silver2 = 5,
	Silver1 = 4,
	Bronze3 = 3,
	Bronze2 = 2,
	Bronze1 = 1,
	Unranked = 0,
};