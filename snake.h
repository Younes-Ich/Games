#ifndef SNAKE_H
#define SNAKE_H

#include <iostream>
#include<ctime>
#include<conio.h>
#include<cstdlib>
#include<signal.h>
#include<Windows.h>
using namespace  std;

enum Direction {Top=1,Down,Right,Left};
struct Map{int Width,Height,FruitX,FruitY;};
struct Snake{int HeadX,HeadY,Tail_N,Tail_X[50],Tail_Y[50]; Direction Dir;};
struct Player{int score; bool lose;};
struct GameState {
    Map m_map;
    Snake snake;
    Player player;
};


extern FILE* file;
extern int scoreTab[6];
extern Map Map1;
extern Snake Snake1;
extern Player Player1;

void handle_sigint(int sig);

void Shift_Right(int arr[],int size);


void Draw();

void Generate_fruit();
void Input();

void setup();
void Move();

void save(int score);

void affiche_topS();

void Game();

void continu();

void menu();


#endif
