#ifndef __ABSPITCH_H__
#define __ABSPITCH_H__


#define MAX_ANSWER_LENGTH 10

#define ANS_CORRECT 1;
#define ANS_WRONG 0;

#define WRONG_ANS_SOUND  150

#define FREEPLAY 1
#define GAMEPLAY 2
#define EXIT 3

void start_musicGame();
void freeplay();
void gameplay();


void wrongsound();
void correctSound();

void printWelcomeMSG();
void printKeyMap();
void playAnswer(int);
int checkAnswer(int);
void backSlash(int);

int checkKeyBoard_g(int);
int checkKeyBoard_f(int,int);
void colorKeyNote(int,int);

#endif