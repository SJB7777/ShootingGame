#pragma once


#define NAMELEN 10 //9+1
#define MAX_SIZE 10
#define BOARD_SIZE 3

#include <iostream>


struct Node
{
	char name[NAMELEN];
	int score;

	Node* Left;
	Node* Right;
};

class scoreboard 
{
public:
	scoreboard();
	~scoreboard();

	
	Node* GetHead();
	int write_score(char* Name, int new_score);
	void delete_low();
	int GetHighscore();
	int GetSize();


private:

	Node* Head;
	int highscore=0;
	int size = 0;

};

