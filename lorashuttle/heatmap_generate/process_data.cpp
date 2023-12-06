#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <numeric>
#include <unordered_map>
#include <algorithm>
#include <unordered_set>
using namespace std;


int LoRa_X = 930;
int LoRa_Y = 540;

int LOST = 327670;


int main(int argc, char const *argv[])
{	
	ifstream Reader;
	Reader.open("data.csv");

	unordered_map<int, int> m;

	while(1){
		int temp;
		Reader >> temp;
		if(Reader.eof())
			break;
		m[temp]++;
	}


	for(auto c : m){
		cout << c.first << " " << c.second << endl;
	}



	return 0;
}