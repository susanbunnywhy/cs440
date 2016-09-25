#include "a_star.h"
using namespace std;
vector<coord> neighbor;
map<coord, int> cost_to_end;
map<coord, int> totalCost;
vector<coord> openSet;
vector<coord> closedSet;
map<coord, coord> cameFrom;
map<coord, int> pathCost;

int optimal_cost(coord cur, coord end){
	int euc_val = floor(sqrt(pow((cur.x - end.x), 2) + pow((cur.y - end.y), 2)));	
	int md_val = MD(cur, end);
	return max(euc_val, md_val);
}



coord min_cost(vector<coord> openSet, map<coord, int> cost){
	vector<coord>::iterator it;
	int current_min = 10000;
	coord current_node;
	current_node.x = -1;
	current_node.y = -1;
	for (int i = 0; i < openSet.size(); i++)
	{
		if (cost[openSet[i]] < current_min){
			current_min = cost[openSet[i]];
			current_node.x = openSet[i].x;
			current_node.y = openSet[i].y;
		}
	}
	return current_node;
}

void _draw_path(char * maze)
{
	int i = 0;
	for (i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			cout << maze[i * column + j];
		}
		cout << endl;
	}
}

void find_neighbor(char* maze, coord current){
	coord left;
	left.x = current.x - 1;
	left.y = current.y;
	coord right;
	right.x = current.x + 1;
	right.y = current.y;
	coord up;
	up.x = current.x;
	up.y = current.y - 1;
	coord down;
	down.x = current.x;
	down.y = current.y + 1;
	if (check_cell(maze, left) >= 1){
		neighbor.push_back(left);
	}
	if (check_cell(maze, right) >= 1)
	{
		neighbor.push_back(right);
	}
	if (check_cell(maze, up) >= 1)
	{
		neighbor.push_back(up);
	}
	if (check_cell(maze, down) >= 1)
	{
		neighbor.push_back(down);
	}
}

/*int a_star(char *maze, coord start, coord end){
	int curPathcost;
	map<coord, coord> cameFrom;
	vector<coord> openSet;
	vector<coord> closedSet;
	map<coord, int> pathCost;
	map<coord, int> totalCost;
	coord current;

	openSet.push_back(start);
	pathCost[start] = 0;
	totalCost[start] = MD(start, end);

	while (!openSet.empty()){
		current = min_cost(openSet, totalCost);
		//cout << "current.x:" << current.x << ' ' << "current.y" << current.y << endl;
		//cout << "current cost:" << totalCost[current] << endl;
		if ((current.x == end.x) && (current.y == end.y)){
			vector<coord> result = totalPath(cameFrom, current);
			for (vector<coord>::iterator it = result.begin(); it != result.end(); it++)
			{
		 		if ((it->x == start.x) && (it->y == start.y))
		 		{
		 			continue;
		 		}
		 		maze[it->y * column + it->x] = '.';
			}
			return 1;
		}
		openSet.erase(remove(openSet.begin(),openSet.end(), current), openSet.end());
		closedSet.push_back(current);
		neighbor.clear();
		find_neighbor(maze, current);
		for (int i = 0; i < neighbor.size(); i++)
		{
			//cout << "neighbor" << neighbor[i].x << ' ' << neighbor[i].y << endl;
			if (find(closedSet.begin(), closedSet.end(), neighbor[i]) != closedSet.end()){
				continue;
			}
			pathCost[neighbor[i]] = 10000;
			curPathcost = pathCost[current] + 1;
			if (find(openSet.begin(), openSet.end(), neighbor[i]) == openSet.end())
			{
				openSet.push_back(neighbor[i]);
			}
			else if (curPathcost >= pathCost[neighbor[i]])
			{
				continue;
			}
			cameFrom[neighbor[i]] = current;
			pathCost[neighbor[i]] = curPathcost;
			totalCost[neighbor[i]] = pathCost[neighbor[i]] + MD(neighbor[i], end);
			//cout << "neighbor totalCost:" << totalCost[neighbor[i]] << endl;
		}
	}
	return 0;
}*/

vector<coord> totalPath(map<coord, coord> path, coord current){
	vector<coord> totalPath;
	totalPath.push_back(current);
	while(path.find(current) != path.end()){
		current = path[current];
		totalPath.push_back(current);
	}
	return totalPath;
}

int _a_star(char *maze, coord start, vector<coord> endset){
	
	openSet.push_back(start);
	pathCost[start] = 0;
	totalCost[start] = optimal_cost(start, endset[0]);
	return _a_star_test(maze, start, endset);
}

int _a_star_test(char *maze, coord start, vector<coord> endset)
{
	coord current;
	int curPathcost;
	coord prev_end,end;
	
	//While frontier is not empty
	while (!openSet.empty())
	{
		if(!(prev_end == end))
		{
			//update all total_cost
			update_all_total_cost(end, openSet);
			prev_end  = end;
		}
		current = min_cost(openSet, totalCost);
		//cout << "current.x:" << current.x << ' ' << "current.y" << current.y << endl;
		//cout << "current cost:" << totalCost[current] << endl;

		//Check if we have reached 1 end point
		if ((current.x == end.x) && (current.y == end.y))
		{	


			endset.erase(remove(endset.begin(),endset.end(), current), endset.end());
			if(endset.empty())
			{
				//Append current location to the path
				vector<coord> result = totalPath(cameFrom, current);

				//Since we have reached one point, draw the path onto the maze map
				for (vector<coord>::iterator it = result.begin(); it != result.end(); it++)
				{
					//Skip starting point 'p'
			 		if ((it->x == start.x) && (it->y == start.y))
			 		{
			 			continue;
			 		}
			 		maze[it->y * column + it->x] = '.';
				}
				return 1;
			}

			else
			{
				multi_update(current, endset);
				end = min_cost(endset, cost_to_end);				
			}
		}

		//Move the current location out of the frontier
		openSet.erase(remove(openSet.begin(),openSet.end(), current), openSet.end());

		//Mark current loc. as explored
		closedSet.push_back(current);

		//Reset neighbor for ucpoming update
		neighbor.clear();

		//Apeend curr loc's neighbor to the vector
		find_neighbor(maze, current);

		//Iterate through every neighbor
		for (int i = 0; i < neighbor.size(); i++)
		{
			//cout << "neighbor" << neighbor[i].x << ' ' << neighbor[i].y << endl;

			//If neighbor is already explored, skip them
			if (find(closedSet.begin(), closedSet.end(), neighbor[i]) != closedSet.end()){
				continue;
			}

			//Calc neighbor's current path cost
			curPathcost = pathCost[current] + 1;

			//Push neighbor into froniter if not already
			if (find(openSet.begin(), openSet.end(), neighbor[i]) == openSet.end())
			{
				openSet.push_back(neighbor[i]);

				//Initialize path cost of neighbor to MAX
				pathCost[neighbor[i]] = 10000;
			}

			//Compare existing pathcost to the current 
			else if (curPathcost >= pathCost[neighbor[i]])
			{
				continue;
			}

			// from current loc to neighbor is closer than original path, therefore throw away prevoious path to neighbor
			cameFrom[neighbor[i]] = current;
			pathCost[neighbor[i]] = curPathcost;
			
			//Update total cost
			totalCost[neighbor[i]] = pathCost[neighbor[i]] + optimal_cost(neighbor[i], end);
			//cout << "neighbor totalCost:" << totalCost[neighbor[i]] << endl;
		}
		multi_update(current, endset);
		end = min_cost(endset, cost_to_end);
	}
	return 0;
}

void multi_update(coord curr, vector<coord> &endset)
{
	for(int i = 0; i < endset.size(); i++)
	{
		cost_to_end[endset[i]] = optimal_cost(curr, endset[i]);
	}
}

void update_all_total_cost(coord end, vector<coord> &openSet)
{
	for(int i = 0; i < openSet.size(); i++)
	{
		totalCost[openSet[i]] = optimal_cost(end, openSet[i]);
	}
}
