//#define OUTPUT_GAME_DATA
//#define TIME_MEASURERMENT
//#define DEBUG_ONE_TURN

#ifndef REDIRECT_INPUT

#pragma GCC optimize("O3","unroll-loops","omit-frame-pointer","inline") //Optimization flags
#pragma GCC option("arch=native","tune=native","no-zero-upper") //Enable AVX
#pragma GCC target("avx")  //Enable AVX
#include <x86intrin.h> //AVX/SSE Extensions
#include <bits/stdc++.h> //All main STD libraries

#else

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <stack>
#include <set>
#include <queue>
#include <algorithm>
#include <ctime>
#include <deque>
#include <cmath>
#include <climits>
#include <cstring>
#include <fstream>
#include <iterator>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <cassert>

#endif // REDIRECT_INPUT

using namespace std;

static const string INPUT_FILE_NAME = "input.txt";
static const string OUTPUT_FILE_NAME = "output.txt";
static const string EMPTY_STRING = "";
static const string SPACE = " ";

static constexpr int INVALID_IDX = -1;

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class Flags {
public:
	Flags() : flags{0} {}

	Flags& operator=(const Flags rhs) {
		flags = rhs.flags;
		return *this;
	}

	// Helper flags functions
	void setFlag(const unsigned flag) { flags |= flag; }
	void unsetFlag(const unsigned flag) { flags &= ~flag; }
	bool hasFlag(const unsigned flag) const { return flags & flag; }
	void reset() { flags = 0; }

private:
	unsigned flags; ///< 32 bit flags
};

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class Game {
public:
	Game();
	~Game();

	void initGame();
	void gameBegin();
	void gameEnd();
	void gameLoop();
	void getGameInput();
	void getTurnInput();
	void turnBegin();
	void makeTurn();
	void turnEnd();
	void play();

	void debug() const;

private:
	// Game specific members

	int turnsCount;
	int stopGame;
};

//*************************************************************************************************************
//*************************************************************************************************************

Game::Game() :
	turnsCount{ 0 },
	stopGame{ false }
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Game::~Game() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::initGame() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::gameBegin() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::gameEnd() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::gameLoop() {
	while (!stopGame) {
#ifdef TIME_MEASURERMENT
		chrono::steady_clock::time_point begin = chrono::steady_clock::now();
#endif // TIME_MEASURERM

		getTurnInput();
		turnBegin();
		makeTurn();
		turnEnd();

#ifdef TIME_MEASURERMENT
		chrono::steady_clock::time_point end = chrono::steady_clock::now();
		cerr << "Turn[" << turnsCount - 1 << "] execution time: " << chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " [ms]" << std::endl;
#endif // TIME_MEASURERM

#ifdef DEBUG_ONE_TURN
		break;
#endif // DEBUG_ONE_TURN
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::getGameInput() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::getTurnInput() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::turnBegin() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::makeTurn() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::turnEnd() {
	++turnsCount;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::play() {
	initGame();
	getGameInput();
	gameBegin();
	gameLoop();
	gameEnd();
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::debug() const {
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

#ifdef TESTS
#include "debug.h"
#endif // TESTS

void printCPUInfo() {
	std::ifstream cpuinfo("/proc/cpuinfo");
	if (!cpuinfo.is_open()) {
		std::cerr << "Failed to open /proc/cpuinfo" << std::endl;
		return;
	}

	std::string line;
	std::map<std::string, std::string> stats;
	while (std::getline(cpuinfo, line)) {
		if (line.find("model name") != std::string::npos) {
			stats["model name"] = line;
		}

		if (line.find("cpu cores") != std::string::npos) {
			stats["cpu cores"] = line;
		}

		if (line.find("cpu MHz") != std::string::npos) {
			stats["cpu MHz"] = line;
		}
	}

	std::cerr << stats["model name"] << std::endl;
	std::cerr << stats["cpu cores"] << std::endl;
	std::cerr << stats["cpu MHz"] << std::endl;

	cpuinfo.close();
}

int main(int argc, char** argv) {
#ifdef TESTS
	doctest::Context context;
	int res = context.run();
#else

#ifdef REDIRECT_INPUT
	cerr << endl << endl << endl << "!!! REDIRECT_INPUT !!!" << endl << endl << endl;
	ifstream in(INPUT_FILE_NAME);
	streambuf* cinbuf = cin.rdbuf();
	cin.rdbuf(in.rdbuf());

	ofstream out(OUTPUT_FILE_NAME);
	streambuf* coutbuf = cout.rdbuf();
	cout.rdbuf(out.rdbuf());
#else
	printCPUInfo();
#endif // REDIRECT_INPUT

	Game game;
	game.play();

#endif // TESTS

	return 0;
}
