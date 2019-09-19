#ifndef DUCK_DUCK_GOOSE_H
#define DUCK_DUCK_GOOSE_H

#include <cstdlib>
#include <iostream>
#include <string>

#include "circular_list_int.h"

// ----------------------------------------
// CS104 HW2 Problem 3 skeleton header
// ----------------------------------------

// STUDENTS: DO NOT MODIFY THIS HEADER -- YOU MUST USE THIS STRUCT AND FUNCTION AS-IS

// data for a game of duck duck goose
struct GameData
{
	// Contains all players other than the "it" player.
	CircularListInt playerList;

	// ID of player who is currently it.
	// At the end of the last round, after the program outputs the winner, this should be set to 0 to indicate that there is no it.
	int itPlayerID;

	// allow default no-argument constructor
	GameData() = default;

	// no copy constructor
	GameData(GameData const & other) = delete;
};

// Simulates 1 round of duck duck goose according to the gameplay rules,
// and updates the passed GameData struct with the results.
// Will always be called with a valid GameData with an it player and at least 1 non-it player
void simulateDDGRound(GameData * gameData, std::ostream & output);


#endif