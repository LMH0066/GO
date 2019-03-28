#ifndef _SETSCORE_CPP_
#define _SETSCORE_CPP_

#include "Engine.h"

#define Score_1 100
#define Score_2	40

#define	Score_Factor 20

SGroup	group[GRID_COUNT];						//群内的信息
int	boardGroup[GRID_NUM][GRID_NUM] = {0};		//记录相应的子在第几个群里面

int CountGroup(int x,int y,int index,int dir = 0)
{
	float eyes = 0;

	if (group[index].count > 100)
	{
		eyes = 0;
	}
	if (boardGroup[x][y])
	{
		return 0;
	}
	
	if (board[x][y] != WHITE && board[x][y] != BLACK )
		return 0;

	boardGroup[x][y] = index;
	group[index].count += 1;

	//左方向
	if (board[x-1][y] == board[x][y] && dir != 3)
	{
		CountGroup(x-1,y,index,1);
	}else if (board[x-1][y] == NOSTONE)
	{
		//boardGroup[x-1][y] = ;
		eyes += 1.0/4;
	}

	//有方向
	if (board[x+1][y] == board[x][y] && dir != 1)
	{
		CountGroup(x+1,y,index,3);
	}else if (board[x+1][y] == NOSTONE)
	{
		eyes += 1.0/4;
	}

	//上方向
	if (board[x][y-1] == board[x][y] && dir != 2)
	{
		CountGroup(x,y-1,index,4);
	}else if (board[x][y-1] == NOSTONE)
	{
		eyes += 1.0/4;
	}

	//下方向
	if (board[x][y+1] == board[x][y] && dir != 4)
	{
		CountGroup(x,y+1,index,2);
	}else if (board[x][y+1] == NOSTONE)
	{
		eyes += 1.0/4;
	}

	group[index].eyes += eyes;

	return index;
}

bool	GetGoMove(SMove*	pMove)
{

	//if (player[chessColor ^ 3].know == 0 && player[chessColor].total == 0)
	//{
	//	pMove->score = 0;
	//	pMove->x = rand() % 7 + 2;
	//	pMove->y = rand() % 7 + 2;
	//	return true;
	//}

	int score = 0;

	SMove preMove;

	//int	boardScore[GRID_NUM][GRID_NUM] = {0};


	//对所有的有点位置评分
	int count = 0;
	int tempI,tempJ;
	int groupIndex = 0;

	int boardScore[GRID_NUM][GRID_NUM] = {0};
	
	memset(group,0,sizeof(group));
	memset(boardGroup,0,sizeof(boardGroup));

	preMove.x = pMove->x;
	preMove.y = pMove->y;
	preMove.score = pMove->score;

	for(int i = 1 ; i < 10 ; i++)
	{
		for(int j = 1 ; j < 10 ; j++)
		{
			if (board[i][j] != NOSTONE && boardGroup[i][j] == 0)
			{
				CountGroup(i,j,++groupIndex);				
			}
		}
	}


	pMove->score = 0;
	pMove->x = 0;
	pMove->y = 0;
	for(int i = 1 ; i < 10 ; i++)
	{
		for(int j = 1 ; j < 10 ; j++)
		{
			if (board[i][j] == NOSTONE)
			{
				//计算每一点的分数
				if (preMove.x == i && preMove.y == j)
				{
					score = -10;
				}else
				{
					score = GetScore(i,j);
				}
								
				boardScore[i][j] = score;

				//寻找最大值
				if (pMove->score < score)
				{
					pMove->score = score;
					pMove->x = i;
					pMove->y = j;
				}
			}
		}
	}

	if (pMove->score > 0)
	{
		printf("ComputeGet\nScore: %d\n",pMove->score);
		return true;
	} else
	{
		//随机产生招法
		int num = 0;
		int index ;
		for(int i = 1 ; i < 10 ; i++)
		{
			for(int j = 1 ; j < 10 ; j++)
			{
				if (board[i][j] == NOSTONE && boardScore[i][j] >= 0)
				{
					num++;
				}
			}
		}
		if (num == 0)
		{
			return false;
		}
		index = rand() % num;

		for(int i = 1 ; i < 10 ; i++)
		{
			for(int j = 1 ; j < 10 ; j++)
			{
				if (board[i][j] == NOSTONE && boardScore[i][j] >= 0)
				{
					index--;
					if (index < 0)
					{
						pMove->x = i;
						pMove->y = j;
						printf("RandGet!\n");
						return true;
					}
				}
			}
		}

		return false;
	}
}

int GetScore(int x,int y)
{
	//分数
	int score = 0;

	//我的棋子数量
	int selfG = 0;
	//对手棋子数量
	int oppG = 0;
	int selfGIndex[4];
	int oppGIndex[4];
	
	float eyes = 0;
	//临时位置XY
	int tempX, tempY;
	//边缘数量
	int edge = 0;

	//如果是自己的棋子
	if (board[x-1][y] == chessColor)
	{
		selfGIndex[selfG] = boardGroup[x-1][y];
		selfG++;
	} 
	//如果是对手的棋子
	else if (board[x-1][y] == (chessColor ^ 3))
	{
		oppGIndex[oppG] = boardGroup[x-1][y];
		oppG++;
	} 
	//如果没有棋子
	else if (board[x-1][y] == NOSTONE)
	{
		tempX = x-1;
		tempY = y;
		if (board[tempX-1][tempY] != chessColor)
		{
			eyes += 0.25;
		}
		if (board[tempX+1][tempY] != chessColor)
		{
			eyes += 0.25;
		}
		if (board[tempX][tempY-1] != chessColor)
		{
			eyes += 0.25;
		}
		if (board[tempX][tempY+1] != chessColor)
		{
			eyes += 0.25;
		}
	} 
	//如果是边界
	else if (board[x-1][y] == BORDER)
	{
		edge++;
	}

	if (board[x][y-1] == chessColor)
	{
		selfGIndex[selfG] = boardGroup[x][y-1];
		selfG++;
	}else if (board[x][y-1] == (chessColor ^ 3))
	{
		oppGIndex[oppG] = boardGroup[x][y-1];
		oppG++;
	}else if (board[x][y-1] == NOSTONE)
	{
		tempX = x;
		tempY = y-1;
		if (board[tempX-1][tempY] != chessColor)
		{
			eyes += 0.25;
		}
		if (board[tempX+1][tempY] != chessColor)
		{
			eyes += 0.25;
		}
		if (board[tempX][tempY-1] != chessColor)
		{
			eyes += 0.25;
		}
		if (board[tempX][tempY+1] != chessColor)
		{
			eyes += 0.25;
		}
	}else if (board[x][y-1] == BORDER)
	{
		edge++;
	}

	if (board[x+1][y] == chessColor)
	{
		selfGIndex[selfG] = boardGroup[x+1][y];
		selfG++;
	}else if (board[x+1][y] == (chessColor ^ 3))
	{
		oppGIndex[oppG] = boardGroup[x+1][y];
		oppG++;
	}else if (board[x+1][y] == NOSTONE)
	{
		tempX = x+1;
		tempY = y;
		if (board[tempX-1][tempY] != chessColor)
		{
			eyes += 0.25;
		}
		if (board[tempX+1][tempY] != chessColor)
		{
			eyes += 0.25;
		}
		if (board[tempX][tempY-1] != chessColor)
		{
			eyes += 0.25;
		}
		if (board[tempX][tempY+1] != chessColor)
		{
			eyes += 0.25;
		}
	}else if (board[x+1][y] == BORDER)
	{
		edge++;
	}

	if (board[x][y+1] == chessColor)
	{
		selfGIndex[selfG] = boardGroup[x][y+1];
		selfG++;
	}else if (board[x][y+1] == (chessColor ^ 3))
	{
		oppGIndex[oppG] = boardGroup[x][y+1];
		oppG++;
	}else if (board[x][y+1] == NOSTONE)
	{
		tempX = x;
		tempY = y+1;
		if (board[tempX-1][tempY] != chessColor)
		{
			eyes += 0.25;
		}
		if (board[tempX+1][tempY] != chessColor)
		{
			eyes += 0.25;
		}
		if (board[tempX][tempY-1] != chessColor)
		{
			eyes += 0.25;
		}
		if (board[tempX][tempY+1] != chessColor)
		{
			eyes += 0.25;
		}
	}else if (board[x][y+1] == BORDER)
	{
		edge++;
	}

	switch(selfG)
	{
	case 0:
		//周围没有自己的子
		switch(oppG)
		{
		case 0:
			break;
		case 1:
			if (group[oppGIndex[0]].count < 2)
			{
				score = 4 - group[oppGIndex[0]].eyes;
			}
			break;
		case 2:
			if (edge == 2)
			{
				score = -10;
			}
			break;
		case 3:
			if (edge == 1)
			{
				score = -10;
			}
			break;
		case 4:
			score = -10;
			break;
		}
		break;
	case 1:
		//周围有一个自己的群
		switch(oppG)
		{
		case 0:
			score =  group[selfGIndex[0]].count * group[selfGIndex[0]].eyes ;
			break;
		case 1:
			score =  group[selfGIndex[0]].count * group[selfGIndex[0]].eyes 
						- group[oppGIndex[0]].count * group[oppGIndex[0]].eyes ;
			break;
		case 2:
			score =  group[selfGIndex[0]].count * group[selfGIndex[0]].eyes 
						- group[oppGIndex[0]].count * group[oppGIndex[0]].eyes 
						- group[oppGIndex[1]].count * group[oppGIndex[1]].eyes;
			break;
		case 3:
			break;
		}
		break;
	case 2:
		//周围由两个自己的群
		switch(oppG)
		{
		case 0:
			if (selfGIndex[0] == selfGIndex[1] && (board[x-1][y] == board[x+1][y] || board[x][y-1] == board[x][y+1]))
			{
				score =  group[selfGIndex[0]].count * group[selfGIndex[0]].eyes 
						+ group[selfGIndex[1]].count * group[selfGIndex[1]].eyes + Score_1;						
			}else if (edge == 2)
			{
				score = -10;
			}else  if (board[x-1][y] == board[x+1][y] || board[x][y-1] == board[x][y+1] || selfGIndex[0] != selfGIndex[1])
			{
				score =  group[selfGIndex[0]].count * group[selfGIndex[0]].eyes 
						+ group[selfGIndex[1]].count * group[selfGIndex[1]].eyes;
			}
			break;
		case 1:
			if (selfGIndex[0] != selfGIndex[1] || board[x-1][y] == board[x+1][y] || board[x][y-1] == board[x][y+1])
			{
				score =  group[selfGIndex[0]].count * group[selfGIndex[0]].eyes 
							+ group[selfGIndex[1]].count * group[selfGIndex[1]].eyes 
							+ group[oppGIndex[0]].count * group[oppGIndex[0]].eyes;
			}else if (selfGIndex[0] == selfGIndex[1] && (board[x-1][y] == board[x+1][y] || board[x][y-1] == board[x][y+1]))
			{
				score =  group[selfGIndex[0]].count * group[selfGIndex[0]].eyes 
						+ group[selfGIndex[1]].count * group[selfGIndex[1]].eyes + Score_1;						
			}else if (board[x-1][y] == board[x+1][y] || board[x][y-1] == board[x][y+1] || selfGIndex[0] != selfGIndex[1])
			{
				score =  group[selfGIndex[0]].count * group[selfGIndex[0]].eyes 
							+ group[selfGIndex[1]].count * group[selfGIndex[1]].eyes 
							- group[oppGIndex[0]].count * group[oppGIndex[0]].eyes;
			}
			break;
		case 2:
			score =  group[selfGIndex[0]].count * group[selfGIndex[0]].eyes 
							+ group[selfGIndex[1]].count * group[selfGIndex[1]].eyes 
							- group[oppGIndex[0]].count * group[oppGIndex[0]].eyes
							- group[oppGIndex[1]].count * group[oppGIndex[1]].eyes;
			break;
		}
		break;
	case 3:
		//周围有三个自己的群
		switch(oppG)
		{
		case 0:
			if (selfGIndex[0] != selfGIndex[1] ||selfGIndex[0] != selfGIndex[2] ||selfGIndex[2] != selfGIndex[1] )
			{
				score =  group[selfGIndex[0]].count * group[selfGIndex[0]].eyes 
							+ group[selfGIndex[1]].count * group[selfGIndex[1]].eyes 
							+ group[selfGIndex[2]].count * group[selfGIndex[2]].eyes ;
			}
			if (edge == 1)
			{
				score = -10;
			}
			break;
		case 1:
			if (selfGIndex[0] != selfGIndex[1] ||selfGIndex[0] != selfGIndex[2] ||selfGIndex[2] != selfGIndex[1] )
			{
				score =  group[selfGIndex[0]].count * group[selfGIndex[0]].eyes 
							+ group[selfGIndex[1]].count * group[selfGIndex[1]].eyes 
							+ group[selfGIndex[2]].count * group[selfGIndex[2]].eyes 
							+ group[oppGIndex[0]].count * group[oppGIndex[0]].eyes;
			}else
			{
				score =  group[selfGIndex[0]].count * group[selfGIndex[0]].eyes 
					- group[oppGIndex[0]].count * group[oppGIndex[0]].eyes;
			}
			break;
		}
		break;
	case 4:
		score = -10;
		break;
	}

	//if (score > 1000)
	//{
	//	score /= 8;
	//}
	//if (score > 500)
	//{
	//	score /= 2;
	//}

	//提子加分
	for (int i = 0 ; i < oppG ; i++)
	{
		if (group[oppGIndex[i]].eyes  < 2 * 0.25)
		{
			score += group[oppGIndex[i]].count * 80;
		}
	}


	//若不为零的话加上基础分数
	if (score) 
		score += boardBaseScore[x][y] + eyes * 3;

	return score;
}


#endif