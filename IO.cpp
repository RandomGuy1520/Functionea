#pragma once
#include "Includes.cpp"

static constexpr int duration = 50; // milliseconds per character
static const char* logFile = "log.txt";
static const int MAXLINES = 9;
static const std::vector<std::string> lst = { "Has played", "Player name", "Current level", "Current level number", "Level remaining tries", "Maximum tries", "Clue num", "Stars num", "Last time playing"};

class IO
{
public:
	std::vector<std::string> ReadLog()
	{
		std::ifstream stream(logFile);
		std::vector<std::string> res;
		for (int i = 0; i < MAXLINES; i++)
		{
			std::string str;
			getline(stream, str);
			res.push_back(str);
		}
		return res;
	}
	void Log(std::vector<std::string> text)
	{
		std::ofstream stream(logFile);
		for (std::string i : text)
			stream << i << '\n';
	}
};
static IO out;
static inline IO operator<< (IO io, int num)
{
	std::vector<int> vec;
	if (num == 0)
	{
		putchar('0');
		Sleep(duration);
		return io;
	}
	while (num) vec.push_back(num % 10), num /= 10;
	std::reverse(vec.begin(), vec.end());
	for (int i : vec) putchar(i + '0'), Sleep(duration);
	return io;
}
static inline IO operator<< (IO io, const std::string &t)
{
	for (auto i : t) std::cout << i, Sleep(duration);
	return io;
}
