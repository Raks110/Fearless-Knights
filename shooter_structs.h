#ifndef SHOOTER_STRUCTS_H
#define SHOOTER_STRUCTS_H

#include "shooter_utils.h"

#include <algorithm>
#include <chrono>
#include <random>
#include <vector>
#include <string.h>
#include <ncurses.h>
#include <iostream>

using namespace std;

typedef struct coordinates{
    int x;
    int y;
}coordinates;

namespace dialogues{

  typedef struct scoreDict{
    const char *name;
    long long int score;
  }scoresReg;

  vector<string> speaks;
  vector<scoresReg> highScores;
  int secondsLeft = 7;
  int i;
  int currentDialogue;
  char base[80];

  bool operator==(const scoresReg &lhs, const scoresReg &rhs){
    return lhs.name == rhs.name && lhs.score == rhs.score;
  }

  void initializeSpeaks(){

    speaks.push_back("HAHA. How will you escape the Wrath of Khan?");
    speaks.push_back("We monsters are always hungry!");
    speaks.push_back("Bullets filled with destruction will devour you!");
    speaks.push_back("HAHAHAHAHAHAHAHAHAHAHAHAHAHA!!");
    speaks.push_back("Mojo Jojo was my disciple! I trained him myself!");
    speaks.push_back("I won't waste your time trash talking, but I will certainly take my time and trash you!");
    speaks.push_back("LOL. Look at your sweet ass tryna escape me.");
    speaks.push_back("Ever heard of oblivion? Well, you'll soon be there.");

    for(int j=0;j<40;j++){
      base[2*j] = '/';
      base[(2*j) + 1] = '\\';
    }
  }

  void displaySpeaks(int rowDisplay, bool SHOULD_CHANGE){

      if(speaks.size() == 0){
        initializeSpeaks();

        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle(speaks.begin(), speaks.end(), default_random_engine(seed));
      }

      if(SHOULD_CHANGE){

        const char *temp = speaks[i].c_str();

        currentDialogue = i;

        init_pair(7,COLOR_RED,COLOR_BLACK);

        attron(A_DIM);

        mvprintw(rowDisplay,1,base);
        mvprintw(rowDisplay + 1,1,"<");
        mvprintw(rowDisplay + 1,80,">");
        mvprintw(rowDisplay + 2,1,"<");
        mvprintw(rowDisplay + 2,80,">");
        mvprintw(rowDisplay + 3,1,"<");
        mvprintw(rowDisplay + 3,80,">");
        mvprintw(rowDisplay + 4,1,base);

        attroff(A_DIM);

        attron(A_BOLD);
        mvprintw(rowDisplay+2,(80-strlen(temp))/2,temp);
        attron(A_BOLD);

        i++;

        if(i == speaks.size()){
          unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
          std::shuffle(speaks.begin(), speaks.end(), std::default_random_engine(seed));

          i = 0;
        }
      }
      else{

        const char *temp = speaks[currentDialogue].c_str();

        attron(A_DIM);

        mvprintw(rowDisplay,1,base);
        mvprintw(rowDisplay + 1,1,"|");
        mvprintw(rowDisplay + 1,80,"|");
        mvprintw(rowDisplay + 2,1,"|");
        mvprintw(rowDisplay + 2,80,"|");
        mvprintw(rowDisplay + 3,1,"|");
        mvprintw(rowDisplay + 3,80,"|");
        mvprintw(rowDisplay + 4,1,base);

        attroff(A_DIM);

        attron(A_BOLD);
        mvprintw(rowDisplay+2,(80-strlen(temp))/2,temp);
        attron(A_BOLD);
      }

  }

  bool forSortHighScores(scoresReg a, scoresReg b){
    return a.score > b.score;
  }

  void getHighScore(int rowLine, int score, const char *name){

      int a=6;

      int r,c;
      getmaxyx(stdscr,r,c);

      scoresReg temp = {name,score};

      highScores.push_back(temp);
      sort(highScores.begin(),highScores.end(),forSortHighScores);

      for(a=0;a<highScores.size();a++){
        if(highScores[a] == temp){
          break;
        }
      }

      if(highScores.size() > 5){
        highScores.erase(highScores.begin() + 5);
      }

      for(int b=0;b<highScores.size();b++){
        if(b==5){
          break;
        }
        if(b==a){
          attron(A_BOLD);
          mvprintw(rowLine + b,(c-11)/2,"%d",b+1);
          printw(". ");
          attron(A_UNDERLINE);
          printw(highScores[b].name);
          attroff(A_UNDERLINE);
          printw(" ");
          printw("%d",highScores[b].score);
          attroff(A_BOLD);
        }
        else{
          attron(A_DIM);
          mvprintw(rowLine + b,(c-11)/2,"%d",b+1);
          printw(". ");
          printw(highScores[b].name);
          printw(" ");
          printw("%d",highScores[b].score);
          attroff(A_DIM);
        }
    }
  }
}

#endif
