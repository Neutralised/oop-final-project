#include "solver.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>

std::string timeToString(const system_clock::time_point& tp) {
	time_t t = system_clock::to_time_t(tp);
	tm tm_ = *localtime(&t);
	char buffer[20];
	sprintf(buffer, "%04d-%02d-%02d %02d:%02d", 
	tm_.tm_year + 1900, tm_.tm_mon + 1, tm_.tm_mday, 
	tm_.tm_hour, tm_.tm_min);
	return buffer;
}
bool checkFlightID(const std::string& id) {
	return id.length() == 6;
}
bool checkDate(const std::string& date) {
	for(int _idx = 0; _idx < 10; ++_idx)
		if(_idx != 4 && _idx != 7
		&& !isdigit(date[_idx])) {
		return false;
	}
	return true;
}
bool checkTime(const std::string& time) {
	for(int _idx = 0; _idx < 16; ++_idx)
		if(_idx != 4 && _idx != 7 && _idx != 10 && _idx != 13
		&& !isdigit(time[_idx])) {
		return false;
	}
	return true;
}

Solver::Solver() {
	flights_.clear();
	ht_id_.clear();
	ht_depart_.clear();
	ht_terminal_.clear();
}

Solver::~Solver() {
	flights_.clear();
	ht_id_.clear();
	ht_depart_.clear();
	ht_terminal_.clear();
}

void Solver::addFlight() {
	std::string id, start, terminal, depart, arrive;
	int count;
	Log("ADD", "Adding a flight");
	std::cout << "- Flight ID: ";
	std::cin >> id;
	if(!checkFlightID(id)) {
		Log("ERROR", "Flight ID format error");
		return;
	}
	std::cout << "- Departure Station: ";
	std::cin.ignore(), getline(std::cin, start);
	std::cout << "- Terminal Station: ";
	getline(std::cin, terminal);
	std::cout << "- Departure Time: ";
	getline(std::cin, depart);
	if(!checkTime(depart)) {
		Log("ERROR", "Time format error");
		return;
	}
	std::cout << "- Arrival Time: ";
	getline(std::cin, arrive);
	if(!checkTime(arrive)) {
		Log("ERROR", "Time format error");
		return;
	}
	std::cout << "- Ticket Count: ";
	std::cin >> count;
	if(count < 0) {
		Log("ERROR", "Ticket count must be >= 0");
		return;
	}
	flights_.push_back(Airline(id, start, terminal, depart, arrive, count));
	int idx = static_cast<int>(flights_.size()) - 1;
	ht_id_[id] = idx;
	ht_depart_[depart.substr(0, 10)].emplace_back(idx); // 同一天的航班一起存储
	ht_terminal_[terminal].emplace_back(idx);
}

Airline& Solver::queryFlight(const std::string& key, int type) {
	switch(type) {
		case 1:
			// find by ID
			if(ht_id_.find(key) == ht_id_.end()) {
				Log("ERROR", "Flights with ID " + key + " doesn't exist");
				return null_flight;
			}
			return flights_[ht_id_[key]];
			break;
		case 2:
			// find by Departure Date
			if(ht_depart_.find(key) == ht_depart_.end()) {
				Log("ERROR", "Flights with Departure Date " + key + " doesn't exist");
				return null_flight;
			}
			if(static_cast<int>(ht_depart_[key].size()) == 0) {
				Log("ERROR", "Flights with Departure Date " + key + " doesn't exist");
				return null_flight;
			}
			if(static_cast<int>(ht_depart_[key].size()) > 1) {
				Log("NOTICE", "There are multiple flights with Departure Date " + key + ", listed below:");
				for(int idx : ht_depart_[key]) {
					std::cout << "- ";
					Log("FLIGHT", flights_[idx].id() + ", precise time: " + timeToString(flights_[idx].depart_time()).substr(10));
				}
				std::cout << "- Please choose one of them with ID: ";
				std::string id_needed;
				std::cin >> id_needed;
				return queryFlight(id_needed, 1);
			}
			return flights_[ht_depart_[key][0]]; // only one flight with this departure date
			break;
		case 3:
			// find by Terminal Station
			if(ht_terminal_.find(key) == ht_terminal_.end()) {
				Log("ERROR", "Flights with Terminal Station " + key + " doesn't exist");
				return null_flight;
			}
			if(static_cast<int>(ht_terminal_[key].size()) == 0) {
				Log("ERROR", "Flights with Terminal Station " + key + " doesn't exist");
				return null_flight;
			}
			if(static_cast<int>(ht_terminal_[key].size()) > 1) {
				Log("NOTICE", "There are multiple flights with Terminal Station " + key + ", listed below:");
				for(int idx : ht_terminal_[key]) {
					std::cout << "- ";
					Log("FLIGHT", flights_[idx].id());
				}
				std::cout << "Please choose one of them with ID: ";
				std::string id_needed;
				std::cin >> id_needed;
				return queryFlight(id_needed, 1);
			}
			return flights_[ht_terminal_[key][0]]; // only one flight with this terminal station
			break;
		default:
			Log("ERROR", "Type invalid calling queryFlight");
			break;
	}
	return null_flight;
}

Airline& Solver::queryFlightInteract() {
	std::string key;
	std::cout << "- Querying Flights: \n";
	std::cout << "- Input 1 for ID, 2 for Departure Date, 3 for Terminal Station: ";
	std::string op;
	std::cin >> op;
	if (op == "1") {
		std::cout << "- ID Format: XXYYYY, where Xs are alphas and Ys are numbers.\n";
		std::cout << "- Please input Flight ID: ";
		std::cin >> key;
		if(key.length() != 6) {
			Log("ERROR", "ID format error");
			return null_flight;
		}
	} else if (op == "2") {
		std::cout << "- Date format: YYYY-MM-DD\n";
		std::cout << "- Please input Departure Date: ";
		std::cin.ignore(), getline(std::cin, key);
		if(!checkDate(key)) {
			Log("ERROR", "Date format error: not formated correctly");
			return null_flight;
		}
	} else if (op == "3") {
		std::cout << "- Please input Terminal Station: ";
		std::cin.ignore(), getline(std::cin, key);
	} else {
		Log("ERROR", "Input format error: option not included");
		return null_flight;
	}
	return queryFlight(key, std::stoi(op));
}

void Solver::reserveTicket(const std::string& user_id, int count, Airline& flight) {
	if(flight.id() == "$") {
		Log("ERROR", "Reserving Ticket from default flight");
		return;
	}
	int code = flight.addReservation(user_id, count);
	if(code == 1) {
		Log("ERROR", "User ID format error");
		return;
	}
	if(code == 2) {
		Log("ERROR", "Ticket of flight " + flight.id() + " is not enough for reservation request!");
		return;
	}
	Log("OP", "Reservation from user " + user_id + " for flight " + flight.id() + " with " + std::to_string(count) + " tickets succeeded.");
}

void Solver::cancelTicket(const std::string& user_id, int count, Airline& flight) {
	if(flight.id() == "$") {
		Log("ERROR", "Canceling Reservation from default flight");
		return;
	}
	int code = flight.delReservation(user_id, count);
	if(code == 1) {
		Log("ERROR", "User ID format error");
		return;
	}
	if(code == 2) {
		Log("ERROR", "User " + user_id + " hasn't reserved any ticket for flight " + flight.id());
		return;
	}
	Log("OP", "Cancelation from user " + user_id + " for flight " + flight.id() + " with " + std::to_string(count) + " tickets succeeded.");
	if(code == 3) {
		Log("NOTICE", "User canceled with a ticket count larger than reservation, so system cleared all reservation from user");
	}
}

void Solver::reserveFlight(Airline& flight) {
	std::string user_id;
	int count = 0;
	Log("OP", "Reservation");
	if(flight.id() == "$") {
		Log("ERROR", "Flight query failed");
		return;
	}
	std::cout << "- Please input your user ID: ";
	std::cin.ignore(), getline(std::cin, user_id);
	std::cout << "- Please input ticket amount: ";
	std::cin >> count;
	reserveTicket(user_id, count, flight);
}

void Solver::cancelFlight(Airline& flight) {
	std::string user_id;
	int count = 0;
	Log("OP", "Canceling Reservation");
	if(flight.id() == "$") {
		Log("ERROR", "Flight query failed");
		return;
	}
	std::cout << "- Please input your user ID: ";
	std::cin.ignore(), getline(std::cin, user_id);
	std::cout << "- Please input ticket amount: ";
	std::cin >> count;
	cancelTicket(user_id, count, flight);
}

void Solver::reserveFlight() {
	std::string user_id;
	int count = 0;
	Log("OP", "Reservation");
	Airline& flight = queryFlightInteract();
	if(flight.id() == "$") {
		Log("ERROR", "Flight query failed");
		return;
	}
	std::cout << "- Please input your user ID: ";
	std::cin.ignore(), getline(std::cin, user_id);
	std::cout << "- Please input ticket amount: ";
	std::cin >> count;
	reserveTicket(user_id, count, flight);
}

void Solver::cancelFlight() {
	std::string user_id;
	int count = 0;
	Log("OP", "Canceling Reservation");
	Airline& flight = queryFlightInteract();
	if(flight.id() == "$") {
		Log("ERROR", "Flight query failed");
		return;
	}
	std::cout << "- Please input your user ID: ";
	std::cin.ignore(), getline(std::cin, user_id);
	std::cout << "- Please input ticket amount: ";
	std::cin >> count;
	cancelTicket(user_id, count, flight);
}

void Solver::addFlightFromFile(const std::string& path) {
	std::ifstream file(path);
	if(!file.is_open()) {
		Log("ERROR", "Failed to open file: " + path);
		return;
	}
	std::string line;
	std::getline(file, line); // 跳过表头
	while(std::getline(file, line)) {
		std::stringstream ss(line);
		std::string id, start, terminal, depart, arrive, ticket_count_str, reserved_users_str;
		std::getline(ss, id, ',');
		std::getline(ss, start, ',');
		std::getline(ss, terminal, ',');
		std::getline(ss, depart, ',');
		std::getline(ss, arrive, ',');
		std::getline(ss, ticket_count_str, ',');
		std::getline(ss, reserved_users_str);
		int ticket_count = std::stoi(ticket_count_str);
		Airline flight(id, start, terminal, depart, arrive, ticket_count);
		// 处理预订信息，若无则忽略
		if(reserved_users_str.length() > 0) {
			std::stringstream users_ss(reserved_users_str);
			std::string user_info;
			while(std::getline(users_ss, user_info, ';')) {
				std::stringstream user_ss(user_info);
				std::string user_id, count_str;
				std::getline(user_ss, user_id, ':');
				std::getline(user_ss, count_str);
				int count = std::stoi(count_str);
				flight.addReservation(user_id, count); // 直接调用航班的添加预订方法
			}
		}
		flights_.push_back(flight);
		int idx = static_cast<int>(flights_.size()) - 1;
		ht_id_[id] = idx;
		ht_depart_[depart.substr(0, 10)].emplace_back(idx); // date
		ht_terminal_[terminal].emplace_back(idx);
	}
	file.close();
	Log("OP", "Flights added successfully from file: " + path);
}

void Solver::saveFlightToFile(const std::string& path) {
	std::ofstream file(path);
	if(!file.is_open()) {
		Log("ERROR", "Failed to open file: " + path);
		return;
	}
	// 表头（格式固定）
	file << "Flight ID,Departure Station,Terminal Station,Departure Time,Arrival Time,Initial Ticket Count,Reserved Users\n";
	for(const auto& flight : flights_) {
		std::string reserved_users_str;
		const auto& reserved_users = flight.reserved_users();
		for(const auto& [user_id, count] : reserved_users) {
			if(count == 0) continue;
			if(reserved_users_str.length() > 0) {
				reserved_users_str += ";";
			}
			reserved_users_str += user_id + ":" + std::to_string(count);
		}
		file << flight.id() << "," 
			<< flight.start() << "," 
			<< flight.terminal() << "," 
			<< timeToString(flight.depart_time()) << "," 
			<< timeToString(flight.arrive_time()) << "," 
			<< flight.initial_ticket_count() << "," 
			<< reserved_users_str 
			<< "\n";
	}
	file.close();
	Log("OP", "Flights saved successfully to file: " + path);
}
void Solver::startInteract() {
	// TODO : main loop
	Log("NOTICE", "Welcome to the flight reservation system");
	auto showMenu = [&]() -> void {
		Log("NOTICE", "Operation Menu:");
		std::cout << "1. Add Flight by Input in Console\n";
		std::cout << "2. Add Flight from Local CSV File\n";
		std::cout << "3. Query Flight Information\n";
		std::cout << "4. Start Reservation\n";
		std::cout << "5. Cancel Reservation\n";
		std::cout << "6. Save Flight and Reservation Information to Local CSV File\n";
		std::cout << "7. Show Format Help\n";
		std::cout << "8. Show This Menu Again\n";
		std::cout << "9. Exit\n";
		Log("NOTICE", "Operation Menu END");
	};
	auto showFormatHelp = [&]() -> void {
		Log("NOTICE", "Format Help: ");
		std::cout << "- Departure and Arrival Time: YYYY-MM-DD HH:MM\n";
		std::cout << "- CSV File should contain the following columns:\n";
		std::cout << "- Flight ID,Departure Station,Terminal Station,Departure Time,Arrival Time,Initial Ticket Count,(Reserved Users)\n";
		std::cout << "- Reserved Users (optional): user_id:count;user_id:count...\n";
		Log("NOTICE", "Format Help END");
	};
	showMenu();
	while(true) {
		std::string op;
		std::cout << "- Please input your choice (type 8 to show menu again): ";
		std::cin >> op;
		std::string path, confirm;
		if (op == "1") {
			addFlight();
			continue;
		} else if (op == "2") {
			std::cout << "- Please input CSV file path: ";
			std::cin >> path;
			addFlightFromFile(path);
			continue;
		} else if (op == "3") {
			Airline& flight = queryFlightInteract();
			if(flight.id() == "$") {
				Log("ERROR", "Flight query failed");
				continue;
			}
			Log("NOTICE", "Found flight with ID " + flight.id());
			std::cout << "- Do you want to show the flight info? (y/n): ";
			std::cin >> confirm;
			if (confirm == "y") {
				flight.printInfo();
			}
			std::cout << "- Do you want to reserve tickets for this flight? (y/n): ";
			std::cin >> confirm;
			if (confirm == "y") {
				reserveFlight(flight);
			}
			std::cout << "- Do you want to cancel reservation for this flight? (y/n): ";
			std::cin >> confirm;
			if (confirm == "y") {
				cancelFlight(flight);
			}
			continue;
		} else if (op == "4") {
			reserveFlight();
			continue;
		} else if (op == "5") {
			cancelFlight();
			continue;
		} else if (op == "6") {
			std::cout << "- Please input saving path: ";
			std::cin >> path;
			saveFlightToFile(path);
			continue;
		} else if (op == "7") {
			showFormatHelp();
			continue;
		} else if (op == "8") {
			showMenu();
			continue;
		} else if (op == "9") {
			Log("NOTICE", "Thank you for using the flight reservation system");
			return;
		} else {
			Log("ERROR", "Invalid choice!");
			continue;
		}
	}
}