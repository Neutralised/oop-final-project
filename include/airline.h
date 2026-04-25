#ifndef AIRLINE_H
#define AIRLINE_H

#include "log.h"
#include <iomanip>
#include <chrono>
#include <string>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/hash_policy.hpp>
using namespace std::chrono;

class Airline {
private:
	std::string id_; // 航班号
	std::string start_; // 始发站
	std::string terminal_; // 终到站
	system_clock::time_point depart_time_; // 起飞时间
	system_clock::time_point arrive_time_; // 到达时间
	int ticket_count_; // 初始票数量
	int reserved_count_; // 已预订票数量
	__gnu_pbds::gp_hash_table<std::string, int> reserved_users_; // 当前预订此航班的客户 id 以及数量
public:
	explicit Airline(std::string id = "$",
		std::string start = "$",
		std::string terminal = "$",
		std::string depart = "1970-01-01 00:00",
		std::string arrive = "1970-01-01 00:00",
		int ticket_count = -1);
	~Airline();
	std::string id() const;
	std::string start() const;
	std::string terminal() const;
	system_clock::time_point depart_time() const;
	system_clock::time_point arrive_time() const;
	int ticket_count() const; // 当前余票
	int initial_ticket_count() const; // 初始总票数
	const __gnu_pbds::gp_hash_table<std::string, int>& reserved_users() const; // 预订用户信息
	minutes calcDuration(); // 计算飞行时长并以分钟返回
	int addReservation(std::string user_id, int count); // 为该航班添加一位客户的预订
	int delReservation(std::string user_id, int count); // 为该航班取消一位客户的预订
	void printTime(const system_clock::time_point& tp); // 打印 timepoint
	void printInfo(); // 打印信息：1.航班号 2.起点/终点 3.起飞/到达时间 4.飞行时长 5.余票
};

#endif