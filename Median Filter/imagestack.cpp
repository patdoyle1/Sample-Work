#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

/** CS515 Assignment X
File: imagestack.cpp
Name: Patrick Doyle
Section: 1
Date: 9/27
Collaboration Declaration: assistance received from TA, PAC etc.
*/ 

int** initmyVals(int height, int width){
	int** beforeArr = new int*[height];
	for(int i = 0; i < height; ++i){
		beforeArr[i] = new int[width];
	}
	return beforeArr;
	    

}

void writeFile(string filename, int** myArr, int height, int width, int scale, string type ){

	ofstream myFile;
	myFile.open(filename);
	myFile << type << "\n";
	myFile << width << ' ' << height << "\n" ;
	myFile << scale << "\n";
	for(int x = 0; x < height; x++){
		for(int y = 0; y < width; y++){
			myFile << myArr[x][y] << ' ';
		}
		myFile << "\n";
	}
}


int calcMedianAt(int** myImage, int y, int x,  int height, int width){
	int radius = 1;
	int result;
	int numNeighbors = 0;
	if (x == 0 && y == 0){

		int myVals[3];
		//top left corner
		//3 neighbors right, under, bottom right diag
		numNeighbors = 3;
		myVals[0] = myImage[y][x+1]; //right
		myVals[1] = myImage[y+1][x]; //under
		myVals[2] = myImage[y+1][x+1]; //bottom right diag

		sort(myVals, myVals + 3);

		return myVals[1];


	}
	else if (x == (width-1) && y == (height-1)){

		//bottom right corner
		//3 neighbors left, up, topleft diag
		int myVals[3];
		numNeighbors = 3;
		myVals[0] = myImage[y][x-1]; //left
		myVals[1] = myImage[y-1][x]; //up
		myVals[2] = myImage[y-1][x-1]; //top left diag

		sort(myVals, myVals + 3);

		return myVals[1];

	}
	else if (x == (width-1) && y == 0){
		//top right corner
		//3 neighbors, left, under, bottom left diag

		int myVals[3];
		numNeighbors = 3;
		myVals[0] = myImage[y][x-1]; //left
		myVals[1] = myImage[y+1][x]; //under
		myVals[2] = myImage[y+1][x-1]; //bottom left diag

		sort(myVals, myVals + 3);

		return myVals[1];


	}
	else if (x == 0 && y == (height-1)){
		//bottom left corner
		//3 neighbors, up, right, top right diag


		int myVals[3];
		numNeighbors = 3;
		myVals[0] = myImage[y][x+1]; //right
		myVals[1] = myImage[y-1][x]; //up
		myVals[2] = myImage[y-1][x+1]; //top right diag

		sort(myVals, myVals + 3);

		return myVals[1];


	}
	else if (y == 0){
		//top row not corner
		//5 neighbors, left, bottom left diag, under, bottom right diag, right

		numNeighbors = 5;
		int myVals[5];
		myVals[0] = myImage[y][x-1]; //left
		myVals[1] = myImage[y+1][x]; //under
		myVals[2] = myImage[y+1][x-1]; //bottom left diag
		myVals[4] = myImage[y+1][x+1]; //bottom right diag
		myVals[3] = myImage[y][x+1]; //right



		sort(myVals, myVals + 5);

		return myVals[2];


	}
	else if (x == 0){
		//first column not corner
		//5 neighbors, up, top right diag, right, bottom right diag, under		
		numNeighbors = 5;
		int myVals[5];
		myVals[1] = myImage[y-1][x]; //up
		myVals[2] = myImage[y-1][x+1]; //top right diag
		myVals[3] = myImage[y][x+1]; //right
		myVals[4] = myImage[y+1][x+1]; //bottom right diag
		myVals[0] = myImage[y+1][x]; // under

		sort(myVals, myVals + 5);

		return myVals[2];


	}
	
	else if (x == (width-1)){
		//last column not corner
		//5 neighbors, up, top left diag, left, bottom left diag, under		
		numNeighbors = 5;
		int myVals[5];
		myVals[1] = myImage[y-1][x]; //up
		myVals[2] = myImage[y-1][x-1]; //top left diag
		myVals[3] = myImage[y][x-1]; //left
		myVals[4] = myImage[y+1][x-1]; //bottom left diag
		myVals[0] = myImage[y+1][x]; // under

		sort(myVals, myVals + 5);

		return myVals[2];


	}
	else if (y == (height-1)){
		//last row not corner
		//5 neighbors, left, top left diag, up, top right diag, right	
		numNeighbors = 5;	
		int myVals[5];
		myVals[3] = myImage[y][x-1]; //left
		myVals[2] = myImage[y-1][x-1]; //top left diag
		myVals[1] = myImage[y-1][x]; //up
		myVals[4] = myImage[y-1][x+1]; //top right diag
		myVals[0] = myImage[y][x+1]; // right

		sort(myVals, myVals + 5);

		return myVals[2];


	}
	else{
		//in picture
		//8 neighbros, top left diag, up, top right diag, right, bottom right diag, under, bottom left diag,  left
		numNeighbors = 8;
		int myVals[8];
		myVals[3] = myImage[y][x-1]; //left
		myVals[2] = myImage[y-1][x-1]; //top left diag
		myVals[1] = myImage[y-1][x]; //up
		myVals[4] = myImage[y-1][x+1]; //top right diag
		myVals[0] = myImage[y+1][x]; // under
		myVals[5] = myImage[y][x+1]; // right
		myVals[6] = myImage[y+1][x-1]; // bottom left diag
		myVals[7] = myImage[y+1][x+1]; // bottom right diag

		sort(myVals, myVals + 8);

		int toReturn = ((myVals[3] + myVals[4])/2);

		return toReturn;

	}
}

void median(int*** imagestack, int** result, int stacksize, int height, int width, int scale){
	int numPixels = height * width;
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			int temp[stacksize];
			int counterwidth = 0;
			for(int i = 0; i < stacksize; i++){
				int currMedian = calcMedianAt(imagestack[i], y, x, height, width);
				temp[counterwidth] = currMedian;
				counterwidth++;
			}
			sort(temp, temp + (counterwidth+1));
			int resultMedian;
			if(counterwidth % 2 == 0){
				resultMedian = temp[counterwidth/2]	;
			}
			else{
				resultMedian = (temp[counterwidth/2] + temp[(counterwidth+1)/2])/2;
			}
			result[y][x] = resultMedian;
			

		}
		
	}



}
void delete3D(int*** array, int numPics, int height){
	for(int i = 0; i < numPics; i++){
		for(int x = 0; x < height; x++){
			delete [] array[i][x];
		}
		delete [] array[i];
	}
	delete [] array;
}
void delete2D(int** array, int height){
	for(int i = 0; i < height; i++){
		delete [] array[i];
	}
	delete [] array;
}


int main(int argc, char *argv[]){

	
	//ofstream outfile(arv[2]);
	int height;
	int width;
	int*** imagestack = new int**[argc - 2	];

	string type;
	int scale;
	int numPics = argc -2;
	cout << " this is num pics " << numPics << endl;
	int counter = 0;
	for(int i = 1; i < numPics +1; i++){
		ifstream file(argv[i]);
		if(file.is_open()){
			int currHeight;
			int currWidth;
			int currScale;
			string currType;
			getline(file, currType);
			file >> currWidth;
			file >> currHeight;
			file >> currScale;
			if(i == 1){
				height = currHeight;
				width = currWidth;
				scale = currScale;
				type = currType;
			}
			if(currType != "P2"){
				cerr << "wrong file type" << endl;
				return 0;
			}
			else if(height != currHeight || width != currWidth || currScale != scale || currType != type){
				cerr << "Image setup is not consistent" << endl;
				return 0;
			}
			//setup each image
			int** currImage = initmyVals(height,width);
			for(int x = 0; x < height; x++){
				for(int y = 0; y < width; y++){
	  				file >> currImage[x][y];
				}
			}
			imagestack[counter] = currImage;
			counter++;
		}

	}
	int** finalImage = initmyVals(height, width);
	median(imagestack, finalImage, numPics, height,width, scale);
	writeFile(argv[argc-1], finalImage, height, width, scale, type);
	delete3D(imagestack, numPics, height);
	delete2D(finalImage, height);

		
	
	
	
	



	return 0;
}