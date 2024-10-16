#ifndef MAZE_SOLVER_UTILS_H
#define MAZE_SOLVER_UTILS_H

#include <iostream>
#include <vector>
#include <chrono>


#define COLORS_IN_TERMINAL true

// #define AP_LOG(x) std::cout <<  __FILE__  << "(" << __LINE__ << ") " << x << std::endl;

const std::string red("\033[0;31m");
const std::string green("\033[0;32m");
const std::string blue("\033[0;34m");
const std::string reset("\033[0m");

#define LOG(x) std::cout << x << std::endl;

#if COLORS_IN_TERMINAL == true

#define LOG_r(x) LOG(red << x << reset);
#define LOG_g(x) LOG(green << x << reset);
#define LOG_b(x) LOG(blue << x << reset);

#else

#define LOG_r(x) LOG(x);
#define LOG_g(x) LOG(x);
#define LOG_b(x) LOG(x);

#endif

#define LOGERR(x) LOG_r(x)

#define START_TIMER std::chrono::steady_clock::time_point begin_time = std::chrono::steady_clock::now();
#define END_TIMER                                                                \
	std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now(); \
	std::cout << "ΔT = " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - begin_time).count() << "[ns] " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - begin_time).count() << "[µs] " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count() << "[ms]" << std::endl;
#define END_TIMER_MICROSECONDS                                                   \
	std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now(); \
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end_time - begin_time).count() << std::endl;

template <typename T>
void printVec(const std::vector<T> &vec)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		LOG(i << "->" << vec.at(i));
	}
}


#endif // MAZE_SOLVER_UTILS_H