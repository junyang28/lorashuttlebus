#include "matplotlibcpp.h"
#include <cmath>
#include <fstream>
#include <iostream>

namespace plt = matplotlibcpp;
using namespace std;

#define SF7 0
#define SF8 1
#define SF9 2
#define SF10 3
#define SF11 4
#define SF12 5


int main()
{

	std::vector<int> vx, vy;
	ifstream reader;
	ofstream writer;
	reader.open("test380.csv");
	writer.open("results.csv");
	int lastx = -1, lasty = -1;
	vector<int> last;
	int linecnt = 0;
	if(reader.is_open()){
		while(1){
			int rssi, noise, x, y;
			char temp;
			vector<vector<int>> cur;
			for(int i = 0; i < 6; i++){
				reader >> rssi >> temp >> noise >> temp >> x >> temp >> y;
				cur.push_back({rssi, noise, x*10 , y*10});
			}
			linecnt++;
			if(reader.eof())
				break;

			int sf = SF11;
			writer << cur[sf][0] << temp << cur[sf][1] << temp << cur[sf][2] << temp << cur[sf][3] << endl;


/*
			if(x != 327670 && y != 327670 && lastx != x && lasty != y && x <= 150 && y <= 120){
				//cout << linecnt << " ";
				vx.push_back(x);
				vy.push_back(y);
				plt::clf();
				plt::scatter(vx, vy);
				plt::xlim(0, 150);
				plt::ylim(0, 120);
				plt::pause(0.1);
			}
			lastx = x;
			lasty = y;
*/
		}
	}
	//plt::show();
	reader.close();
	writer.close();
}