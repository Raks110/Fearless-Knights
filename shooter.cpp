#include "shooter_utils.h"
#include "shooter_verbose.h"

#include <ncurses.h>

int main()
{

    int dimensions;

    bool SHOULD_CONTINUE = 1;

    utils::initializeStdScreen();
    while(SHOULD_CONTINUE){

        clear();
        verbose::initializeDimension(&dimensions);
        verbose::startGame(dimensions, &SHOULD_CONTINUE);
    }

    clear();
    utils::writeToDisplayAtCenterScreen("Thank you for playing Fearless Knight! More versions coming soon!");
    getch();
    clear();
    attron(A_BOLD);
    utils::writeToDisplayAtCenterScreen("Copyright 2019 Rakshit GL.");
    attroff(A_BOLD);
    attron(A_DIM);
    utils::writeToDisplayAtCenter(2, "Press any key twice to exit. Do visit the Fearless Kingdom again!");
    attroff(A_DIM);
    getch();
    attron(A_BOLD);
    utils::writeToDisplayAtCenterScreen("Fearless Knights v1 is developed and owned by Rakshit GL.");
    attroff(A_BOLD);
    getch();
    endwin();
}
