#include <iostream>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

using namespace std;

int screenWidth = 90;
int screenHeight = 30;

float PlayersX = 8.0f;
float PlayersY = 8.0f;
float PlayersZ = 8.0f;


int mapHeight = 16;
int mapWidth = 16;

float FOVfloat = 3.14159 / 4.0;

float depth = 16.0f;


void playMusic() {
		PlaySound(TEXT("rainSfx.wav"), NULL, SND_FILENAME | SND_ASYNC);
}



void Introduction()
{

	thread musicThread(playMusic);
	musicThread.join();

	bool intro = true;

	// INTRODUCTION
	while (intro)
	{
		string s = "The world flickers in shades of grey, a desolate wasteland sculpted by chrome and concrete... Write 'more' to continue...";
		for (char c : s)
		{
			this_thread::sleep_for(chrono::milliseconds(100));
			cout << c << flush;
		}
		
		int o;
		cin >> o;

		s = "You are Agent 13, a lone operative tasked with navigating the depths of Oblivion, STARTING THE GAME......................";
		for (char c : s)
		{
			this_thread::sleep_for(chrono::milliseconds(100));
			cout << c << flush;
		}

		intro = false;
			
		return;

	}

}




int main() {



	Introduction();

	wchar_t* screen = new wchar_t[screenWidth * screenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	wstring map;

	map += L"#####..#########";
	map += L"#......#.......#";
	map += L"#...#..#..##...#";
	map += L"#...####...#...#";
	map += L"#...##.....#...#";
	map += L"#...########...#";
	map += L"#...#..........#";
	map += L"#...#..........#";
	map += L"#.##########...#";
	map += L"#.#........#...#";
	map += L"#.#........#...#";
	map += L"#.#...#....#...#";
	map += L"#.#...#....#...#";
	map += L"#.....#........#";
	map += L"#.....#........#";
	map += L"################";


	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();





	while (true) 
	{

		auto tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float elapsedTimefloat = elapsedTime.count();


		//
		//// ROTATION
		//
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
		{
			PlayersZ -= 1.0f * elapsedTimefloat;
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
		{
			PlayersZ += 1.0f * elapsedTimefloat;
		}
		//
		//// MOVEMENT
		//
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			PlayersX += sinf(PlayersZ) * 5.0f * elapsedTimefloat;
			PlayersY += cosf(PlayersZ) * 5.0f * elapsedTimefloat;

			if (map[(int)PlayersY * mapWidth + (int)PlayersX] == '#') 
			{
				PlayersX -= sinf(PlayersZ) * 5.0f * elapsedTimefloat;
				PlayersY -= cosf(PlayersZ) * 5.0f * elapsedTimefloat;
			}
			



		}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			PlayersX -= sinf(PlayersZ) * 5.0f * elapsedTimefloat;
			PlayersY -= cosf(PlayersZ) * 5.0f * elapsedTimefloat;

			if (map[(int)PlayersY * mapWidth + (int)PlayersX] == '#')
			{
				PlayersX += sinf(PlayersZ) * 5.0f * elapsedTimefloat;
				PlayersY += cosf(PlayersZ) * 5.0f * elapsedTimefloat;
			}

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


			//
			//// WALL SHADING
			//

			short wshade = ' ';

			if (distanceToWall <= depth / 4.0f)
				wshade = 0x2588; //full block shade
			else if (distanceToWall < depth / 3.0f)
				wshade = 0x2593; // medium high shade
			else if (distanceToWall < depth / 2.0f)
				wshade = 0x2592; // medium shade
			else if (distanceToWall < depth)
				wshade = 0x2591; // low shade
			else
				wshade = ' '; // no shade


			


			for (int y = 0; y < screenHeight; y++) 
			{
				if (y < celling) 
				{
					//
					//// Celling SHADING
					//

					short cshade = ' ';

					float b = 1.0f - (((float)y - screenHeight / 2.0f) / ((float)screenHeight / 2.0f));
					if (b < 0.25)
						cshade = '#';
					else if (b < 0.5)
						cshade = 'x';
					else if (b < 0.75)
						cshade = '.';
					else if (b < 0.9)
						cshade = '-';
					else
						cshade = ' ';

					screen[y * screenWidth + x] = ' ';
				}
				else if(y > celling && y <= floor)
				{
					screen[y * screenWidth + x] = wshade;
				}
				else
				{ 
					//
					//// FLOOR SHADING
					//

					short fshade = ' ';

					float b = 1.0f - (((float)y - screenHeight / 2.0f) / ((float)screenHeight / 2.0f));
					if (b < 0.25)
						fshade = '#';
					else if (b < 0.5)
						fshade = 'x';
					else if (b < 0.75)
						fshade = '.';
					else if (b < 0.9)
						fshade = '-';
					else
						fshade = ' ';
					
					screen[y * screenWidth + x] = fshade;
				}


			}


		}



		screen[screenWidth * screenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight, { 0, 0 }, &dwBytesWritten);

	}




	
	return 0;

}


