#include "Snake.hpp"

Snake::Snake(int height, int width)
{
    _direction = 1;
    _speed = 200;   // update time in milliseconds
    _score = 0;

    _height = height;
    _width = width;

    _chance = 2;
    _currentChance = _chance;

    this->initNecessaryLibraries();
    this->initBoard();
    this->printBoard(0);
}

Snake::~Snake(void)
{

}

void Snake::initNecessaryLibraries(void)
{
    // Save current terminal settings
    tcgetattr(STDIN_FILENO, &_oldt);
    _newt = _oldt;

    // Disable canonical mode (line buffering) and echoing
    _newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &_newt);

    // Set non-blocking mode for stdin
    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
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
        _board[point.first][point.second] = SNAKE;
        _order.push(point);
        _usedIndexes.insert(point);
        _freeIndexes.erase(point);
    }

    auto frog = this->getRandomPoint();
    _board[frog.first][frog.second] = FROG;

}

void Snake::printBoard(int mode)
{
    if (mode == 0)\
        cout << "\rPress ESC to quit.\n\rScore: " << CYAN << _score << RESET << endl;
    else
        cout << RESET;

    cout << "\r+" << string(_width, '-') << "+" << endl;

    for (auto& line : _board)
    {
        cout << "\r|";
        for (auto& str : line)
            cout << str;
        cout << "|" << endl;
    }

    cout << "\r+" << string(_width, '-') << "+" << endl;
    cout << "\r \u00A9 Author: Vladimir Khlghatyan" << endl;
}

void Snake::updateDirection(int key)
{
    switch (key) {
    // right
    case 67 :
        _direction = (_direction == 3) ? 3 : 1;
        break;
    // down
    case 66 :
        _direction = (_direction == 4) ? 4 : 2;
        break;
    // left
    case 68 :
        _direction = (_direction == 1) ? 1 : 3;
        break;
    // up
    case 65 :
        _direction = (_direction == 2) ? 2 : 4;
        break;
    default :
        break;
    }
}

bool Snake::updateBoard(void)
{
    bool frogEated{false};

    switch (_direction) {
    // right
    case 1 :
        if (_head.second == _width - 1)
        {
            if (_currentChance--)
                return false;
            this->gameOver();
            exit(0);
        }
        ++_head.second;
        if (_board[_head.first][_head.second] == SNAKE)
            this->gameOver(), exit(0);
        frogEated = (_board[_head.first][_head.second] == FROG);
        break;
    // down
    case 2 :
        if (_head.first == _height - 1)
        {
            if (_currentChance--)
                return false;
            this->gameOver();
            exit(0);
        }
        ++_head.first;
        if (_board[_head.first][_head.second] == SNAKE)
            this->gameOver(), exit(0);
        frogEated = (_board[_head.first][_head.second] == FROG);
        break;
    // left
    case 3 :
        if (_head.second == 0)
        {
            if (_currentChance--)
                return false;
            this->gameOver();
            exit(0);
        }
        --_head.second;
        if (_board[_head.first][_head.second] == SNAKE)
            this->gameOver(), exit(0);
        frogEated = (_board[_head.first][_head.second] == FROG);
        break;
    // up
    case 4 :
        if (_head.first == 0)
        {
            if (_currentChance--)
                return false;
            this->gameOver();
            exit(0);
        }
        --_head.first;
        if (_board[_head.first][_head.second] == SNAKE)
            this->gameOver(), exit(0);
        frogEated = (_board[_head.first][_head.second] == FROG);
        break;

    default :
        break;
    }    

    _board[_head.first][_head.second] = SNAKE;
    _order.push(_head);
    _usedIndexes.insert(_head);

    if (!frogEated)
    {
        _board[_order.front().first][_order.front().second] = " ";
        _freeIndexes.erase(_order.front());
        _order.pop();
    }
    else
    {
        ++_score;
        auto frog = this->getRandomPoint();
        _board[frog.first][frog.second] = FROG;

        if (_score % 5 == 0)
            _speed -= 50;
    }

    return true;
}

void Snake::clearTerminal(void)
{
    for (int i{}; i < _height + 5; ++i)
		cout << "\033[A" << "\033[K";
}

pii Snake::getRandomPoint(void)
{
    srand((unsigned)time(NULL));
    int random = rand() % _freeIndexes.size();
    auto it = _freeIndexes.begin();
    while (random--)
        ++it;
    return *it;
}

void Snake::play(void)
{
    char key;

    while (true)
    {
        // Check if a character is available in stdin
        while (read(STDIN_FILENO, &key, 1) > 0)
		{
            // Arrow key sequence, read the next two characters
            if (read(STDIN_FILENO, &key, 1) > 0 && key == 91)
                read(STDIN_FILENO, &key, 1);
        }

        if (key == 27) // Check for escape character (ASCII value 27)
            break;
        this_thread::sleep_for(chrono::milliseconds(_speed));
        this->updateDirection((int)key);
        if (this->updateBoard())
            _currentChance = _chance;
        this->clearTerminal();
        this->printBoard(0);
    }
}

void Snake::gameOver(void)
{
    this->clearTerminal();
    this->gameOverBoard();
    this->printBoard(1);

    // Restore original terminal settings before exiting
    tcsetattr(STDIN_FILENO, TCSANOW, &_oldt);
}

void Snake::gameOverBoard(void)
{
    vs gameOver = {"                                        ",
                   "                                        ",
                   "   #####      ##    ##   ##  #######    ",
                   "  ##   ##    ####   ### ###   ##  ##    ",
                   "  ##        ##  ##  #######   ##        ",
                   "  ##        ######  ## # ##   ####      ",
                   "  ##  ###   ##  ##  ##   ##   ##        ",
                   "  ##   ##   ##  ##  ##   ##   ##  ##    ",
                   "   #####    ##  ##  ##   ##  #######    ",
                   "                                        ",
                   "   #####    ##  ##  #######  ######     ",
                   "  ##   ##   ##  ##   ##  ##   ##  ##    ",
                   "  ##   ##   ##  ##   ##       ##  ##    ",
                   "  ##   ##   ##  ##   ####     #####     ",
                   "  ##   ##   ##  ##   ##       ####      ",
                   "  ##   ##    ####    ##  ##   ## ##     ",
                   "   #####      ##    #######  ###  ##    ",
                   "                                        ",
                   "                                        ",
                   "                                        "};

    gameOver.back() = "         You scored " + to_string(_score) + " points.";
    gameOver.back() += string(40 - gameOver.back().size(), ' ');

    for (int j{}; j < _board.size(); ++j)
        for (int i{}; i < _board[j].size(); ++i)
                _board[j][i] = gameOver[j][i];

    _board.back()[0] = GREEN + string(1, ' ');
    _board.back().back() = RESET + string(1, ' ');
    _board.push_back(_board[gameOver.size() - 2]);
    _board.push_back(_board[gameOver.size() - 2]);
}

