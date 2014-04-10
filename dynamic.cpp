#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
using namespace std;

vector<vector<int> > fillEVector(string str1, string str2){
	vector<vector<int> > eVector;
	for(int x = 0; x < str1.size(); x++){
		vector<int> currRow;
		for(int y = 0; y < str2.size(); y++){
			if(x==0)
				currRow.push_back(y);
			else if(y == 0)
				currRow.push_back(x);
			else
				currRow.push_back(100000);
		}
		eVector.push_back(currRow);
	}
	return eVector;
}

//vertical axis of eVector corresponds to str1, while the horizontal axis corresponds to str2
void printEVector(const vector<vector<int> >& eVector, const string& str1, const string& str2){
	for(int x = 0; x <= eVector.size(); x++){
		for(int y = 0; y <= eVector[0].size(); y++){
			if(x != 0 && y != 0)
				cout << eVector[x-1][y-1];
			else if(x == 0 && y == 0)
				cout << " ";
			else if(y == 0 && x != 0)
				cout << str1.at(x-1);
			else if(x == 0 && y != 0)
				cout << str2.at(y-1);
		}
		cout << endl;
	}
}

int diff(string str1, string str2, int str1Pos, int str2Pos){
	if(str1.at(str1Pos -1) == str2.at(str2Pos-1))
		return 0;
	else
		return 1;	
}

int min(int x, int y, int z){
	if(x <= y && x <= z)
		return x;
	else if(y <= x && y <= z)
		return y;
	else if(z <= x && z <= y)
		return z;
	return 0;
}

int minDist(string str1, string str2){
	vector<vector<int> > eVector = fillEVector(str1, str2);
	for(int x = 1; x < str1.size(); x++){
		for(int y = 1; y < str2.size(); y++)
			eVector[x][y] = min(1+eVector[x-1][y], 1+eVector[x][y-1], diff(str1, str2, x, y) + eVector[x-1][y-1]);	
	}
	printEVector(eVector, str1, str2);
	return eVector[eVector.size()-1][eVector[0].size()-1];
}

void longestPalSubstr( string str )
{
    int n = str.length(); 
    bool table[n][n];
    memset( table, 0, sizeof( table ) );
 
    int maxLength = 1;
    for( int i = 0; i < n; ++i )
        table[i][i] = true; //setting each 1 char to true
 
    int start = 0;
    for( int i = 0; i < n-1; ++i )
    {
        if( str[i] == str[i+1] ) // this is for 2 chars
        {
            table[i][i+1] = true;
            start = i;
            maxLength = 2;
        }
    }
    for( int k = 3; k <= n; ++k ) // made it 3 because 1 and 2 are always palindromes
    {
        for( int i = 0; i < n - k + 1 ; ++i )
        {
            int j = i + k - 1;
            if( table[i+1][j-1] && str[i] == str[j] )
            {
                table[i][j] = true;
 
                if( k > maxLength )
                {
                    start = i;
                    maxLength = k;
                }
            }
        }
    }
 
    cout << ("Longest palindrome substring is: ");
    cout << str.substr(start , start + maxLength) << endl;
 
}

int main(int argc, char *argv[]){
	if( argc != 3 ){
        cerr << "Usage: " << argv[ 0 ] << " string1 string2" << endl;
        return 1;
    }
    string str1 = argv[1];
    string str2 = argv[2];
    cout << "[" << str1 << "] and [" << str2 << "] has a min dist of [" << minDist(str1, str2) << "]" << endl;
	string str3 = "BBABCBAB";
	longestPalSubstr(str3);



	return 0;
}