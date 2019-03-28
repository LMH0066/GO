#ifndef	_ENGINE_CPP_
#define	_ENGINE_CPP_

#include "Engine.h"
#include "time.h"
//若蒙特卡罗方法得到的估值大于置信值believe_point即2000，则选择蒙特卡罗的最佳走法，否则调用GetGomove利用条件估值重新走子
bool	SearchMove(SMove*	pMove)
{
	int start = clock();
	if (player[chessColor].know > 3 && MonteCarlo(pMove)){
		int end = clock();
		printf("ms: %d\n",end-start);
		return true;
	}
	return GetGoMove(pMove);
}


#endif
