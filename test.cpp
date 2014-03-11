#include <iostream>
#include <fstream>
#include <deque>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

/* Num to win */
const int kMaxFood = 20;

/* Constants representing tile types */
const char kEmptyTile = ' ';
const char kWallTile = '#';
const char kSnakeTile = '*';
const char kFoodTile = '$';
const double kWaitTime = 0.1;
const string kClearCommand = "clear";

struct pointT
{
	int row, col;
};

struct gameT
{
	vector<string> world;
	int numRows, numCols;

	deque<pointT> snake;
	int dx, dy;

	int numEaten;
};

void InitializeGame(gameT &game);
void RunSimulation(gameT &game);
void OpenUserFile(ifstream &input);
void LoadWorld(gameT &game, ifstream &input);
pointT MakePoint(int row, int col);
string GetLine();
void Pause();
void PrintWorld(gameT &game);
void DisplayResult(gameT &game);

int main()
{
	gameT game;
	InitializeGame(game);
	RunSimulation(game);

	return 0;
}

void InitializeGame(gameT &game)
{
	cout << "Initialize game" << endl;
	ifstream input;
	OpenUserFile(input);
	LoadWorld(game, input);
	PrintWorld(game);
}

void RunSimulation(gameT &game)
{
	cout << "Run simulation" << endl;
	/* Keep looping while we haven't eaten max food */
	while(game.numEaten < kMaxFood)
	{
		PrintWorld(game);
		//PerformAI(game);

        // if(!MoveSnake(game))
		// {
		// 	break;
		// }
		Pause();
	}
	DisplayResult(game);
}

void OpenUserFile(ifstream &input)
{
	while(true)
	{
		// cout << "Enter filename: ";
		// string filename = GetLine();

		string filename = "level.txt";

		input.open(filename.c_str());
		if(!input.fail()) return;

		cout << "Sorry, I can't find the file " << filename << endl;
		input.clear();
	}
}

void LoadWorld(gameT &game, ifstream &input)
{
	input >> game.numRows >> game.numCols;
	game.world.resize(game.numRows);

	input >> game.dx >> game.dy;

	/* There is still a \n on the stream from the extraction operators. So if we
	   want to use getline we have to eat that newline and then continue */
	string dummy;
	getline(input, dummy);

	for(int row = 0; row < game.numRows; ++row)
	{
		getline(input, game.world[row]);
		int col = game.world[row].find(kSnakeTile);
		if(col != string::npos)
		{
			game.snake.push_back(MakePoint(row, col));
		}
	}

	game.numEaten = 0;
}

pointT MakePoint(int row, int col)
{
	pointT result;
	result.row = row;
	result.col = col;
	return result;
}

string GetLine()
{
	string result;
	getline(cin, result);
	return result;
}

void Pause()
{
	clock_t startTime = clock();

	while(static_cast<double>(clock() - startTime) / CLOCKS_PER_SEC < kWaitTime);
}

void PrintWorld(gameT &game)
{
	system(kClearCommand.c_str());
	int row;
	for(row = 0; row < game.numRows; ++row)
	{
		cout << game.world[row] << endl;
	}
	cout << "Food eaten: " << game.numEaten << endl;

	cout << "game.numRows: " << game.numRows << endl;
}

void DisplayResult(gameT& game)
{
	PrintWorld(game);
	if(game.numEaten == kMaxFood)
	{
		cout << "The snake ate enough food and wins!" << endl;
	}
	else
	{
		cout << "Oh no! The snake crashed!" << endl;
	}
}
