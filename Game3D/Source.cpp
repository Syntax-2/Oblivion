#include <iostream>
#include <Windows.h>

using namespace std;

int screenWidth = 120;
int screenHeight = 40;

float PlayersX = 0.0f;
float PlayersY = 0.0f;
float PlayersZ = 0.0f;


int mapHeight = 16;
int mapWidth = 16;

float FOVfloat = 3.14159 / 4.0;

float depth = 16.0f;


int main() {

	wchar_t* screen = new wchar_t[screenWidth * screenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	wstring map;

	map += L"################";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"################";

	while (true) 
	{

		for (int x = 0; x < screenWidth; x++)
		{
			float RayAngle = (PlayersZ - FOVfloat / 2.0f) + ((float)x / (float)screenWidth) * FOVfloat;

			float distanceToWall = 0;

			bool didHitWall = false;

			float eyeX = sinf(RayAngle);
			float eyeY = cosf(RayAngle);


			while (!didHitWall && distanceToWall < depth) 
			{

				distanceToWall += 0.1f;


				int testX = (int)(PlayersX + eyeX * distanceToWall);
				int testY = (int)(PlayersY + eyeY * distanceToWall);

				if (testX < 0 || testX >= mapWidth || testY < 0 || testY >= mapHeight) 
				{
					didHitWall = true;
					distanceToWall = depth;

				}
				else 
				{

					if (map[testY * mapWidth + testX] == '#') 
					{
						didHitWall = true;
					}
				}
			}

			int celling = (float)(screenHeight / 2.0) - screenHeight / ((float)distanceToWall);
			int floor = screenHeight - celling;


		}



		screen[screenWidth * screenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight, { 0, 0 }, &dwBytesWritten);

	}




	
	return 0;

}