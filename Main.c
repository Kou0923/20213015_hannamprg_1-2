#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <string.h>
#include <time.h>
#include <conio.h>

int menu;
bool gameover = false;
char GO_sel;

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
	MAP_FLAG_EDGE,
	PLAYER,
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
MENU:
	printf("The Invisible Road\n\n");
	printf("┌──────────────┐\n");
	printf("│ 1. 게임 진행 │\n");
	printf("│ 2. 상점 진입 │\n");
	printf("│ 3. 게임 종료 │\n");
	printf("└──────────────┘\n\n");
	printf("원하시는 메뉴의 번호를 입력해주세요: ");
WRONG_SEL:
	scanf_s("%d", &menu);

	switch (menu) {
	case 1:
		system("cls");

		int X = 1, Y = 1;
		int key;

		int map[SIZE1][SIZE1];

		COORD startPoint = getRandomStartingPoint();

		srand((unsigned int)time(NULL));
		memset(map, MAP_FLAG_WALL, sizeof(map));

		generateMap(startPoint.Y, startPoint.X, map);

		for (int i = 5; i > 0; i--) {
			for (int i = 0; i < SIZE1; ++i) {
				for (int j = 0; j < SIZE1; ++j) {
					printf("%s", map[i][j] == MAP_FLAG_WALL ? "■" : "  ");
				}
				printf("\n");
			}
			printf("%d초 남았습니다.", i);
			Sleep(1000);
			system("cls");
		}
		map[X][Y] = PLAYER;

		for (int i = 0; i < SIZE1; ++i) {
			for (int j = 0; j < SIZE1; ++j) {
				printf("%s", map[i][j] == PLAYER ? "★" : "  ");
			}
			printf("\n");
		}
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

		while (1) {
			if (_kbhit()) {
				key = _getch();
				if (key == 224 || key == 0) {
					key = _getch();
					switch (key) {
					case 72:
						if (map[X - 1][Y] == MAP_FLAG_WALL) {
							gameover = true;
							break;
						}
						system("cls");
						map[X][Y] = MAP_FLAG_EMPTY;
						map[X - 1][Y] = PLAYER;
						X--;
						for (int i = 0; i < SIZE1; i++) {
							for (int j = 0; j < SIZE1; j++) {
								printf("%s", map[i][j] == PLAYER ? "★" : "  ");
							}
							printf("\n");
						}
						break;
					case 75:
						if (map[X][Y - 1] == MAP_FLAG_WALL) {
							gameover = true;
							break;
						}
						system("cls");
						map[X][Y] = MAP_FLAG_EMPTY;
						map[X][Y - 1] = PLAYER;
						Y--;
						for (int i = 0; i < SIZE1; i++) {
							for (int j = 0; j < SIZE1; j++) {
								printf("%s", map[i][j] == PLAYER ? "★" : "  ");
							}
							printf("\n");
						}
						break;
					case 77:
						if (map[X][Y + 1] == MAP_FLAG_WALL) {
							gameover = true;
							break;
						}
						system("cls");
						map[X][Y] = MAP_FLAG_EMPTY;
						map[X][Y + 1] = PLAYER;
						Y++;
						for (int i = 0; i < SIZE1; i++) {
							for (int j = 0; j < SIZE1; j++) {
								printf("%s", map[i][j] == PLAYER ? "★" : "  ");
							}
							printf("\n");
						}
						break;
					case 80:
						if (map[X + 1][Y] == MAP_FLAG_WALL) {
							gameover = true;
							break;
						}
						system("cls");
						map[X][Y] = MAP_FLAG_EMPTY;
						map[X + 1][Y] = PLAYER;
						X++;
						for (int i = 0; i < SIZE1; i++) {
							for (int j = 0; j < SIZE1; j++) {
								printf("%s", map[i][j] == PLAYER ? "★" : "  ");
							}
							printf("\n");
						}
						break;
					}
				}
			}
			if (gameover) {
				system("cls");
				printf("GAME OVER\n\n");
				gameover = false;
				printf("메인 화면으로 돌아가시겠습니까?(Y/N) >> ");
				while (1) {
					getchar();
					scanf_s("%c", &GO_sel);
					if (GO_sel == 'Y' || GO_sel == 'y') {
						system("cls");
						goto MENU;
					}
					else if (GO_sel == 'N' || GO_sel == 'n') {
						printf("게임을 종료합니다.");
						break;
					}
					else {
						printf("Y/N 으로 입력해주세요: ");
					}
				}
				break;
			}
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
		getchar();
		goto WRONG_SEL;
	}

	return 0;
}
