#ifndef SHOOTER_UTILS_H
#define SHOOTER_UTILS_H

#include<ncurses.h>
#include<string.h>
#include <algorithm>
#include <chrono>
#include <random>
#include <vector>
#include<math.h>
#include "shooter_structs.h"

using namespace std;

namespace utils{

    void initializeStdScreen(){

          initscr();
          raw();
          noecho();
          start_color();
    }

    void writeToDisplay(const char *textToWrite){

        printw(textToWrite);
    }

    void writeToDisplayAtXY(int x, int y, const char *textToWrite){

        mvprintw(x,y,textToWrite);
    }

    void writeToDisplayAtCenter(int row, const char *textToWrite){

        int r,c;
        getmaxyx(stdscr,r,c);

        writeToDisplayAtXY(row,(c - strlen(textToWrite))/2,textToWrite);
    }

    void writeToDisplayAtCenterOffset(int row, int offset, const char *textToWrite){

        int r,c;
        getmaxyx(stdscr,r,c);

        writeToDisplayAtXY(row,(c - offset)/2,textToWrite);
    }

    void writeToDisplayAtCenterScreen(const char *textToWrite){

        int r,c;
        getmaxyx(stdscr,r,c);

        writeToDisplayAtXY(r/2,(c-strlen(textToWrite))/2,textToWrite);
    }

    void writeIntegerToDisplay(const int intToWrite){

        printw("%d",intToWrite);
    }

    void writeIntegerToDisplayAtXY(int x, int y, const int intToWrite){

        mvprintw(x,y,"%d",intToWrite);
    }

    void writeIntegerToDisplayAtCenter(int row, const int intToWrite){

        int r,c;
        getmaxyx(stdscr,r,c);

        writeIntegerToDisplayAtXY(row,c/2,intToWrite);
    }

    int getInputFromDisplay(){

        int input;
        keypad(stdscr,TRUE);
        input = getch();

        return input;
    }

    int chooseDimension(int *initVal, int input){

        switch(input){

            case KEY_UP:
            case KEY_LEFT:
                *initVal -= 1;
                if(*initVal < 0)
                    *initVal = 2;
                return 1;

            case KEY_DOWN:
            case KEY_RIGHT:
                *initVal += 1;
                if(*initVal > 2)
                    *initVal = 0;
                return 1;

            case 10:
                return 0;
        }
    }

    void highlightDimension(int input){

        int rows,cols;
        getmaxyx(stdscr,rows,cols);

        switch(input){

          case 0:
              attron(A_STANDOUT);
              mvprintw(6,(cols/2),"%d",10);
              attroff(A_STANDOUT);
              attron(A_NORMAL);
              mvprintw(7,(cols/2),"%d",15);
              mvprintw(8,(cols/2),"%d",20);
              attroff(A_NORMAL);
              return;

          case 1:
              attron(A_STANDOUT);
              mvprintw(7,(cols/2),"%d",15);
              attroff(A_STANDOUT);
              attron(A_NORMAL);
              mvprintw(6,(cols/2),"%d",10);
              mvprintw(8,(cols/2),"%d",20);
              attroff(A_NORMAL);
              return;

          case 2:
              attron(A_STANDOUT);
              mvprintw(8,(cols/2),"%d",20);
              attroff(A_STANDOUT);
              attron(A_NORMAL);
              mvprintw(7,(cols/2),"%d",15);
              mvprintw(6,(cols/2),"%d",10);
              attroff(A_NORMAL);
              return;

      }
    }

    int moveCursorToCenter(int row){

        int r,c;
        getmaxyx(stdscr,r,c);

        move(row,c/2);
    }

    void constructBase(int dimensions){

        char displayBase[60];

        for(int i=0;i<dimensions*4;i++){
            switch(i%4){
              case 0:
                  displayBase[i] = '.';
                  break;
              case 1:
                  displayBase[i] = '_';
                  break;
              case 2:
                  displayBase[i] = '_';
                  break;
              case 3:
                  displayBase[i] = '.';
                  break;
            }
        }

        const char *base = displayBase;
        writeToDisplay(base);
    }

    int cleanBulletVector(vector<coordinates> &bullets, int dimensions, int &lives, int userPosition){

        int scoreUpgrade = 0;

        for(int i=0;i<bullets.size();i++){

            bullets[i].x++;

            if(bullets[i].x >= dimensions){

                bullets.erase(bullets.begin() + i);
                scoreUpgrade++;
            }

            if(bullets[i].x == dimensions -1){

                if((bullets[i].y * 4) - 1 >= userPosition && (bullets[i].y * 4) - 1 <= userPosition+4){
                    lives--;
                }
            }
        }

        return scoreUpgrade;
    }

    void cleanLifeVector(vector<coordinates> &lifelines, int dimensions, int &lives, int userPosition){

        for(int i=0;i<lifelines.size();i++){

            lifelines[i].x++;

            if(lifelines[i].x >= dimensions){

                lifelines.erase(lifelines.begin() + i);
            }

            if(lifelines[i].x == dimensions -1){

                if((lifelines[i].y * 4) - 1 >= userPosition && (lifelines[i].y * 4) - 1 <= userPosition+4){
                    lives++;
                }
            }
        }
    }

    void createBullets(vector<coordinates> &bullets, int dimensions){

        vector<int> dimensionList;
        for(int i=0;i<dimensions;i++){
            dimensionList.push_back(i);
        }

        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle(dimensionList.begin(), dimensionList.end(), default_random_engine(seed));

        for(int i=0;i<floor((dimensions/4)+1);i++){

            coordinates temp = {0,dimensionList[i]};
            bullets.push_back(temp);
        }
    }

    void createLives(vector<coordinates> &lifelines, int dimensions){

        vector<int> dimensionList;
        for(int i=0;i<50;i++){
            dimensionList.push_back(i);
        }

        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle(dimensionList.begin(), dimensionList.end(), default_random_engine(seed));

        if(dimensionList[0] >= dimensions){
            return;
        }
        coordinates temp = {0,dimensionList[0]};
        lifelines.push_back(temp);
    }

    int moveUser(int *position, int input, int dimensions){

        switch(input){

            case KEY_UP:
            case KEY_LEFT:
                *position -= 4;
                if(*position < 1)
                    *position += 4;
                return 1;

            case KEY_DOWN:
            case KEY_RIGHT:
                *position += 4;
                if(*position > (dimensions-1)*4)
                    *position -= 4;
                return 1;

            case 10:
                return 0;

            default:
                return 1;
        }
    }

    void constructBullets(vector<coordinates> bullets, const int ROW_OFFSET){

        for(int i=0;i<bullets.size();i++){

            writeToDisplayAtXY(bullets[i].x + ROW_OFFSET,(bullets[i].y*4)-1,"V");
        }
    }

    void constructLives(vector<coordinates> lifelines, const int ROW_OFFSET){

        for(int i=0;i<lifelines.size();i++){

            writeToDisplayAtXY(lifelines[i].x + ROW_OFFSET,(lifelines[i].y*4)-1,"+");
        }
    }
}

#endif
