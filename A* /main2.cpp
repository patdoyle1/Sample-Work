 #include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <utility>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;

int diagonalUnit = 14;
int straightLineUnit = 10;

//g dist is from the start (manhattan)
//h dist is from the end (straingt line)

class Node{
public:
	Node(const char& nVal, const int nx, const int ny){
		value = nVal;
		xVal = nx;
		yVal = ny;
		parent = NULL;
	}
	char value;
	int hVal;
	int gVal;  // current G cost
	int fVal;

	Node* parent;

	int xVal;  //x coordinate
	int yVal;  //y coordinate

	void calcFVal(){
		fVal = hVal + gVal;
	}

};

vector<vector<Node*> > makeGraphOfNodes(const vector<string>& stringGraph){
	vector<vector<Node*> > graph;
	for(int x = 0; x < stringGraph.size(); x++){
		vector<Node*> gLine;
		for(int y = 0; y < stringGraph[x].size(); y++){
			Node* newNode = new Node(stringGraph[x].at(y), x, y);
			gLine.push_back(newNode);
		}
		graph.push_back(gLine);
	}
	return graph;
}

void printGraph(const vector<vector<Node*> >& graph){
	for(int x = 0; x < graph.size(); x++){
		for(int y = 0; y < graph[x].size(); y++)
			cout << graph[x][y]->value;
		cout << endl;
	}
}

Node* findStartNode(vector<vector<Node*> >& graph){
	for(int x = 0; x < graph.size(); x++)
		for(int y = 0; y < graph[x].size(); y++)
			if(graph[x][y]->value == 'S')
				return graph[x][y];
}

Node* findEndNode(vector<vector<Node*> >& graph){
	for(int x = 0; x < graph.size(); x++)
		for(int y = 0; y < graph[x].size(); y++)
			if(graph[x][y]->value == 'E')
				return graph[x][y];
}

bool isEndNode(const Node* currNode){	
	if(currNode->value == 'E')
		return true;
	return false;
}

bool isEndNodePresentp(const vector<Node*>& openList){
	for(int x = 0; x < openList.size(); x++)
		if(openList[x]->value == 'E')
			return false;
	return true;
}

bool isValidMovePosition(const Node* currNode){
	if(currNode->value != '#' && currNode->value != '%')
		return true;
	return false;
}

int getGCost(Node* currNode, Node* prevNode, vector<vector<Node*> >& graph){
	int posDelta = abs(currNode->xVal - prevNode->xVal) + abs(currNode->yVal - prevNode->yVal);
	if(posDelta == 2)
		currNode->gVal = prevNode->gVal + diagonalUnit;
	else
		currNode->gVal = prevNode->gVal + straightLineUnit;
	return currNode->gVal;
}

int getHCost(Node* currNode, vector<vector<Node*> >& graph){
	Node* endNode = findEndNode(graph);
	int hCost = abs(currNode->xVal - endNode->xVal) + abs(currNode->yVal - endNode->yVal);
	//cout << "end node pos: (" << endNode->xVal << ", " << endNode->yVal << ")" <<endl;
	return hCost;
}

bool onClosedList(Node* currNode, vector<Node*> closedList){
	for(int x = 0; x < closedList.size(); x++)
		if(currNode == closedList[x])
			return true;
	return false;
}

vector<Node*> getAdjNodes(Node* currNode, vector<Node*> openList, vector<Node*> closedList, vector<vector<Node*> >& graph){
	vector<Node*> adjNodes;
	if(0 < currNode->xVal-1)
		if(!onClosedList(graph[currNode->xVal - 1][currNode->yVal], closedList) && isValidMovePosition(graph[currNode->xVal - 1][currNode->yVal])) // (x-1, y)
			adjNodes.push_back(graph[currNode->xVal - 1][currNode->yVal]);
	if(currNode->xVal + 1 < graph.size())
		if(!onClosedList(graph[currNode->xVal + 1][currNode->yVal], closedList) && isValidMovePosition(graph[currNode->xVal + 1][currNode->yVal])) // (x+1, y)
			adjNodes.push_back(graph[currNode->xVal + 1][currNode->yVal]);
	if(0 < currNode->xVal-1 && 0 < currNode->yVal-1)
		if(!onClosedList(graph[currNode->xVal - 1][currNode->yVal-1], closedList) && isValidMovePosition(graph[currNode->xVal - 1][currNode->yVal-1])) // (x-1, y-1)
			adjNodes.push_back(graph[currNode->xVal - 1][currNode->yVal -1]);
	if(currNode->xVal + 1 < graph.size() && 0 < currNode->yVal-1)
		if(!onClosedList(graph[currNode->xVal + 1][currNode->yVal-1], closedList) && isValidMovePosition(graph[currNode->xVal + 1][currNode->yVal-1])) // (x+1, y-1)
			adjNodes.push_back(graph[currNode->xVal +1][currNode->yVal -1]);
	if(currNode->xVal + 1 < graph.size() && currNode->yVal + 1 < graph[0].size())
		if(!onClosedList(graph[currNode->xVal + 1][currNode->yVal+1], closedList) && isValidMovePosition(graph[currNode->xVal + 1][currNode->yVal+1])) // (x+1, y+1)
			adjNodes.push_back(graph[currNode->xVal +1][currNode->yVal+1]);
	if(0 < currNode->xVal-1 && currNode->yVal + 1 < graph[0].size())
		if(!onClosedList(graph[currNode->xVal - 1][currNode->yVal+1], closedList) && isValidMovePosition(graph[currNode->xVal - 1][currNode->yVal+1])) // (x-1, y+1)
			adjNodes.push_back(graph[currNode->xVal - 1][currNode->yVal+1]);
	if(0 < currNode->yVal-1)
		if(!onClosedList(graph[currNode->xVal][currNode->yVal-1], closedList) && isValidMovePosition(graph[currNode->xVal][currNode->yVal-1])) // (x, y-1)
			adjNodes.push_back(graph[currNode->xVal][currNode->yVal -1]);
	if( currNode->yVal + 1 < graph[0].size())
		if(!onClosedList(graph[currNode->xVal][currNode->yVal+1], closedList) && isValidMovePosition(graph[currNode->xVal][currNode->yVal+1])) // (x, y+1)
			adjNodes.push_back(graph[currNode->xVal][currNode->yVal+1]);
	return adjNodes;
}

Node* getNodeWithLowestFCost(vector<Node*>& openList, vector<Node*>& closedList, bool& randomEventHappened){
	int lowestFval = 10000000;
	Node* returnNode;
	int listPos;
	for(int x = 0; x < openList.size(); x++){
		if(openList[x]->fVal < lowestFval){
			lowestFval = openList[x]->fVal;
			returnNode = openList[x];
			listPos = x;
		}
	}
	openList.erase(openList.begin() + listPos);	
	closedList.push_back(returnNode);
	return returnNode;
}

void induceRandomEvent(vector<Node*>& closedList){
	srand(time(NULL));
	int randNum = rand() % closedList.size();
	if(closedList[randNum]->value != 'S' && closedList[randNum]->value != 'E')
		closedList[randNum]->value = '#';
	else
		induceRandomEvent(closedList);
}

bool RandomEventInduced = false;

void resetGraph(vector<vector<Node*> >& graph){
	for(int x = 0; x < graph.size(); x++)
		for(int y = 0; y < graph[x].size(); y++){
			if(graph[x][y]->value == '*')
				graph[x][y]->value = '.';
			graph[x][y]->hVal = 0;
			graph[x][y]->gVal = 0;
			graph[x][y]->fVal = 0;
			graph[x][y]->parent = NULL;
		}	
}

void aStar(vector<vector<Node*> >& graph){
	vector<Node*> openList;
    vector<Node*> closedList;
    Node* startNode = findStartNode(graph);
    openList.push_back(startNode);
    startNode->hVal = getHCost(startNode, graph);
    startNode->gVal = 0;
    startNode->calcFVal();
    Node* currNode;
    bool randomEventHappened = false;
    while(openList.size() > 0 && isEndNodePresentp(openList)){
    	currNode = getNodeWithLowestFCost(openList, closedList, randomEventHappened);
    	vector<Node*> adjNodes = getAdjNodes(currNode, openList, closedList, graph);
    	for(int x = 0; x < adjNodes.size(); x++){
    		if(!onClosedList(adjNodes[x], openList)){ // if node is not present on open list, record values and add it to it
    			adjNodes[x]->gVal = getGCost(adjNodes[x], currNode, graph);
    			adjNodes[x]->hVal = getHCost(adjNodes[x], graph);
    			adjNodes[x]->calcFVal();
    			adjNodes[x]->parent = currNode;
    			openList.push_back(adjNodes[x]);
    		}
    		else{
    			if(getGCost(adjNodes[x], currNode, graph) < adjNodes[x]->gVal){
    				currNode->parent = adjNodes[x];
    				currNode->gVal = getGCost(currNode, adjNodes[x], graph);
    				currNode->calcFVal();
    			}
    		}
    	}
    }
    vector<Node*> finalPath;
    currNode = findEndNode(graph);
    if(!isEndNodePresentp(openList)){
	    while(currNode != startNode){
	    	finalPath.push_back(currNode);
	    	//cout << "at node at position (" << currNode->xVal << ", " << currNode->yVal << "), where value is: " << currNode->value << endl;
	    	if(currNode->value != 'S' && currNode->value != 'E' && currNode->value != '#')
	    		currNode->value = '0';
	    	currNode = currNode->parent;
	    }
	    if(!RandomEventInduced){
	    	RandomEventInduced = true;
	    	induceRandomEvent(finalPath);
	    	resetGraph(graph);
	    	aStar(graph);
	    	return;
		}
	    for(int x = 0; x < closedList.size(); x++)
	    	if(!onClosedList(closedList[x], finalPath) && closedList[x]->value != 'E'){
	    		//cout << "node [" << closedList[x]->value << "]" << " at pos (" << closedList[x]->xVal << ", " << closedList[x]->yVal << ") is on the closed list but not the path" << endl;
	    		//closedList[x]->value = '$';
	    	}
	    printGraph(graph);
	}	
	else
		cout << "Maze is not solvable" << endl;
}


void setRandoms(vector<vector<Node*> >& graph)
{
	srand(time(NULL));
	int randomXindex = rand()%10+1;
    
	//Get Random start node
	int randomYindex = rand()%10+1;
	cout << "before old" << endl;
	Node* oldStart = findStartNode(graph);
	cout << "between" << endl;
	cout << randomXindex << " <-- new x     new y -->  " << randomYindex << endl;
	Node* newStart = graph[randomYindex][randomXindex];
	cout << "after new" << endl;
	char sym = newStart->value;
	oldStart->value = sym;
	cout << "between set vals" << endl;
	newStart->value = 'S';
    
	//reset end node
    
	int newHval = rand()%25 + 1;
	int newHyval = rand()%25  + 1;    
	Node* oldEnd = findEndNode(graph);
	Node* newEnd = graph[randomYindex+ newHyval][randomXindex+newHval];
	oldEnd->value = newEnd->value;
	newEnd->value = 'E';
	
}



int main(int argc, char *argv[]){
	if( argc != 2 ){
        cerr << "Usage: " << argv[ 0 ] << " graphfile" << endl;
        return 1;
    }
    ifstream inFile( argv[ 1 ] );
    if( !inFile ){
        cerr << "Cannot open " << argv[ 1 ] << endl;
        return 1;
    }
    vector<string> stringGraph;
    string graphLine;
    while(inFile >> graphLine){
    	stringGraph.push_back(graphLine);
    }
    vector<vector<Node*> > graph = makeGraphOfNodes(stringGraph);
    setRandoms(graph);
    aStar(graph);
	return 0;
}