#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <ncurses.h>
#include <thread>
#include <chrono>
#include <algorithm>

#define RED		"\33[1;31m"
#define GREEN	"\33[1;32m"
#define YELLOW	"\33[1;33m"
#define BLUE	"\33[1;34m"
#define PINK	"\33[1;35m"
#define CYAN	"\33[1;36m"
#define WHITE	"\33[1;37m"
#define RESET	"\33[0;m"

using namespace std;
using vs = vector<string>;
using vvs = vector<vector<string>>;
using pii = pair<int, int>;
using qpii = queue<pair<int, int>>;

class Snake
{
	private:
		int		_height;
		int		_width;
		vvs		_board;
		pii 	_head;
		qpii	_order;
		int		_direction;			// 1 right, 2 down, 3 left, 4 up
		int		_speed; 			// update time in milliseconds
		int 	_score;

	public:
		Snake(int height, int width);
		~Snake(void);

		void 	initBoard(void);
		void	printBoard(int mode);
		void	updateDirection(int key);
		void	updateBoard(void);
		void	clearTerminal(void);
		void	play(void);
		void	gameOver(void);
};
