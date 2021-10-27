#include        "c_extern.h"

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifdef  _WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
static  U8      KeyState[256+16*8]   ;

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
S32     ScanToAscii(S32 scan)
{
        S32     res             ;
        char    buf[2]          ;
        char    buf2[4]         ;

        memset(KeyState, 0, sizeof(KeyState))   ;

        // dummy in case of previous dead_key
        ToAscii(scan, 0, KeyState, (LPWORD)buf, 0)      ;

        // translate key
        res = ToAscii(scan, 0, KeyState, (LPWORD)buf, 0);

        // if dead-key, send a 'p'
        if(res==-1)
        {
                res = ToAscii(VkKeyScan('p')&0xFF, 0, KeyState, (LPWORD)buf, 0);
                if(res!=2)      res = 0 ;
        }

        if(!res|(buf[0]<32))    return 0 ;

        buf[1] = 0      ;

        CharToOem(buf, buf2)    ;

        return  toupper(buf2[0]);
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
U32     GetBit(U32 val);

#pragma aux GetBit =            \
"       bsf     eax, eax"       \
        parm    [eax]

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
S32     KeyDown()
{
        S32     i       ;
        S32     found   ;
        U32     *Joys, *OldJoys ;

	DetectJoys()	;
        GetJoys((U32*)(TabKeys+256))    ;
        ManageKeyboard();

        found = 0       ;

        for(i=255; i>=0; i--)
        {
                U8      key     ;

                key = (U8)(~KeyState[i]&0x80);

                KeyState[i] = TabKeys[i];

                TabKeys[i] &= key       ;

                if(     (i>VK_MBUTTON)  &&
                        (i!=VK_EXECUTE) &&
                        (i!=VK_LWIN)    &&
                        (i!=VK_RWIN)    &&
                        !found&&TabKeys[i]      )
                {
                        found = i       ;
                }
        }

        OldJoys = (U32*)&KeyState[256]  ;
        Joys = (U32*)&TabKeys[256]      ;

        for(i=0; i<2*16; i++)
        {
                U32 bits        ;

                bits = ~OldJoys[i]      ;
                OldJoys[i] = Joys[i]    ;
                bits &= Joys[i]         ;
                Joys[i] = bits         	;

                if(!found&&bits)
                {
                        found = 256+i*32+GetBit(bits) ;
                }
        }

        return  found   ;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#else// _WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
typedef struct {
        S32     ScanCode ;
        char    Ascii    ;
       } T_KEY ;

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
T_KEY   ScanUnknown[] = {
        { K_A, 'Q' },
        { K_Z, 'W' },
        { K_E, 'E' },
        { K_R, 'R' },
        { K_T, 'T' },
        { K_Y, 'Y' },
        { K_U, 'U' },
        { K_I, 'I' },
        { K_O, 'O' },
        { K_P, 'P' },
        { K_CHAPEAU, '[' },
        { K_DOLLAR, ']' },
        { K_Q, 'A' },
        { K_S, 'S' },
        { K_D, 'D' },
        { K_F, 'F' },
        { K_G, 'G' },
        { K_H, 'H' },
        { K_J, 'J' },
        { K_K, 'K' },
        { K_L, 'L' },
        { K_M, ';' },
        { K_OU, '\'' },
        { K_MUL, '*' },
        { K_INF, '\\' },
        { K_W, 'Z' },
        { K_X, 'X' },
        { K_C, 'C' },
        { K_V, 'V' },
        { K_B, 'B' },
        { K_N, 'N' },
        { K_VIRGULE, 'M' },
        { K_PT_VIRGULE, ',' },
        { K_2_PTS, '.' },
        { K_EXC, '/' },
        { K_CARRE, '~' },
        { K_PARENTHESE, '-' },
        { K_EGAL, '+' },
        { 0, 0 }        // End
        } ;

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#define DisableKeyb()   keyboard_cmd(0xAD)

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#define EnableKeyb()    keyboard_cmd(0xAE)

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void    InitInt15()
{
        union   REGS    r               ;
        struct  SREGS   sr              ;
        void    far     *fh             ;

        if(Old_PM15_Sel) return         ;

        if(     !AIL_vmm_lock_range(&Int15CodeStart, &Int15CodeEnd)     OR
                !AIL_vmm_lock_range(&Int15DataStart, &Int15DataEnd)             )
        {
                exit(1);
        }

//----- Save Old Protected Mode Vector 15 ------
        r.x.eax = 0x3515                ;//     Function 35h, for Int 15
        sr.ds = sr.es = 0               ;//     Security (kill the cat)
        int386x( 0x21, &r, &r, &sr )    ;//     Call Int 21
        Old_PM15_Sel = (U32)sr.es       ;//     ES:EBX=Old Interrupt 15
        Old_PM15_Off = r.x.ebx          ;

//----- Install New Protected Vector 15 --------
        r.x.eax = 0x2515                ;//     Function 25h for int 15
        fh = (void far*)Int15Handler    ;//     Get far Pointer
        r.x.edx = FP_OFF( fh )          ;//     Get Offset
        sr.ds   = FP_SEG( fh )          ;//     Get Segment
        sr.es   = 0                     ;//     Security
        int386x( 0x21, &r, &r, &sr )    ;//     Invoke DPMI

        RegisterInitClear(InitInt15, ClearInt15);
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void    ClearInt15()
{
        union   REGS    r               ;
        struct  SREGS   sr              ;

        if(!Old_PM15_Sel) return        ;

//----- Restore Old Protected Vector 15 --------
        r.x.eax = 0x2515                ;//     Set Vector Interupt 15
        r.x.edx = Old_PM15_Off          ;//     Adr Old Interrupt 15
        sr.ds = (U16)Old_PM15_Sel       ;
        sr.es = 0                       ;//     Security
        int386x( 0x21, &r, &r, &sr )    ;//     Invoke int 21

        Old_PM15_Sel = 0                ;
        Old_PM15_Off = 0                ;

        AIL_vmm_unlock_range(&Int15CodeStart, &Int15CodeEnd)    ;
        AIL_vmm_unlock_range(&Int15DataStart, &Int15DataEnd)    ;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void    ForceScanCode(U8 scan)
{
        Int15Trans = scan       ;

/*
        keyboard_cmd(0xD0)      ;
        scan = keyboard_read()  ;
        keyboard_cmd(0xD1)      ;
        keyboard_write(scan|0x10);
*/
        keyboard_write(0xEE)    ;

        while(Int15Trans)       ;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
S32     ScanToAscii(S32 scan)
{
        S32     memoasciimode   ;
        S32     key, res        ;
        U32     timer           ;
        S32     restart = FALSE ;
        S32     i ;

        for(i=0; ScanUnknown[i].ScanCode; i++)
        {
                if(ScanUnknown[i].ScanCode==scan)       return ScanUnknown[i].Ascii ;
        }

        return 0 ;

        scan &= 0xFF            ;       // ignore high bits

        memoasciimode = AsciiMode;      // backup AsciiMode

        if((scan>127)||!scan)
        {
                return  0       ;
        }

        if(!Old_PM15_Sel)               // initialized ?
        {
                InitInt15()     ;       // init
        }

//      DisableKeyb()           ;

        AsciiMode = TRUE        ;       // set AsciiMode

        while(GetAscii())       ;       // empty Keyb buffer

        // Key DOWN

        ForceScanCode((U8)scan) ;       // send down scan code

        timer = TimerSystemHR+105;      // wait between 100 and 105 ms

        do
        {
                key = GetAscii();
                if(TimerSystemHR>timer)
                {
                        if(restart)     break;

                        if(scan==0x1F)  break;

                        ForceScanCode(0x1F);            // send down scan code for S

                        timer = TimerSystemHR+105;      // wait between 100 and 105 ms

                        restart = TRUE ;
                }
        }
        while(!key);                    // wait until Keyb Int taken

        res = key&0xFF  ;
        key >>= 8       ;

        if((key==0x1F)&&(scan!=0x1F))   res = 0 ;

        res = toupper(res);

        // Key UP

        if(restart)
        {
                ForceScanCode(0x1F|0x80);//send up scan code for S
        }

        ForceScanCode((U8)(scan|0x80)); // send up scan code

        // Reset status

        if(scan==0x3A)                  // CAPS LOCK
        {
                ForceScanCode(0x2A)     ;       // send shift down
                ForceScanCode(0x2A|0x80);       // send shift up
        }
        else if((scan==0x45)||(scan==0x46))// NUM LOCK/SCROLL LOCK
        {
                ForceScanCode((U8)scan);                // send down scan code
                ForceScanCode((U8)(scan|0x80)); // send up scan code
        }

        // clear and exit

        AsciiMode = memoasciimode;      // reset AsciiMode

//      EnableKeyb()            ;

        while(GetAscii())       ;       // empty Keyb buffer

        return(res)             ;       // return Ascii only
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif//_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
