#pragma once
#include "Includes.h"

static constexpr int duration = 50; // milliseconds per character
static const char* logFile = "log.txt";
static const int MAXLINES = 33, MAXACHIEVEMENTS = 18;

static const std::vector<std::string> lst = {
	"Has played",
	"Player name",
	"Total time played",
	"Current level",
	"Current level number",
	"Level remaining tries",
	"Maximum tries",
	"Clue num",
	"Stars num",
	"Last time playing",
	"Last daily challenge",
	"Last completed daily challenge",
	"Completed daily challenges num"
	// "Last daily challenge xnum"
	// "Last daily challenge expression"
};

static const std::vector<std::string> achievements_lst = {
	"Common               : Play one game                     ",
	"Unusual              : Complete a daily challenge        ",
	"Rare                 : Complete first level              ",
	"Epic                 : Complete second level             ",
	"Legendary            : Complete third level              ",
	"Mythic               : Complete fourth level             ",
	"Ultra                : Complete the whole game           ",
	"Super                : Complete EVERY VISIBLE ACHIEVEMENT",
	"Trader               : First trade                       ",
	"Rich                 : Earn 20 stars                     ",
	"Wealthy              : Earn 50 stars                     ",
	"Bill Gates           : Earn 100 stars                    ",
	"Challenger           : Complete 5 daily challenges       ",
	"Super Challenger     : Complete 20 daily challenges      ",
	"Tsai Joe Durian      : ???                               ",
	"Gandhi               : ???                               ",
	"Midnight Snack       : ???                               ",
	"using namespace std; : ???                               ",
};


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
