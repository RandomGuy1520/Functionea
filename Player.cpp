#pragma once
#include "Includes.h"
#include "Function.cpp"
#include "IO.cpp"

static int MAXTRY = 10;

/*
Log File Format:
LINE 0: Has Played
LINE 1: Player Name
LINE 2: Current Level
LINE 3: Level Number
LINE 4: Level Remaining Tries
LINE 5: Max Tries
LINE 6: Clue Num
LINE 7: Stars Num
LINE 8: Last Playing Date
LINE 9: Last Daily Challenge
LINE 10: Last Completed Daily Challenge
LINE 11: Last Daily Challenge Expression x-num
LINE 11: Last Daily Challenge Expression
*/

class Player
{
public:
	bool has_played = true;
	std::string name;
	int total_time_played;
	Functions::Level level;
	int level_num;
	int level_tries;
	int max_tries;
	int clue_num;
	int star_num;
	int last_playing_date;
	int last_daily_challenge;
	int last_completed_daily_challenge;
	int completed_daily_challenge_num;
	int daily_challenge_xnum;
	std::string daily_challenge_expr;
	bool achievements[18];
	Player()
	{
		char app_data_loc[MAX_PATH];
		SHGetSpecialFolderPathA(NULL, app_data_loc, CSIDL_APPDATA, TRUE);
		logFile = app_data_loc + (std::string)"\\Functionea\\log.txt";
		std::vector<std::string> vec = out.ReadLog();
		if (vec[0] == "")
		{
			has_played = false;
			out.Log({
				"True", // 0
				"Guest", // 1
				"0", // 2
				"0", // 3
				"1", // 4
				std::to_string(MAXTRY), // 5
				"10", // 6
				"0", // 7
				"0", // 8
				"0", // 9
				"0", // 10
				"0", // 11
				"0", // 12
				"0", // 13
				"", // 14
				"0", "0", "0", "0", "0",
				"0", "0", "0", "0", "0",
				"0", "0", "0", "0", "0",
				"0", "0", "0"               // 18 Achievements
			});
			vec = out.ReadLog();
		}
		name = vec[1];
		total_time_played = std::stoi(vec[2]);
		level = (Functions::Level)(std::stoi(vec[3]));
		level_num = std::stoi(vec[4]);
		level_tries = std::stoi(vec[5]);
		max_tries = std::stoi(vec[6]);
		clue_num = std::stoi(vec[7]);
		star_num = std::stoi(vec[8]);
		last_playing_date = std::stoi(vec[9]);
		last_daily_challenge = std::stoi(vec[10]);
		last_completed_daily_challenge = std::stoi(vec[11]);
		completed_daily_challenge_num = std::stoi(vec[12]);
		daily_challenge_xnum = std::stoi(vec[13]);
		daily_challenge_expr = vec[14];
		for (int i = 0; i < 18; i++)
			achievements[i] = std::stoi(vec[i + 15]);
	}
	void LogInfoToFile()
	{
		std::vector<std::string> vec = {
			"True",
			name,
			std::to_string(total_time_played),
			std::to_string(level),
			std::to_string(level_num),
			std::to_string(level_tries),
			std::to_string(max_tries),
			std::to_string(clue_num),
			std::to_string(star_num),
			std::to_string(last_playing_date),
			std::to_string(last_daily_challenge),
			std::to_string(last_completed_daily_challenge),
			std::to_string(completed_daily_challenge_num),
			std::to_string(daily_challenge_xnum),
			daily_challenge_expr,
			std::to_string(achievements[0]),
			std::to_string(achievements[1]),
			std::to_string(achievements[2]),
			std::to_string(achievements[3]),
			std::to_string(achievements[4]),
			std::to_string(achievements[5]),
			std::to_string(achievements[6]),
			std::to_string(achievements[7]),
			std::to_string(achievements[8]),
			std::to_string(achievements[9]),
			std::to_string(achievements[10]),
			std::to_string(achievements[11]),
			std::to_string(achievements[12]),
			std::to_string(achievements[13]),
			std::to_string(achievements[14]),
			std::to_string(achievements[15]),
			std::to_string(achievements[16]),
			std::to_string(achievements[17]),
		};
		out.Log(vec);
	}
};
