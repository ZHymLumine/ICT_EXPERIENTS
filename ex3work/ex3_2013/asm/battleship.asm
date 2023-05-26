		ORG 0		/ interrupt entry point
ST0,	HEX 0		/ interrupt return address
		BUN I_HND	/ goto I_HND (interrupt handler)

		ORG 10		/ program entry point
INI, / initialize data
		CLA				/ AC         <- 0
		STA BYE			/ M[BYE]     <- 0
		STA NXT_BYE		/ M[NXT_BYE] <- 0
		STA STT			/ M[STT]     <- 0
		STA NXT_STT		/ M[NXT_STT] <- 0
		LDA VH1			/ AC         <- 1
		STA NXT_INP		/ M[NXT_INP] <- 1 (change to input state after output process)
		STA OUT_STT		/ M[OUT_STT] <- 1 (output state)
/ set start message
		LDA VH2			/ AC <- 2
		BSA SET_ML		/ call SET_ML (set message list)
		SYM MG_WELC		/ (arg1) "WELCOME TO BATTLE-SHIP!"
		SYM MG_WHG		/ (arg2) "\nwho goes first?(0[YOU],1[ME],q):"
/ setup IO registers
		LDA VH4			/ AC <- 4
		IMK				/ IMSK <- 0100 (S_OUT enabled)
		SIO				/ IOT <- 1 (serial-IO selected)
		ION				/ enable interrupt
/ wait until (M[BYE] = 1)
L0,		
		LDA BYE			/ AC <- M[BYE]
		SZA				/ (M[BYE] == 0) ? skip next
		HLT
		BUN L0			/ goto L0

/////////// interrupt handler ///////////
I_HND,	
/ store AC & E to memory
		STA BA			/ M[BA] <- AC	(store AC)
		CIL				/ AC[0] <- E	(AC[15:1] is not important here...)
		STA BE			/ M[BE] <- AC	(store E)

/////////// state machine ///////////
/ M[OUT_PD] = 0 (process input), 1 (process output)
/ check state :
		LDA OUT_STT		/ AC <- M[OUT_STT]
		SZA				/ (M[OUT_STT] == 0) skip next
		BUN PUT_OUT		/ goto PUT_OUT (process output)

/////////// process input ///////////
/ M[OUT_STT] = 0 : input mode (M[TMI] <- INPR)
		SKI				/ (FGI = 0) ? skip next
_B_,	BUN IRT			/ goto IRT (return from interrupt handler) --> this should not happen...
		CLA				/ AC      <- 0
		INP				/ AC[7:0] <- INPR
_M_,	STA TMI			/ M[TMI]  <- INPR

/////////// state switch : M[STT] = 0, 1, 2, 3, 4, 5, 6 ///////////
STT_SW,
		LDA STT			/ AC <- M[STT]
		ADD VM1			/ AC <- M[STT] - 1
		SPA				/ (M[STT] >= 1) ? skip next
		BUN STT_0		/ goto STT_0 (M[STT] = 0: setup new game) --> M[NXT_STT] = 1 (get your move), 2 (put my move)
		ADD VM1			/ AC <- M[STT] - 1 - 1
		SPA				/ (M[STT] >= 2) ? skip next
		BUN STT_1		/ goto STT_1 (M[STT] = 1: get your move)  --> M[NXT_STT] = 2 (put my move), 4 (end game)
		ADD VM1			/ AC <- M[STT] - 2 - 1
		SPA				/ (M[STT] >= 3) ? skip next
		BUN STT_2		/ goto STT_2 (M[STT] = 2: put my move)    --> M[NXT_STT] = 3 (show my move)
		ADD VM1			/ AC <- M[STT] - 3 - 1
		SPA				/ (M[STT] >= 4) ? skip next
		BUN STT_3		/ goto STT_3 (M[STT] = 3: show my move)   --> M[NXT_STT] = 1 (get your move), 4 (end game)
		BUN STT_4		/ goto STT_4 (M[STT] = 4: end game)       --> M[NXT_STT] = 0 (setup new game)

/////////// end tic-tac-toe ///////////
END_TTT,
		LDA VH1			/ AC         <- 1
		BSA SET_ML		/ call SET_ML (set message list)
		SYM MG_BYE		/ (arg1) "bye-bye!"
		STA NXT_BYE		/ M[NXT_BYE] <- 1
		BUN PRP_OUT		/ goto PRP_OUT (prepare output)

/////////// return from interrupt handler ///////////
IRT,	
		LDA BE			/ AC  <- M[BE]
		CIR				/ E   <- AC[0]	(restore E)
		LDA BA			/ AC  <- M[BA]	(restore AC)
		ION				/ IEN <- 1		(enable interrupt)
		BUN ST0 I		/ return from ST0 (interrupt handler)

/////////// prepare output ///////////
PRP_OUT,
		LDA VH1			/ AC         <- 1
		STA OUT_STT		/ M[OUT_STT] <- 1 (output state)
/		LDA VH4			/ AC         <- 4
/		IMK				/ IMASK      <- 0100 (S_OUT enabled)
		BUN IRT			/ goto IRT (return from interrupt handler)

PRP_TURN, HEX 0		/ return address
/////////// subroutine (prepare next turn)  ///////////
		LDA TURN		/ AC         <- M[TURN]
		SZA				/ (AC = 0) ? skip next
		BUN PRP_MT		/ goto PRP_MT (prepare my turn)
/ prepare "your turn"
		LDA VH1			/ AC         <- 1
		STA NXT_STT		/ M[NXT_STT] <- 1 (next state : get your move)
		STA NXT_INP		/ M[INP]     <- 1 (change to input state after output process)
		BUN PRP_TURN I	/ return from PRP_TURN
/ prepare "my turn"
PRP_MT,
		LDA VH2			/ AC         <- 2
		STA NXT_STT		/ M[NXT_STT] <- 2 (next state : put my move)
		BUN PRP_TURN I	/ return from PRP_TURN

/////////// M[STT] = 0 : setup new game  ///////////
STT_0,
/ determine who gets the opening move 
		LDA CH_0		/ AC <- '0'
		CMA
		INC				/ AC <- -'0'
		ADD TMI			/ AC <- M[TMI] - '0'
		STA TURN		/ M[TURM] <- M[TMI] - '0'
		SPA				/ (M[TMI] >= '0') ? skip next
		BUN END_TTT		/ goto END_TTT (end tic-tac-toe : M[TMI] < '0')
		ADD VM2			/ AC <- M[TURM] - 2
		SNA				/ (M[TURM] < 2) ? skip next
		BUN END_TTT		/ goto END_TTT (end tic-tac-toe : M[TMI] >= '2')
/ clear board
		LDA VM20			/ AC       <- -20
		STA CNT_1		/ M[CNT1]  <- -20
		LDA A_BRD		/ AC       <- M[A_BRD] (BRD)
		STA P_BRD		/ M[P_BRD] <- BRD
        LDA A_INI_BRD
        STA P_INI_BRD
		/CLA				/ AC       <- 0
L_CLR_BRD, / loop clear board
        LDA P_INI_BRD I
		STA	P_BRD I		/ M[M[P_BRD]] <- 0
		ISZ P_BRD		/ ++M[P_BRD]
        ISZ P_INI_BRD
		ISZ CNT_1		/ ((++M[CNT_1]) == 0) ? skip next
		BUN L_CLR_BRD	/ goto L_CLR_BRD (loop clear board)
/ clear bullets and speed of bullets
		LDA TOTAL_BULLET
		CMA
		INC
		STA CNT_BULLET
		LDA A_MY_BULLET_X	
		STA P_MY_BULLET_X
		LDA A_MY_BULLET_Y
		STA P_MY_BULLET_Y
		LDA A_PC_BULLET_X
		STA P_PC_BULLET_X
		LDA A_PC_BULLET_Y
		STA P_PC_BULLET_Y
		LDA A_MY_BULLET_SPEED
		STA P_MY_BULLET_SPEED
		LDA A_PC_BULLET_SPEED
		STA P_PC_BULLET_SPEED
L_CLR_BULLET,	/ loop clear bullets and speed
		LDA VM1
		STA P_MY_BULLET_X I
		STA P_MY_BULLET_Y I 
		STA P_PC_BULLET_X I 
		STA P_PC_BULLET_Y I 
		CLA 
		STA P_MY_BULLET_SPEED I 
		STA P_PC_BULLET_SPEED I 
		ISZ P_MY_BULLET_X
		ISZ P_MY_BULLET_Y
		ISZ P_PC_BULLET_X
		ISZ P_PC_BULLET_Y
		ISZ P_MY_BULLET_SPEED
		ISZ P_PC_BULLET_SPEED
		ISZ CNT_BULLET
		BUN L_CLR_BULLET
/ initialize move_count, winner
        CLA
		STA WINNER		/ M[WINNER]  <- 0
		STA MY_WIN
		STA PC_WIN
		LDA VD1
		STA MY_Y
		LDA VD3
		STA PC_Y
		LDA VM100			/ AC         <- −100
		STA CNT_MOV		/ M[CNT_MOV] <- -100
		BSA PRP_TURN	/ call PRP_TURN (prepare next turn)
		BSA SHOW_GAME	/ call SHOW_GAME
		BUN PRP_OUT		/ goto PRP_OUT (prepare output)

/////////// M[STT] = 1 : get your move ///////////
STT_1,
        LDA CH_a
        BSA CHK_CH
        SZA             / (AC = 0) ? skip next (not a)
        BUN LEFT        
        LDA CH_d
        BSA CHK_CH
        SZA             / (AC = 0) ? skip next (not d)
        BUN RIGHT 
        LDA CH_s
        BSA CHK_CH
        SZA
        BUN KEEP        / not move
        LDA CH_j
        SZA             / (AC = 0) ? skip next (not j)
        BUN FIRE
        BUN NG_YM       /   not a or d or j or s
LEFT,   LDA MY_Y
        ADD VM1
        SPA             / MY - 1 >= 0, can move
        BUN NG_YM
        STA MY_Y
        BUN MOVE
RIGHT,  LDA MY_Y
        ADD VM3
        SNA
        BUN NG_YM       / MY_Y >=3 , can't move
        LDA MY_Y        / MY_Y < 3, cant move        
        ADD VH1
        STA MY_Y
        BUN MOVE
MOVE, / put your move to board
        LDA MY_Y
        CMA
        INC 
        STA CNT_MOVE
		LDA A_BRD		/ AC        <- M[A_BRD] (BRD)
		ADD VD16	    / AC        <- BRD + 16
		STA P_BRD		/ M[P_BRD]  <- BRD + M[TMI]
        LDA CH_O
        STA P_BRD I
        ISZ P_BRD
        STA P_BRD I
        ISZ P_BRD
        STA P_BRD I
        ISZ P_BRD
        STA P_BRD I
        LDA A_BRD		/ AC        <- M[A_BRD] (BRD)
		ADD VD16	    / AC        <- BRD + 16
		STA P_BRD		/ M[P_BRD]  <- BRD + 16
        LDA CNT_MOVE
        SZA
        BUN LOOP
        BUN PUT_ME
LOOP,       / loop increase board
        ISZ P_BRD
        ISZ CNT_MOVE
        BUN LOOP
PUT_ME,     / put new position
        LDA CH_M
        STA P_BRD I
        BUN PRE_UPD_TURN
NG_YM, / your move is invalid
		LDA VH1			/ AC     <- 1
		BSA SET_ML		/ call SET_ML (set message list)
		SYM MG_IVMV		/ (arg1) "invalid move!"
		LDA VH1			/ AC     <- 1
		STA NXT_INP		/ M[INP] <- 1
		BUN PRP_OUT		/ goto PRP_OUT (prepare output)
FIRE,   
        LDA A_MY_BULLET_X
        STA P_MY_BULLET_X
        LDA A_MY_BULLET_Y
        STA P_MY_BULLET_Y
		LDA A_MY_BULLET_SPEED
		STA P_MY_BULLET_SPEED
        LDA TOTAL_BULLET
        CMA
        INC
        STA CNT_BULLET      / times of loop
FIND_BULLET,        
        LDA P_MY_BULLET_X I  
        SPA                 / bullet >= 0, can't shoot
        BUN LOAD_BULLET   
        ISZ P_MY_BULLET_X
        ISZ P_MY_BULLET_Y
        ISZ P_MY_BULLET_SPEED      
        ISZ CNT_BULLET      / if can't find position, default action: goto KEEP 
        BUN FIND_BULLET     / find next position for bullet
KEEP,   
        LDA MY_SPEED
        INC
        STA MY_SPEED
        BUN PRE_UPD_TURN
LOAD_BULLET,
        LDA MY_X
        STA P_MY_BULLET_X I
        LDA MY_Y
        STA P_MY_BULLET_Y I
        LDA MY_SPEED
		ADD VM1			/ AC = MY_SPEED - 2
		SZA
		BUN MY_SPEED_2	
		BUN MY_SPEED_1
MY_SPEED_1,
		LDA VH1
		STA P_MY_BULLET_SPEED I		/ MY_SPEED = 1, put 1 to the array
		BUN PRE_UPD_TURN
MY_SPEED_2,
		LDA VH2
		STA P_MY_BULLET_SPEED I		/ MY_SPEED >= 2
		LDA VH1
		STA MY_SPEED
        BUN PRE_UPD_TURN

PRE_UPD_TURN,
        BSA UPD_TURN
        LDA CNT_TURN
        INC
        STA CNT_TURN
        SZA				/ CNT_TURN = 0, goto UPD_BULLET, update bullet every 2 turns
        BUN PRP_OUT
		BSA UPD_BULLET
        BUN PRP_OUT
		BSA CHK_WIN		/ call CHK_WIN
		SZA				/ (AC == 0) ? skip next (AC = WINNER)
		BUN END_GAME	/ goto END_TURN (winner mark != 0)
		BUN PRP_OUT
END_GAME,
		LDA VH4			/ AC         <- 4
		STA NXT_STT		/ M[NXT_STT] <- 4 (next state : end game)
		BSA SHOW_GAME	/ call SHOW_GAME
		BUN PRP_OUT

UPD_BULLET,	HEX 0		/ update position of bullets
        LDA VM2			/ initialize CNT_TURN to -2
        STA CNT_TURN
        LDA TOTAL_BULLET
        CMA
        INC 
        STA CNT_BULLET
        LDA A_MY_BULLET_X		/ only need to update x coordinate of bullet
        STA P_MY_BULLET_X
        LDA A_MY_BULLET_SPEED
        STA P_MY_BULLET_SPEED
LOOP_UPDATE_MY_BULLET,       		/ update every buttet 
        LDA P_MY_BULLET_SPEED I 
        CMA 
        INC 					/ AC = -speed
        ADD P_MY_BULLET_X I      / x = x - speed
        STA P_MY_BULLET_X I
        ISZ P_MY_BULLET_SPEED   / speed[i ++]
        ISZ P_MY_BULLET_X       / x[i ++]
        ISZ CNT_BULLET
        BUN LOOP_UPDATE_MY_BULLET
/ prepare to update pc's bullets
		LDA TOTAL_BULLET
        CMA
        INC 
        STA CNT_BULLET
        LDA A_PC_BULLET_X		/ only need to update x coordinate of bullet
        STA P_PC_BULLET_X
        LDA A_PC_BULLET_SPEED
        STA P_PC_BULLET_SPEED
LOOP_UPDATE_PC_BULLET,
		LDA P_PC_BULLET_X I 
		ADD VM5
		SPA
		BUN UPD_PC_BULLET_SPEED
		BUN INI_BULLET	
INI_BULLET,		
		LDA VM1
		STA P_PC_BULLET_X I
		BUN	NXT 
UPD_PC_BULLET_SPEED,
		LDA P_PC_BULLET_SPEED I 			/ AC = pc_speed
        ADD P_PC_BULLET_X I      / x = x + speed
        STA P_PC_BULLET_X I
NXT,
        ISZ P_PC_BULLET_SPEED   / speed[i ++]
        ISZ P_PC_BULLET_X       / x[i ++]
        ISZ CNT_BULLET
        BUN LOOP_UPDATE_PC_BULLET
UPDATE_BRD,				/ after updating all bullets, draw bullets on the board
		LDA A_BRD		/ AC        <- M[A_BRD] (BRD)
		ADD VD4	    / AC        <- BRD + 4
		STA P_BRD		/ M[P_BRD]  <- BRD + M[TMI]
        LDA VM11
        STA CNT_CLR_BULLET
        LDA CH_DOT
        STA P_BRD I
CLR_BULLET,       		/ first, put the 12 mass to . 
        ISZ P_BRD
        STA P_BRD I
        ISZ CNT_CLR_BULLET
        BUN CLR_BULLET
		LDA TOTAL_BULLET
		CMA
		INC 
		STA CNT_BULLET		/ total number of bullets (control loop)
	/	BUN DRAW_PC_BULLET
        LDA A_MY_BULLET_X 
		STA P_MY_BULLET_X
		LDA A_MY_BULLET_Y
		STA P_MY_BULLET_Y
LOOP_DRAW_BULLET,
		LDA A_BRD
		STA P_BRD
		LDA P_MY_BULLET_X I 
		SPA
		BUN NXT_BULLET		/ only draw bullets with x >= 0
		CIL
		CIL					/ X = X * 4
		ADD P_MY_BULLET_Y I / AC = 4*X + Y
		CMA
		INC
		STA CNT_FIND_BULLET	/ CNT_CLR_BULLET = - (4*X + Y)
		SZA
        BUN LOOP_FIND_BULLET
        BUN PUT_BULLET 
LOOP_FIND_BULLET,			/ find postion to draw bullet
		ISZ P_BRD
		ISZ CNT_FIND_BULLET
		BUN LOOP_FIND_BULLET
PUT_BULLET,					/ put bullet to the board
		LDA CH_I
		STA P_BRD I
NXT_BULLET,		
		ISZ P_MY_BULLET_X
		ISZ P_MY_BULLET_Y
		ISZ CNT_BULLET
		BUN LOOP_DRAW_BULLET
		BSA SHOW_GAME
		BUN UPD_BULLET I

DRAW_PC_BULLET,
		LDA A_BRD		/ AC        <- M[A_BRD] (BRD)
		STA P_BRD		/ M[P_BRD]  <- BRD + 4
		LDA TOTAL_BULLET
		CMA
		INC 
		STA CNT_BULLET
		LDA A_PC_BULLET_X 
		STA P_PC_BULLET_X
		LDA A_PC_BULLET_Y
		STA P_PC_BULLET_Y
LOOP_DRAW_PC_BULLET,
		LDA A_BRD
		STA P_BRD
		LDA P_PC_BULLET_X I 
		SPA
		BUN NXT_PC_BULLET		/ x < 0, skip initial value
		ADD VM5
		SNA
		BUN NXT_PC_BULLET		/ only draw bullets with x <= 4
		LDA P_PC_BULLET_X I
		CIL
		CIL					/ X = X * 4
		ADD P_PC_BULLET_Y I / AC = 4*X + Y
		CMA
		INC
		STA CNT_FIND_BULLET	/ CNT_CLR_BULLET = - (4*X + Y)
		SZA
        BUN LOOP_FIND_PC_BULLET
        BUN PUT_PC_BULLET 
LOOP_FIND_PC_BULLET,			/ find postion to draw bullet
		ISZ P_BRD
		ISZ CNT_FIND_BULLET
		BUN LOOP_FIND_PC_BULLET
PUT_PC_BULLET,					/ put bullet to the board
		LDA CH_I
		STA P_BRD I
NXT_PC_BULLET,		
		ISZ P_PC_BULLET_X
		ISZ P_PC_BULLET_Y
		ISZ CNT_BULLET
		BUN LOOP_DRAW_PC_BULLET
		BSA SHOW_GAME
		BUN UPD_BULLET I


/////////// M[STT] = 2 : put my move  ///////////
STT_2,
		BSA MY_MOV		/ call MY_MOV (next move position stored in M[P_BRD])
/ put my move to board
        LDA PC_ACTION
        ADD VM1			/ AC <- M[STT] - 1
		SPA				/ (M[STT] >= 1) ? skip next
		BUN PC_STT_0		/ goto PC_STT_0 (move to left)
		ADD VM1			
		SPA				/ (M[STT] >= 2) ? skip next
		BUN PC_STT_1		/ goto PC_STT_1 (move to right)
		ADD VM1			/ AC <- M[STT] - 2 - 1
		SPA				/ (M[STT] >= 3) ? skip next
		BUN PC_STT_2
		ADD VM1			/ AC <- M[STT] - 3 - 1
		SPA				/ (M[STT] >= 4) ? skip next
		BUN PC_STT_3		/ goto PC_STT_3 (don't move)  
PC_STT_0,   / move to left
		LDA PC_Y
        ADD VM1
        SPA             / MY - 1 >= 0, can move
        BUN PC_STT_1
        STA PC_Y
        BUN PC_MOVE
		/INC				/ AC           <- - BRD
		/ADD P_BRD		/ AC           <- P_BRD - BRD (= pos)
		/ADD CH_1		/ AC           <- pos + '1'
		/STA MG_MY_MV0	/ M[MG_MY_MV0]  <- pos + '1'
PC_STT_1,   / move to right
        LDA PC_Y
        ADD VM3
        SNA
        BUN PC_STT_0       / MY_Y >=3 , can't move
        LDA PC_Y        / MY_Y < 3, cant move        
        ADD VH1
        STA PC_Y
        BUN PC_MOVE
PC_STT_2,   / PC fires
        CLA
        BUN PC_FIRE
PC_STT_3,    / PC don't move
        LDA PC_SPEED
        INC
        STA PC_SPEED
        BUN SET_PC_MSG
PC_MOVE, / put  my(PC) move to board
        LDA PC_Y
        CMA
        INC 
        STA CNT_MOVE
		LDA A_BRD		/ AC        <- M[A_BRD] (BRD)
		STA P_BRD		/ M[P_BRD]  <- BRD + M[TMI]
        LDA CH_O        / put O to the first line
        STA P_BRD I
        ISZ P_BRD
        STA P_BRD I
        ISZ P_BRD
        STA P_BRD I
        ISZ P_BRD
        STA P_BRD I
        LDA A_BRD		/ AC        <- M[A_BRD] (BRD)
		STA P_BRD		/ M[P_BRD]  <- BRD + 16   
        LDA CNT_MOVE
        SZA
        BUN LOOP_PC
        BUN PUT_PC
LOOP_PC,   / loop increase board
        ISZ P_BRD
        ISZ CNT_MOVE
        BUN LOOP_PC
PUT_PC,     / put new position
        LDA CH_E
        STA P_BRD I
SET_PC_MSG, / set output message
		BSA SET_MGP		/ call SET_MGP (set message to P_OUT)
		SYM MG_MY_MV	/ (arg1) myMove
		LDA VH3			/ AC           <- 3
		STA NXT_STT		/ M[NXT_STT]   <- 3
		BUN PRP_OUT		/ goto PRP_OUT (prepare output)

PC_FIRE,
		LDA A_PC_BULLET_X
        STA P_PC_BULLET_X
        LDA A_PC_BULLET_Y
        STA P_PC_BULLET_Y
		LDA A_PC_BULLET_SPEED
		STA P_PC_BULLET_SPEED
        LDA TOTAL_BULLET
        CMA
        INC
        STA CNT_BULLET      / times of loop
PC_FIND_BULLET,        
        LDA P_PC_BULLET_X I  
        SPA                 / bullet >= 0, can't shoot
        BUN PC_LOAD_BULLET   
        ISZ P_PC_BULLET_X
        ISZ P_PC_BULLET_Y
        ISZ P_PC_BULLET_SPEED      
        ISZ CNT_BULLET      / if can't find position, default action: goto KEEP 
        BUN PC_FIND_BULLET     / find next position for bullet
		BUN PC_STT_3
PC_LOAD_BULLET,
        LDA PC_X
        STA P_PC_BULLET_X I
        LDA PC_Y
        STA P_PC_BULLET_Y I
        LDA PC_SPEED
		ADD VM1			/ AC = MY_SPEED - 2
		SZA
		BUN PC_SPEED_2	
		BUN PC_SPEED_1
PC_SPEED_1,
		LDA VH1
		STA P_PC_BULLET_SPEED I		/ MY_SPEED = 1, put 1 to the array
		BUN SET_PC_MSG
PC_SPEED_2,
		LDA VH2
		STA P_PC_BULLET_SPEED I		/ MY_SPEED >= 2
		LDA VH1
		STA PC_SPEED
        BUN SET_PC_MSG

/////////// M[STT] = 3 : show my move  ///////////
STT_3,
		BUN PRE_UPD_TURN
        /BSA UPD_TURN	/ call UPD_TURN (update turn)
		/BUN PRP_OUT		/ goto PRP_OUT (prepare output)

/////////// M[STT] = 4 : end game  ///////////
STT_4,
		LDA WINNER		/ AC         <- M[WINNER] 0 or 1 or 2
		ADD VM2
		SPA 
		BUN WHO_WON		/ BUN WHO_WON (who won??)
		BUN TIE 		/ WINNER = 2, tie	
TIE,		
		LDA A_MG_TIE	/ AC         <- M[A_MG_TIE] (MG_TIE : "it's a tie")
		STA RESULT		/ M[RESULT]  <- "it's a tie"
		BUN STT_4_1		/ goto STT_4_1
WHO_WON,
		LDA MY_WIN
		SZA				
		BUN MY_WON		/ my_win = 1
		BUN PC_WON		/ my_win = 0
MY_WON,		
		LDA A_MG_YWN	/ AC         <- M[A_MG_YWN] (MG_YWN : "you win!")
		STA RESULT		/ M[RESULT]  <- "you win!"
		BUN STT_4_1		/ goto STT_4_1
PC_WON,
		LDA A_MG_IWN	/ AC         <- M[A_MG_IWN] (MG_IWN : "I win!")
		STA RESULT		/ M[RESULT]  <- "I win!"
STT_4_1,
		LDA VH2			/ AC         <- 2
		BSA SET_ML		/ call SET_ML (set message list)
RESULT,	HEX 0			/ (arg1) "you win!" or "I win!" or "it's a tie..."
		SYM MG_WHG		/ (arg2) "\nwho goes first?(0[YOU], 1[ME], q):"
		LDA VH1			/ AC         <- 1
		STA NXT_INP		/ M[INP]     <- 1
		CLA				/ AC         <- 0
		STA NXT_STT		/ M[NXT_STT] <- 0
		BUN PRP_OUT		/ goto PRP_OUT (prepare output)

SHOW_GAME, HEX 0		/ return address
/////////// subroutine (show game)  ///////////
		LDA A_BRD		/ AC        <- M[A_BRD] (BRD)
		STA P_BRD		/ M[P_BRD]  <- BRD + 1
		LDA A_MG_BRD	/ AC        <- M[A_MG_BRD] (MG_BRD)
		INC				/ AC        <- MG_BRD + 1
		STA PTR_MG		/ M[PTR_MG] <- MG_BRD + 1
		LDA VM5			/ AC        <- -5  print 5 lines
		STA CNT_1		/ M[CNT_1]  <- -5
		LDA CH_1		/ AC        <- '1'
		STA TMA			/ M[TMA]    <- '1'
L_PUT_BD, / loop print board
		BSA PRT_BRD		/ call PRT_BRD (print board)
		BSA PRT_BRD		/ call PRT_BRD (print board)
		BSA PRT_BRD		/ call PRT_BRD (print board)
        BSA PRT_BRD     / call PRT_BRD (print board)
		ISZ PTR_MG		/ ++M[PTR_MG] : skip newline position on BD_MG
		ISZ CNT_1		/ ((++M[CNT_1]) == 0) ? skip next
		BUN L_PUT_BD	/ goto L_PUT_BD (loop print board)
/ check winner
		LDA WINNER		/ AC <- M[WINNER]
		SZA				/ (M[WINNER] == 0) ? skip next
		BUN SET_GM_OVR	/ goto SET_GM_OVR (M[WINNER] != 0)
		LDA CNT_MOV		/ AC <- M[CNT_MOV]
		SNA				/ (M[CNT_MOV] < 0) ? skip next
		BUN SET_GM_OVR	/ goto SET_GM_OVR (M[CNT_MOV] == 0)
/ set NXT_MV ("your move!" or "my move!")
		LDA TURN		/ AC <- M[TURN]
		SZA				/ (M[TURN] == 0) ? skip next
		BUN SET_MMV		/ goto SET_MMV
/ set "your move!"
		LDA A_MG_YMV	/ AC <- M[A_MG_YMV] (MG_YMV: "your move!")
		STA NXT_MV		/ M[NXT_MV] <- "your move!"
		BUN SET_MG_BRD	/ goto SET_MG_BRD
SET_MMV, / set "my move!"
		LDA A_MG_MMV	/ AC <- M[A_MG_MMV] (MG_MMV: "my move!")
		STA NXT_MV		/ M[NXT_MV] <- "your move!"
		BUN SET_MG_BRD	/ goto SET_MG_BRD
SET_GM_OVR, / set "game over!"
		LDA A_MG_GMO	/ AC <- M[A_MG_GMO] (MG_GMO: "game over!")
		STA NXT_MV		/ M[NXT_MV] <- "game over!"
SET_MG_BRD, / set MG_BRD
		LDA VH4			/ AC <- 4
		BSA SET_ML		/ call SET_ML (set message list)
		SYM MG_SEPR		/ (arg1) MG_SEPR
		SYM MG_BRD		/ (arg2) MB_BRD
		SYM MG_SEPR		/ (arg3) MG_SEPR
NXT_MV,	HEX 0			/ (arg4) "your move!" or "my move!" or "game over!"
		BUN SHOW_GAME I	/ return from SHOW_GAME

PRT_BRD,HEX 0			/ return address
/////////// subroutine (print board)  ///////////
		LDA P_BRD I		/ AC           <- M[M[P_BRD]]
		SZA				/ (AC == 0) ? skip next
		BUN CPY_BD		/ goto CPY_BD
		/LDA TMA			/ AC           <- M[TMA]
CPY_BD,	STA PTR_MG I	/ M[M[PTR_MG]] <- AC
		ISZ PTR_MG		/ ++M[PTR_MG]
		ISZ P_BRD		/ ++M[P_BRD]
		ISZ TMA			/ ++M[TMA]
		BUN PRT_BRD I	/ return from PRT_BRD

UPD_TURN, HEX 0
/////////// subroutine (update turn)  ///////////
		LDA TURN		/ AC         <- M[TURN]
		CMA				/ AC         <- - M[TURN] - 1
		INC				/ AC         <- - M[TURN]
		INC				/ AC         <- 1 - M[TURN]
		STA TURN		/ M[TURN]    <- 1 - M[TURN]
		BSA CHK_WIN		/ call CHK_WIN
		SZA				/ (AC == 0) ? skip next (AC = WINNER)
		BUN END_TURN	/ goto END_TURN (winner mark != 0)
		ISZ CNT_MOV		/ ((++M[CNT_MOV]) == 0) ? skip next
		BUN NXT_TURN	/ goto NXT_TURN
END_TURN,
		LDA VH4			/ AC         <- 4
		STA NXT_STT		/ M[NXT_STT] <- 4 (next state : end game)
		BSA SHOW_GAME	/ call SHOW_GAME
		BUN UPD_TURN I	/ return from UPD_TURN
NXT_TURN,
		BSA PRP_TURN	/ call PRP_TURN (prepare next turn)
		BSA SHOW_GAME	/ call SHOW_GAME
		BUN UPD_TURN I	/ return from UPD_TURN

CHK_CH,	HEX 0			/ return address
////////// subroutine (check character) ///////////
/ arg0 (AC) : character to identify
/ return AC = 1 : character matched
/ return AC = 0 : character not matched
		CMA				/ AC <- ~AC
		INC				/ AC <- AC + 1 (AC = - arg0)
		ADD TMI			/ AC <- AC + M[TMI] (M[TMI] - arg0)
		SZA				/ (M[TMI] = arg0) ? skip next
		LDA VM1			/ AC <- M[VM1] (-1) (no match)
		INC				/ AC <- AC + 1
		BUN CHK_CH I	/ return from CHK_CH

CHK_WIN, HEX 0
/////////// subroutine (check winner)  ///////////
/ return AC (winner) : 0(no winner) or 1 or 2 
		LDA A_MY_BULLET_X
		STA P_MY_BULLET_X
		LDA A_MY_BULLET_Y
		STA P_MY_BULLET_Y
		LDA TOTAL_BULLET
		CMA
		INC
		STA CNT_BULLET
CHK_MY_WIN,
		LDA P_MY_BULLET_X I 
		SZA 
		BUN CHK_NXT_MY_WIN	/ X != 0 check next bullet
		LDA P_MY_BULLET_Y I	/ X = 0
		CMA
		INC
		ADD PC_Y
		SZA
		BUN CHK_NXT_MY_WIN
		LDA VH1				/ y coordinate of bullet is equal 0, my win!!
		STA MY_WIN
		BUN PRP_CHK_PC_WIN	/ prepare to check whether pc wins
CHK_NXT_MY_WIN,	
		ISZ P_MY_BULLET_X
		ISZ P_MY_BULLET_Y
		ISZ CNT_BULLET
		BUN CHK_MY_WIN
PRP_CHK_PC_WIN,
		LDA A_PC_BULLET_X
		STA P_PC_BULLET_X
		LDA A_PC_BULLET_Y
		STA P_PC_BULLET_Y
		LDA TOTAL_BULLET
		CMA
		INC
		STA CNT_BULLET
CHK_PC_WIN,		
		LDA P_PC_BULLET_X I 
		ADD VM4
		SPA
		BUN CHK_NXT_PC_WIN		/ x of PC's bulltet is less than 4, check next bullet
		LDA P_PC_BULLET_Y I 	/ x >= 4, check y coordinate
		CMA
		INC
		ADD MY_Y
		SZA
		BUN CHK_NXT_PC_WIN
		LDA VH1				/ y coordinate of bullet is equal to my_y, pc win!!
		STA PC_WIN
		BUN CHK_WIN_OVER
CHK_NXT_PC_WIN,
		ISZ P_PC_BULLET_X
		ISZ P_PC_BULLET_Y
		ISZ CNT_BULLET
		BUN CHK_PC_WIN
CHK_WIN_OVER,
		LDA PC_WIN
		ADD MY_WIN
		STA WINNER			/ WINNER = 0, no one wins, WINNER = 1, at least one wins, WINNER = 2, tie(my win and pc win)
		BUN CHK_WIN I

MY_MOV,	HEX 0
////////// subroutine (my move) //////////
        BSA RAND
        CLA
        LDA SEED
        STA PC_ACTION
		BUN MY_MOV I	/ return from MY_MOV
RAND,  HEX 0 
       CLA
       STA P
       LDA SEED    / Load the seed
       STA X
       LDA MULTIPLIER       / Multiply by A
       STA Y
       BSA MUL
       CLA
       LDA P
       ADD CONST       / Add Const
       STA P
	   LDA MULTIPLIER
	   INC
	   STA MULTIPLIER
	   LDA CONST
	   INC
	   STA CONST
       BSA DIVIDE       / Modulo M
       LDA R 
       STA SEED
       BUN RAND I         / Return
MUL,   HEX 0
MUL_SUB, 	CLE			/ E <- 0
            LDA Y		/ AC <- M[Y]
            SZA			/ (AC == 0) ? skip next
            BUN LY		/ goto LY
            BUN MUL I
/ M[Y] >>= 1
LY,	        CIR			/ {AC[15:0], E} <- {E, AC[15:0]}
            STA Y		/ M[Y] <- AC
            SZE			/ (E == 0) ? skip next
            BUN LP		/ goto LP
/ M[X] <<= 1
LX,	        LDA X		/ AC <- M[X]
            CIL			/ {E, AC[15:0]} <- {AC[15:0], E}
            STA X		/ M[X] <- AC
            BUN MUL_SUB		/ goto LO
/ M[P] += M[X]
LP,         LDA X		/ AC <- M[X]
            ADD P		/ AC <- AC + M[P]
            STA P		/ M[P} <- AC
            CLE			/ E <- 0
            BUN LX		/ goto LX
DIVIDE, HEX 0
        CLA
        STA Q 
        STA R
        LDA P
        STA A
        LDA K_INI
        STA K
        LDA MOD
        CMA
        INC
        STA NB
LSUB,   CLE
        LDA A
        CIL
        STA A
        LDA R
        CIL
        STA R
        ADD NB
        SNA
        STA R
        LDA Q
        CIL
        STA Q
        ISZ K
        BUN LSUB
        BUN DIVIDE I
		

SET_MGP,	HEX 0
////////// subroutine (set single message for P_OUT) //////////
/ arg(1) : message address
		CLA				/ AC <- 0
		CMA				/ AC <- -1
		STA CNT_MG		/ M[CNT_MG]      <- - 1 (message count = 1)
		LDA A_MG_LST	/ AC             <- M[A_MG_LST] (MG_LIST)
		STA P_MG_LST	/ M[P_MG_LST]    <- MG_LIST
		LDA SET_MGP I	/ AC             <- M[M[SET_MGP]] (arg(i))
		STA P_MG_LST I	/ M[MG_LIST]     <- arg(1)
/ load 1st message
		BSA LD_MSG		/ call LD_MSG (load message info)
/ output to P_OUT
		PIO				/ IOT <- 0 (parallel-IO selected)
		LDA VH1			/ AC <- 1
		IMK				/ IMSK <- 0001 (P_OUT enabled)
		ISZ SET_MGP		/ ++M[SET_MGP]
		BUN SET_MGP I	/ return from SET_MGP

SET_ML,	HEX 0
////////// subroutine (set message list for S_OUT) //////////
/ arg0 (AC) : msg_count
/ arg(1), arg(2), ... : message addresses (# of arguments = msg_count)
		INC				/ AC             <- (msg_count + 1)
		CMA				/ AC             <- - (msg_count + 2)
		STA CNT_MG		/ M[CNT_MG]      <- - (msg_count + 2) (2 newlines)
		INC
		INC
		STA TMA			/ M[TMA]         <- - msg_count
		LDA A_MG_LST	/ AC             <- M[A_MG_LST] (MG_LIST)
		STA P_MG_LST	/ M[P_MG_LST]    <- MG_LIST
		STA TMB			/ M[TMB]         <- MG_LIST
/ put 1st newline at head
		LDA A_MG_NL		/ AC             <- M[A_MG_NL] (MG_NL)
		STA TMB I		/ M[M[TMB]]      <- MG_NL
		ISZ TMB			/ ++M[TMB]
L_SET_ML,	/ loop set message : i = 1, 2, ..., msg_count
		LDA SET_ML I	/ AC             <- M[M[SET_ML]] (arg(i))
		STA TMB I		/ M[MG_LIST + i] <- arg(i)
		ISZ SET_ML		/ ++M[SET_ML]
		ISZ TMB			/ ++M[TMB]
		ISZ TMA			/ ((++M[TMA]) == 0) ? skip next
		BUN L_SET_ML	/ goto L_SET_ML (loop set message)
/ put 2nd newline at head
		LDA A_MG_NL		/ AC             <- M[A_MG_NL] (MG_NL)
		STA TMB I		/ M[MG_LIST + msg_count + 1] <- MG_NL
/ load 1st message
		BSA LD_MSG		/ call LD_MSG (load message info)
/ output to S_OUT
		LDA VH4			/ AC <- 4
		IMK				/ IMSK <- 0100 (S_OUT enabled)
		BUN SET_ML I	/ return from SET_ML

LD_MSG,HEX 0
////////// subroutine (load message info) //////////
		LDA P_MG_LST I	/ AC        <- M[M[P_MG_LST]] (message address)
		STA PTR_MG		/ M[PTR_MG] <- M[M[P_MG_LST]] (message address)
		LDA PTR_MG I	/ AC        <- M[M[PTR_MG]] (message length)
		CMA
		INC				/ AC        <- - (message length)
		STA CNT_CH		/ M[CNT_CH] <- message count
		ISZ PTR_MG		/ ++M[PTR_MG] (message starts from the next address)
		ISZ P_MG_LST	/ ++M[P_MG_LST]
		BUN LD_MSG I	/ return from SET_MSG

////////// process output //////////
PUT_OUT,
		SKO				/ (FGO = 0) ? skip next
_M_,	BUN IRT			/ goto IRT (return from interrupt handler) --> this should not happen...
/ output 1 message character at address M[PTR_MG]
		LDA PTR_MG I	/ AC   <- M[M[PTR_MG]]
		OUT				/ OUTR <- AC(7:0)
		ISZ PTR_MG		/ ++M[PTR_MG]
		ISZ CNT_CH		/ ((++M[CNT_CH]) == 0) ? skip next
/ message string pending ...
		BUN IRT			/ goto IRT (return from interrupt handler)
/ message string terminated
		ISZ CNT_MG		/ ((++M[CNT_MG]) = 0) ? skip next
		BUN NXT_MSG		/ goto NXT_MSG (load next message)
/ all message processed
		CLA				/ AC         <- 0
		STA OUT_STT		/ M[OUT_STT] <- 0
		LDA NXT_STT		/ AC         <- M[NXT_STT]
		STA STT			/ M[STT]     <- M[NXT_STT]
		SIO				/ IOT <- 1 (serial-IO selected)
		LDA NXT_BYE		/ AC         <- M[NXT_BYE]
		STA BYE			/ M[BYE]     <- M[NXT_BYE]
		SZA				/ (AC = 0) ? skip next
		BUN CHG_INP		/ goto CHG_INP (M[BYE] = 1 : prepare to terminate program...)
		LDA NXT_INP		/ AC <- M[NXT_INP]
		SZA				/ (AC = 0) ? skip next
		BUN CHG_INP		/ goto CHG_INP (M[NXT_INP] = 1 : change to input mode)
		BUN STT_SW		/ goto STT_SW (state switch)
NXT_MSG, / load next message
		BSA LD_MSG		/ call LD_MSG (load message info)
		BUN IRT			/ goto IRT (return from interrupt handler)
CHG_INP, / change to input mode
		CLA				/ AC         <- 0
		STA NXT_INP		/ M[NXT_INP] <- 0
		LDA VH8			/ AC         <- 8
		IMK				/ IMSK       <- 1000 (S_IN enabled)
		BUN IRT			/ goto IRT (return from interrupt handler)

/ data (no initialization)
TMA,	DEC 0		/ temporal
TMB,	DEC 0		/ temporal
TMI,	DEC 0		/ char (raw) input
BA,		DEC 000		/ backup storage for AC during interrupt handling
BE,		DEC 000		/ backup storage for  E during interrupt handling
PTR_MG, HEX 0		/ message pointer
CNT_1,	DEC 0		/ counter 1
CNT_2,	DEC 0		/ counter 2
CNT_CH,	DEC 0		/ char counter
CNT_MOVE, DEC 0     / where to put M
CNT_CLR_BULLET, DEC 0
CNT_FIND_BULLET,	DEC 0
CNT_TURN,   DEC -2
P_BRD,	HEX 0		/ pointer to BRD
P_MY_LNC, HEX 0		/ pointer to MY_LNC
P_YR_LNC, HEX 0		/ pointer to YR_LNC
P_INI_BRD, HEX 0    / pointer to INI_BRD   
P_MY_BULLET_X,   HEX 0   / pointer to MY_BULLET_X
P_MY_BULLET_Y,   HEX 0   / pointer to MY_BULLET_Y
P_PC_BULLET_X,   HEX 0   / pointer to PC_BULLET_X
P_PC_BULLET_Y,   HEX 0   / pointer to PC_BULLET_Y
P_MY_BULLET_SPEED, 	HEX 0    /pointer to MY_BULLET_SPEED
P_PC_BULLET_SPEED,	HEX 0
BRD_OFS,DEC 0		/ BRD offset
BRD_POS,DEC 0		/ BRD pos
N_P_BRD,DEC 0		/ pointer to BRD for my next move
BRD_1,	DEC 0		/ brd[pos1]
BRD_2,	DEC 0		/ brd[pos2]
BRD_SUM_N, DEC 0	/ -(brd[pos1] + brd[pos2])
MY_LN,	DEC 0		/ myLine
MY_BLK,	DEC 0		/ myBlock

/ data (need initialization code : one-time)
BYE,	DEC 0		/ (init: 0) bye
NXT_BYE,DEC 0		/ (init: 0) next bye
STT,	DEC 0		/ (init: 0) current state
NXT_STT,DEC 0		/ (init: 0) next state
OUT_STT,DEC 0		/ (init: 0) output state
NXT_INP,DEC 0		/ (init: 0) next process input

CNT_MG,	DEC 0	/ message count
P_MG_LST, HEX 0	/ message list pointer
A_MG_LST, SYM MG_LST
MG_LST, HEX 0	/ message pointer list (up to 8+2 messages)
		HEX 0
		HEX 0
		HEX 0
		HEX 0
		HEX 0
		HEX 0
		HEX 0
		HEX 0
		HEX 0

/ data (need initialization code : after every game -> INI_GM)
CNT_MOV,DEC 0		/ move counter
TURN,	DEC 0		/ 0 : your turn, 1 : my turn
WINNER, DEC 0		/ winner
/ BRD : brd[9]
/ position : on screen
/    012       123
/    345       456
/    678       789
A_BRD,	SYM BRD
BRD,	CHR O (C1)
		CHR O (C1)
		CHR O (C1)
		CHR E (C1)
		CHR . (C2)
		CHR . (C2)
		CHR . (C2)
		CHR . (C2)
		CHR . (C3)
        CHR . (C3)
        CHR . (C3)
        CHR . (C3)
        CHR . (C4)
        CHR . (C4)
        CHR . (C4)
        CHR . (C4)
        CHR O (C5)
        CHR M (C5)
        CHR O (C5)
        CHR O (C5)

A_INI_BRD,  SYM INI_BRD
INI_BRD,    CHR O (C1)
            CHR O (C1)
            CHR O (C1)
            CHR E (C1)
            CHR . (C2)
            CHR . (C2)
            CHR . (C2)
            CHR . (C2)
            CHR . (C3)
            CHR . (C3)
            CHR . (C3)
            CHR . (C3)
            CHR . (C4)
            CHR . (C4)
            CHR . (C4)
            CHR . (C4)
            CHR O (C5)
            CHR M (C5)
            CHR O (C5)
            CHR O (C5)

TOTAL_BULLET,   DEC 4
CNT_BULLET,     DEC 0       / control loop

A_MY_BULLET_X,    SYM MY_BULLET_X
MY_BULLET_X,      DEC -1
                  DEC -1
                  DEC -1
                  DEC -1

A_MY_BULLET_Y,    SYM MY_BULLET_Y
MY_BULLET_Y,      DEC -1
                  DEC -1
                  DEC -1
                  DEC -1

A_MY_BULLET_SPEED,      SYM MY_BULLET_SPEED 
MY_BULLET_SPEED,        DEC 0
                       	DEC 0
                       	DEC 0
                       	DEC 0

A_PC_BULLET_X,     SYM PC_BULLET_X
PC_BULLET_X,       DEC -1
                   DEC -1
                   DEC -1
                   DEC -1

A_PC_BULLET_Y,     SYM PC_BULLET_Y
PC_BULLET_Y,       DEC -1
                   DEC -1
                   DEC -1
                   DEC -1

A_PC_BULLET_SPEED,  SYM PC_BULLET_SPEED
PC_BULLET_SPEED,	DEC 0
					DEC 0
					DEC 0
					DEC 0

/ data (read-only)
AMK,	HEX FFF0	/ AMK = FFF0 (and mask)
AMKN,	HEX 000F	/ AMKN = 000F (and mask negated)
VD1,    DEC 1
VH1,	HEX 1		/ VH1 = 1
VH2,	HEX 2		/ VH2 = 2
VH3,	HEX 3		/ VH3 = 3
VD3,    DEC 3
VH4,	HEX 4		/ VH4 = 4
VH5,	HEX 5		/ VH5 = 5
VH8,	HEX 8		/ VH8 = 8
VH12,   HEX 12
VD4,    DEC 4
VD15,   DEC 15
VD16,   DEC 16      / VD16 = 16   
VM1,	DEC -1		/ VM1 = -1
VM2,	DEC -2		/ VM2 = -2
VM3,	DEC -3		/ VM2 = -3
VM4,	DEC -4		/ VM2 = -4
VM5,    DEC -5      / VM5 = -5
VM8,	DEC -8		/ VM2 = -8
VM9,	DEC -9		/ VM2 = -9
VM10,	DEC -10		/ VM10 = -10
VM11,   DEC -11
VM20,   DEC -20     / VM20 = -20
VM100,  DEC -100
CH_0,	CHR 0
CH_1,	CHR 1
CH_a,   HEX 61
CH_d,   HEX 64
CH_j,   HEX 6A
CH_s,   HEX 73
CH_M,   CHR M
CH_E,   CHR E
CH_DOT, CHR .
CH_O,   CHR O
CH_I,   CHR I
CH_LY,	CHR y
MY_MK,	CHR X
YR_MK,	CHR O		/ "ooh"
MY_MK_2,HEX	B0		/ (0x58('X') * 2)
YR_MK_2,HEX	9E		/ (0x4F('O') * 2)
MG_WELC,DEC 23	/ MG_WELC length
		CHR W
		CHR E
		CHR L
		CHR C
		CHR O
		CHR M
		CHR E
		HEX 20	/ ' '
		CHR T
		CHR O
		HEX 20	/ ' '
		CHR B
		CHR A
		CHR T
		CHR T
		CHR L
		CHR E
		CHR -
		CHR S
		CHR H
		CHR I
		CHR P
		CHR !

MG_MY_MV, DEC 1	/ MG_MY_MV length
MG_MY_MV0,	HEX 0

A_MG_NL,SYM MG_NL
MG_NL,	DEC 1	/ MG_NL length
		HEX 0A	/ '\n'

MG_BYE,	DEC 8	/ MG_BYE length
		CHR b
		CHR y
		CHR e
		CHR -
		CHR b
		CHR y
		CHR e
		CHR !

A_MG_YMV, SYM MG_YMV			
MG_YMV,	DEC 10	/ MG_YMV length
		CHR y
		CHR o
		CHR u
		CHR r
		HEX 20	/ ' '
		CHR m
		CHR o
		CHR v
		CHR e
		CHR !
		
A_MG_MMV, SYM MG_MMV
MG_MMV,	DEC 8	/ MG_MMV length
		CHR m
		CHR y
		HEX 20	/ ' '
		CHR m
		CHR o
		CHR v
		CHR e
		CHR !

A_MG_GMO, SYM MG_GMO
MG_GMO,	DEC 10	/ MG_GMO length
		CHR g
		CHR a
		CHR m
		CHR e
		HEX 20	/ ' '
		CHR o
		CHR v
		CHR e
		CHR r
		CHR !
		
A_MG_YWN, SYM MG_YWN
MG_YWN, DEC 8	/ MG_YWN length
		CHR y
		CHR o
		CHR u
		HEX 20	/ ' '
		CHR w
		CHR i
		CHR n
		CHR !

A_MG_IWN,	SYM MG_IWN		
MG_IWN,	DEC 6	/ MG_IWN length
		CHR I
		HEX 20	/ ' '
		CHR w
		CHR i
		CHR n
		CHR !
		
A_MG_TIE, SYM MG_TIE
MG_TIE,	DEC 13	/ MG_TIE length
		CHR i
		CHR t
		CHR '
		CHR s
		HEX 20	/ ' '
		CHR a
		HEX 20	/ ' '
		CHR t
		CHR i
		CHR e
		CHR .
		CHR .
		CHR .

MG_IVMV,DEC 13	/ MG_IVMV length
		CHR i
		CHR n
		CHR v
		CHR a
		CHR l
		CHR i
		CHR d
		HEX 20	/ ' '
		CHR m
		CHR o
		CHR v
		CHR e
		CHR !

MG_WHG,DEC 34	/ MG_WHG length
		HEX 0A	/ '\n'
		CHR w
		CHR h
		CHR o
		HEX 20	/ ' '
		CHR g
		CHR o
		CHR e
		CHR s
		HEX 20	/ ' '
		CHR f
		CHR i
		CHR r
		CHR s
		CHR t
		CHR ?
		HEX 20	/ ' '
		CHR (
		CHR 0
		CHR :
		CHR Y
		CHR O
		CHR U
		CHR ,
		HEX 20	/ ' '
		CHR 1
		CHR :
		CHR M
		CHR E
		CHR ,
		HEX 20	/ ' '
		CHR q
		CHR )
		CHR :

MG_SEPR,DEC 4	/ MG_SEPR length
		CHR -
		CHR -
		CHR -
		HEX 0A	/ '\n'

A_MG_BRD,	SYM MG_BRD
MG_BRD,	DEC 25	/ MG_BRD length
		CHR -
		CHR -
		CHR -
        CHR -
		HEX 0A	/ '\n'
		CHR -
		CHR -
		CHR -
        CHR -
		HEX 0A	/ '\n'
		CHR -
		CHR -
		CHR -
        CHR -
		HEX 0A	/ '\n'
        CHR -
		CHR -
		CHR -
        CHR -
		HEX 0A	/ '\n'
        CHR -
		CHR -
		CHR -
        CHR -
		HEX 0A	/ '\n'

MY_X,   DEC 4
MY_Y,   DEC 1
PC_X,    DEC 0
PC_Y,    DEC 3
MY_WIN,	 HEX 0
PC_WIN,	 HEX 0
MY_SPEED,    DEC 1
PC_SPEED,    DEC 1
PC_ACTION,   DEC 0 
SEED,         DEC 1       / Initial seed
MULTIPLIER,     DEC 5       / Multiplier
CONST,          HEX 3       / Increment
MOD,            HEX 4       / Modulus (2^3)
X,      DEC 0
Y,      DEC 0
A,      HEX 0000
B,      HEX 0
P,      DEC 0
NB,     HEX 0000
R,      HEX 0000
Q,      HEX 0000 
K_INI,  DEC -16
K,      DEC 0
END