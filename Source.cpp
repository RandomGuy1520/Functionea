#pragma once
#include "Includes.h"
#include "Function.cpp"
#include "IO.cpp"
#include "CWindows.cpp"
#include "Player.cpp"

/*
Commands:
/close : Back To Start Menu
/clue  : retrieve a clue
*/

char input_choice;
bool game_completed = false, game_close = false;
Player player;

static inline void to_lower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
}
static inline void str_replace(std::string& str, const std::string& replace_word, const std::string& replace_by)
{
	size_t pos = str.find(replace_word);
	while (pos != std::string::npos)
	{
		str.replace(pos, replace_word.size(), replace_by);
		pos = str.find(replace_word, pos + replace_by.size());
	}
}
static inline std::string is_valid(std::string str)
{
	to_lower(str);
	if (str == "rere37")
		return "administrator username";
	std::vector<std::string> swear_words = { "fuck", "shit", "stupid", "bitch", "dick", "ass", "damn" };
	for (char i : str)
		if (!('0' <= i && i <= '9' || 'a' <= i && i <= 'z'))
			return "invalid character(s)";
	for (std::string swear_word : swear_words)
		if (str.find(swear_word) != std::string::npos)
			return "swear words";
	return "valid";
}
static inline void ChooseMode()
{
	out << "Choose one below:\n";
	std::cout << "1. Play Game\n2. Daily Challenge\n3. Shop\n4. See Stats\n5. See Rules\n6. Close Game\n";
	input_choice = getc_without_output();
	while (input_choice <= '0' || input_choice >= '7')
		input_choice = getc_without_output();
	system("cls");
}
static inline bool confirm()
{
	std::string confirm;
	while (_kbhit()) _getch();
	std::cin >> confirm;
	to_lower(confirm);
	while (confirm != "yes" && confirm != "no")
	{
		std::cout << "\nInvalid input. Please type yes or no below:\n";
		while (_kbhit()) _getch();
		std::cin >> confirm;
		to_lower(confirm);
	}
	return confirm == "yes";
}
static inline std::pair<std::string, int> random_expr()
{
	std::mt19937_64 rd(std::chrono::system_clock::now().time_since_epoch().count());
	std::vector<std::string> x = { "x", "x^2", "x^3", "sqrt(x)", "sin(x)", "cos(x)", "tan(x)", "log(x)", "exp(x)" };
	std::vector<char> op = { '+', '-', '*', '/' };
	std::vector<int> idx;
	std::string res;
	int rdxnum = rd() % 9, x_num;
	if      (rdxnum < 1) x_num = 1;
	else if (rdxnum < 1 + 2) x_num = 2;
	else if (rdxnum < 1 + 2 + 3) x_num = 3;
	else if (rdxnum < 1 + 2 + 3 + 2) x_num = 4;
	else if (rdxnum < 1 + 2 + 3 + 2 + 1) x_num = 5;
	else x_num = -1; // Impossible
	for (int i = 0; i < x_num; i++)
	{
		if (i != 0)
		{
			idx.push_back(rd() % 4), res += op[idx.back()];
			if (idx[idx.size() - 2] >= 2 && idx.back() < 4)
			{
				int q = rd() % 5 + 4;
				while (std::find(idx.begin(), idx.end(), q) != idx.end())
					q = rd() % 5 + 4;
				idx.push_back(q), res += x[q];
			}
			else {
				int q = rd() % 9;
				while (std::find(idx.begin(), idx.end(), q) != idx.end())
					q = rd() % 9;
				idx.push_back(q), res += x[q];
			}
		}
		if (i == 0)
		{
			int q = rd() % 9;
			while (std::find(idx.begin(), idx.end(), q) != idx.end())
				q = rd() % 9;
			idx.push_back(q), res += x[q];
		}
	}
	return make_pair(res, x_num);
}
static inline std::vector<double> try_values()
{
	std::vector<double> try_vec;
	for (int i = -10; i < 10; i++)
		if (i != 0)
		{
			try_vec.push_back(e / i);
			try_vec.push_back(e * i);
			try_vec.push_back(pi / i);
			try_vec.push_back(pi * i);
		}
	for (int i = -100; i < 100; i++)
		try_vec.push_back(i);
	for (int i = -10; i < 0; i++)
		try_vec.push_back(pow(2, i));
	for (int i = 7; i < 20; i++)
		try_vec.push_back(pow(2, i));
	return try_vec;
}
static inline bool check_achievement_super()
{
	if (player.achievements[7]) return false;
	bool done = true;
	for (int i = 0; i < 14; i++)
		if (i != 7 && !player.achievements[i])
			done = false;
	if (done)
	{
		player.achievements[7] = true;
		player.star_num += 8;
		player.LogInfoToFile();
		out << "Achievement completed: Super\n";
		return true;
	}
	return false;
}
static inline void check_achievement_star()
{
	if (!player.achievements[9] && player.star_num >= 20)
	{
		player.achievements[9] = true;
		player.star_num += 4;
		player.LogInfoToFile();
		out << "Achievement completed: Rich\n";
	}
	if (!player.achievements[10] && player.star_num >= 50)
	{
		player.achievements[10] = true;
		player.star_num += 4;
		player.LogInfoToFile();
		out << "Achievement completed: Wealthy\n";
	}
	if (!player.achievements[11] && player.star_num >= 100)
	{
		player.achievements[11] = true;
		player.star_num += 4;
		player.LogInfoToFile();
		out << "Achievement completed: Bill Gates\n";
	}
	if (check_achievement_super())
		check_achievement_star();
}
static inline void check_achievement_challenge()
{
	if (!player.achievements[12] && player.completed_daily_challenge_num >= 5)
	{
		player.achievements[12] = true;
		player.star_num += 4;
		player.LogInfoToFile();
		check_achievement_star();
		out << "Achievement completed: Challenger\n";
	}
	if (!player.achievements[13] && player.completed_daily_challenge_num >= 20)
	{
		player.achievements[13] = true;
		player.star_num += 4;
		player.LogInfoToFile();
		check_achievement_star();
		out << "Achievement completed: Super Challenger\n";
	}
}
static inline void achievement_playing_time_checker()
{
	if (player.achievements[15] && player.achievements[16]) return;
	while (!game_close)
	{
		if (!player.achievements[15] && player.total_time_played + (clock() / 1000) > 18000)
		{
			player.achievements[15] = true;
			player.star_num += 4;
			player.LogInfoToFile();
			out << "\n\nMessage: Hidden achievement completed: Gandhi\n\n";
			check_achievement_star();
			if (player.achievements[16]) return;
		}
		if (!player.achievements[16] && __TIME__[0] == '0' && __TIME__[1] == '0')
		{
			player.achievements[16] = true;
			player.star_num += 4;
			player.LogInfoToFile();
			out << "\n\nMessage: Hidden achievement completed: Midnight Snack\n\n";
			check_achievement_star();
			if (player.achievements[15]) return;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}
std::thread achievement_playing_time_check(achievement_playing_time_checker);
BOOL WINAPI ConsoleHandler(DWORD cevent)
{
	if (cevent == CTRL_C_EVENT || cevent == CTRL_BREAK_EVENT || cevent == CTRL_CLOSE_EVENT || cevent == CTRL_LOGOFF_EVENT || cevent == CTRL_SHUTDOWN_EVENT)
	{
		game_close = true;
		achievement_playing_time_check.join();
		player.total_time_played += (clock() / 1000);
		player.LogInfoToFile();
	}
	return TRUE;
}
static inline bool check_same(std::string u, std::string v)
{
	if (u == v) return true;
	std::string _u, _v;
	for (char i : u) if (i != ' ') _u += i;
	for (char i : v) if (i != ' ') _v += i;
	return _u == _v;
}
static inline void PlayGame()
{
	using namespace Functions;
	while (true)
	{
		const std::vector<std::variant<double, ErrorType>> vec_ans = evaluate_expr(exprs[player.level][player.level_num - 1], try_values());
		out << "Level " << player.level + 1 << ", Number " << player.level_num << ";\n"; Sleep(1000);
		std::cout << "Start!\n";
		bool level_completed = false;
		for (int tries = player.level_tries; tries >= 0; tries--)
		{
			player.level_tries = tries;
			player.LogInfoToFile();
		StartTry:
			std::cout << tries << ": >>> ";
			std::string input, currentLevelExpr = exprs[player.level][player.level_num - 1];
			input = getstr_clbuff();
			if (input == "/close")
			{
				system("cls");
				return;
			}
			if (input == "/clue")
			{
				if (player.clue_num > 0)
				{
					player.clue_num--;
					std::cout << "Clue: " << clues[player.level][player.level_num - 1] << "\n";
					if (!player.achievements[14] && player.level == LEVEL1 && player.level_num == 1)
					{
						player.achievements[14] = true;
						player.star_num += 4;
						player.LogInfoToFile();
						out << "Hidden achievement completed: Tsai Joe Durian\n";
						check_achievement_star();
					}
				}
				else std::cout << "You have no clues left.\n", player.clue_num--, player.LogInfoToFile();
				goto StartTry;
			}
			if (!player.achievements[17] && input == "using namespace std;")
			{
				player.achievements[17] = true;
				player.star_num += 4;
				player.LogInfoToFile();
				out << "Hidden achievement completed: using namespace std;\n";
				check_achievement_star();
			}
			bool is_num = is_const_expr(input);
			if (is_num && tries) {
				std::variant<double, ErrorType> res = evaluate_expr(input);
				if (IsError(res))
					std::cout << ErrorType() << std::endl;
				else std::cout << evaluate_expr(currentLevelExpr, get<double>(res)) << std::endl;
			}
			else {
				bool same = true;
				if (!check_same(currentLevelExpr, input))
				{
					std::vector<double> try_vec = try_values();
					std::vector<std::variant<double, ErrorType>> vec_input;
					vec_input = evaluate_expr(input, try_vec);
					for (int i = 0; i < try_vec.size(); i++)
						if (vec_input[i] != vec_ans[i])
						{
							same = false;
							break;
						}
					if (same) std::cout << "Format Incorrect!\n";
					else std::cout << "Function Incorrect!\n";
					goto StartTry;
				}
				else {
					level_completed = true;
					std::cout << "Correct!\n";
					out << "Level Completed.";
					break;
				}
			}
		}
		if (player.level == LEVEL1 && player.level_num == 1)
		{
			player.achievements[0] = true;
			player.star_num += 2;
			check_achievement_star();
			player.LogInfoToFile();
		}
		if (player.level_num == num[player.level])
		{
			out << "Achivemenet completed: ";
			player.star_num += 4;
			if (player.level == LEVEL1)
				player.achievements[2] = true, out << "Rare\n";
			if (player.level == LEVEL2)
				player.achievements[3] = true, out << "Epic\n";
			if (player.level == LEVEL3)
				player.achievements[4] = true, out << "Legendary\n";
			if (player.level == LEVEL4)
				player.achievements[5] = true, out << "Mythic\n";
			check_achievement_star();
			if (player.level == LEVEL5)
			{
				player.star_num += 4; // total : +8
				player.achievements[6] = true;
				out << "Ultra\n"; Sleep(1000);
				check_achievement_star();
				std::cout << "GAME COMPLETED!"; Sleep(5000);
				game_completed = true;
				break;
			}
			else player.level = (Level)(player.level + 1), player.level_num = 1, std::cout << "Prepare for NEXT WHOLE LEVEL!\n";
		}
		else player.level_num++;
		player.star_num += points[player.level][player.level_num - 1];
		player.level_tries = player.max_tries;
		check_achievement_star();
		player.LogInfoToFile();
		Sleep(2000);
		system("cls");
	}
}
static inline void Shop()
{
	out << "Welcome to Shop!\n"; Sleep(1000);
	out << "You can buy anything here to use... with stars.\n"; Sleep(1000);
	out << "You have " << player.star_num << " stars now. Here are some options.\n"; Sleep(1500);
	system("cls");
PrintOptions:
	std::cout << "Options: \n\n";
	std::cout << "1. 1 star  -> 1 more try for current level\n";
	std::cout << "2. 4 stars -> 1 clue for any level\n";
	std::cout << "3. 9 stars -> 1 additional tries for all levels\n";
	std::cout << "4. Close shop\n";
GetInput:
	char c = getc_without_output();
	while (c != '1' && c != '2' && c != '3' && c != '4')
		c = getc_without_output();
	if (c == '1' && player.star_num < 1 || c == '2' && player.star_num < 3 || c == '3' && player.star_num < 5)
	{
		out << "You don't have so many stars. Try again.\n";
		goto GetInput;
	}
	out << "Do you confirm? <yes or no>\n";
	if (!confirm()) goto GetInput;
	if (c == '1') player.star_num -= 1, player.level_tries += 1;
	if (c == '2') player.star_num -= 4, player.clue_num += 1;
	if (c == '3') player.star_num -= 9, player.max_tries += 1;
	if (c == '4') {
		system("cls");
		return;
	}
	if (!player.achievements[8])
	{
		player.achievements[8] = true;
		player.star_num += 2;
		check_achievement_star();
		player.LogInfoToFile();
		out << "Achievement completed: Trader\n";
	}
	check_achievement_star();
	out << "Purchase Completed!"; Sleep(1000);
	system("cls");
	goto PrintOptions;
}
static inline void PrintStats()
{
	out << "STATS:\n\n";
	std::vector<std::string> log = out.ReadLog();
	for (int i = 0; i < lst.size(); i++)
		if (lst[i] == "Current level")
			std::cout << lst[i] << ": Level " << std::stoll(log[i]) + 1 << "\n";
		else std::cout << lst[i] << ": " << log[i] << "\n";
	out << "\nAchievements:\n\n";
	for (int i = 0; i < MAXACHIEVEMENTS; i++)
		std::cout << achievements_lst[i] << " : " << (std::stoi(log[i + MAXLINES - MAXACHIEVEMENTS]) == 1 ? "DONE" : "") << "\n";
	out << "\n<Press Any Key To Close Stats>\n";
	getc_without_output();
	system("cls");
}
static inline void ExplainRules()
{
	out << "Rules:\n"; Sleep(500);
	std::cout << "Main Game:\n"; Sleep(500);
	std::cout << "    1. This is a mathematical game of guessing functions. Every turn you input a parameter or guess the function. \n";
	std::cout << "       Every time you input a parameter, your number of tries for this level will subtract one.\n";
	std::cout << "       However, guessing the function does not change your number of tries.\n";
	std::cout << "       Remember that when entering the function, you don't need to type in 'y=' or 'f(x)='.\n\n"; Sleep(3000);
	std::cout << "    2. If you input a parameter, it will output the value of f(x).\n\n"; Sleep(1500);
	std::cout << "    3. If you guess the function, there will be three cases: Function Incorrect, Format Incorrect, or Correct.\n";
	std::cout << "        (1) Function Incorrect means that your function is not the target function.\n";
	std::cout << "        (2) Format Incorrect means that your function is equal to the target function but only after reformatting.\n";
	std::cout << "            You need to input again and reformat your function to exactly match the target one.\n";
	std::cout << "        (3) Correct means that you have passed the level and stars are automatically rewarded. You can move on to the next level.\n\n"; Sleep(4000);
	std::cout << "    4. Here is the basic function list that you can use. Entering other functions will cause errors.\n"; Sleep(2000);
	std::cout << "        abs, acos, acosh, asin, asinh, atan, atanh, avg, ceil, cos, cosh, cot, csc, exp, floor, frac, log, log10, log2, max, min, mod, pow, root, round, sec, sin, sinc, sinh, sqrt, sum, tan, tanh, deg2rad, rad2deg\n\n"; Sleep(1000);
	std::cout << "        You might find things interesting in this list. Here are some important things to notice:\n"; Sleep(2000);
	std::cout << "        (1) There is no factorial function.\n"; Sleep(500);
	std::cout << "        (2) arcsin, arccos, arctan are written as asin, acos, and atan.\n"; Sleep(500);
	std::cout << "        (3) You can only use log(x), log10(x), and log2(x) as logarithms.\n"; Sleep(1500);
	std::cout << "Stars & Commands:\n"; Sleep(500);
	std::cout << "    (I) How to earn stars:\n"; Sleep(500);
	std::cout << "        1. Earn stars by logging in the game. Every single day you play the game, your number of stars increases by one.\n\n"; Sleep(1000);
	std::cout << "        2. Earn stars by completing levels, and spend them to solve more levels.\n";
	std::cout << "           For example, at first, you have 10 tries to enter parameters, but you can add more by purchasing them in the shop.\n\n"; Sleep(2000);
	std::cout << "        3. Earn stars by completing achievements. Not only does it gives you stars, but also joy and fun. (See Stats Page)\n\n"; Sleep(1000);
	std::cout << "        4. Earn stars by completing daily challenges. Every day has a new challenge waiting for you to complete.\n\n"; Sleep(1000);
	std::cout << "    (II) Commands:\n"; Sleep(500);
	std::cout << "        1. '/close': If you want to stop playing the main game, you can enter the command '/close' into the input area of the level. You will be back to the menu page.\n\n"; Sleep(1000);
	std::cout << "        2. '/clue' : If you are stuck on a level, you can purchase a clue using three stars and enter the command '/clue' into the input area of the level.\n\n"; Sleep(1000);
	out << "YOU ROCK! Remember the rules above and start playing the game!\n";
	out << "<Press Any Key to Close Rules>\n";
	getc_without_output();
	system("cls");
}
static inline void DailyChallenge()
{
	using namespace Functions;
	if (ymd() == player.last_completed_daily_challenge) // ERROR!
	{
		out << "You already completed today's daily challenge.\n\n";
		std::cout << "<Press Any Key To Close Daily Challenge>\n";
		getc_without_output();
		system("cls");
		return;
	}
	else {
		if (player.last_daily_challenge != ymd())
		{
			std::pair<std::string, int> expr = random_expr();
			player.daily_challenge_expr = expr.first;
			player.daily_challenge_xnum = expr.second;
			player.last_daily_challenge = ymd();
			player.LogInfoToFile();
		}
		const std::vector<std::variant<double, ErrorType>> vec_ans = evaluate_expr(player.daily_challenge_expr, try_values());
		out << "Welcome to today's Daily Challenge!\n";
		out << "Today's challenge is a function with " << player.daily_challenge_xnum << " 'x'es.\n";
		Sleep(1000);
		std::cout << "Start!\n";
		bool level_completed = false;
		while (true)
		{
			std::cout << ">>> ";
			std::string input, currentLevelExpr = player.daily_challenge_expr;
			input = getstr_clbuff();
			if (input == "/close")
			{
				system("cls");
				return;
			}
			bool is_num = is_const_expr(input);
			if (is_num) {
				std::variant<double, ErrorType> res = evaluate_expr(input);
				if (IsError(res))
					std::cout << ErrorType() << std::endl;
				else std::cout << evaluate_expr(currentLevelExpr, get<double>(res)) << std::endl;
			}
			else {
				bool same = true;
				if (!check_same(currentLevelExpr, input))
				{
					std::vector<double> try_vec = try_values();
					std::vector<std::variant<double, ErrorType>> vec_input;
					vec_input = evaluate_expr(input, try_vec);
					for (int i = 0; i < try_vec.size(); i++)
						if (vec_input[i] != vec_ans[i])
						{
							same = false;
							break;
						}
					if (same) std::cout << "Format Incorrect!\n";
					else std::cout << "Function Incorrect!\n";
				}
				else {
					std::cout << "Correct!\n";
					if (!player.achievements[1])
					{
						player.achievements[1] = true;
						player.star_num += 2;
						out << "Achievement completed: Unusual\n";
					}
					player.star_num += 2;
					player.completed_daily_challenge_num++;
					player.last_completed_daily_challenge = ymd();
					player.LogInfoToFile();
					check_achievement_challenge();
					check_achievement_star();
					out << "Daily Challenge Completed.\n";
					level_completed = true;
					Sleep(2000);
					system("cls");
					break;
				}
			}
		}
	}
}
static inline void init()
{
	system("color E4");
	SetConsoleCtrlHandler(ConsoleHandler, TRUE);
	SetSize();
	WordArt();
	system("cls");
	if (ymd() > player.last_playing_date)
	{
		player.last_playing_date = ymd();
		player.star_num++;
		player.LogInfoToFile();
		check_achievement_star();
	}
	if (!player.has_played)
	{
		out << "Since this is your first time playing, you can set your own username.\n"; Sleep(1000);
	EnterUsername:
		out << "\nEnter your username, please:\n\n";
		std::string player_name, res;
		player_name = getstr_clbuff();
		while ((res = is_valid(player_name)) != "valid")
		{
			out << "\nSorry, this name is invalid because it contains " << res << ".\nPlease try again:\n\n";
			player_name = getstr_clbuff();
		}
		out << "\nAre you sure you want this username, " << player_name << "? <yes or no>\n";
		if (!confirm()) goto EnterUsername;
		player.name = player_name;
		player.LogInfoToFile();
		system("cls");
		out << "\nWelcome to Functionea, " << player_name << "!\n\n";
		Sleep(2000);
		out << "Since you're new to this game, you need to know the rules.\n\n";
		out << "Please read carefully, since this game is pretty hard.\n\n"; Sleep(1000);
		system("cls");
		ExplainRules();
	}
	else {
		out << "Welcome back, " << player.name << "!"; Sleep(2000);
		out << "\n\n";
	}
}
signed main()
{
	init();
	while (true)
	{
		ChooseMode();
		if (input_choice == '1')
			PlayGame();
		else if (input_choice == '2')
			DailyChallenge();
		else if (input_choice == '3')
			Shop();
		else if (input_choice == '4')
			PrintStats();
		else if (input_choice == '5')
			ExplainRules();
		else if (input_choice == '6')
		{
			out << "Game closed.\n<Press Any Key To Close Window>\n";
			player.total_time_played += (clock() / 1000);
			player.LogInfoToFile();
			game_close = true;
			achievement_playing_time_check.join();
			getc_without_output();
			return 0;
		}
	}	
}
