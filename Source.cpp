#pragma once
#include "Function.cpp"
#include "IO.cpp"
#include "CWindows.cpp"
#include "Player.cpp"

char input_choice;

const double e = 2.718281828459045;
const double pi = 3.141592653589793;

bool game_completed = false;

Player player;

/*
Commands:
/close : Back To Start Menu
/clue  : retrieve a clue
*/

static void to_lower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
}
static std::string is_valid(std::string str)
{
	to_lower(str);
	std::vector<std::string> swear_words = { "fuck", "shit", "stupid", "bitch", "dick", "ass", "damn" };
	for (char i : str)
		if (!('0' <= i && i <= '9' || 'a' <= i && i <= 'z'))
			return "invalid character(s)";
	for (std::string swear_word : swear_words)
		if (str.find(swear_word) != std::string::npos)
			return "swear words";
	return "valid";
}
static void ChooseMode()
{
	out << "Choose one below:\n1. Play Game\n2. Shop\n3. See Stats\n4. See Rules\n5. Close Game\n";
	input_choice = getc_without_output();
	while (input_choice != '1' && input_choice != '2' && input_choice != '3' && input_choice != '4' && input_choice != '5')
		input_choice = getc_without_output();
	system("cls");
}
static bool confirm()
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
static void init()
{
	system("color E4");
	SetSize();
	WordArt();
	system("cls");
	if (ymd() > player.last_playing_date) player.last_playing_date = ymd(), player.star_num++;
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
		out << "\nWelcome to Functionea, " << player_name << "!\n";
		Sleep(2000);
	}
	else {
		out << "Welcome back, " << player.name << "!"; Sleep(2000);
		out << "\n\n";
	}
}
static std::vector<double> try_values()
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
static bool check_same(std::string u, std::string v)
{
	if (u == v) return true;
	std::string _u, _v;
	for (char i : u) if (i != '(' && i != ' ') _u += i;
	for (char i : v) if (i != '(' && i != ' ') _v += i;
	return _u == _v;
}
static void PlayGame()
{
	using namespace Functions;
	while (true)
	{
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
				return;
			if (input == "/clue")
			{
				if (player.clue_num) std::cout << "Clue: " << clues[player.level][player.level_num - 1] << "\n";
				else std::cout << "You have no clues left.\n", player.clue_num--, player.LogInfoToFile();
				goto StartTry;
			}
			bool is_num = true;
			for (int i = 0; i < input.size(); i++)
				if ((input[i] < '0' || input[i] > '9') && input[i] != '-' && input[i] != '.')
					is_num = false;
			if (is_num && tries) {
				double input_num = stold(input);
				std::cout << evaluate_expr(currentLevelExpr, input_num) << std::endl;
			}
			else {
				bool same = true;
				if (!check_same(currentLevelExpr, input))
				{
					std::vector<double> try_vec = try_values();
					for (double i : try_vec)
						if (evaluate_expr(input, i) != evaluate_expr(currentLevelExpr, i))
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
					out << "Level Completed."; Sleep(3000);
					player.star_num += points[player.level][player.level_num - 1];
					player.LogInfoToFile();
					break;
				}
			}
		}
		if (player.level_num == num[player.level])
		{
			if (player.level == MAXLEVEL - 1)
			{
				std::cout << "GAME COMPLETED!"; Sleep(5000);
				game_completed = true;
				break;
			}
			else player.level = (Level)(player.level + 1), player.level_num = 1;
		}
		else player.level_num++;
		player.level_tries = player.max_tries;
		player.LogInfoToFile();
		system("cls");
	}
}
static void Shop()
{
	out << "Welcome to Shop!\n"; Sleep(1000);
	out << "You can buy anything here to use... with stars.\n"; Sleep(1000);
	out << "You have " << player.star_num << " stars now. Here are some options.\n"; Sleep(3000);
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
	out << "Purchase Completed!"; Sleep(1000);
	system("cls");
	goto PrintOptions;
}
static void PrintStats()
{
	std::vector<std::string> log = out.ReadLog();
	for (int i = 0; i < MAXLINES; i++)
		if (lst[i] == "Current level")
			out << lst[i] << ": Level " << std::stoll(log[i]) + 1 << "\n";
		else out << lst[i] << ": " << log[i] << "\n";
	out << "\n<Press Any Key To Close Stats>\n";
	getc_without_output();
	system("cls");
}
static void ExplainRules()
{
	out  << "Rules:\n"; Sleep(500);
	std::cout << "Main Game:\n"; Sleep(500);
	std::cout << "    1. This is a mathematical game of guessing functions. Every turn you input a parameter or guess the function. \n";
	std::cout << "        Every time you input a parameter, your number of tries for this level will subtract one.\n";
	std::cout << "        However, guessing the function does not change your number of tries.\n\n"; Sleep(3000);
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
	std::cout << "Special Abilities:\n"; Sleep(500);
	std::cout << "    1. You can earn stars by completing levels, and spend them to solve more levels.\n";
	std::cout << "        At first, you have 10 tries to enter parameters, but you can add more by purchasing them in the shop.\n\n"; Sleep(2000);
	std::cout << "    2. If you are stuck on a level, you can purchase a clue using three stars and enter the command '/clue' into the input area of the level.\n\n"; Sleep(1000);
	std::cout << "    3. You can also earn stars by logging in the game. Every single day you play the game, your number of stars increases by one.\n\n"; Sleep(1000);
	out  << "YOU ROCK! Now start playing the game!\n";
	out  << "<Press Any Key to Close Rules>\n";
	getc_without_output();
	system("cls");
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
			Shop();
		else if (input_choice == '3')
			PrintStats();
		else if (input_choice == '4')
			ExplainRules();
		else if (input_choice == '5')
		{
			out << "Game closed.\n<Press Any Key To Close Window>\n";
			getc_without_output();
			return 0;
		}
	}
}
