#include <iostream>
#include <random>
#include <chrono>
using namespace std;

// Generate random csv file for large scale test

const string stations[] = {
	"Beijing Daxing",
	"Chengdu Shuangliu",
	"Ningbo Lishe",
	"Shanghai HongQiao",
	"Shenzhen Buzhidaomingzi",
	"Guangzhou Yebuzhidaomingzi",
	"Xiamen Najiusuibian",
	"Xian Youjichangma",
	"Lanzhou Youlamian",
	"Chongqing Huoguo"
};

int main() {
	mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
	uniform_int_distribution<int> dis_month(1, 12);
	uniform_int_distribution<int> dis_day(1, 28);
	uniform_int_distribution<int> dis_hour(0, 23);
	uniform_int_distribution<int> dis_minute(0, 59);
	uniform_int_distribution<int> dis_ticket_count(100, 350);
	uniform_int_distribution<int> dis_alpha(0, 25);
	uniform_int_distribution<int> dis_station_id(0, 9);

	cout << "Flight ID,Departure Station,Terminal Station,Departure Time,Arrival Time,Initial Ticket Count\n";
	
	const int N = 10000;
	
	for (int i = 0; i < N; i++) {
		char flight_id[7];
		flight_id[0] = 'A' + dis_alpha(rng);
		flight_id[1] = 'A' + dis_alpha(rng);
		for (int j = 2; j < 6; j++) {
			flight_id[j] = '0' + (rng() % 10);
		}
		flight_id[6] = '\0';
		
		int start_idx = dis_station_id(rng);
		int terminal_idx = (start_idx + 1 + dis_station_id(rng)) % 10;
		
		const int year = 2026;
		int month = dis_month(rng);
		int day = dis_day(rng);
		int dep_hour = dis_hour(rng);
		int dep_min = dis_minute(rng);
		
		int arr_hour = (dep_hour + 1 + rng() % 23) % 24;
		int arr_min = rng() % 60;
		if (arr_hour == dep_hour) {
			arr_min = (dep_min + 1 + rng() % 59) % 60;
		}
		int ticket_count = dis_ticket_count(rng);
		printf("%s,%s,%s,%d-%02d-%02d %02d:%02d,%d-%02d-%02d %02d:%02d,%d\n",
			flight_id, stations[start_idx].c_str(), stations[terminal_idx].c_str(),
			year, month, day, dep_hour, dep_min,
			year, month, day, arr_hour, arr_min,
			ticket_count);
	}
	return 0;
}