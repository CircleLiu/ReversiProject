#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 16
#define BLACK 1
#define WHITE 2
char g_mine = BLACK;//����Ĭ��ֵ
char g_opponent = WHITE;//����Ĭ��ֵ
int g_counter = 0;

void print_board(char board[][SIZE]);//��ӡ���̣������ã�
void initialize_board(char board[][SIZE]);//��ʼ������
int valid_moves(char board[][SIZE], bool moves[][SIZE],char field);//�ж���Ч��λ�ã������ж���
void reverse(char board[][SIZE], unsigned row, unsigned col, char field);//���Ӳ���ת
void brain(char board[][SIZE], bool moves[][SIZE], int*i, int*j);
int chess_value(int row, int col);//�ӹ�ֵ��õ���λ�õļ�ֵ
int chess_num(char board[][SIZE], char field);//��ȡ��ǰ������ĳһ����������

int main()
{
	char board[SIZE][SIZE] = { 0 };
	bool moves[SIZE][SIZE] = { false };//�������ӵ�λ��
	char input[6] = { 0 };
	scanf_s("%s", input, 6);
	if (!strcmp(input, "START"))
	{
		int field = 0;
		scanf_s("%d", &field);
		fflush(stdin);
		initialize_board(board);
		if (field != g_mine)//���ü����Ͷ���
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

		//�յ�PLACEָ��ʱ
		if (!strcmp(input, "PLACE"))
		{
			int row, col;
			scanf_s("%d %d", &row, &col);
			reverse(board, row, col, g_opponent);
			g_counter++;
			//valid_moves(board, moves, g_mine);
		}
		//�յ�TURNָ��ʱ
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
		//�յ�ENDָ��ʱ
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
	char opponent = (field == g_mine) ? g_opponent : g_mine;//����ִ�д˺���ʱ�Ķ���

	for (int i = 0; i < SIZE; i++)//move�����ʼ��
	{
		for (int j = 0; j < SIZE; j++)
		{
			moves[i][j] = false;
		}
	}

	unsigned row = 0;//Ҫ�жϵ�λ��
	unsigned col = 0;
	int d_row = 0;//�з������
	int d_col = 0;//�з������
	unsigned probe_row = 0;//̽��
	unsigned probe_col = 0;
	int mobility = 0;//�ж���

	for (row = 0; row < SIZE; row++)
	{
		for (col = 0; col < SIZE; col++)
		{
			if (board[row][col] == 0)//�����ѱ�ռ��λ��
			{
				//���8������
				for (d_row = -1; d_row <= 1; d_row++)
				{
					for (d_col = -1; d_col <= 1; d_col++)
					{
						if (row + d_row >= SIZE || col + d_col >= SIZE || (d_row == 0 && d_col == 0))
							continue;//��ֹԽ�磬�����Լ�
						if (board[row + d_row][col + d_col] == opponent)
						{
							probe_row = row + d_row;
							probe_col = col + d_col;//ת������
							while (probe_row + d_row < SIZE && probe_col + d_col < SIZE && board[probe_row + d_row][probe_col + d_col] != 0)//�÷�����һ��δԽ���ҷǿ�
							{
								probe_row += d_row;
								probe_col += d_col;//ת����λ
								if (board[probe_row][probe_col] == field)//�Ǽ����������Ƕ���
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

void reverse(char board[][SIZE], unsigned row, unsigned col, char field)//���Ӳ���ת
{
	board[row][col] = field;//����

	char opponent = (field == g_mine) ? g_opponent : g_mine;//����ִ�д˺���ʱ�Ķ���

	int d_row = 0;//�з������
	int d_col = 0;//�з������
	unsigned probe_row = 0;//̽��
	unsigned probe_col = 0;

	for (d_row = -1; d_row <= 1; d_row++)
	{
		for (d_col = -1; d_col <= 1; d_col++)
		{
			if (row + d_row >= SIZE || col + d_col >= SIZE || (d_row == 0 && d_col == 0))
				continue;//��ֹԽ�磬�����Լ�
			if (board[row + d_row][col + d_col] == opponent)
			{
				probe_row = row + d_row;
				probe_col = col + d_col;//ת������
				while (probe_row + d_row < SIZE && probe_col + d_col < SIZE && board[probe_row + d_row][probe_col + d_col] != 0)//�÷�����һ��δԽ���ҷǿ�
				{
					probe_row += d_row;
					probe_col += d_col;//ת����λ
					if (board[probe_row][probe_col] == field)//�Ǽ����������Ƕ���
					{
						while (board[probe_row -= d_row][probe_col -= d_col] == opponent)//����ȥ��ת�������Լ�Ϊֹ
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

void brain(char board[][SIZE], bool moves[][SIZE], int*i, int*j)
{
	int score = 0, row = 0, col = 0;
	int best_score = -10000, best_row = 0, best_col = 0;

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
				int weight1, weight2, weight3;//Ȩ��ֵ
				if (g_counter < 50)
				{
					weight1 = 0;
					weight2 = 0;
					weight3 = 1;
				}
				else if (g_counter < 250)
				{
					weight1 = 0;
					weight2 = 0;
					weight3 = 1;
				}
				else
				{
					weight1 = 0;
					weight2 = 1;
					weight3 = 0;
				}
				score = weight1*(valid_moves(temp_board, temp_moves, g_mine) - valid_moves(temp_board, temp_moves, g_opponent)) \
					  + weight2*(chess_num(temp_board, g_mine) - chess_num(temp_board, g_opponent))\
					  + weight3*chess_value(row, col);
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
		{ 25, 7,21,19,18,18,18,17,17,18,18,18,19,21, 7,25 },
		{ 7 , 6,13,12,11,10,10, 9, 9,10,10,11,12,13, 6, 7 },
		{ 21,12,18,16,16,16,16,16,16,16,16,16,16,18,12,21 },
		{ 19,12,16,15,15,15,15,15,15,15,15,15,15,16,12,19 },
		{ 18,11,16,15,15,15,15,15,15,15,15,15,15,16,11,18 },
		{ 18,10,16,15,15,14,14,14,14,14,14,15,15,16,10,18 },
		{ 18,10,16,15,15,14,14,14,14,14,14,15,15,16,10,18 },
		{ 17, 9,16,15,15,14,14,14,14,14,14,15,15,16, 9,17 },
		{ 17, 9,16,15,15,14,14,14,14,14,14,15,15,16, 9,17 },
		{ 18,10,16,15,15,14,14,14,14,14,14,15,15,16,10,18 },
		{ 18,10,16,15,15,14,14,14,14,14,14,15,15,16,10,18 },
		{ 18,11,16,15,15,15,15,15,15,15,15,15,15,16,11,18 },
		{ 19,12,16,15,15,15,15,15,15,15,15,15,15,16,12,19 },
		{ 21,12,18,16,16,16,16,16,16,16,16,16,16,18,12,21 },
		{ 7 , 6,13,12,11,10,10, 9, 9,10,10,11,12,13, 6, 7 },
		{ 25, 7,21,19,18,18,18,17,17,18,18,18,19,21, 7,25 },
	};
	return value_board[row][col];
}