#include "airline.h"
#include <iostream>

Airline::Airline(std::string id,
		std::string start,
		std::string terminal,
		std::string depart,
		std::string arrive,
		int ticket_count) :
		id_(id), start_(start), terminal_(terminal),
		ticket_count_(ticket_count) {
	reserved_users_.clear();
	reserved_count_ = 0;
	// 处理起飞时间和到达时间
	auto getTimePoint = [&](const std::string& s) -> system_clock::time_point {
		int year, mon, day, hour, min;
		// "XXXX-XX-XX XX:XX"
		year = mon = day = hour = min = 0;
		for(int _idx = 0; _idx <= 3; ++_idx)
			year = 10 * year + s[_idx] - 48;
		for(int _idx = 5; _idx <= 6; ++_idx)
			mon = 10 * mon + s[_idx] - 48;
		for(int _idx = 8; _idx <= 9; ++_idx)
			day = 10 * day + s[_idx] - 48;
		for(int _idx = 11; _idx <= 12; ++_idx)
			hour = 10 * hour + s[_idx] - 48;
		for(int _idx = 14; _idx <= 15; ++_idx)
			min = 10 * min + s[_idx] - 48;
		std::tm t{};
		t.tm_year = year - 1900;
		t.tm_mon = mon - 1;
		t.tm_mday = day;
		t.tm_hour = hour;
		t.tm_min = min;
		return system_clock::from_time_t(std::mktime(&t));
	};
	depart_time_ = getTimePoint(depart);
	arrive_time_ = getTimePoint(arrive);
}

Airline::~Airline() {
	reserved_users_.clear();
}

std::string Airline::id() const {
	return id_;
}
std::string Airline::start() const {
	return start_;
}
std::string Airline::terminal() const {
	return terminal_;
}
system_clock::time_point Airline::depart_time() const {
	return depart_time_;
}
system_clock::time_point Airline::arrive_time() const {
	return arrive_time_;
}
int Airline::ticket_count() const {
	return ticket_count_ - reserved_count_;
}

int Airline::initial_ticket_count() const {
	return ticket_count_;
}

const std::map<std::string, int>& Airline::reserved_users() const {
	return reserved_users_;
}

minutes Airline::calcDuration() {
	return duration_cast<minutes>(this->arrive_time() - this->depart_time());
}
int Airline::addReservation(std::string user_id, int count) {
	// 返回码：0 有效预订 1 无效id 2 余票不足
	if(user_id.length() == 0) return 1; // null id
	if(reserved_count_ + count > ticket_count_) return 2;
	reserved_users_[user_id] += count;
	reserved_count_ += count;
	return 0;
}
int Airline::delReservation(std::string user_id, int count) {
	// 返回码：0 有效取消预订 1 无效id 2 未预订过 3 取消预订的数量大于当前预订数
	if(user_id.length() == 0) return 1; // null id
	if(reserved_users_[user_id] == 0) return 2;
	reserved_count_ -= reserved_users_[user_id];
	int temp = reserved_users_[user_id];
	reserved_users_[user_id] = std::max(temp - count, 0);
	if(temp < count) return 3; // 在这种情况下，仍然取消所有的预订，但返回错误码
	return 0;
}
void Airline::printTime(const system_clock::time_point& tp) {
	time_t t = system_clock::to_time_t(tp);
	tm tm_ = *localtime(&t);
	printf("%04d-%02d-%02d %02d:%02d\n", tm_.tm_year + 1900, tm_.tm_mon + 1, tm_.tm_mday, tm_.tm_hour, tm_.tm_min);
}
void Airline::printInfo() {
	// 打印以下信息：1.航班号 2.起点/终点 3.起飞/到达时间 4.飞行时长 5.余票
	Log("INFO", "Flight Information");
	std::cout << "- Flight ID: " << this->id() << '\n';
	std::cout << "- Departure Station: " << this->start() << '\n';
	std::cout << "- Terminal Station: " << this->terminal() << '\n';
	minutes duration = this->calcDuration();
	std::cout << "- Departure Time: ";
	this->printTime(this->depart_time());
	std::cout << "- Arrival Time: ";
	this->printTime(this->arrive_time());
	std::cout << "- Flight Duration: " << duration.count() / 60 << "h" << duration.count() % 60 << "min\n";
	std::cout << "- Remaining Ticket: " << this->ticket_count() << '\n';
}