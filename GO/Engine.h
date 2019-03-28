#ifndef	_ENGINE_H_
#define	_ENGINE_H_

#include "Define.h"

bool	SearchMove(SMove*	pMove);     //若pass，返回false，否则返回true以及招法
bool	MonteCarlo(SMove*	pMove);     //蒙特卡罗生成招法
bool	GetGoMove(SMove*	pMove);	    //普通的产生招法。若pass，返回false，否则返回true以及招法
int     GetScore(int x,int y);
int     clean(SMove pMove, char color, char temp_board[][GRID_NUM]);


#endif