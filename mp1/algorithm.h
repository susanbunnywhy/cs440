#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <string>
#include <iostream>
#include <queue>
#include <cmath>
#include <stdio.h>
#include <cstdlib>
#include <stack>
using namespace std;
extern int x_max, y_max, dot_counter;
extern int pcost, nvisit;
				//Commmon Class + functions//
//----------------------------------------------------------//
class coord
{
	public:
		int x;
		int y;
		bool operator==(const coord &other) const;
		bool operator<(const coord &other) const;
};

extern std::vector<coord> endset;
//Manhatten distance calc.
int MD(coord curr, coord end);

//convert the endpoint from array to vector form
vector<coord> conversion(coord* array);

//coordinate for maze array calc
int get_cord(coord curr);

//Cell status + boundary check
int check_cell(char* maze, coord curr);

//Duplicated main.cpp draw path function
void draw_ppath(char * maze);

			//Search method + helper function//
//---------------------------------------------------------//

//A* Search
int AS(void);

//Bread first search
int BFS(char *maze, coord start, coord end);
int BFS_R(char *maze, coord end, std::queue<coord> &frontier, coord*parent);

//Depth first search
int DFS(char *maze, coord start, coord end);
int DFS_R(char *maze, coord end, std::stack<coord> &frontier, coord *parent);

//Greedy first search
int GFS(char* maze, coord start, coord end);
int GFS_move(char* maze, coord start, coord end);
int GFS_Recurr(char* maze, coord curr, coord end, coord* path, int &point);

#endif