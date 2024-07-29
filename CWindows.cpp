#pragma once
#include "Includes.h"

static inline char getc_without_output()
{
	while (_kbhit()) _getch();
	return _getch();
}
static inline std::string getstr_clbuff()
{
	std::string res;
	while (_kbhit()) _getch();
	std::cin >> res;
	return res;
}
static inline void WordArt()
{
	printf("\n\n\n");
	printf("\033[2m");
	printf("   _________                          ______  _________  _________    _______                ________              \n");
	printf("   |          |       |  |\\     |   /             |          |       /       \\    |\\     |  |               /\\      \n");
	printf("   |          |       |  | \\    |  /              |          |      /         \\   | \\    |  |              /  \\     \n");
	printf("   |________  |       |  |  \\   |  |              |          |      |         |   |  \\   |  |________     /____\\    \n");
	printf("   |          \\       /  |   \\  |  |              |          |      \\         /   |   \\  |  |            /      \\   \n");
	printf("   |           \\     /   |    \\ |  \\              |          |       \\       /    |    \\ |  |           /        \\  \n");
	printf("   |            -----    |     \\|   \\-------      |      ---------    -------     |     \\|   --------  /          \\ \n"); Sleep(2000);
	printf("                                                                                                                     \n");
	printf("                                                    VERSION: 1.0                                                     \n");
	printf("                                              <Press Any Key To Start>                                               \n");
	getc_without_output();
}
static inline void SetSize()
{
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 20;
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	system("cls");
}
static int ymd()
{
	using namespace std::chrono;
	const time_point now{ system_clock::now() };
	const year_month_day ymd{ floor<days>(now) };
	return 10000 * static_cast<signed>(ymd.year()) + 100 * static_cast<unsigned>(ymd.month()) + static_cast<unsigned>(ymd.day());
}
