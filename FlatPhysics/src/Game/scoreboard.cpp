#include "scoreboard.h"


inline scoreboard::scoreboard() {

	Head = new Node[1];
	memcpy(Head->name, "N/A", NAMELEN);
	Head->Left = Head;
	Head->Right = Head;

	return;
}

scoreboard::~scoreboard() {
	Node* ptr = Head;
	Node* D = NULL;

	while (ptr->Right!=NULL)
	{
		D = ptr;
		ptr = ptr->Right;
		delete[] D;
	}

	delete Head;
}

Node* scoreboard::GetHead()
{
	return Head;
}

int scoreboard::GetHighscore()
{
	return highscore;
}

int scoreboard::GetSize()
{
	return size;
}



void scoreboard::delete_low() {
	Node* ptr = Head;

	//pointer 조작
	ptr = ptr->Left;
	ptr->Left->Right = Head;
	Head->Left = ptr->Left;

	delete[] ptr;
}



int scoreboard::write_score(char* Name, int new_score) {

	Node* ptr = Head;

	if (size == 0) //head
	{
		memcpy(Head->name, Name,strnlen(Name,NAMELEN));
		Head->score = new_score;
		size++;
		return 1;
	}

	else if (size <= MAX_SIZE)
	{
		if (size == MAX_SIZE) //oversize
			delete_low();

		//new node
		Node* new_node = new Node[1];
		memcpy(new_node->name, Name, NAMELEN);
		new_node->score = new_score;

		//find location
		for (int i = 1; i < size && ptr->Right->score >= new_score; i++) //우측 노드가 더 크거나 같으면 이동
		{
			ptr = ptr->Right;
		}

		//포인터 수정
		new_node->Left = ptr;
		new_node->Right = ptr->Right;
		ptr->Right = new_node;
		ptr->Right->Left = new_node;

		size++;
		return 1;
	}

	return 0; //error
}