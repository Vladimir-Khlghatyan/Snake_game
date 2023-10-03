#include "Snake.hpp"

int main(void)
{
    Snake s(20, 40);
    s.play();
    s.gameOver();

    return 0;
}
























// int main()
// {
    // initscr();              // Initialize the ncurses library
    // raw();                  // Disable line buffering
    // keypad(stdscr, TRUE);   // Enable special keys
    // noecho();               // Do not display user input
    // nodelay(stdscr, TRUE);  // Enable non-blocking input mode

    // int ch = 0;
    // while (ch != 27)
    // {
    //     // printw("Press any key (ESC to exit): ");
    //     ch = getch();       // Wait for user input
    //     cout << "\rYou pressed: " << ch << flush << endl;
    //     // printw("\nYou pressed: %c\n", ch);

    //     this_thread::sleep_for(chrono::milliseconds(1000));
    // }

    // refresh();              // Refresh the screen
    // endwin();               // End the ncurses session

//     return 0;
// }

