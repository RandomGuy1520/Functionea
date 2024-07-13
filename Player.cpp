#pragma once
#include "Includes.cpp"
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
*/

class Player
{
public:
	bool has_played = true;
	std::string name;
	Functions::Level level; int level_num, level_tries, max_tries, clue_num, star_num, last_playing_date;
	Player()
	{
		std::vector<std::string> vec = out.ReadLog();
		if (vec[0] == "")
		{
			has_played = false;
			out.Log({ "True", "Guest", "0", "1", std::to_string(MAXTRY), "10", "0", "0", "0" });
			vec = out.ReadLog();
		}
		name = vec[1];
		level = (Functions::Level)(std::stoll(vec[2]));
		level_num = std::stoll(vec[3]);
		level_tries = std::stoll(vec[4]);
		max_tries = std::stoll(vec[5]);
		clue_num = std::stoll(vec[6]);
		star_num = std::stoll(vec[7]);
		last_playing_date = std::stoll(vec[8]);
	}
	void LogInfoToFile()
	{
		std::vector<std::string> vec = {
			"True",
			name,
			std::to_string(level),
			std::to_string(level_num),
			std::to_string(level_tries),
			std::to_string(max_tries),
			std::to_string(clue_num),
			std::to_string(star_num),
			std::to_string(last_playing_date)
		};
		out.Log(vec);
	}
};
