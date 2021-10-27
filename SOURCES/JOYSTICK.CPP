#include        "c_extern.h"

//***************************************************************************
U32     MakeMask(U32 val);

#pragma aux MakeMask =          \
"       bsf     ecx, eax"       \
"       mov     eax, 1"         \
"       shl     eax, cl"        \
        parm caller     [eax]   \
        modify          [ecx]

//***************************************************************************

static  U32             JoyThere        = 0     ;
static  U32             JoyUsed         = -1    ;
static  U32             JoyHasPov       = 0     ;
static  S32             JoyRange[16][6][2]      ;
static  JOYINFOEX       JoyInfoEx               ;

//***************************************************************************
void    DetectJoys()
{
        U32             i, j, n, k      ;
        JOYCAPS         caps            ;

        n = min(joyGetNumDevs(), 16) 	;

        JoyInfoEx.dwSize = sizeof(JoyInfoEx)    ;
        JoyInfoEx.dwFlags = JOY_RETURNALL       ;

        for(i=0, j=1; i<n; i++, j<<=1)
        {
                if(JoyThere&j)
                {
                        continue        ;
                }

                for(k=1000; k>0; k--)
                {
                        if(joyGetPosEx(JOYSTICKID1+i, &JoyInfoEx) != JOYERR_NOERROR)
                        {
                                k = 0   ;
                                break   ;
                        }
                        if(!JoyInfoEx.dwButtons)
                        {
                                break   ;
                        }
                }
                if(!k)
                {
                        continue        ;
                }

                if(joyGetDevCaps(JOYSTICKID1+i, &caps, sizeof(JOYCAPS)) != JOYERR_NOERROR)
                {
                        continue        ;
                }

                JoyThere |= j ;

                if(caps.wCaps&JOYCAPS_HASPOV)
                {
                        JoyHasPov |= j  ;
                }

                JoyRange[i][0][0] = (caps.wXmax + caps.wXmin)/2 ;
                JoyRange[i][0][1] = (caps.wXmax - caps.wXmin)/3 ;

                JoyRange[i][1][0] = (caps.wYmax + caps.wYmin)/2 ;
                JoyRange[i][1][1] = (caps.wYmax - caps.wYmin)/3 ;

                JoyRange[i][2][0] = (caps.wZmax + caps.wZmin)/2 ;
                JoyRange[i][2][1] = (caps.wZmax - caps.wZmin)/3 ;

                JoyRange[i][3][0] = (caps.wRmax + caps.wRmin)/2 ;
                JoyRange[i][3][1] = (caps.wRmax - caps.wRmin)/3 ;

                JoyRange[i][4][0] = (caps.wUmax + caps.wUmin)/2 ;
                JoyRange[i][4][1] = (caps.wUmax - caps.wUmin)/3 ;

                JoyRange[i][5][0] = (caps.wVmax + caps.wVmin)/2 ;
                JoyRange[i][5][1] = (caps.wVmax - caps.wVmin)/3 ;
        }
}

//***************************************************************************
static inline U32 JoyMakeBitfield(U32 n, U32 *val, S32 pov)
{
        S32     res, i, j       ;
        S32     temp            ;

        res     = 0 ;

        for(i=0, j=1; i<6; i++, j<<=2)
        {
                temp    = val[i] - JoyRange[n][i][0];

                if(temp > JoyRange[n][i][1])
                {
                        res |= j;
                }
                else if(-temp > JoyRange[n][i][1])
                {
                        res |= j*2;
                }
        }

        switch(pov)
        {
        case JOY_POVRIGHT:
                res |= 1<<12 ;
                break ;
        case JOY_POVLEFT:
                res |= 1<<13 ;
                break ;
        case JOY_POVBACKWARD:
                res |= 1<<14 ;
                break ;
        case JOY_POVFORWARD:
                res |= 1<<15 ;
                break ;
        }

        return res ;
}

//***************************************************************************
void    GetJoys(U32 *bitfield)
{
        S32     i, j;

        if(!JoyUsed)    return  ;

        for(i=0, j=1; i < 16; i++, j<<=1)
        {
                if(!(JoyUsed&JoyThere&j))	continue ;

                // read buttons
                JoyInfoEx.dwSize = sizeof(JoyInfoEx) ;
                JoyInfoEx.dwFlags = JOY_RETURNALL ;
                if(joyGetPosEx(JOYSTICKID1+i, &JoyInfoEx) == JOYERR_NOERROR)
                {
                        if(!(JoyHasPov&j))
                        {
                                JoyInfoEx.dwPOV = -1    ;
                        }
                        bitfield[i*2]  = JoyInfoEx.dwButtons ;
                        bitfield[i*2+1]= JoyMakeBitfield(i, &JoyInfoEx.dwXpos, JoyInfoEx.dwPOV);
                }
                else
                {
                        bitfield[i*2]  = 0 ;
                        bitfield[i*2+1]= 0 ;
                }
        }
}

//***************************************************************************
