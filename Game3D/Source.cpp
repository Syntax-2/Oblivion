#include <iostream>
#include <Windows.h>
#include <chrono>

using namespace std;

int screenWidth = 120;
int screenHeight = 40;

float PlayersX = 8.0f;
float PlayersY = 8.0f;
float PlayersZ = 8.0f;


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
	map += L"#...#..........#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#...............";
	map += L"#...............";
	map += L"#...............";
	map += L"#...............";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#.......#......#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"################";


	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	while (true) 
	{

		auto tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float elapsedTimefloat = elapsedTime.count();

		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
		{
			PlayersZ -= 0.1f * elapsedTimefloat;
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
		{
			PlayersZ += 0.1f * elapsedTimefloat;
		}



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


			short shade = ' ';

			if (distanceToWall <= depth / 4.0f)
				shade = 0x2588; //full block shade
			else if (distanceToWall < depth / 3.0f)
				shade = 0x2593; // medium high shade
			else if (distanceToWall < depth / 2.0f)
				shade = 0x2592; // medium shade
			else if (distanceToWall < depth)
				shade = 0x2591; // low shade
			else
				shade = ' '; // no shade


			for (int y = 0; y < screenHeight; y++) 
			{
				if (y < celling) 
				{
					screen[y * screenWidth + x] = ' ';
				}
				else if(y > celling && y <= floor)
				{
					screen[y * screenWidth + x] = shade;
				}
				else
				{ 
				 	screen[y * screenWidth + x] = ' ';
				}


			}


		}



		screen[screenWidth * screenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight, { 0, 0 }, &dwBytesWritten);

	}




	
	return 0;

}