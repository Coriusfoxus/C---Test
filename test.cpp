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
const double kTurnRate = 0.2;

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

/* Local Function */
void InitializeGame(gameT &game);
void RunSimulation(gameT &game);
void OpenUserFile(ifstream &input);
void LoadWorld(gameT &game, ifstream &input);
pointT MakePoint(int row, int col);
string GetLine();
void Pause();
void PrintWorld(gameT &game);
void DisplayResult(gameT &game);
void PerformAI(gameT &game);
pointT GetNextPosition(gameT &game, int dx, int dy);
bool InWorld(pointT& pt, gameT& game);
bool RandomChance(double probability);
bool MoveSnake(gameT &game);
void PlaceFood(gameT &game);
bool Crashed(pointT headPos, gameT& game);

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
	srand(static_cast<unsigned int>(time(NULL)));

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
		PerformAI(game);

         if(!MoveSnake(game))
		 {
			 break;
		 }
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

void PerformAI(gameT &game)
{
	/* Figure out where we will be after we move this turn. */
	pointT nextHead = GetNextPosition(game, game.dx, game.dy);

	/* If that puts us into a wall or we randomly decide to, turn the snake. */
	if(Crashed(nextHead, game) || RandomChance(kTurnRate))
	{
		int leftDx = -game.dy;
		int leftDy = game.dx;
		int rightDx = game.dy;
		int rightDy = -game.dx;

		bool canLeft = !Crashed(GetNextPosition(game, leftDx, leftDy), game);
		bool canRight = !Crashed(GetNextPosition(game, rightDx, rightDy), game);

		bool willTurnLeft = false;
		if(!canLeft && !canRight){
			return;
		}else if(canLeft && !canRight){
			willTurnLeft = true;
		}else if(!canLeft && canRight){
			willTurnLeft = false;
		}else if(canLeft && canRight){
			willTurnLeft = RandomChance(0.5);
		}

		game.dx = willTurnLeft? leftDx: rightDx;
		game.dy = willTurnLeft? leftDy: rightDy;
	}
}

pointT GetNextPosition(gameT &game, int dx, int dy)
{
	/* Get the head position. */
	pointT result = game.snake.front();
	/* Increment the head position by the current direction */
	result.row += dy;
	result.col += dx;
	return result;
}

bool Crashed(pointT headPos, gameT& game)
{
	return !InWorld(headPos, game) ||
		game.world[headPos.row][headPos.col] == kSnakeTile ||
		game.world[headPos.row][headPos.col] == kWallTile;
}

bool InWorld(pointT& pt, gameT& game)
{
	return (pt.col >= 0) &&
		(pt.row >= 0) &&
		(pt.col < game.numCols) &&
		(pt.row < game.numRows);
}

bool RandomChance(double probability)
{
	return (rand() / (RAND_MAX) < probability);
}

bool MoveSnake(gameT &game)
{
	pointT nextHead = GetNextPosition(game, game.dx, game.dy);
	if(Crashed(nextHead, game))
	{
		return false;
	}

	bool isFood = (game.world[nextHead.row][nextHead.col] == kFoodTile);

	game.world[nextHead.row][nextHead.col] = kSnakeTile;
	game.snake.push_front(nextHead);

	if(!isFood)
	{
		game.world[game.snake.back().row][game.snake.back().col] = kEmptyTile;
		game.snake.pop_back();
	}
	else
	{
		++game.numEaten;
		PlaceFood(game);
	}
	return true;
}

void PlaceFood(gameT& game)
{
	while(true)
	{
		int row = rand() % game.numRows;
		int col = rand() % game.numCols;

		if(game.world[row][col] == kEmptyTile)
		{
			game.world[row][col] = kFoodTile;
			return;
		}
	}
}
