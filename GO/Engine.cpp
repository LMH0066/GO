#ifndef	_ENGINE_CPP_
#define	_ENGINE_CPP_

#include "Engine.h"
#include "time.h"

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