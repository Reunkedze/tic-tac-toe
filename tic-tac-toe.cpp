#include <iostream>
#include <random>
#include <stdlib.h>
#include <chrono>

using namespace std;

enum TCell : char {
	CROSS = 'X',
	ZERO = '0',
	EMPTY = '_'
};

enum TProgress
{
	IN_PROGRESS,
	WON_HUMAN,
	WON_AI,
	DRAW
};

#pragma pack(push, 1)
struct TGame
{
	TCell** ppField = nullptr;
	const size_t SIZE{ 3 };
	TCell human;
	TCell ai;
	TProgress progress{ IN_PROGRESS };
	size_t turn{ 0U }; // Четные числа - ходит человек, нечетные - аи
};
#pragma pack(pop)

struct TCoord
{
	size_t x{ 0U };
	size_t y{ 0U };
};

void clearScr()
{
	//system("cls");
	std::cout << "\x1B[2J\x1B[H";
}

int32_t __fastcall getRandomNum(const int32_t min, const int32_t max)
{
	const static auto seed = chrono::system_clock::now().time_since_epoch().count();
	static mt19937_64 generator(seed);
	uniform_int_distribution<int32_t> dis(min, max);
	return dis(generator);
}

void __fastcall initGame(TGame& g)
{
	g.ppField = new TCell * [g.SIZE];
	for (size_t i = 0; i < g.SIZE; i++)
	{
		g.ppField[i] = new TCell[g.SIZE];
	}

	for (size_t y = 0; y < g.SIZE; y++)
	{
		for (size_t x = 0; x < g.SIZE; x++)
		{
			g.ppField[y][x] = EMPTY;
		}
	}

	if (getRandomNum(0, 1000) > 500)
	{
		g.human = CROSS;
		g.ai = ZERO;
		g.turn = 0U;
	}
	else
	{
		g.human = ZERO;
		g.ai = CROSS;
		g.turn = 1U;
	}
}

void __fastcall printGame(const TGame& g)
{
	cout << "     ";
	for (size_t x = 0; x < g.SIZE; x++)
	{
		cout << x + 1 << "   ";
	}
	cout << endl;

	for (size_t y = 0; y < g.SIZE; y++)
	{
		cout << " " << y + 1 << " | ";
		for (size_t x = 0; x < g.SIZE; x++)
		{
			cout << g.ppField[y][x] << " | ";
		}
		cout << endl;
	}

	cout << endl << "Human: " << g.human << endl << "Computer: " << g.ai << endl << endl;
}

void __fastcall congrats(const TGame & g)
{
	if (g.progress == WON_HUMAN)
	{
		cout << "Human won! =)" << endl;
		cout << "                       ______" << endl;
		cout << "                     <((((((\\\\\\" << endl;
		cout << "                     /      . }\\" << endl;
		cout << "                     ;--..--._|}" << endl;
		cout << "  (\\                 '--/\--'  )" << endl;
		cout << "   \\\\                | '-'  :'|" << endl;
		cout << "    \\\\               . -==- .-|" << endl;
		cout << "     \\\\               \.__.'   \\--._" << endl;
		cout << "     [\\\\          __.--|       //  _/'--." << endl;
		cout << "     \\ \\       .'-._ ('-----'/ __/      \\" << endl;
		cout << "      \\ \\     /    __>|      | '--.      |" << endl;
		cout << "       \\ \\   |    \\   |     /    /       /" << endl;
		cout << "        \\ '\\ /     \\  |     |  _/       /" << endl;
		cout << "         \\  \\       \\ |     | /        /" << endl;
		cout << "          \\  \\      \\        /" << endl;
	}
	else if (g.progress == WON_AI)
	{
		cout << "Computer won! =/" << endl;
	}
	else if (g.progress == DRAW)
	{
		cout << "Drow! =(" << endl;
	
	}
}

void __fastcall deinitGame(TGame& g)
{
	for (size_t i = 0; i < g.SIZE; i++)
	{
		delete [] g.ppField[i];
	}
	delete [] g.ppField;
}

TProgress __fastcall getWon(const TGame& g)
{
	// Выйгрыш в строках
	for (size_t y = 0; y < g.SIZE; y++)
	{
		if (g.ppField[y][0] == g.ppField[y][1] && g.ppField[y][0] == g.ppField[y][2])
		{
			if (g.ppField[y][0] == g.human)
			{
				return WON_HUMAN;
			}
			else if (g.ppField[y][0] == g.ai)
			{
				return WON_AI;
			}
		}
	}

	// Выйгрыш в столбцах
	for (size_t x = 0; x < g.SIZE; x++)
	{
		if (g.ppField[0][x] == g.ppField[1][x] && g.ppField[0][x] == g.ppField[2][x])
		{
			if (g.ppField[0][x] == g.human)
			{
				return WON_HUMAN;
			}
			else if (g.ppField[0][x] == g.ai)
			{
				return WON_AI;
			}
		}
	}

	// Выйгрыш в диагоналях

	if ((g.ppField[0][0] == g.ppField[1][1] && g.ppField[0][0] == g.ppField[2][2]) || (g.ppField[2][0] == g.ppField[1][1] && g.ppField[2][0] == g.ppField[0][2]))
	{
		if (g.ppField[1][1] == g.human)
		{
			return WON_HUMAN;
		}
		else if (g.ppField[1][1] == g.ai)
		{
			return WON_AI;
		}
	}

	// Проверка на ничью
	bool draw{true};

	for (size_t y = 0; y < g.SIZE; y++)
	{
		for (size_t x = 0; x < g.SIZE; x++)
		{
			if (g.ppField[x][y] == EMPTY)
			{
				draw = false;
			}
		}
		if (!draw)
			break;
	}

	if (draw)
		return DRAW;

	return IN_PROGRESS;
}

TCoord __fastcall getHumanCoord(const TGame & g)
{
	TCoord c;
	do
	{
		cout << "Enter x: ";
		cin >> c.x;
		cout << "Enter y: ";
		cin >> c.y;
		c.x--;
		c.y--;
	} while (c.x > 2 || c.y > 2 || g.ppField[c.y][c.x] != EMPTY);
	return c;
}

TCoord __fastcall getAICoord(const TGame& g)
{
	for (size_t y = 0; y < g.SIZE; y++)
	{
		for (size_t x = 0; x < g.SIZE; x++)
		{
			if (g.ppField[y][x] == EMPTY)
			{
				g.ppField[y][x] = g.ai;
				if (getWon(g) == WON_AI)
				{
					g.ppField[y][x] = EMPTY;
					cout << x << " " << y << endl;
					return { x, y };
				}
				g.ppField[y][x] = EMPTY;
			}
		}
	}

	for (size_t y = 0; y < g.SIZE; y++)
	{
		for (size_t x = 0; x < g.SIZE; x++)
		{
			if (g.ppField[y][x] == EMPTY)
			{
				g.ppField[y][x] = g.human;
				if (getWon(g) == WON_HUMAN)
				{
					g.ppField[y][x] = EMPTY;
					return { x, y };
				}
				g.ppField[y][x] = EMPTY;
			}
		}
	}

	// центр
	if (g.ppField[1][1] == EMPTY)
	{
		return { 1, 1 };
	}

	// углы
	TCoord buf[4];
	size_t num{ 0 };

	if (g.ppField[0][0] == EMPTY)
	{
		buf[num] = { 0, 0 };
		num++;
	}
	if (g.ppField[2][2] == EMPTY)
	{
		buf[num] = { 2, 2 };
		num++;
	}

	if (g.ppField[2][0] == EMPTY)
	{
		buf[num] = { 0, 2 };
		num++;
	}

	if (g.ppField[0][2] == EMPTY)
	{
		buf[num] = { 2, 0 };
		num++;
	}

	if (num > 0)
	{
		int check;
		const size_t index = getRandomNum(0, 1000) % num;
		return buf[index];
	}

	// грани
	num = 0;

	if (g.ppField[0][1] == EMPTY)
	{
		buf[num] = { 1, 0 };
		num++;
	}

	if (g.ppField[1][0] == EMPTY)
	{
		buf[num] = { 0, 1 };
		num++;
	}

	if (g.ppField[2][1] == EMPTY)
	{
		buf[num] = { 1, 2 };
		num++;
	}

	if (g.ppField[1][2] == EMPTY)
	{
		buf[num] = { 2, 1 };
		num++;
	}
	if (num > 0)
	{
		const size_t index = getRandomNum(0, 1000) % num;
		return buf[index];
	}
}

int main()
{
	TGame g;
	initGame(g);
	clearScr();
	printGame(g);

	do
	{
		if (g.turn % 2 == 0)
		{
			TCoord c = getHumanCoord(g);
			g.ppField[c.y][c.x] = g.human;
		}
		else
		{
			TCoord c = getAICoord(g);
			g.ppField[c.y][c.x] = g.ai;
		}

		clearScr();
		printGame(g);
		g.progress = getWon(g);
		g.turn++;

	} while (g.progress == IN_PROGRESS);

	congrats(g);

	deinitGame(g);

	return 0;
}
