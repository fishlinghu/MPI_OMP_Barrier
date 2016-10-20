#include <omp.h>
#include <stdio.h>
#include <math.h>
#include<stdlib.h>
#include <stdbool.h>
#include<sys/time.h>
#define winner 0
#define loser 1
#define bye 2
#define champion 3
#define dropout 4


struct round_t
{
	int role;
	int vpid;
	int tb_round;
	bool *opponent;
	bool flag;
};
typedef struct round_t round_t;
round_t array[100][100];

void tournament_barrier( int vpid, bool *sense,int num_rounds)
{
	int round = 0;
	while(1)
	{

		if(array[vpid][round].role == loser)
		{
			*( array[vpid][round] ).opponent = *sense;
			while( array[vpid][round].flag != *sense );
			break;
		}

		if( array[vpid][round].role == winner )
		{
			while( array[vpid][round].flag != *sense );
		}
		if( array[vpid][round].role == champion )
		{
			while( array[vpid][round].flag != *sense );
			*( array[vpid][round] ).opponent = *sense;
			break;
		}

		if(round < num_rounds)
		{
			round = round + 1;
		}
	}
	while(1) 
	{
		if( round > 0 )
		{
			round = round - 1;
		}

		if( array[vpid][round].role == winner )
		{
			*( array[vpid][round] ).opponent = *sense;
		}
		if( array[vpid][round].role == dropout )
		{
			break;
		}
	}

	*sense = !*sense;
}