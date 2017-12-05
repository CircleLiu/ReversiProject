#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 16
#define BLACK 1
#define WHITE 2
char g_mine = BLACK;//����Ĭ��ֵ
char g_opponent = WHITE;//����Ĭ��ֵ

void print_board(char board[][SIZE]);//��ӡ���̣������ã�
void initialize_board(char board[][SIZE]);//��ʼ������
int valid_moves(char board[][SIZE], bool moves[][SIZE],char field);//�ж���Ч��λ�ã������ж���
void reverse(char board[][SIZE], unsigned row, unsigned col, char field);//���Ӳ���ת

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
			//valid_moves(board, moves, g_mine);
		}
		//�յ�TURNָ��ʱ
		else if (!strcmp(input, "TURN"))
		{
			int i, j;
			valid_moves(board, moves, g_mine);
			for (i = 0; i < SIZE; i++)
			{
				for (j = 0; j < SIZE; j++)
				{
					if (moves[i][j] != 0)
						goto jump_out;//�˴�goto�����������ѭ��
				}
			}
		jump_out:
			reverse(board, i, j, g_mine);
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