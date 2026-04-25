#ifndef LOG_H
#define LOG_H

#include <windows.h>
#include <iostream>
#include <string>

inline void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	/*
		7  : 白色
		10 : 绿色
		12 : 红色
		14 : 黄色
		11 : 浅蓝色
		9  : 深蓝色
	*/
}
inline void Log(std::string type, std::string msg) {
	if(type == "NOTICE") {
		setColor(14); // 黄色
	} else if(type == "ERROR") {
		setColor(12); // 红色
	} else if(type == "OP") {
		setColor(9); // 深蓝色
	} else if(type == "INFO") {
		setColor(10); // 绿色
	} else if(type == "FLIGHT") {
		setColor(11); //浅蓝色
	} else {
		setColor(7); // 白色
	}
	std::cout << "[" << type << "] ";
	setColor(7);
	std::cout << msg << '\n';
}


#endif