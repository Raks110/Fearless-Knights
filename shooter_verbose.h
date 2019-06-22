#ifndef SHOOTER_VERBOSE_H
#define SHOOTER_VERBOSE_H

#include <ncurses.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <ctype.h>
#include "shooter_utils.h"
#include "shooter_structs.h"

using namespace std;

namespace verbose{

  const char *dimensionInfoLine1 = "Please choose the dimensions for the game.";
  const char *dimensionInfoLine2 = "Max dimension allowed is 15.";
  const char *dimensionInfoLine3 = "Also keep the console covering the whole screen.";
  const char *continueMsg1 = "Alright! Let's play Fearless Knight Dimensions ";
  const char *continueMsg2 = "! Press anything to continue.";
  const char *getNameMsg = "Please enter a three letter name for your knight: ";

  char name[3];

  void initializeDimension(int *dimensions){

        utils::writeToDisplayAtCenter(1,dimensionInfoLine1);
        utils::writeToDisplayAtCenter(2,dimensionInfoLine2);
        utils::writeToDisplayAtCenter(4,dimensionInfoLine3);

        utils::writeIntegerToDisplayAtCenter(6,5);
        utils::writeIntegerToDisplayAtCenter(7,10);
        utils::writeIntegerToDisplayAtCenter(8,15);

        int initVal = 0;
        int notDone = 1;

        while(notDone){

            utils::highlightDimension(initVal);

            utils::moveCursorToCenter(10);
            int input = utils::getInputFromDisplay();
            notDone = utils::chooseDimension(&initVal,input);
        }

        int r,c;
        getmaxyx(stdscr,r,c);

        utils::writeToDisplayAtCenterOffset(10,strlen(getNameMsg) + 3,getNameMsg);
        noecho();
        for(int i=0;i<3;i++){
            char c = toupper(getch());
            if((c>='A' && c<='Z')||(c>='a' && c<='z')){
                char ca[1] = {c};
                utils::writeToDisplay(ca);
                name[i] = c;
            }
        }

        utils::writeToDisplayAtCenterOffset(12, strlen(continueMsg1) + strlen(continueMsg2) + 1, continueMsg1);
        utils::writeIntegerToDisplay((initVal + 2) * 5);
        utils::writeToDisplay(continueMsg2);

        curs_set(0);
        getch();
        clear();

        *dimensions = (initVal + 2) * 5;
  }

  void startGame(const int dimensions, bool *should_continue){

      int lives = 3;
      long long int score = 0;
      bool playing = 1;

      init_pair(1,COLOR_CYAN,COLOR_BLACK);
      init_pair(2, COLOR_RED, COLOR_BLACK);
      init_pair(3,COLOR_GREEN, COLOR_BLACK);
      init_pair(4,COLOR_RED, COLOR_BLACK);

      vector<coordinates> bullets;
      vector<coordinates> lifelines;
      int userRowNumber = dimensions-1;
      int userPosition = 1;

      noecho();
      int SKIP_CREATING_BULLETS = 3;
      int turn = 0;
      int DELAY_TIME = 5;
      const int ROW_OFFSET = 5;
      int dialogue_skip = 0;
      int DIALOGUE_DELAY_TIME = 20;

      while(playing){

          if(lives <= 0){

              playing = 0;
              break;
          }

          if(score > 50){
              DELAY_TIME = 3;
              DIALOGUE_DELAY_TIME = 35;
          }
          if(score > 100){
              SKIP_CREATING_BULLETS = 2;
              DELAY_TIME = 2;
              DIALOGUE_DELAY_TIME = 45;
          }
          if(score > 150){
              SKIP_CREATING_BULLETS = 1;
              DELAY_TIME = 1;
              DIALOGUE_DELAY_TIME = 70;
          }

          attron(A_BOLD);
          utils::writeToDisplayAtXY(1,1,"SCORE: ");
          utils::writeIntegerToDisplay(score);

          utils::writeToDisplayAtXY(3,1,"LIVES: ");
          utils::writeIntegerToDisplay(lives);

          move(5,1);
          attron(COLOR_PAIR(2));
          utils::constructBase(dimensions);
          attroff(COLOR_PAIR(2));
          attroff(A_BOLD);

          if(lives > 2){

              attron(A_BOLD | COLOR_PAIR(1));
              utils::writeToDisplayAtXY(userRowNumber+ROW_OFFSET,userPosition,".__.");
              attroff(A_BOLD | COLOR_PAIR(1));
          }
          else if(lives > 1){

              attron(COLOR_PAIR(1));
              utils::writeToDisplayAtXY(userRowNumber+ROW_OFFSET,userPosition,".__.");
              attroff(COLOR_PAIR(1));
          }
          else{

              attron(A_DIM | COLOR_PAIR(1));
              utils::writeToDisplayAtXY(userRowNumber+ROW_OFFSET,userPosition,".__.");
              attroff(A_DIM | COLOR_PAIR(1));
          }

          if(turn == 0)
            utils::createBullets(bullets, dimensions);
          else if(turn == 1){
              utils::createLives(lifelines, dimensions);
          }

          score += utils::cleanBulletVector(bullets, dimensions, lives, userPosition);
          utils::cleanLifeVector(lifelines,dimensions,lives,userPosition);

          attron(A_DIM);
          utils::constructBullets(bullets,ROW_OFFSET);
          attroff(A_DIM);
          attron(A_BOLD);
          attron(COLOR_PAIR(3));
          utils::constructLives(lifelines,ROW_OFFSET);
          attroff(COLOR_PAIR(3));
          attroff(A_BOLD);

          attron(COLOR_PAIR(4));
          if(dialogue_skip == 0){

              dialogues::displaySpeaks(ROW_OFFSET + dimensions + 4,1);
              dialogue_skip++;
          }
          else{

              dialogues::displaySpeaks(ROW_OFFSET + dimensions + 4,0);
              dialogue_skip++;
          }
          attroff(COLOR_PAIR(4));

          curs_set(0);

          halfdelay(DELAY_TIME);
          int input = getch();
          playing = utils::moveUser(&userPosition, input, dimensions);

          turn++;
          if(turn > SKIP_CREATING_BULLETS){
              turn = 0;
          }

          if(dialogue_skip > DIALOGUE_DELAY_TIME){

              dialogue_skip=0;
          }
          clear();
      }

      clear();
      utils::writeToDisplayAtCenter(5,"You have scored: ");
      utils::writeIntegerToDisplay(score);
      utils::writeToDisplayAtCenter(7,"HIGH SCORES");
      dialogues::getHighScore(9,score,name);
      utils::writeToDisplayAtCenter(17,"Press Enter to exit, and anything else to continue playing the awesome knight!");
      raw();
      int c=0;
      while(1){
          c = getch();
          if(c == 10){
              *should_continue = 0;
              break;
          }
          else{
              *should_continue = 1;
              break;
          }
      };
  }
}

#endif
