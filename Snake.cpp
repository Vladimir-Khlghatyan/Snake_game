#include "Snake.hpp"

Snake::Snake(int height, int width)
{
    _direction = 1;
    _speed = 200;   // update time in milliseconds
    _score = 0;

    _height = height;
    _width = width;

    // initialize <ncurses.h> library  --------------------------
    initscr();              // Initialize the ncurses library
    raw();                  // Disable line buffering
    keypad(stdscr, TRUE);   // Enable special keys
    noecho();               // Do not display user input
    nodelay(stdscr, TRUE);  // Enable non-blocking input mode
    //-----------------------------------------------------------

    this->initBoard();
    this->printBoard(0);
}

Snake::~Snake(void)
{

}

void Snake::initBoard(void)
{
    for (int i{}; i < _height; ++i)
        _board.push_back(vs(_width, " "));

    for (int j{}; j < _height; ++j)
        for (int i{}; i < _width; ++i)
            _freeIndexes.insert({j, i});

    vector<pair<int, int>> initPos = {{3, 1},{2, 1},{1, 1},{1, 2}};

    _head = initPos.back();

    for (auto& point : initPos)
    {
        _board[point.first][point.second] = RED + string(1, '*') + RESET;
        _order.push(point);
        _usedIndexes.insert(point);
        _freeIndexes.erase(point);
    }
}

void Snake::printBoard(int mode)
{
    if (mode == 0)  cout << "\rPress ESC to quit." << endl;
    else            cout << RESET <<"\rGame over!\nYour score is: " << _score << endl;

    cout << "\r+" << string(_width, '-') << "+" << endl;

    for (auto& line : _board)
    {
        cout << "\r|";
        for (auto& str : line) cout << str;
        cout << "|" << endl;
    }

    cout << "\r+" << string(_width, '-') << "+" << endl;
}

void Snake::updateDirection(int key)
{
    switch (key) {
    // right
    case 261 :
        _direction = (_direction == 3) ? 3 : 1;
        break;
    // down
    case 258 :
        _direction = (_direction == 4) ? 4 : 2;
        break;
    // left
    case 260 :
        _direction = (_direction == 1) ? 1 : 3;
        break;
    // up
    case 259 :
        _direction = (_direction == 2) ? 2 : 4;
        break;
    default :
        break;
    }
}

void Snake::updateBoard(void)
{
    switch (_direction) {
    // right
    case 1 :
        if (_head.second == _width - 1)
            this->gameOver(), exit(0);
        ++_head.second;
        break;
    // down
    case 2 :
        if (_head.first == _height - 1)
            this->gameOver(), exit(0);
        ++_head.first;
        break;
    // left
    case 3 :
        if (_head.second == 0)
            this->gameOver(), exit(0);
        --_head.second;
        break;
    // up
    case 4 :
        if (_head.first == 0)
           this->gameOver(), exit(0);
        --_head.first;
        break;

    default :
        break;
    }

    _board[_order.front().first][_order.front().second] = " ";
    _order.pop();

    _board[_head.first][_head.second] = RED + string(1, '*') + RESET;
    _order.push({_head.first, _head.second});
}

void Snake::clearTerminal(void)
{
    for (int i{}; i < _height + 3; ++i)
		cout << "\033[A" << "\033[K";
}

void Snake::play(void)
{
    while (true)
    {
        int key = getch();  // Wait for user input;
        if (key == 27)
            break;
        this_thread::sleep_for(chrono::milliseconds(_speed));
        this->updateDirection(key);
        this->updateBoard();
        this->clearTerminal();
        this->printBoard(0);
    }
}

void Snake::gameOver(void)
{
    refresh();  // Refresh the screen
    endwin();   // End the ncurses session
    this->printBoard(1);
}

