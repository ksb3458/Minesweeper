// 1991018 김수빈
// 지뢰찾기

#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// 색상 정의
#define BLACK	0
#define BLUE1	1
#define GREEN1	2
#define CYAN1	3
#define RED1	4
#define MAGENTA1 5
#define YELLOW1	6
#define GRAY1	7
#define GRAY2	8
#define BLUE2	9
#define GREEN2	10
#define CYAN2	11
#define RED2	12
#define MAGENTA2 13
#define YELLOW2	14
#define WHITE	15

#define STAR "★"
#define SQUARE "■"
#define SQUARE_B "□"
#define ONE "①"
#define TWO "②"
#define THREE "③"
#define FOUR "④"
#define FIVE "⑤"
#define SIX "⑥"
#define SEVEN "⑦"
#define EIGHT "⑧"
#define FLAG "ⓕ"
#define BLANK ' ' // ' ' 로하면 흔적이 지워진다 

#define ESC 0x1b //  ESC 누르면 종료

#define SPECIAL1 0xe0 // 특수키는 0xe0 + key 값으로 구성된다.
#define SPECIAL2 0x00 // keypad 경우 0x00 + key 로 구성된다.

#define UP  0x48 // Up key는 0xe0 + 0x48 두개의 값이 들어온다.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d
#define SPACE 0x20
#define TAB 0x09
#define ENTER 0x0d

#define UP2	'w'
#define DOWN2	's'
#define LEFT2	'a'
#define RIGHT2	'd'

#define WIDTH 80
#define HEIGHT 24

int map[50][50];
int map2[50][50];  //지뢰 저장 
int map3[50][50] = { 0 };  //map_control
int map4[50][50] = { 0 };  //FLAG
int Delay = 100; // 100 msec delay, 이 값을 줄이면 속도가 빨라진다.
int keep_moving = 0; // 1:계속이동, 0:한칸씩이동.
int x, y; // 가로, 세로
int select_menu;
int num_mine;
int x_value, y_value; //x좌표, y좌표
int push_key = 0;

void removeCursor(void) { // 커서를 안보이게 한다

	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API 함수입니다. 이건 알필요 없어요
}

void putstar(int x, int y, char ch)
{
	gotoxy(x, y);
	putchar(ch);
}
void erasestar(int x, int y)
{
	gotoxy(x, y);
	putchar(BLANK);
}

void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}

// 화면 지우기고 원하는 배경색으로 설정한다.
void cls(int bg_color, int text_color)
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);
}

// box 그리기 함수, ch 문자열로 (x1,y1) ~ (x2,y2) box를 그린다.
void draw_box(int x1, int y1, int x2, int y2, char ch)
{
	int x, y;

	for (x = x1; x <= x2; x += 1) {
		gotoxy(x, y1);
		printf("%c", ch);
		gotoxy(x, y2);
		printf("%c", ch);
	}
	for (y = y1; y <= y2; y += 1) {
		gotoxy(x1, y);
		printf("%c", ch);
		gotoxy(x2, y);
		printf("%c", ch);
	}
}

void start_first() {
	gotoxy(1, 2);
	printf("\n");
	textcolor(WHITE, WHITE);
	printf("      ");
	textcolor(RED2, WHITE);
	printf("1. 초급");
	textcolor(WHITE, WHITE);
	printf(" (7x7, 지뢰 10개)    \n");
	printf("\n");
	textcolor(WHITE, WHITE);
	printf("      ");
	textcolor(RED2, WHITE);
	printf("2. 중급");
	textcolor(WHITE, WHITE);
	printf(" (12x12, 지뢰 30개)   \n");
	printf("\n");
	textcolor(WHITE, WHITE);
	printf("      ");
	textcolor(RED2, WHITE);
	printf("3. 고급");
	textcolor(WHITE, WHITE);
	printf(" (20x20, 지뢰 60개) \n");
	printf("\n");
	textcolor(WHITE, WHITE);
	printf("      ");
	textcolor(RED2, WHITE);
	printf("4. 사용자 지정  ");
	textcolor(YELLOW1, WHITE);
	draw_box(1, 0, 40, 12, '*');
}

int key_control(x_value, y_value) {
	push_key = 0;
	gotoxy(6, 3);
	textcolor(RED2, GRAY1);
	printf("1. 초급");
	textcolor(WHITE, GRAY1);
	printf(" (7x7, 지뢰 10개) ");
	textcolor(BLACK, WHITE);
	gotoxy(x_value - 2, y_value);
	printf(">");
	while (1) {
		removeCursor();
		int n = getch();
		switch (n) {
		case UP: {
			if (y_value > 3) {
				textcolor(BLACK, WHITE);
				gotoxy(x_value - 2, y_value);
				printf(" ");
				gotoxy(x_value - 2, --y_value);
				gotoxy(x_value - 2, --y_value);
				printf(">");
				if (y_value == 3) {
					start_first();
					gotoxy(6, 3);
					textcolor(RED2, GRAY1);
					printf("1. 초급");
					textcolor(WHITE, GRAY1);
					printf(" (7x7, 지뢰 10개) ");
					textcolor(BLACK, WHITE);
					gotoxy(x_value - 4, y_value);
					printf(" ");
					printf("> ");
				}
				else if (y_value == 5) {
					start_first();
					gotoxy(6, 5);
					textcolor(RED2, GRAY1);
					printf("2. 중급");
					textcolor(WHITE, GRAY1);
					printf(" (12x12, 지뢰 30개) ");
					textcolor(BLACK, WHITE);
					gotoxy(x_value - 4, y_value);
					printf(" ");
					printf("> ");
				}
				else if (y_value == 7) {
					start_first();
					gotoxy(6, 7);
					textcolor(RED2, GRAY1);
					printf("3. 고급");
					textcolor(WHITE, GRAY1);
					printf(" (20x20, 지뢰 60개) ");
					textcolor(BLACK, WHITE);
					gotoxy(x_value - 4, y_value);
					printf(" ");
					printf("> ");
				}
				else if (y_value == 9) {
					start_first();
					gotoxy(6, 7);
					textcolor(RED2, GRAY1);
					printf("4. 사용자 지정   ");
					textcolor(BLACK, WHITE);
					gotoxy(x_value - 4, y_value);
					printf(" ");
					printf("> ");
				}
			}
			break;
		}

		case DOWN: {
			if (y_value < 8) {
				gotoxy(x_value - 2, y_value);
				printf(" ");
				gotoxy(x_value - 2, ++y_value);
				gotoxy(x_value - 2, ++y_value);
				printf(">");
				if (y_value == 3) {
					start_first();
					gotoxy(6, 3);
					textcolor(RED2, GRAY1);
					printf("1. 초급");
					textcolor(WHITE, GRAY1);
					printf(" (7x7, 지뢰 10개) ");
					textcolor(BLACK, WHITE);
					gotoxy(x_value - 4, y_value);
					printf(" ");
					printf("> ");
				}
				else if (y_value == 5) {
					start_first();
					gotoxy(6, 5);
					textcolor(RED2, GRAY1);
					printf("2. 중급");
					textcolor(WHITE, GRAY1);
					printf(" (12x12, 지뢰 30개) ");
					textcolor(BLACK, WHITE);
					gotoxy(x_value - 4, y_value);
					printf(" ");
					printf("> ");
				}
				else if (y_value == 7) {
					start_first();
					gotoxy(6, 7);
					textcolor(RED2, GRAY1);
					printf("3. 고급");
					textcolor(WHITE, GRAY1);
					printf(" (20x20, 지뢰 60개) ");
					textcolor(BLACK, WHITE);
					gotoxy(x_value - 4, y_value);
					printf(" ");
					printf("> ");
				}
				else if (y_value == 9) {
					start_first();
					gotoxy(6, 9);
					textcolor(RED2, GRAY1);
					printf("4. 사용자 지정  ");
					textcolor(BLACK, WHITE);
					gotoxy(x_value - 4, y_value);
					printf(" ");
					printf("> ");
				}
			}
			break;
		}

		case '1': 
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': {
			select_menu = n - 48;
			return select_menu;
		}

		case SPACE: {
			push_key = y_value - 2;
			return push_key; // 1,2,3,4
		}
		}
	}
}

int menu_select() {
	gotoxy(1, 14);
	textcolor(WHITE, WHITE);
	printf("                                                     \n");
	printf("                                                     ");
	textcolor(WHITE, GREEN1);
	gotoxy(1, 16);
	printf(">>스페이스바로 메뉴를 선택 혹은 원하시는 메뉴의 번호를 입력해주세요<<");
	gotoxy(1, 14);
	textcolor(BLACK, WHITE);
	printf("메뉴를 선택하세요");
	key_control(6,3);

	if (select_menu == 1 || push_key == 1) {
		gotoxy(19, 14);
		printf("1");
		Sleep(100);
		start_loading();
	}
	else if (select_menu == 2 || push_key == 3) {
		gotoxy(19, 14);
		printf("2");
		Sleep(100);
		start_loading();
	}
	else if (select_menu == 3 || push_key == 5) {
		gotoxy(19, 14);
		printf("3");
		Sleep(100);
		start_loading();
	}
	else if (select_menu == 4 || push_key==7) {
		gotoxy(19, 14);
		printf("4");
		gotoxy(1, 16);
		printf("                                                                             ");
		gotoxy(1, 16);
		printf("원하시는 가로 세로 크기를 입력해주세요 ");
		scanf("%d %d", &x, &y);
		printf(" 원하시는 지뢰 수를 입력해주세요 ");
		scanf("%d", &num_mine);
		while (num_mine >= x * y) {
			gotoxy(1, 16);
			printf("                                                                ");
			gotoxy(1, 16);
			printf("가능한 지뢰수를 초과하였습니다. %d x %d 이내로 입력해주세요 ",x,y);
			gotoxy(33,17);
			printf("             ");
			gotoxy(33, 17);
			scanf("%d", &num_mine);
		}
		start_loading();
	}
	else {
		gotoxy(1, 15);
		printf("해당하는 메뉴가 없습니다. 다시 입력해주세요. ");
		//removeCursor();
		Sleep(1500);
		menu_select();
	}
}

int start_loading() {
	cls(WHITE, WHITE);
	removeCursor();
	gotoxy(0, 0);
	for (int i = 0; i < 20; i++) {
		textcolor(WHITE, YELLOW1);
		gotoxy(0, i);
		if (i == 10) {
			printf("                           MINESWEEPER                              ");
		}
		else
			printf("                                                                    ");
		Sleep(100);
	}
	draw_mine();
}

int draw_mine() {
	/*textcolor(WHITE, WHITE);
	cls(WHITE, WHITE);*/
	textcolor(GRAY1, GRAY2);
	gotoxy(31, 4);
	printf("■");
	gotoxy(31, 5);
	printf("■");
	gotoxy(23, 6);
	printf("■  ■■■■■  ■");
	textcolor(YELLOW1, YELLOW1);
	gotoxy(25, 6);
	printf("  ");
	gotoxy(37, 6);
	printf("  ");
	textcolor(GRAY1, GRAY2);
	gotoxy(25, 7);
	printf("■■■■■■■");
	gotoxy(23, 8);
	printf("■■■■■■■■■");
	gotoxy(23, 9);
	printf("■■■■■■■■■");
	gotoxy(19, 10);
	printf("■■■■■■■■■■■■■");
	gotoxy(31, 16);
	printf("■");
	gotoxy(31, 15);
	printf("■");
	gotoxy(23, 14);
	printf("■  ■■■■■  ■");
	textcolor(YELLOW1, YELLOW1);
	gotoxy(25, 14);
	printf("  ");
	gotoxy(37, 14);
	printf("  ");
	textcolor(GRAY1, GRAY2);
	gotoxy(25, 13);
	printf("■■■■■■■");
	gotoxy(23, 12);
	printf("■■■■■■■■■");
	gotoxy(23, 11);
	printf("■■■■■■■■■");
	textcolor(WHITE, WHITE);
	gotoxy(27, 8);
	printf("■■");
	gotoxy(27, 9);
	printf("■■");
	removeCursor();
	Sleep(1000);
	game_first();
}

int game_first() {
	textcolor(WHITE, WHITE);
	if (select_menu == 1 || push_key==1) {
		num_mine = 10;
		x = 7;
		y = 7;
		shape(7, 7);
	}
	else if (select_menu == 2 || push_key==3) {
		num_mine = 30;
		x = 12;
		y = 12;
		shape(12, 12);
	}
	else if (select_menu == 3 || push_key==5) {
		num_mine = 60;
		x = 20;
		y = 20;
		shape(20, 20);
	}
	else if (select_menu == 4 || push_key==7) {
		shape(x, y);
	}
}

int shape(int x, int y) { // 기본 틀
	cls(WHITE, WHITE);
	gotoxy(0, 2);
	textcolor(GRAY1, GRAY2);
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			map[i][j] = 0;
		}
	}

	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (j == 0) {
				textcolor(WHITE, WHITE);
				printf("     ");
			}
			if (map[i][j] == 0) {
				textcolor(GRAY1, GRAY2);
				printf("■");
			}
		}
		printf("\n");
	}
}

void show_time(int remain_time) //플레이 시간
{
	printf("%4d", remain_time);
}

void put_mine() {

	textcolor(GRAY1, GRAY2);
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			map[i][j] = 0;
			map2[i][j] = 0;
		}
	}

	int random_x = 0;
	int random_y = 0;
	for (int i = num_mine; i > 0; i--) {
		srand((unsigned)time(NULL));
		random_x = rand() % x;
		random_y = rand() % y;
		while (map2[random_x][random_y] != 0) {
			random_x = rand() % x;
			random_y = rand() % y;
		}
		map2[random_x][random_y] = 10;
	}
}

int put_number() {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (i == 0 && j == 0 && map2[i][j] != 10) {
				if (map2[i + 1][j] == 10)
					map2[i][j]++;
				if (map2[i][j + 1] == 10)
					map2[i][j]++;
				if (map2[i + 1][j + 1] == 10)
					map2[i][j]++;
			}
			else if (i == x - 1 && j == 0 && map2[i][j] != 10) {
				if (map2[i - 1][j] == 10)
					map2[i][j]++;
				if (map2[i - 1][j + 1] == 10)
					map2[i][j]++;
				if (map2[i][j + 1] == 10)
					map2[i][j]++;
			}
			else if (i == 0 && j == y - 1 && map2[i][j] != 10) {
				if (map2[i][j - 1] == 10)
					map2[i][j]++;
				if (map2[i + 1][j - 1] == 10)
					map2[i][j]++;
				if (map2[i + 1][j] == 10)
					map2[i][j]++;
			}
			else if (i == x - 1 && j == y - 1 && map2[i][j] != 10) {
				if (map2[i - 1][j - 1] == 10)
					map2[i][j]++;
				if (map2[i][j - 1] == 10)
					map2[i][j]++;
				if (map2[i - 1][j] == 10)
					map2[i][j]++;
			}
			else if (i == 0 && map2[i][j] != 10) {
				if (map2[i][j - 1] == 10)
					map2[i][j]++;
				if (map2[i + 1][j - 1] == 10)
					map2[i][j]++;
				if (map2[i + 1][j] == 10)
					map2[i][j]++;
				if (map2[i + 1][j + 1] == 10)
					map2[i][j]++;
				if (map2[i][j + 1] == 10)
					map2[i][j]++;
			}
			else if (i == x - 1 && map2[i][j] != 10) {
				if (map2[i][j - 1] == 10)
					map2[i][j]++;
				if (map2[i - 1][j - 1] == 10)
					map2[i][j]++;
				if (map2[i - 1][j] == 10)
					map2[i][j]++;
				if (map2[i - 1][j + 1] == 10)
					map2[i][j]++;
				if (map2[i][j + 1] == 10)
					map2[i][j]++;
			}
			else if (j == 0 && map2[i][j] != 10) {
				if (map2[i - 1][j] == 10)
					map2[i][j]++;
				if (map2[i - 1][j + 1] == 10)
					map2[i][j]++;
				if (map2[i][j + 1] == 10)
					map2[i][j]++;
				if (map2[i + 1][j + 1] == 10)
					map2[i][j]++;
				if (map2[i + 1][j] == 10)
					map2[i][j]++;
			}
			else if (j == y - 1 && map2[i][j] != 10) {
				if (map2[i - 1][j] == 10)
					map2[i][j]++;
				if (map2[i - 1][j - 1] == 10)
					map2[i][j]++;
				if (map2[i][j - 1] == 10)
					map2[i][j]++;
				if (map2[i + 1][j - 1] == 10)
					map2[i][j]++;
				if (map2[i + 1][j] == 10)
					map2[i][j]++;
			}
			else if (map2[i][j] != 10) {
				if (map2[i - 1][j - 1] == 10)
					map2[i][j]++;
				if (map2[i][j - 1] == 10)
					map2[i][j]++;
				if (map2[i + 1][j - 1] == 10)
					map2[i][j]++;
				if (map2[i - 1][j] == 10)
					map2[i][j]++;
				if (map2[i + 1][j] == 10)
					map2[i][j]++;
				if (map2[i - 1][j + 1] == 10)
					map2[i][j]++;
				if (map2[i][j + 1] == 10)
					map2[i][j]++;
				if (map2[i + 1][j + 1] == 10)
					map2[i][j]++;
			}
		}
	}
}

int updown = 0;
int control_gameover() {
	int n = getch();
	if (n == UP) {
		gotoxy(5, y + 3);
		textcolor(WHITE, WHITE);
		printf("                                                                      ");
		gotoxy(x - 1, y / 2 + 2);
		textcolor(WHITE, BLACK);
		printf("> 1. 메   뉴\n");
		gotoxy(x - 1, y / 2 + 3);
		printf("  2. 종   료");
		updown = 0;
	}
	else if (n == DOWN) {
		gotoxy(5, y + 3);
		textcolor(WHITE, WHITE);
		printf("                                                                      ");
		gotoxy(x - 1, y / 2 + 2);
		textcolor(WHITE, BLACK);
		printf("  1. 메   뉴\n");
		gotoxy(x - 1, y / 2 + 3);
		printf("> 2. 종   료");
		updown = 1;
	}
	else if (n == SPACE) {
		if (updown == 0) {
			cls(WHITE, WHITE);
			main();
		}
		else if (updown == 1) {
			textcolor(WHITE, BLACK);
			exit(1);
		}
	}
}

int game_over() {
	while (1) {
		control_gameover();
		gotoxy(x - 1, y / 2);
		textcolor(RED1, BLACK);
		printf("> Game Over <");
		Sleep(200);
		control_gameover();
		gotoxy(x - 1, y / 2);
		textcolor(GREEN1, BLACK);
		printf("> Game Over <");
		Sleep(200);
		control_gameover();
		gotoxy(x - 1, y / 2);
		textcolor(YELLOW1, BLACK);
		printf("> Game Over <");
		Sleep(200);
		control_gameover();
		gotoxy(x - 1, y / 2);
		textcolor(BLUE1, BLACK);
		printf("> Game Over <");
		Sleep(200);
		control_gameover();
		gotoxy(x - 1, y / 2);
		textcolor(CYAN1, BLACK);
		printf("> Game Over <");
		Sleep(200);
		control_gameover();
		gotoxy(x - 1, y / 2);
		textcolor(MAGENTA1, BLACK);
		printf("> Game Over <");
		Sleep(200);
	}
}

int game_success1() {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (map2[i][j] == 10)
				map3[i][j] == 11;
		}
	}
}

int game_success2() {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (map3[i][j] == 0)
				return;
			if (i == x && j == y && map3[i][j] > 0)
				game_success3();
		}
	}
}

int game_success3() {
	while (1) {
		control_gameover();
		gotoxy((x + 8) / 2, y / 2);
		textcolor(RED1, BLACK);
		printf("> Game Success <");
		Sleep(200);
		control_gameover();
		gotoxy((x + 8) / 2, y / 2);
		textcolor(GREEN1, BLACK);
		printf("> Game Success <");
		Sleep(200);
		control_gameover();
		gotoxy((x + 8) / 2, y / 2);
		textcolor(YELLOW1, BLACK);
		printf("> Game Success <");
		Sleep(200);
		control_gameover();
		gotoxy((x + 8) / 2, y / 2);
		textcolor(BLUE1, BLACK);
		printf("> Game Success <");
		Sleep(200);
		control_gameover();
		gotoxy((x + 8) / 2, y / 2);
		textcolor(CYAN1, BLACK);
		printf("> Game Success <");
		Sleep(200);
		control_gameover();
		gotoxy((x + 8) / 2, y / 2);
		textcolor(MAGENTA1, BLACK);
		printf("> Game Success <");
		Sleep(200);
	}
}

int map_control(x_value, y_value) {
	textcolor(WHITE, GRAY2);
	gotoxy(5, y + 3);
	printf(">>선택 : 스페이스바<<\n");
	gotoxy(5, y + 4);
	printf(">>깃발 : TAB키<<\n");
	gotoxy(x_value - 2, y_value);
	int run_time = 0, start_time;
	start_time = time(NULL);
	int count = 0;
	int count_flag = 0;
	int num_mine_flag = num_mine;
	while (1) {
		int n = getch();
		gotoxy(5, 1);
		printf(FLAG" %3d", num_mine_flag);
		gotoxy((2 * x + 8) / 2, 1);
		run_time = time(NULL) - start_time;
		show_time(run_time);
		
		switch (n) {
		case UP: {
			if (y_value > 2) {
				textcolor(GRAY1, GRAY2);
				gotoxy(x_value - 2, y_value);
				if (map4[y_value - 2][(x_value - 7) / 2] == 1)
					printf(FLAG);

				else if (map3[y_value - 2][(x_value - 7) / 2] == 0)
					printf(SQUARE);

				else {
					if (map2[y_value - 2][(x_value - 7) / 2] == 1) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(ONE);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 2) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(TWO);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 3) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(THREE);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 4) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(FOUR);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 5) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(FIVE);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 6) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(SIX);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 7) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(SEVEN);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 8) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(EIGHT);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 10) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(STAR);
					}
					else {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(SQUARE_B);
					}
				}
				gotoxy(x_value - 2, --y_value);
				textcolor(GRAY2, GRAY1);
				printf(SQUARE);
			}
			break;
		}

		case DOWN: {
			if (y_value < y + 1) {
				textcolor(GRAY1, GRAY2);
				gotoxy(x_value - 2, y_value);
				if (map4[y_value - 2][(x_value - 7) / 2] == 1)
					printf(FLAG);

				else if (map3[y_value - 2][(x_value - 7) / 2] == 0)
					printf(SQUARE);

				else {
					if (map2[y_value - 2][(x_value - 7) / 2] == 1) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(ONE);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 2) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(TWO);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 3) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(THREE);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 4) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(FOUR);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 5) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(FIVE);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 6) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(SIX);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 7) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(SEVEN);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 8) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(EIGHT);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 10) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(STAR);
					}
					else {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(SQUARE_B);
					}
				}
				gotoxy(x_value - 2, ++y_value);
				textcolor(GRAY2, GRAY1);
				printf(SQUARE);
			}
			break;
		}

		case RIGHT: {
			if (x_value < x * 2 + 5) {
				textcolor(GRAY1, GRAY2);
				gotoxy(x_value - 2, y_value);
				if (map4[y_value - 2][(x_value - 7) / 2] == 1)
					printf(FLAG);

				else if (map3[y_value - 2][(x_value - 7) / 2] == 0)
					printf(SQUARE);

				else {
					if (map2[y_value - 2][(x_value - 7) / 2] == 1) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(ONE);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 2) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(TWO);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 3) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(THREE);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 4) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(FOUR);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 5) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(FIVE);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 6) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(SIX);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 7) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(SEVEN);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 8) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(EIGHT);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 10) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(STAR);
					}
					else {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(SQUARE_B);
					}
				}
				gotoxy(++x_value - 2, y_value);
				gotoxy(++x_value - 2, y_value);
				textcolor(GRAY2, GRAY1);
				printf(SQUARE);
			}
			break;
		}

		case LEFT: {
			if (x_value > 7) {
				textcolor(GRAY1, GRAY2);
				gotoxy(x_value - 2, y_value);
				if (map4[y_value - 2][(x_value - 7) / 2] == 1)
					printf(FLAG);

				else if (map3[y_value - 2][(x_value - 7) / 2] == 0)
					printf(SQUARE);

				else {
					if (map2[y_value - 2][(x_value - 7) / 2] == 1) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(ONE);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 2) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(TWO);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 3){
						gotoxy(x_value - 2, y_value);
							textcolor(RED1, GRAY2);
							printf(THREE);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 4) {
						gotoxy(x_value - 2, y_value);
							textcolor(RED1, GRAY2);
							printf(FOUR);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 5) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(FIVE);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 6) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(SIX);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 7) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(SEVEN);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 8) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(EIGHT);
					}
					else if (map2[y_value - 2][(x_value - 7) / 2] == 10) {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(STAR);
					}
					else {
						gotoxy(x_value - 2, y_value);
						textcolor(RED1, GRAY2);
						printf(SQUARE_B);
					}
				}
				gotoxy(--x_value - 2, y_value);
				gotoxy(--x_value - 2, y_value);
				textcolor(GRAY2, GRAY1);
				printf(SQUARE);
			}
			break;
		}
		
		case SPACE: {
			map3[y_value - 2][(x_value - 7) / 2]++;

			if (map4[y_value - 2][(x_value - 7) / 2] == 1) {
				map3[y_value - 2][(x_value - 7) / 2]--;
			}

			else if (map2[y_value-2][(x_value - 7)/2] > 9) {
				gotoxy(x_value - 2, y_value);
				textcolor(RED1, GRAY2);
				printf(STAR);
				for (int i = 0; i < y + 5; i++) {
					gotoxy(1, i);
					for (int j = 0; j < x + 10; j++)
						printf("  ");
					Sleep(100);
				}
				cls(WHITE, BLACK);
				show_mine();
				gotoxy(5, y + 3);
				textcolor(WHITE, GREEN1);
				printf("아무 키나 입력하세요");
				game_over();
			}

			else if (map2[y_value - 2][(x_value - 7)/2] == 1) {
				gotoxy(x_value - 2, y_value);
				textcolor(RED1, GRAY2);
				printf(ONE);
			}
			else if (map2[y_value - 2][(x_value - 7) / 2] == 2) {
				gotoxy(x_value - 2, y_value);
				textcolor(RED1, GRAY2);
				printf(TWO);
			}
			else if (map2[y_value - 2][(x_value - 7) / 2] == 3) {
				gotoxy(x_value - 2, y_value);
				textcolor(RED1, GRAY2);
				printf(THREE);
			}
			else if (map2[y_value - 2][(x_value - 7) / 2] == 4) {
				gotoxy(x_value - 2, y_value);
				textcolor(RED1, GRAY2);
				printf(FOUR);
			}
			else if (map2[y_value - 2][(x_value - 7) / 2] == 5) {
				gotoxy(x_value - 2, y_value);
				textcolor(RED1, GRAY2);
				printf(FIVE);
			}
			else if (map2[y_value - 2][(x_value - 7) / 2] == 6) {
				gotoxy(x_value - 2, y_value);
				textcolor(RED1, GRAY2);
				printf(SIX);
			}
			else if (map2[y_value - 2][(x_value - 7) / 2] == 7) {
				gotoxy(x_value - 2, y_value);
				textcolor(RED1, GRAY2);
				printf(SEVEN);
			}
			else if (map2[y_value - 2][(x_value - 7) / 2] == 8) {
				gotoxy(x_value - 2, y_value);
				textcolor(RED1, GRAY2);
				printf(EIGHT);
			}
			else if (map2[y_value - 2][(x_value - 7) / 2] == 10) {
				gotoxy(x_value - 2, y_value);
				textcolor(RED1, GRAY2);
				printf(STAR);
			}
			else {
				gotoxy(x_value - 2, y_value);
				textcolor(RED1, GRAY2);
				printf(SQUARE_B);
			}
			break;
		}

		case TAB: {
			if (map4[y_value - 2][(x_value - 7) / 2] == 0 && map3[y_value - 2][(x_value - 7) / 2] == 0) {
				map4[y_value - 2][(x_value - 7) / 2]++;
				gotoxy(x_value - 2, y_value);
				textcolor(BLUE1, GRAY2);
				printf(FLAG);
				num_mine_flag--;
			}
			else if (map4[y_value - 2][(x_value - 7) / 2] == 1) {
				map4[y_value - 2][(x_value - 7) / 2]--;
				gotoxy(x_value - 2, y_value);
				textcolor(RED1, GRAY2);
				printf("  ");
				num_mine_flag++;
			}
		}
		}
	}
}

int show_mine() {
	gotoxy(0, 2);
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (j == 0) {
				textcolor(WHITE, WHITE);
				printf("     ");
			}
			textcolor(GRAY1, GRAY2);
			if (map2[i][j] == 10) {
				textcolor(RED1, GRAY2);
				printf(STAR);
			}
			else
				printf(SQUARE);
			//printf("%d ", map2[i][j]);
		}
		printf("\n");
	}
}

int show_number() {
	gotoxy(0, 2);
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (j == 0) {
				textcolor(WHITE, WHITE);
				printf("     ");
			}
			textcolor(GRAY1, GRAY2);

			if (map2[i][j] > 9) {
				textcolor(RED1, GRAY2);
				printf(STAR);
			}
			else if (map2[i][j] == 1) {
				textcolor(RED1, GRAY2);
				printf(ONE);
			}
			else if (map2[i][j] == 2) {
				textcolor(RED1, GRAY2);
				printf(TWO);
			}
			else if (map2[i][j] == 3) {
				textcolor(RED1, GRAY2);
				printf(THREE);
			}
			else if (map2[i][j] == 4) {
				textcolor(RED1, GRAY2);
				printf(FOUR);
			}
			else if (map2[i][j] == 5) {
				textcolor(RED1, GRAY2);
				printf(FIVE);
			}
			else if (map2[i][j] == 6) {
				textcolor(RED1, GRAY2);
				printf(SIX);
			}
			else if (map2[i][j] == 7) {
				textcolor(RED1, GRAY2);
				printf(SEVEN);
			}
			else if (map2[i][j] == 8) {
				textcolor(RED1, GRAY2);
				printf(EIGHT);
			}
			else
				printf(SQUARE_B);
			//printf("%d ", map2[i][j]);
		}
		printf("\n");
	}
}

int main() {
	textcolor(WHITE, WHITE);
	cls(WHITE, WHITE);
	game_success1();
	gotoxy(0, 1);
	start_first();
	menu_select();
	Sleep(1500);
	put_mine();
	put_number();
	map_control(7, 2);
}