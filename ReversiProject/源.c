#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define SIZE 16
#define BLACK 1
#define WHITE 2
char g_mine = BLACK;//己方默认值
char g_opponent = WHITE;//对手默认值
int g_counter = 0;

void print_board(char board[][SIZE]);//打印棋盘（调试用）
void initialize_board(char board[][SIZE]);//初始化棋盘
int valid_moves(char board[][SIZE], bool moves[][SIZE],char field);//判断有效的位置，返回行动力
void reverse(char board[][SIZE], unsigned row, unsigned col, char field);//落子并翻转
int chess_value(int row, int col);//从估值表得到该位置的价值
int chess_num(char board[][SIZE], char field);//获取当前棋盘上某一方棋子数量
double variance(char board[][SIZE], char field);//用标准差判断棋子的离散程度
void brain(char board[][SIZE], bool moves[][SIZE], int*i, int*j);

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
			g_counter++;
			//valid_moves(board, moves, g_mine);
		}
		//收到TURN指令时
		else if (!strcmp(input, "TURN"))
		{
			int i, j;
			valid_moves(board, moves, g_mine);
			brain(board, moves, &i, &j);
			reverse(board, i, j, g_mine);
			g_counter++;
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
	g_counter = 4;
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
						if (row + d_row < SIZE && col + d_col < SIZE && (d_row != 0 || d_col != 0))//防止越界，跳过自己
						{
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
			if (row + d_row < SIZE && col + d_col < SIZE && (d_row != 0 || d_col != 0))//防止越界，跳过自己
			{
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
}

int chess_num(char board[][SIZE], char field)
{
	int count = 0;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (board[i][j] == field)
				count++;
		}
	}
	return count;
}

int chess_value(int row, int col)
{
	int value_board[SIZE][SIZE] =
	{
		{ 35, 3,25,22,21,21,21,20,20,21,21,21,22,25, 3,35 },
		{ 3 , 2,13,12,11,10,10, 9, 9,10,10,11,12,13, 2, 3 },
		{ 25,12,18,16,16,16,16,16,16,16,16,16,16,18,12,25 },
		{ 22,12,16,15,15,15,15,15,15,15,15,15,15,16,12,22 },
		{ 21,11,16,15,15,15,15,15,15,15,15,15,15,16,11,21 },
		{ 21,10,16,15,15,14,14,14,14,14,14,15,15,16,10,21 },
		{ 21,10,16,15,15,14,14,14,14,14,14,15,15,16,10,21 },
		{ 20, 9,16,15,15,14,14,14,14,14,14,15,15,16, 9,20 },
		{ 20, 9,16,15,15,14,14,14,14,14,14,15,15,16, 9,20 },
		{ 21,10,16,15,15,14,14,14,14,14,14,15,15,16,10,21 },
		{ 21,10,16,15,15,14,14,14,14,14,14,15,15,16,10,21 },
		{ 21,11,16,15,15,15,15,15,15,15,15,15,15,16,11,21 },
		{ 22,12,16,15,15,15,15,15,15,15,15,15,15,16,12,22 },
		{ 25,12,18,16,16,16,16,16,16,16,16,16,16,18,12,25 },
		{ 3 , 2,13,12,11,10,10, 9, 9,10,10,11,12,13, 2, 3 },
		{ 35, 3,25,22,21,21,21,20,20,21,21,21,22,25, 3,35 },
	};
	return value_board[row][col];
}

double variance(char board[][SIZE], char field)
{
	double v_row = 0, v_col = 0;
	int n = chess_num(board, field);
	int sum_row = 0, sum_col = 0;
	for (int row = 0; row < SIZE; row++)
	{
		for (int col = 0; col < SIZE; col++)
		{
			if (board[row][col] == field)
			{
				sum_row += row;
				sum_col += col;
			}
		}
	}
	double ave_row = (double)sum_row / n, ave_col = (double)sum_col / n;
	double square_sum_row = 0, square_sum_col = 0;
	for (int row = 0; row < SIZE; row++)
	{
		for (int col = 0; col < SIZE; col++)
		{
			if (board[row][col] == field)
			{
				square_sum_row += pow(row - ave_row, 2);
				square_sum_col += pow(col - ave_col, 2);
			}
		}
	}
	v_row = square_sum_row / n;
	v_col = square_sum_col / n;
	return sqrt(v_row + v_col);
}

void brain(char board[][SIZE], bool moves[][SIZE], int*i, int*j)
{
	double score = 0;
	int row = 0, col = 0;
	double best_score = -10000;
	int best_row = 0, best_col = 0;

	for (row = 0; row < SIZE; row++)
	{
		for (col = 0; col < SIZE; col++)
		{
			if (moves[row][col])
			{
				char temp_board[SIZE][SIZE];
				bool temp_moves[SIZE][SIZE];
				memcpy_s(temp_board, SIZE*SIZE, board, SIZE*SIZE);
				memcpy_s(temp_moves, SIZE*SIZE, moves, SIZE*SIZE);
				reverse(temp_board, row, col, g_mine);
				double weight1, weight2, weight3, weight4;//权重值
				if (g_counter < 40)
				{
					weight1 = 0.5;
					weight2 = 0.5;
					weight3 = 1.0;
					weight4 = 1.0;
				}
				else if (g_counter < 250)
				{
					weight1 = 1.5;
					weight2 = 0.5;
					weight3 = 1.5;
					weight4 = 1.0;
				}
				else
				{
					weight1 = 0.0;
					weight2 = 1.0;
					weight3 = 1.0;
					weight4 = 0.0;
				}
				score = weight1*(valid_moves(temp_board, temp_moves, g_mine) - valid_moves(temp_board, temp_moves, g_opponent)) \
					  + weight2*(chess_num(temp_board, g_mine) - chess_num(temp_board, g_opponent))\
					  + weight3*chess_value(row, col)\
					  + weight4*(variance(temp_board, g_opponent) - variance(temp_board, g_mine));
				if (score >= best_score)
				{
					best_score = score;
					best_row = row;
					best_col = col;
				}
			}
		}
	}
	*i = best_row;
	*j = best_col;
}
