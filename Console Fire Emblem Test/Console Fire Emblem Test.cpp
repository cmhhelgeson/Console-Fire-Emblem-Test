#include <iostream>
#include <list>
#include <thread>
using namespace std;

#include <Windows.h>
#include <vector>

int screen_width = 120;
int screen_height = 30;

struct Vec2
{
	int x;
	int y;

	bool operator == (const Vec2& rhs) const {
		return (this->x == rhs.x && this->y == rhs.y);
	}
};

struct Control {
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
	bool x = false;
	bool z = false;

	Control& operator *=(const bool& wipe) {
		if (wipe) {
			left = true; right = true; up = true; down = true;
			x = true; z = true;
		} else {
			left = false; right = false; up = false; down = false;
			x = false; z = false;
		}
		return *this;
	}

};

int player_collide(const Vec2 cur, const std::vector<Vec2>& vec)  {
	int x = 0;
	for (Vec2 i : vec) {
		if (cur == i) {
			return x;
		}
		x++;
	}
	return -1;
}

/*void key_test(bool k1, bool k2) {
	return (k1 && !k2)
}

void inside_key_test() {

} */


int main()
{
	// Create Screen Buffer
	wchar_t* screen = new wchar_t[screen_width * screen_height];
	for (int i = 0; i < screen_width * screen_height; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;


	std::vector<Vec2> players = { { 60, 15 }, { 60, 20 } };
	Vec2 player = { 60, 15 };
	Vec2 cursor = { 60, 10 };
	Vec2 prev_cursor = cursor;
	Control control;
	bool key_left = false, key_right = false, key_prev_left = false, key_prev_right = false;
	bool key_up = false, key_down = false, key_prev_up = false, key_prev_down = false;
	bool key_x = false, key_prev_x = false;
	bool key_z = false, key_prev_z = false;
	bool intend = false; 


	while (1)
	{

		key_left = (0x8000 & GetAsyncKeyState((unsigned char)('\x25'))) != 0;
		key_up = (0x8000 & GetAsyncKeyState((unsigned char)('\x26'))) != 0;
		key_right = (0x8000 & GetAsyncKeyState((unsigned char)('\x27'))) != 0;
		key_down = (0x8000 & GetAsyncKeyState((unsigned char)('\x28'))) != 0;
		key_x = (0x8000 & GetAsyncKeyState((unsigned char)('\x58'))) != 0;
		key_z = (0x8000 & GetAsyncKeyState((unsigned char)('\x5a'))) != 0;

		if (key_left && !key_prev_left) {
			control.left = true;
		}
		if (key_right && !key_prev_right) {
			control.right = true;
		}
		if (key_up && !key_prev_up) {
			control.up = true;
		}
		if (key_down && !key_prev_down) {
			control.down = true;
		}
		if (key_x && !key_prev_x) {
			control.x = true;
		}
		if (key_z && !key_prev_z) {
			control.z = true;
		}
		key_prev_left = key_left;
		key_prev_right = key_right;
		key_prev_up = key_up;
		key_prev_down = key_down;
		key_prev_x = key_x;
		key_prev_z = key_z;

		int activePlayer = player_collide(cursor, players);
		if (control.x) {
			if (!intend && activePlayer >= 0) {
				intend = true;
			}
		}
		if (control.z) {
			intend = false;
		}
		int horizontal = control.right - control.left;
		int vertical = control.down - control.up;
		if (!intend) {
			cursor.x += horizontal;
			cursor.y += vertical;
			prev_cursor.x += horizontal;
			prev_cursor.y += vertical;
		} else {
			cursor.x += horizontal;
			cursor.y += vertical;
			players[activePlayer].x += horizontal;
			players[activePlayer].y += vertical;
		}
		// Clear Screen
		for (int i = 0; i < screen_width * screen_height; i++) screen[i] = L' ';

		//Draw Cursor
		if (!intend) {
			screen[cursor.y * screen_width + cursor.x] = 0x2593;
		}
		//Draw Player
		for (Vec2 p : players) {
			if (cursor == p && !intend) {
				screen[p.y * screen_width + p.x] = L'A';
			} else {
				screen[p.y * screen_width + p.x] = L'X';
			}
		}

		// Display Frame
		WriteConsoleOutputCharacter(hConsole, screen, screen_width * screen_height, { 0,0 }, &dwBytesWritten);


		/*
		while (!bDead)
		{
			// Get Input,
			bKeyRight = (0x8000 & GetAsyncKeyState((unsigned char)('\x27'))) != 0;
			bKeyLeft = (0x8000 & GetAsyncKeyState((unsigned char)('\x25'))) != 0;

			if (bKeyRight && !bKeyRightOld)
			{

			}

			if (bKeyLeft && !bKeyLeftOld)
			{

			}

			bKeyRightOld = bKeyRight;
			bKeyLeftOld = bKeyLeft;

			// Clear Screen
			for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';

			// Draw Stats & Border
			for (int i = 0; i < nScreenWidth; i++)
			{
				screen[i] = L'=';
				screen[2 * nScreenWidth + i] = L'=';
			}
			wsprintf(&screen[nScreenWidth + 5], L"www.OneLoneCoder.com - S N A K E ! !                SCORE: %d", nScore);


			// Draw Snake Body
			for (auto s : snake)
				screen[s.y * nScreenWidth + s.x] = bDead ? L'+' : L'O';

			// Draw Snake Head
			screen[snake.front().y * nScreenWidth + snake.front().x] = bDead ? L'X' : L'@';

			// Draw Food
			screen[nFoodY * nScreenWidth + nFoodX] = L'%';

			if (bDead)
				wsprintf(&screen[15 * nScreenWidth + 40], L"    PRESS 'SPACE' TO PLAY AGAIN    ");

			// Display Frame
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		}

		// Wait for space
		while ((0x8000 & GetAsyncKeyState((unsigned char)('\x20'))) == 0);
	}
	*/
		control *= false;
	}
	return 0;
}
