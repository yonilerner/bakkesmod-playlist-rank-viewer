#pragma once

#include "Enums.h"
#include <array>

struct Color { int r, g, b; };
struct PlayerStatStruct { Color color; string text; };
struct RankInfo { Color color; string name; string abbvr; };

map<PLAYLIST, string> playlistNames = {
	{UNRANKED, "Casual"},
	{SOLO, "Solo"},
	{TWOS, "Doubles"},
	{THREES, "Standard"},
	{HOOPS, "Hoops"},
	{RUMBLE, "Rumble"},
	{DROPSHOT, "Dropshot"},
	{SNOWDAY, "Snowday"},
	{TOURNAMENT, "Tournament"}
};

array<PLAYLIST, 9> playlistsToCheck = { UNRANKED, SOLO, TWOS, THREES, HOOPS, RUMBLE, DROPSHOT, SNOWDAY, TOURNAMENT };

map<RANK, RankInfo> RankTiers = {
	{ RANK::Unranked   ,{{ 255, 255, 255 }, "Unranked", "Unranked" }},
	{ RANK::Bronze1    ,{{ 255, 128, 0 }, "Bronze 1", "B1" }},
	{ RANK::Bronze2    ,{{ 255, 128, 0 }, "Bronze 2", "B2" }},
	{ RANK::Bronze3    ,{{ 255, 128, 0 }, "Bronze 3", "B3" }},
	{ RANK::Silver1    ,{{ 190, 190, 190 }, "Silver 1", "S1" }},
	{ RANK::Silver2    ,{{ 190, 190, 190 }, "Silver 2", "S2" }},
	{ RANK::Silver3    ,{{ 190, 190, 190 }, "Silver 3", "S3" }},
	{ RANK::Gold1      ,{{ 237, 233, 17 }, "Gold 1", "G1" }},
	{ RANK::Gold2      ,{{ 237, 233, 17 }, "Gold 2", "G2" }},
	{ RANK::Gold3      ,{{ 237, 233, 17 }, "Gold 3", "G3" }},
	{ RANK::Platinum1  ,{{ 106, 242, 247 }, "Platinum 1", "P1" }},
	{ RANK::Platinum2  ,{{ 106, 242, 247 }, "Platinum 2", "P2" }},
	{ RANK::Platinum3  ,{{ 106, 242, 247 }, "Platinum 3", "P3" }},
	{ RANK::Diamond1   ,{{ 2, 188, 255 }, "Diamond 1", "D1" }},
	{ RANK::Diamond2   ,{{ 2, 188, 255 }, "Diamond 2", "D2" }},
	{ RANK::Diamond3   ,{{ 2, 188, 255 }, "Diamond 3", "D3" }},
	{ RANK::Champ1     ,{{ 202, 137, 255 }, "Champion 1", "C1" }},
	{ RANK::Champ2     ,{{ 202, 137, 255 }, "Champion 2", "C2" }},
	{ RANK::Champ3     ,{{ 202, 137, 255 }, "Champion 3", "C3" }},
	{ RANK::GrandChamp1 ,{{ 255, 50, 50 }, "Grand Champion 1", "GC1" }},
	{ RANK::GrandChamp2 ,{{ 255, 50, 50 }, "Grand Champion 2", "GC2" }},
	{ RANK::GrandChamp3 ,{{ 255, 50, 50 }, "Grand Champion 3", "GC3" }},
	{ RANK::SupersonicLegend ,{{ 120, 255, 120 }, "Supersonic Legend", "SSL" }}
};
