#include "duck_duck_goose.h"
#include <iostream>
#include <fstream>
using namespace std;


int main(int argc, char *argv[])
{
	// checks if a configuration file is entered by the user
	if(argc == 1)
	{
		cerr << "No configuration file was entered" << endl;
		cerr << "No output file was entered" << endl;
	}
	else
	{
		size_t randomSeed;
		int numPlayers;
		int itID;
		int playerID;
		GameData * game;
		ifstream inputFile(argv[1]);
		// Checks if the filename is found
		if(inputFile.is_open())
		{
			game = new GameData();
			// Stores the first three data points of the file into variables
			inputFile >> randomSeed >> numPlayers >> itID;
			// Stores the itID value from the file into the itPlayerID member variable of the GameData class
			game->itPlayerID = itID;
			// Creates the linkedList using all playerID's inside the configuration file as values for each node
			while(inputFile.eof() != true)
			{
				inputFile >> playerID;
				game->playerList.push_back(playerID);
			}
			// Initializes the initial random seed value;
			srand(randomSeed);
			// Initializes the output filestream using the output file name that was passed as a command line argument
			ofstream outputFile(argv[2]);
			// Runs a full game of Duck Duck Goose until a winner has been determined
			while(game->itPlayerID != 0)
				simulateDDGRound(game,outputFile);	
			delete game;
		}
	} 
	return 0;
}

// Runs one round of Duck Duck Goose
void simulateDDGRound(GameData * gameData, std::ostream & output) 
{
	// Calculates how many people the it player will call duck on. It's value is pseudo-random
	size_t numPlacesIT = (rand() % (4*(gameData->playerList.size())));
	int gooseID = gameData->playerList.get(numPlacesIT);
	// This for loop prints ID's of all players that are ducks. 
	// I do not use the get function for efficiency reasons.
	for(size_t i=0; i<numPlacesIT; i++)
		output << gameData->playerList.get(i) << " is a Duck." << endl;
	output <<  gooseID << " is a Goose!" << endl;
	int gooseSpeed = (rand() % 49) + 1;
	int itSpeed = (rand() % 49) + 1;
	// Compares the random numbers of the goose and the it player ([1,49])
	// If they are equal, both players will pseudo-randomly select another number until they have different values
	while(itSpeed == gooseSpeed)
	{
		gooseSpeed = (rand() % 49) + 1;
		itSpeed = (rand() % 49) + 1;
	} 
	// If the it player selects the highest value, the it and the goose player will swap spots
	if(itSpeed > gooseSpeed)
	{
		output << gameData->itPlayerID << " took " << gooseID <<"'s spot!" << endl;
		int tempID = gameData->itPlayerID;
		gameData->itPlayerID = gooseID;
		gameData->playerList.set(numPlacesIT, tempID);
	}
	// If the goose player selects the highest value, the it will leave the game
	else
	{
		output << gameData->itPlayerID << " is out!" << endl;
		// If there is at least 2 players in the game left, a new it player gets selected
		if(gameData->playerList.size() > 1)
		{
			size_t newitIndex = (rand() % (4*(gameData->playerList.size())));
			gameData->itPlayerID = gameData->playerList.get(newitIndex);
			gameData->playerList.remove(newitIndex);
			output << gameData->itPlayerID <<" was chosen as the new it." << endl;
		}
		// If there is only one player in the game left, the last player wins the game
		else
		{
			output << "Winner is " << gooseID << "!" << endl;
			gameData->itPlayerID = 0;
		}
	} 
}
