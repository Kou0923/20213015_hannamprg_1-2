#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <string.h>
#include <time.h>
#include <conio.h>

int menu, shop;
int level = 0;
int point = 0;
int plusMinute = 0;
bool gameover = false;
bool gameclear = false;
bool playing = false;
bool skip = false;
char GO_sel;

#define SIZE 19

struct item {
	int cost;
};

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
	MAP_FLAG_POINT,
	MAP_FLAG_OBSTACLE,
	MAP_FLAG_GOAL,
	PLAYER,
} MapFlag;

const int DIR[4][2] = { {0,-2},{0,2},{-2,0},{2,0} };

void shuffleArray(int array[], int size) {
	int i, r, temp;

	for (i = 0; i < (size - 1); ++i) {
		r = i + (rand() % (size - i));
		temp = array[i];
		array[i] = array[r];
		array[r] = temp;
	}
}

int inRange(int y, int x) {
	return (y < SIZE - 1 && y > 0) && (x < SIZE - 1 && x > 0);
}

void generateMap(int y, int x, int map[SIZE][SIZE]) {
	int i, nx, ny;
	int directions[4] = {
		DIRECTION_UP,
		DIRECTION_RIGHT,
		DIRECTION_DOWN,
		DIRECTION_LEFT
	};

	map[y][x] = MAP_FLAG_VISITED;

	shuffleArray(directions, 4);

	for (i = 0; i < 4; i++) {
		nx = x + DIR[directions[i]][0];
		ny = y + DIR[directions[i]][1];

		if (inRange(ny, nx) && map[ny][nx] == MAP_FLAG_WALL) {
			generateMap(ny, nx, map);
			if (ny != y)
				map[(ny + y) / 2][x] = MAP_FLAG_EMPTY;
			else
				map[y][(x + nx) / 2] = MAP_FLAG_EMPTY;
			map[ny][nx] = MAP_FLAG_EMPTY;
		}
	}
	if (ny != 1 && nx != 1) {
		map[ny][nx] = MAP_FLAG_POINT;
	}
}

void crushEvent(int map[SIZE][SIZE], int x, int y) {
	if (map[x][y] == MAP_FLAG_WALL) {
		gameover = true;
		skip = true;
	}

	if (map[x][y] == MAP_FLAG_EDGE) {
		skip = true;
	}

	if (map[x][y] == MAP_FLAG_POINT) {
		point += 1;
	}

	if (map[x][y] == MAP_FLAG_OBSTACLE) {
		point -= 1;
	}

	if (map[x][y] == MAP_FLAG_GOAL) {
		gameclear = true;
		skip = true;
	}
}

void display(int map[SIZE][SIZE]) {
	int num;
	int icon[7] = { "□", "■", "☆", "▲", "⊙", "★", "  " };
	printf("현재 레벨: %d\n", level);
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			switch (map[i][j]) {
			case MAP_FLAG_WALL:
				if (!playing) {
					num = 0;
				}
				else {
					num = 6;
				}
				break;
			case MAP_FLAG_EDGE:
				num = 1;
				break;
			case MAP_FLAG_POINT:
				num = 2;
				break;
			case MAP_FLAG_OBSTACLE:
				num = 3;
				break;
			case MAP_FLAG_GOAL:
				num = 4;
				break;
			case PLAYER:
				num = 5;
				break;
			default:
				num = 6;
			}
			printf("%s", icon[num]);
		}
		printf("\n");
	}
	if (playing) printf("보유 포인트: %d\n", point);
}

COORD getRandomStartingPoint() {
	int x = 1 + rand() % (SIZE - 1);
	int y = 1 + rand() % (SIZE - 1);
	if (x % 2 == 0)
		x--;
	if (y % 2 == 0)
		y--;
	return (COORD) { x, y };
}

int main(void) {
	struct item increase_timer = { 15 };

MENU:
	system("cls");
	printf("The Invisible Road\n\n");
	printf("┌──────────────┐\n");
	printf("│ 1. 게임 진행 │\n");
	printf("│ 2. 상점 진입 │\n");
	printf("│ 3. 게임 종료 │\n");
	printf("└──────────────┘\n\n");
	printf("원하시는 메뉴의 번호를 입력해주세요: ");
WRONG_SEL:
	scanf_s("%d", &menu);
	getchar();

	switch (menu) {
	case 1:
	GAMESTART:
		system("cls");
		int map[SIZE][SIZE];

		int X = 1, Y = 1, change_point = 0;
		int key;
		bool goal_loc = true;

		playing = false;


		COORD startPoint = getRandomStartingPoint();

		srand((unsigned int)time(NULL));

		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				map[i][j] = MAP_FLAG_WALL;
			}
		}
		generateMap(startPoint.Y, startPoint.X, map);

		for (int i = 0; i < SIZE; i++) {
			map[0][i] = map[SIZE - 1][i] = MAP_FLAG_EDGE;
		}

		for (int i = 0; i < SIZE; i++) {
			map[i][0] = map[i][SIZE - 1] = MAP_FLAG_EDGE;
		}

		while (change_point < 3 * (1 + level / 4)) {
			srand(time(NULL));
			int ran_x = rand() % (SIZE - 2) + 1;
			int ran_y = rand() % (SIZE - 2) + 1;
			if (ran_x != 1 && ran_y != 1 && map[ran_x][ran_y] == MAP_FLAG_EMPTY) {
				map[ran_x][ran_y] = MAP_FLAG_OBSTACLE;
				change_point++;
			}
		}

		while (goal_loc) {
			srand(time(NULL));
			int ran_x = rand() % ((SIZE - 1) / 2) + 1 + (SIZE / 4);
			int ran_y = rand() % ((SIZE - 1) / 2) + 1 + (SIZE / 4);
			if (map[ran_x][ran_y] == MAP_FLAG_WALL) {
				map[ran_x][ran_y] = MAP_FLAG_GOAL;
				goal_loc = false;
			}
		}

		for (int i = 10 + plusMinute; i > 0; i--) {
			display(map);
			printf("%d초 남았습니다.", i);
			Sleep(200 * i);
			system("cls");
		}
		map[1][1] = PLAYER;
		playing = true;
		display(map);
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		while (1) {
			skip = false;
			if (_kbhit()) {
				key = _getch();
				if (key == 224 || key == 0) {
					key = _getch();
					switch (key) {
					case 72:
						crushEvent(map, X - 1, Y);
						if (skip) {
							break;
						}
						system("cls");
						map[X][Y] = MAP_FLAG_EMPTY;
						map[X - 1][Y] = PLAYER;
						X--;
						display(map);
						break;
					case 75:
						crushEvent(map, X, Y - 1);
						if (skip) {
							break;
						}
						system("cls");
						map[X][Y] = MAP_FLAG_EMPTY;
						map[X][Y - 1] = PLAYER;
						Y--;
						display(map);
						break;
					case 77:
						crushEvent(map, X, Y + 1);
						if (skip) {
							break;
						}
						system("cls");
						map[X][Y] = MAP_FLAG_EMPTY;
						map[X][Y + 1] = PLAYER;
						Y++;
						display(map);
						break;
					case 80:
						crushEvent(map, X + 1, Y);
						if (skip) {
							break;
						}
						system("cls");
						map[X][Y] = MAP_FLAG_EMPTY;
						map[X + 1][Y] = PLAYER;
						X++;
						display(map);
						break;
					}
				}
			}
			if (gameover) {
				system("cls");
				printf("GAME OVER\n\n");
				gameover = false;
				printf("재도전 하시겠습니까?(Y/N) >> ");
				while (1) {
					getchar();
					scanf_s("%c", &GO_sel);
					if (GO_sel == 'Y' || GO_sel == 'y') {
						system("cls");
						goto GAMESTART;
					}
					else if (GO_sel == 'N' || GO_sel == 'n') {
						printf("메뉴로 돌아갑니다.");
						goto MENU;
					}
					else {
						printf("Y/N 으로 입력해주세요: ");
					}
				}
				break;
			}
			else if (gameclear) {
				getchar();
				level += 1;
				gameclear = false;
				goto GAMESTART;
			}
		}
		break;
	case 2:
SHOP:
		system("cls");
		printf("Shop\n\n");
		printf("┌───────────────────────────┐\n");
		printf("│ 1. 추가 시간 (+1초) - %dP │\n", increase_timer.cost);
		printf("│ 0. 메뉴로 돌아가기        │\n");
		printf("└───────────────────────────┘\n");
		printf("보유 포인트: %d\n", point);
		printf("원하시는 메뉴의 번호를 입력해주세요: ");
WRONG_SEL_SHOP:
		getchar();
		scanf_s("%d", &shop);
		while (1) {
			switch (shop) {
			case 1:
				if (point >= increase_timer.cost) {
					printf("추가 시간이 1초 늘어났습니다.\n\n");
					point -= increase_timer.cost;
					plusMinute += 1;
					Sleep(200 * 3);
					goto SHOP;
				}
				else {
					printf("포인트가 부족합니다.\n\n");
					Sleep(200 * 3);
					goto SHOP;
				}
				break;
			case 0:
				goto MENU;
	        default:
				printf("존재하지 않는 메뉴입니다. 다시 입력해주세요: ");
				getchar();
				goto WRONG_SEL_SHOP;
			}
		}
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
