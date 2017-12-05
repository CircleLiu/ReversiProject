#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 16
#define BLACK 1
#define WHITE 2
char g_mine = BLACK;//己方默认值
char g_opponent = WHITE;//对手默认值

void print_board(char board[][SIZE]);//打印棋盘（调试用）
void initialize_board(char board[][SIZE]);//初始化棋盘
int valid_moves(char board[][SIZE], bool moves[][SIZE],char field);//判断有效的位置，返回行动力
void reverse(char board[][SIZE], unsigned row, unsigned col, char field);//落子并翻转

int main()
{
	char board[SIZE][SIZE] = { 0 };
	bool moves[SIZE][SIZE] = { false };//可以落子的位置
	char input[6] = { 0 };
	scanf_s("%s", input, 6);
	if (!strcmp(input, "START"))
	{
		int field = 0;
		scanf_s("%d", &field);
		fflush(stdin);
		initialize_board(board);
		if (field != g_mine)//设置己方和对手
		{
			g_mine = WHITE;
			g_opponent = BLACK;
		}
	}
	printf("OK\n");
	fflush(stdout);

	while (true)
	{
		fflush(stdin);
		for (int i = 0; i < 6; i++)
			input[i] = 0;
		scanf_s("%s", input, 6);

		//收到PLACE指令时
		if (!strcmp(input, "PLACE"))
		{
			int row, col;
			scanf_s("%d %d", &row, &col);
			reverse(board, row, col, g_opponent);
			//valid_moves(board, moves, g_mine);
		}
		//收到TURN指令时
		else if (!strcmp(input, "TURN"))
		{
			int i, j;
			valid_moves(board, moves, g_mine);
			for (i = 0; i < SIZE; i++)
			{
				for (j = 0; j < SIZE; j++)
				{
					if (moves[i][j] != 0)
						goto jump_out;//此处goto用于跳出多层循环
				}
			}
		jump_out:
			reverse(board, i, j, g_mine);
			printf("%d %d\n", i, j);
			fflush(stdout);
		}
		//收到END指令时
		else if (!strcmp(input, "END"))
			break;
	}

	system("pause");
	return 0;
}

void print_board(char board[][SIZE])
{
	printf("mine:%d   opponent:%d\n", g_mine, g_opponent);
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
			printf("%d ", board[i][j]);
		printf("\n");
	}
}

void initialize_board(char board[][SIZE])
{
	board[SIZE/2-1][SIZE/2] = BLACK;
	board[SIZE/2][SIZE/2-1] = BLACK;
	board[SIZE/2-1][SIZE/2-1] = WHITE;
	board[SIZE/2][SIZE/2] = WHITE;
}

int valid_moves(char board[][SIZE], bool moves[][SIZE], char field)
{
	char opponent = (field == g_mine) ? g_opponent : g_mine;//设置执行此函数时的对手

	for (int i = 0; i < SIZE; i++)//move数组初始化
	{
		for (int j = 0; j < SIZE; j++)
		{
			moves[i][j] = false;
		}
	}

	unsigned row = 0;//要判断的位置
	unsigned col = 0;
	int d_row = 0;//行方向控制
	int d_col = 0;//列方向控制
	unsigned probe_row = 0;//探针
	unsigned probe_col = 0;
	int mobility = 0;//行动力

	for (row = 0; row < SIZE; row++)
	{
		for (col = 0; col < SIZE; col++)
		{
			if (board[row][col] == 0)//跳过已被占的位置
			{
				//检查8个方向
				for (d_row = -1; d_row <= 1; d_row++)
				{
					for (d_col = -1; d_col <= 1; d_col++)
					{
						if (row + d_row >= SIZE || col + d_col >= SIZE || (d_row == 0 && d_col == 0))
							continue;//防止越界，跳过自己
						if (board[row + d_row][col + d_col] == opponent)
						{
							probe_row = row + d_row;
							probe_col = col + d_col;//转到对手
							while (probe_row + d_row < SIZE && probe_col + d_col < SIZE && board[probe_row + d_row][probe_col + d_col] != 0)//该方向下一个未越界且非空
							{
								probe_row += d_row;
								probe_col += d_col;//转到这位
								if (board[probe_row][probe_col] == field)//是己方，否则是对手
								{
									if (moves[row][col] == false)
									{
										moves[row][col] = true;
										mobility++;
									}
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	return mobility;
}

void reverse(char board[][SIZE], unsigned row, unsigned col, char field)//落子并翻转
{
	board[row][col] = field;//落子

	char opponent = (field == g_mine) ? g_opponent : g_mine;//设置执行此函数时的对手

	int d_row = 0;//行方向控制
	int d_col = 0;//列方向控制
	unsigned probe_row = 0;//探针
	unsigned probe_col = 0;

	for (d_row = -1; d_row <= 1; d_row++)
	{
		for (d_col = -1; d_col <= 1; d_col++)
		{
			if (row + d_row >= SIZE || col + d_col >= SIZE || (d_row == 0 && d_col == 0))
				continue;//防止越界，跳过自己
			if (board[row + d_row][col + d_col] == opponent)
			{
				probe_row = row + d_row;
				probe_col = col + d_col;//转到对手
				while (probe_row + d_row < SIZE && probe_col + d_col < SIZE && board[probe_row + d_row][probe_col + d_col] != 0)//该方向下一个未越界且非空
				{
					probe_row += d_row;
					probe_col += d_col;//转到这位
					if (board[probe_row][probe_col] == field)//是己方，否则是对手
					{
						while (board[probe_row -= d_row][probe_col -= d_col] == opponent)//倒回去翻转，翻到自己为止
						{
							board[probe_row][probe_col] = field;
						}
						break;
					}
				}
			}
		}
	}
}