#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <string.h>
#include <time.h>

int menu;

#define SIZE1 11
#define SIZE2 19
#define SIZE3 27
#define SIZE4 35
#define SIZE5 43

typedef enum _Direction {
	DIRECTION_LEFT,
	DIRECTION_UP,
	DIRECTION_RIGHT,
	DIRECTION_DOWN
} Direction;

typedef enum _MapFlag {
	MAP_FLAG_WALL,
	MAP_FLAG_EMPTY,
	MAP_FLAG_VISITED,
} MapFlag;

const int DIR[4][2] = { {0,-2},{0,2},{-2,0},{2,0} };

void shuffleArray(int array[], int size)
{
	int i, r, temp;

	for (i = 0; i < (size - 1); ++i)
	{
		r = i + (rand() % (size - i));
		temp = array[i];
		array[i] = array[r];
		array[r] = temp;
	}
}

int inRange(int y, int x)
{
	return (y < SIZE1 - 1 && y > 0) && (x < SIZE1 - 1 && x > 0);
}

void generateMap(int y, int x, int map[SIZE1][SIZE1])
{
	int i, nx, ny;
	int directions[4] = {
		DIRECTION_UP,
		DIRECTION_RIGHT,
		DIRECTION_DOWN,
		DIRECTION_LEFT
	};

	map[y][x] = MAP_FLAG_VISITED;

	shuffleArray(directions, 4);

	for (i = 0; i < 4; i++)
	{
		nx = x + DIR[directions[i]][0];
		ny = y + DIR[directions[i]][1];

		if (inRange(ny, nx) && map[ny][nx] == MAP_FLAG_WALL)
		{
			generateMap(ny, nx, map);
			if (ny != y)
				map[(ny + y) / 2][x] = MAP_FLAG_EMPTY;
			else
				map[y][(x + nx) / 2] = MAP_FLAG_EMPTY;
			map[ny][nx] = MAP_FLAG_EMPTY;
		}
	}
}

COORD getRandomStartingPoint()
{
	int x = 1 + rand() % (SIZE1 - 1);
	int y = 1 + rand() % (SIZE1 - 1);
	if (x % 2 == 0)
		x--;
	if (y % 2 == 0)
		y--;
	return (COORD) { x, y };
}

int main(void) {
	printf("The Invisible Road\n\n");
	printf("┌──────────────┐\n");
	printf("│ 1. 게임 진행 │\n");
	printf("│ 2. 상점 진입 │\n");
	printf("│ 3. 게임 종료 │\n");
	printf("└──────────────┘\n\n");
	printf("원하시는 메뉴의 번호를 입력해주세요: ");
Menu:
	scanf_s("%d", &menu);

	switch (menu) {
	case 1:
		system("cls");
		int map[SIZE1][SIZE1];
		COORD startPoint = getRandomStartingPoint();

		srand((unsigned int)time(NULL));
		memset(map, MAP_FLAG_WALL, sizeof(map));

		generateMap(startPoint.Y, startPoint.X, map);

		for (int i = 0; i < SIZE1; ++i)
		{
			for (int j = 0; j < SIZE1; ++j)
				printf("%s", map[i][j] == MAP_FLAG_WALL ? "■" : "  ");
			printf("\n");
		}
		break;
	case 2:
		system("cls");
		printf("조건 만족 2");
		break;
	case 3:
		system("cls");
		printf("게임을 종료합니다.");
		break;
	default:
		printf("존재하지 않는 메뉴입니다. 다시 입력해주세요: ");
		goto Menu;
	}

	return 0;
}