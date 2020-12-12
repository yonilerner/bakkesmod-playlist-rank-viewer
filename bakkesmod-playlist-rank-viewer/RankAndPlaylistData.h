#pragma once

#include "pch.h"
#include "Enums.h"

map<PLAYLIST, string> playlistNames = {
	{UNRANKED, "Unranked"},
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

map<RANK, string> RankTiers = {
	{SupersonicLegend, "SupersonicLegend"},
	{GrandChamp3, "GrandChamp3"},
	{GrandChamp2, "GrandChamp2"},
	{GrandChamp1, "GrandChamp1"},
	{Champ3, "Champ3"},
	{Champ2, "Champ2"},
	{Champ1, "Champ1"},
	{Diamond3, "Diamond3"},
	{Diamond2, "Diamond2"},
	{Diamond1, "Diamond1"},
	{Platinum3, "Platinum3"},
	{Platinum2, "Platinum2"},
	{Platinum1, "Platinum1"},
	{Gold3, "Gold3"},
	{Gold2, "Gold2"},
	{Gold1, "Gold1"},
	{Silver3, "Silver3"},
	{Silver2, "Silver2"},
	{Silver1, "Silver1"},
	{Bronze3, "Bronze3"},
	{Bronze2, "Bronze2"},
	{Bronze1, "Bronze1"},
	{Unranked, "Unranked"}
};

map<RANK, string> RankTiersAbrv = {
	{SupersonicLegend, "SSL"},
	{GrandChamp3, "GC3"},
	{GrandChamp2, "GC2"},
	{GrandChamp1, "GC1"},
	{Champ3, "C3"},
	{Champ2, "C2"},
	{Champ1, "C1"},
	{Diamond3, "D3"},
	{Diamond2, "D2"},
	{Diamond1, "D1"},
	{Platinum3, "P3"},
	{Platinum2, "P2"},
	{Platinum1, "P1"},
	{Gold3, "G3"},
	{Gold2, "G2"},
	{Gold1, "G1"},
	{Silver3, "S3"},
	{Silver2, "S2"},
	{Silver1, "S1"},
	{Bronze3, "B3"},
	{Bronze2, "B2"},
	{Bronze1, "B1"},
	{Unranked, "Unranked"}
};