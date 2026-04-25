#ifndef SOLVER_H
#define SOLVER_H

#include "airline.h"
#include <vector>

class Solver {
private:
	Airline null_flight; // 用于查找失败时返回的默认航班
	std::vector<Airline> flights_; // 航班序列
	__gnu_pbds::gp_hash_table<std::string, int> ht_id_; // 用于按 id 查找航班在航班序列中的下标
	__gnu_pbds::gp_hash_table<std::string, std::vector<int>> ht_depart_; // 按出发时间查找，此处按日期分类，之后会列出本日所有航班供用户选择
	__gnu_pbds::gp_hash_table<std::string, std::vector<int>> ht_terminal_; // 按终点站查找
	// 由于多个航班出发时间和终点站可能相同，需要使用 vector 存储下标序列
public:
	Solver();
	~Solver();
	void addFlight(); // 通过控制台输入添加一个新航班
	Airline& queryFlight(const std::string& key, int type); // 通过 id/出发时间/终点站 获取航班 
	Airline& queryFlightInteract(); // 用户获取航班
	void reserveTicket(const std::string& user_id, int count, Airline& flight); // 向航班中添加用户订票信息
	void cancelTicket(const std::string& user_id, int count, Airline& flight); // 向航班中添加用户取消订票信息
	void reserveFlight(Airline& flight); // 用户订票（已知航班）
	void cancelFlight(Airline& flight); // 用户取消订票（已知航班）
	void reserveFlight(); // 用户订票（未知航班）
	void cancelFlight(); // 用户取消订票（未知航班）
	void addFlightFromFile(const std::string& path); // 从文件中读取新增航班信息
	void saveFlightToFile(const std::string& path); // 保存航班信息到文件
	void startInteract(); // 开始交互
};

#endif