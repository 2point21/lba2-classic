//---------------------------------------------------------------------------
#include	<system\adeline.h>
#include	<system\initimer.h>
#include	<3d\move.h>

//---------------------------------------------------------------------------
void	ChangeSpeedMove(MOVE *pmove, S32 speed)
{
	U32	timer, delta	;

	timer = TimerRefHR	;

	delta = timer-pmove->LastTimer ;

	if(delta|pmove->Speed)
	{
		pmove->Acc += (timer-pmove->LastTimer)*pmove->Speed	;
	 	pmove->LastTimer = timer				;
	}

	pmove->Speed = speed	;
}

//---------------------------------------------------------------------------
void	InitMove(MOVE *pmove, S32 speed)
{
	RestartMove(pmove)	;
	pmove->Speed = speed	;
}

//---------------------------------------------------------------------------
void	ChangeSpeedAngleMove(MOVE *pmove, S32 speed, S32 start, S32 end)
{
	ChangeSpeedMove(pmove, AdjustSpeedAngle(speed, start, end)) ;
}

//---------------------------------------------------------------------------
void	InitAngleMove(MOVE *pmove, S32 speed, S32 start, S32 end)
{
	InitMove(pmove, AdjustSpeedAngle(speed, start, end))	;
}

//---------------------------------------------------------------------------
void	ChangeSpeedBoundAngleMove(BOUND_MOVE *pbmove, S32 speed, S32 end)
{
	ChangeSpeedMove(&(pbmove->Move), AdjustSpeedAngle(speed, pbmove->Cur, end))	;
	pbmove->End = end&4095	;
}

//---------------------------------------------------------------------------
void	InitBoundAngleMove(BOUND_MOVE *pbmove, S32 speed, S32 start, S32 end)
{
	InitAngleMove(&(pbmove->Move), speed, start, end)	;
	pbmove->Cur = start&4095;
	pbmove->End = end&4095	;
}

//---------------------------------------------------------------------------
void	InitBoundMove(BOUND_MOVE *pbmove, S32 speed, S32 start, S32 end)
{
	if(start==end)
	{
		speed = 0	;
	}
	else if(start>end)
	{
		speed = -speed	;
	}

	InitMove(&(pbmove->Move), speed);

	pbmove->Cur = start	;
	pbmove->End = end	;
}

//---------------------------------------------------------------------------
S32	GetDeltaMove(MOVE *pmove)
{
	U32	timer, delta	;

	timer = TimerRefHR	;

	delta = timer-pmove->LastTimer ;

	if(delta|pmove->Speed)
	{
		pmove->Acc += (timer-pmove->LastTimer)*pmove->Speed	;
	 	pmove->LastTimer = timer				;
		return GetDeltaAccMove(&(pmove->Acc))			;
	}
	else
	{
		return 0 ;
	}
}

//---------------------------------------------------------------------------
S32	GetBoundMove(BOUND_MOVE *pbmove)
{
	S32	temp, cur;

	temp = GetDeltaMove(&(pbmove->Move))	;
	cur  = pbmove->Cur	;

	if(temp)
	{
		S32	end	;

		end = pbmove->End	;

		cur += temp		;

		if(pbmove->Move.Speed>0)
		{
			if(cur >= end)
			{
				goto over	;
			}
		}
		else
		{
			if(cur <= end)
			{
over:				cur = end		;
				pbmove->Move.Speed = 0	;
			}
		}

		pbmove->Cur = cur	;
	}

	return cur	;
}

//---------------------------------------------------------------------------
S32	GetBoundAngleMove(BOUND_MOVE *pbmove)
{
	S32	temp, cur;

	temp = GetDeltaMove(&(pbmove->Move))	;
	cur  = pbmove->Cur	;

	if(temp)
	{
		S32	end	;

		end = pbmove->End	;

		if(pbmove->Move.Speed>0)
		{
			if(cur>end)
			{
				end += 4096	;
			}

			cur += temp	;

			if(cur >= end)
			{
				goto over ;
			}
		}
		else
		{
			if(cur<end)
			{
				end -= 4096	;
			}

			cur += temp	;

			if(cur <= end)
			{
over:				cur = end		;
				pbmove->Move.Speed = 0	;
			}
		}

		cur &= 4095		;
		pbmove->Cur = cur	;
	}

	return cur	;
}

//---------------------------------------------------------------------------
