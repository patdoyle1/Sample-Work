#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int** initArray(int height, int width){
	int** beforeArr = new int*[height];
	for(int i = 0; i < height; ++i){
		beforeArr[i] = new int[width];
	}
	return beforeArr;
	    

}
void printArray(int** beforeArr, int height, int width ){
	for(int x = 0; x < height; x++){
		for(int y = 0; y < width; y++){
			cout << beforeArr[x][y] << ' ';
		}
		cout << endl;
	}
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

void negative(int** beforeArr, int height,int width, int scale){

	for(int x = 0; x < height; x++){
		for(int y = 0; y < width; y++){
			int val  = beforeArr[x][y];
			beforeArr[x][y] = scale - val;
		}
		cout << endl;
	}
}

void rotate(int** beforeArr, int** afterArr, int height,int width){
	int counter = 0;
	
	int newArrWidthCounter = height;
	int newArrHeightCounter = width;

	int currHeight = 0;
	
	int x;
	while(counter < width){
		x  = height - 1;
		int currWidth = 0;
		for(; x >= 0; x--){
			afterArr[currHeight][currWidth] = beforeArr[x][counter];
			currWidth++;
		}
		currHeight++;
		counter++;

	}
		
}

int main(int argc, char *argv[]){

	
	if (argc != 4){
		cerr << "wrong number of inputs" << endl;
		return 0;
	}
	else{
		ifstream file(argv[1]);
		string outfileNeg(argv[2]);
		string outfileRotate(argv[3]);
		int height;
		int width;
		string type;
		int scale;

		if(file.is_open()){
			getline(file, type);
			if(type != "P2"){
				cerr << "wrong input file " << endl;
				return 0;
			}
			
			file >> width;
			file >> height;
			file >> scale;
			int** beforeArr = initArray(height,width);
			int** rotateArray = initArray(width,height);
			int** negativeArray = initArray(height,width);
			//load basearr
			for(int x = 0; x < height; x++){
				for(int y = 0; y < width; y++){
	  				file >> beforeArr[x][y];
				}
			}
			for(int x = 0; x < height; x++){
				for(int y = 0; y < width; y++){
	  				negativeArray[x][y] = beforeArr[x][y];
				}
			}
			
			negative(negativeArray, height, width, scale);
			rotate(beforeArr, rotateArray, height, width);
		
			
			writeFile("beforeArr.pgm",beforeArr, height, width, scale, type);
			writeFile(outfileNeg, negativeArray, height, width, scale, type);
			writeFile(outfileRotate, rotateArray, width, height, scale, type);

		}
	}
	
	
	
	
	



	return 0;
}