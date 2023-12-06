#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <numeric>
#include <unordered_map>
#include <algorithm>
using namespace std;


int LoRa_X = 930;
int LoRa_Y = 540;

int LOST = 327670;

//generate rssi variation compared with sf12
int rssi_variations_model(){
	ifstream reader;
	ofstream writer;
	reader.open("test380.csv");
	writer.open("results.csv");
	unordered_map<int, vector<int>> m;
	vector<int> last;
	int lastdis = -1;
	while(reader.is_open()){
		int rssi, noise, x, y;
		char temp;
		vector<vector<int>> cur;
		for(int i = 0; i < 6; i++){
			reader >> rssi >> temp >> noise >> temp >> x >> temp >> y;
			cur.push_back({rssi, noise, x*10 , y*10});
		}
		if(reader.eof())
			break;

		vector<int> rssi_arr;
		for(int i = 0; i < 6; i++){
			rssi_arr.push_back(cur[i][0]);
		}
		int diff = 0;
		int count = 0;
		if(rssi_arr[5] != 32767 && last.size()) {
			for(int i = 0; i < 4; i++){
				if(last[i] != 32767){
					diff += abs(last[i] - rssi_arr[5]);
					count++;
				}
			}
			if(rssi_arr[4] != 32767){
				diff += abs(rssi_arr[4] - rssi_arr[5]);
				count++;
			}
			if(count == 0 && diff > 17)
				continue;
			if(count != 0 && diff/count > 17)
				continue;
			int ans = 0;
			if(count == 0)
				ans = diff;
			else 
				ans = diff/count;
			if(ans > 12)
				ans -= 2;
			writer << ans << endl;
		}
		last = rssi_arr;
	}
	reader.close();
	writer.close();
	return 0;
}

//rssi and prr model relation
int prr_rssi_model(){
	ifstream reader;
	ofstream writer;
	//reader.open("test380.csv");
	reader.open("test381.csv");

	writer.open("results.csv");
	unordered_map<int, vector<int>> m;
	vector<vector<int>> last;
	int lastdis = -1;
	while(reader.is_open()){
		int rssi, noise, x, y;
		char temp;
		vector<vector<int>> cur;
		for(int i = 0; i < 6; i++){
			reader >> rssi >> temp >> noise >> temp >> x >> temp >> y;
			cur.push_back({rssi, noise, x*10 , y*10});
		}
		if(reader.eof())
			break;

		int received_rssi = 0;
		int count = 0;

		for(int i = 0; i < cur.size(); i++){
			if(cur[i][0] != LOST/10){
				received_rssi += cur[i][0];
				count++;
			}
		}

		if(count == 0)
			continue;

		received_rssi /= count;
		//sf=0->SF7 sf=1->SF8 sf=2->SF9 ... sf=5 -> SF12 
		int sf = 5;


		if(cur[sf][0] == LOST/10 || cur[sf][1] == LOST/10 || cur[sf][2] == LOST || cur[sf][3] == LOST){
			//writer << "0" << " " << received_rssi << endl;
			m[received_rssi].push_back(0);
		}
		else {
			//writer << "1" << " " << received_rssi << endl;
			m[received_rssi].push_back(1);
 		}

 		if(m[received_rssi].size() == 100){
 			int sum = std::accumulate(m[received_rssi].begin(), m[received_rssi].end(), 0);
 			//cout << received_rssi << "\t" << sum << endl;
 			writer << received_rssi << "\t" << sum << endl;
 			m[received_rssi].clear();
 		}

		

	}
	reader.close();
	writer.close();
	return 0;
}

//rssi & noise -> prr model
int prr_rssi_noise_model(){
	ifstream reader;
	ofstream writer;
	reader.open("test380.csv");
	//reader.open("test381.csv");
	writer.open("results.csv");
	unordered_map<string, vector<int>> m;
	vector<vector<int>> last;
	int lastdis = -1;
	while(reader.is_open()){
		int rssi, noise, x, y;
		char temp;
		vector<vector<int>> cur;
		for(int i = 0; i < 6; i++){
			reader >> rssi >> temp >> noise >> temp >> x >> temp >> y;
			cur.push_back({rssi, noise, x*10 , y*10});
		}
		if(reader.eof())
			break;

		int received_rssi = 0;
		int count1 = 0;

		int received_noise = 0;
		int count2 = 0;

		for(int i = 0; i < cur.size(); i++){
			if(cur[i][0] != LOST/10){
				received_rssi += cur[i][0];
				count1++;
			}
			if(cur[i][1] != LOST/10){
				received_noise += cur[i][1];
				count2++;
			}
		}
		if(count1 == 0 || count2 == 0)
			continue;
		received_rssi /= count1;
		received_noise /= count2;
		//sf=0->SF7 sf=1->SF8 sf=2->SF9 sf=3->SF10 sf=4->SF11 sf=5 -> SF12 
		int sf = 0;
		string key = to_string(received_rssi) + " " + to_string(received_noise);
		if(cur[sf][0] == LOST/10 || cur[sf][1] == LOST/10 || cur[sf][2] == LOST || cur[sf][3] == LOST){
			//writer << "0" << " " << received_rssi << endl;
			m[key].push_back(0);
		}
		else {
			//writer << "1" << " " << received_rssi << endl;
			m[key].push_back(1);
 		}

 		if(m[key].size() == 100){
 			int sum = std::accumulate(m[key].begin(), m[key].end(), 0);
 			//cout << received_rssi << "\t" << sum << endl;
 			writer << received_rssi << "\t" << received_noise << "\t" << sum << endl;
 			m[key].clear();
 		}
	}
	reader.close();
	writer.close();
	return 0;
}

//x_location & y_location -> prr model
int prr_location_model(){
	ifstream reader;
	ofstream writer;
	reader.open("test380.csv");
	writer.open("results.csv");
	unordered_map<string, vector<int>> m;
	vector<vector<int>> last;
	int lastdis = -1;
	while(reader.is_open()){
		int rssi, noise, x, y;
		char temp;
		vector<vector<int>> cur;
		for(int i = 0; i < 6; i++){
			reader >> rssi >> temp >> noise >> temp >> x >> temp >> y;
			cur.push_back({rssi, noise, x*10 , y*10});
		}
		if(reader.eof())
			break;

		int received_x = 0;
		int count1 = 0;

		int received_y = 0;
		int count2 = 0;

		int received_rssi = 0;
		int count3 = 0;

		for(int i = 0; i < cur.size(); i++){
			if(cur[i][0] != LOST/10){
				received_rssi += cur[i][0];
				count3++;
			}
			if(cur[i][2] != LOST){
				received_x += cur[i][2];
				count1++;
			}
			if(cur[i][3] != LOST){
				received_y += cur[i][3];
				count2++;
			}
		}
		if(count1 == 0 || count2 == 0)
			continue;
		received_rssi /= count3;
		received_x /= count1;
		received_y /= count2;
		//sf=0->SF7 sf=1->SF8 sf=2->SF9 sf=3->SF10 sf=4->SF11 sf=5 -> SF12 
		int sf = 0;
		string key = to_string(received_x) + " " + to_string(received_y);
		if(cur[sf][0] == LOST/10 || cur[sf][1] == LOST/10 || cur[sf][2] == LOST || cur[sf][3] == LOST){
			//writer << "0" << " " << received_rssi << endl;
			m[key].push_back(0);
		}
		else {
			//writer << "1" << " " << received_rssi << endl;
			m[key].push_back(1);
 		}

 		if(m[key].size() == 100){
 			int sum = std::accumulate(m[key].begin(), m[key].end(), 0);
 			//cout << received_rssi << "\t" << sum << endl;
 			writer << received_x << "\t" << received_y << "\t" << received_rssi << "\t" << sum << endl;
 			m[key].clear();
 		}
	}
	reader.close();
	writer.close();
	return 0;
}


//x_location & y_location -> prr model & label
int prr_location_model_label(){
	ifstream reader;
	ofstream writer;
	reader.open("test380.csv");
	writer.open("results.csv");
	unordered_map<string, vector<int>> m;
	vector<vector<int>> last;
	int lastdis = -1;
	while(reader.is_open()){
		int rssi, noise, x, y;
		char temp;
		vector<vector<int>> cur;
		for(int i = 0; i < 6; i++){
			reader >> rssi >> temp >> noise >> temp >> x >> temp >> y;
			cur.push_back({rssi, noise, x*10 , y*10});
		}
		if(reader.eof())
			break;

		int received_x = 0;
		int count1 = 0;

		int received_y = 0;
		int count2 = 0;

		int received_rssi = 0;
		int count3 = 0;

		for(int i = 0; i < cur.size(); i++){
			if(cur[i][0] != LOST/10){
				received_rssi += cur[i][0];
				count3++;
			}
			if(cur[i][2] != LOST){
				received_x += cur[i][2];
				count1++;
			}
			if(cur[i][3] != LOST){
				received_y += cur[i][3];
				count2++;
			}
		}
		if(count1 == 0 || count2 == 0)
			continue;
		received_x /= count1;
		received_y /= count2;
		//sf=0->SF7 sf=1->SF8 sf=2->SF9 sf=3->SF10 sf=4->SF11 sf=5 -> SF12 
		int sf = 0;
		string key = to_string(received_x) + " " + to_string(received_y);
		if(cur[sf][0] == LOST/10 || cur[sf][1] == LOST/10 || cur[sf][2] == LOST || cur[sf][3] == LOST){
			//writer << "0" << " " << received_rssi << endl;
			m[key].push_back(0);
		}
		else {
			//writer << "1" << " " << received_rssi << endl;
			m[key].push_back(1);
 		}

 		if(m[key].size() == 100){
 			int sum = std::accumulate(m[key].begin(), m[key].end(), 0);
 			//cout << received_rssi << "\t" << sum << endl;
 			writer << received_x << "\t" << received_y << "\t" << sum << "\t";
 			x = received_x;
 			y = received_y;
 			if(x >= 0 && x <= 500 && y >= 0 && y <= 600){
 				writer << "1";
 			}
 			else if(x >= 0 && x <= 500 && y >= 600 && y <= 1200){
 				writer << "2";
 			}
 			else if(x >= 500 && x <= 1000 && y >= 0 && y <= 550){
 				writer << "3";
 			} 			
 			else if(x >= 480 && x <= 1500 && y >= 551 && y <= 800){
 				writer << "4";
 			} 			
 			else if(x >= 1000 && x <= 1500 && y >= 801 && y <= 1400){
 				writer << "5";
 			} 			
 			else if(x >= 500 && x <= 1000 && y >= 801 && y <= 1400){
 				writer << "6";
 			}
 			else 
 				writer << "7";

 			writer << endl;

 			m[key].clear();
 		}
	}
	reader.close();
	writer.close();
	return 0;
}


//x_location & y_location -> label
int prr_location_model_label2(){
	ifstream reader;
	ofstream writer;
	reader.open("test380.csv");
	writer.open("results.csv");

	vector<vector<int>> last;
	int lastdis = -1;
	while(reader.is_open()){
		int rssi, noise, x, y;
		char temp;
		vector<vector<int>> cur;
		for(int i = 0; i < 6; i++){
			reader >> rssi >> temp >> noise >> temp >> x >> temp >> y;
			cur.push_back({rssi, noise, x*10 , y*10});
		}
		if(reader.eof())
			break;

		int received_x = 0;
		int count1 = 0;

		int received_y = 0;
		int count2 = 0;

		int received_rssi = 0;
		int count3 = 0;

		int received_noise = 0;
		int count4 = 0;

		for(int i = 0; i < cur.size(); i++){
			if(cur[i][0] != LOST/10){
				received_rssi += cur[i][0];
				count3++;
			}
			if(cur[i][2] != LOST){
				received_x += cur[i][2];
				count1++;
			}
			if(cur[i][3] != LOST){
				received_y += cur[i][3];
				count2++;
			}
		}
		if(count1 == 0 || count2 == 0)
			continue;
		received_x /= count1;
		received_y /= count2;
		//sf=0->SF7 sf=1->SF8 sf=2->SF9 sf=3->SF10 sf=4->SF11 sf=5 -> SF12 
		int sf = 1;

 
		//cout << received_rssi << "\t" << sum << endl;
		writer << cur[sf][0] << "\t" << cur[sf][1] << "\t" << received_x << "\t" << received_y << "\t";
		x = received_x;
		y = received_y;
		/*
		if(x >= 0 && x <= 500 && y >= 0 && y <= 600){
			writer << "1";
		}
		else if(x >= 0 && x <= 500 && y >= 600 && y <= 1200){
			writer << "2";
		}
		else if(x >= 500 && x <= 1000 && y >= 0 && y <= 550){
			writer << "3";
		} 			
		else if(x >= 480 && x <= 1500 && y >= 551 && y <= 800){
			writer << "4";
		} 			
		else if(x >= 1000 && x <= 1500 && y >= 801 && y <= 1400){
			writer << "5";
		} 			
		else if(x >= 500 && x <= 1000 && y >= 801 && y <= 1400){
			writer << "6";
		}
		else 
			writer << "7";
		*/
		if(x >= 900 && x <= 1100 && y >= 530 && y <= 550){
			writer << "1";
		}
		else 
			writer << "0";
		writer << endl;

 		
	}
	reader.close();
	writer.close();
	return 0;
}



int main(int argc, char const *argv[])
{
	prr_location_model_label2();
	return 0;
}