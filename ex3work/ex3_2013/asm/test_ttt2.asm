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
		SYM MG_WELC		/ (arg1) "WELCOME TO TIC-TAC-TOE!"
		SYM MG_WHG		/ (arg2) "\nwho goes first?(0[YOU],1[ME],q):"
/ setup IO registers
		LDA EOM			/ AC <- M[EOM] (initial value = 1)
		IMK				/ IMASK <- AC[1:0] (input disabled, output enabled)
		LDA VH3			/ AC <- 3
		IOS				/ IOSEL <- 3 (uart input, uart output)
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
		BUN IRT			/ goto IRT (return from interrupt handler) --> this should not happen...
		CLA				/ AC      <- 0
		INP				/ AC[7:0] <- INPR
		STA TMI			/ M[TMI]  <- INPR

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
		LDA EOM			/ AC         <- M[EOM] (initial value = 1)
		IMK				/ IMASK      <- AC[1:0] (input disabled, output enabled)
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
		LDA VM9			/ AC       <- -9
		STA CNT_1		/ M[CNT1]  <- -9
		LDA A_BRD		/ AC       <- M[A_BRD] (BRD)
		STA P_BRD		/ M[P_BRD] <- BRD
		CLA				/ AC       <- 0
L_CLR_BRD, / loop clear board
		STA	P_BRD I		/ M[M[P_BRD]] <- 0
		ISZ P_BRD		/ ++M[P_BRD]
		ISZ CNT_1		/ ((++M[CNT_1]) == 0) ? skip next
		BUN L_CLR_BRD	/ goto L_CLR_BRD (loop clear board)
/ initialize move_count, winner
		STA WINNER		/ M[WINNER]  <- 0
		LDA VM9			/ AC         <- -9
		STA CNT_MOV		/ M[CNT_MOV] <- -9
		BSA PRP_TURN	/ call PRP_TURN (prepare next turn)
		BSA SHOW_GAME	/ call SHOW_GAME
		BUN PRP_OUT		/ goto PRP_OUT (prepare output)

/////////// M[STT] = 1 : get your move (must satisfy: '1' <= M[TMI] <= '9')  ///////////
STT_1,
		LDA CH_1		/ AC        <- M[CH_1] ('1')
		CMA
		INC				/ AC        <- -'1'
		ADD TMI			/ AC        <- M[TMI] - '1'
		SPA				/ (M[TMI] >= '1') ? skip next
		BUN NG_YM		/ goto NG_YM (your move is invalid : invalid position)
		STA TMI			/ M[TMI]    <- M[TMI] - '1';
		ADD VM9			/ AC        <- M[TMI] - 9
		SNA				/ (M[TMI] < 9) ? skip next
		BUN NG_YM		/ goto NG_YM (your move is invalid : invalid position)
/ put your move to board
		LDA A_BRD		/ AC        <- M[A_BRD] (BRD)
		ADD TMI			/ AC        <- BRD + M[TMI]
		STA P_BRD		/ M[P_BRD]  <- BRD + M[TMI]
		LDA P_BRD I		/ AC        <- M[BRD + M[TMI]]
		SZA				/ (M[BRD + M[TMI]] == 0) ? skip next
		BUN NG_YM		/ goto NG_YM (your move is invalid : already occupied...)
		LDA YR_MK		/ AC              <- M[YR_MK] ('O')
		STA P_BRD I		/ M[BRD + M[TMI]] <- M[YR_MK] ('O')
		BSA UPD_TURN	/ call UPD_TURN (update turn)
		BUN PRP_OUT		/ goto PRP_OUT (prepare output)
NG_YM, / your move is invalid
		LDA VH1			/ AC     <- 1
		BSA SET_ML		/ call SET_ML (set message list)
		SYM MG_IVMV		/ (arg1) "invalid move!"
		LDA VH1			/ AC     <- 1
		STA NXT_INP		/ M[INP] <- 1
		BUN PRP_OUT		/ goto PRP_OUT (prepare output)

/////////// M[STT] = 2 : put my move  ///////////
STT_2,
		BSA MY_MOV		/ call MY_MOV (next move position stored in M[P_BRD])
/ put my move to board
		LDA MY_MK		/ AC           <- M[MY_MK] ('X')
		STA P_BRD I		/ M[BRD + pos] <- M[MY_MK] ('X')
/ put my move to output
		LDA A_BRD		/ AC           <- M[A_BRD] (BRD)
		CMA
		INC				/ AC           <- - BRD
		ADD P_BRD		/ AC           <- P_BRD - BRD (= pos)
		ADD CH_1		/ AC           <- pos + '1'
		STA MG_MY_MV0	/ M[MG_MY_MV0]  <- pos + '1'
/ set output message
//		BSA SET_MGP		/ call SET_MGP (set message to gpio output)
		LDA VH1			/ AC           <- 1
		BSA SET_ML		/ call SET_ML  (set message list)
		SYM MG_MY_MV	/ (arg1) myMove
		LDA VH3			/ AC           <- 3
		STA NXT_STT		/ M[NXT_STT]   <- 3
		BUN PRP_OUT		/ goto PRP_OUT (prepare output)

/////////// M[STT] = 3 : show my move  ///////////
STT_3,
		BSA UPD_TURN	/ call UPD_TURN (update turn)
		BUN PRP_OUT		/ goto PRP_OUT (prepare output)

/////////// M[STT] = 4 : end game  ///////////
STT_4,
		LDA WINNER		/ AC         <- M[WINNER]
		SZA				/ (M[WINNER] == 0) ? skip next
		BUN WHO_WON		/ BUN WHO_WON (who won??)
		LDA A_MG_TIE	/ AC         <- M[A_MG_TIE] (MG_TIE : "it's a tie")
		STA RESULT		/ M[RESULT]  <- "it's a tie"
		BUN STT_4_1		/ goto STT_4_1
WHO_WON,
		CMA
		INC				/ AC         <- - M[WINNER]
		ADD YR_MK		/ AC         <- M[YR_MK] - M[WINNER]
		SZA				/ (M[YR_MK] == M[WINNER]) ? skip next
		BUN I_WON		/ goto I_WON
		LDA A_MG_YWN	/ AC         <- M[A_MG_YWN] (MG_YWN : "you win!")
		STA RESULT		/ M[RESULT]  <- "you win!"
		BUN STT_4_1		/ goto STT_4_1
I_WON,
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
		LDA VM3			/ AC        <- -3
		STA CNT_1		/ M[CNT_1]  <- -3
		LDA CH_1		/ AC        <- '1'
		STA TMA			/ M[TMA]    <- '1'
L_PUT_BD, / loop print board
		BSA PRT_BRD		/ call PRT_BRD (print board)
		BSA PRT_BRD		/ call PRT_BRD (print board)
		BSA PRT_BRD		/ call PRT_BRD (print board)
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
		LDA TMA			/ AC           <- M[TMA]
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
		SZA				/ (AC == 0) ? skip next (AC = winner mark)
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

CHK_CH_N,	HEX 0
////////// subroutine (check character) ///////////
/ arg0 (AC) : ch (character to identify)
/ return AC : (M[TMI] != ch)
		CMA
		INC				/ AC <- - ch
		ADD TMI			/ AC <- M[TMI] - ch
		BUN CHK_CH_N I	/ return from CHK_CH

CHK_3, HEX 0
/////////// subroutine (check 3-in-a-row)  ///////////
/ arg0 (AC)  : pos
/ M[BRD_OFS] : offset
/ 3 positions : BRD[pos], BRD[pos + offset], BRD[pos + offset * 2]
/ return AC (winner) : 'O' or 'X' or 0 (no winner)
		ADD A_BRD		/ AC        <- pos + M[A_BRD] (BRD + pos)
		STA P_BRD		/ M[P_BRD]  <- BRD + pos
		LDA P_BRD I		/ AC        <- M[BRD + pos]
		SZA				/ (M[BRD + pos] == 0) ? skip next
		BUN CHK_3_1		/ goto CHK_3_1
		BUN R_CHK_3_0	/ goto R_CHK_3_0 (return 0 : no winner)
CHK_3_1,
		STA TMI			/ M[TMI]    <- M[BRD + pos]
		LDA VM2			/ AC        <- -2
		STA CNT_2		/ M[CNT_2]  <- -2
L_CHK_3,	/ loop check board : i = 1, 2 (BRD[pos] == BRD[pos + offset * i])
		LDA P_BRD		/ AC        <- BRD + pos + offset * (i - 1)
		ADD BRD_OFS		/ AC        <- BRD + pos + offset * i
		STA P_BRD		/ M[P_BRD]  <- BRD + pos + offset * i
		LDA P_BRD I		/ AC        <- M[BRD + pos + offset * i]
		BSA CHK_CH_N	/ call check character
		SZA				/ (AC == 0) ? skip next
		BUN R_CHK_3_0	/ goto R_CHK_3_0 (return 0)
		ISZ CNT_2		/ ((++M[CNT_2]) = 0) ? skip next
		BUN L_CHK_3		/ goto L_CHK_3 (loop check board)
/ this is 3-in-a-row!!
		LDA TMI			/ AC        <- M[BRD + pos] (winner mark)
		STA WINNER		/ M[WINNER] <- winner mark
		BUN CHK_3 I		/ return from CHK_3
R_CHK_3_0,
		CLA				/ AC <- 0
		BUN CHK_3 I		/ return from CHK_3

CHK_WIN, HEX 0
/////////// subroutine (check winner)  ///////////
/ return AC (winner) : 'O' or 'X' or 0 (no winner)
		LDA VM8			/ AC         <- -8
		STA CNT_1		/ M[CNT_1]   <- 8
		LDA A_CW		/ AC         <- M[A_CW] (CW)
		STA P_CW		/ M[P_CW]    <- CW
L_CHK_WIN, / loop check winner
		LDA P_CW I		/ AC         <- M[M[P_CW]] (offset)
		STA BRD_OFS		/ M[BRD_OFS] <- offset
		ISZ P_CW		/ ++M[P_CW]
		LDA P_CW I		/ AC         <- M[M[P_CW]] (pos)
		ISZ P_CW		/ ++M[P_CW]
		BSA CHK_3		/ call CHK_3
		SZA				/ (AC == 0) ? skip next
		BUN CHK_WIN I	/ return from CHK_WIN (AC != 0 : winner mark)
		ISZ CNT_1		/ ((++M[CNT_1]) == 0) ? skip next
		BUN L_CHK_WIN	/ goto L_CHK_WIN (loop check winner)
		BUN CHK_WIN I	/ return from CHK_WIN (AC == 0 : no winner)

MY_DM_MOV,	HEX 0
////////// subroutine (my dumb move) //////////
		LDA A_BRD		/ AC <- M[A_BRD] (BRD)
		STA P_BRD		/ M[P_BRD] <- BRD
L_CHK_BRD, / loop check board : pos = 0, 1, ... 8
		LDA	P_BRD I		/ AC <- M[BRD + pos]
		SZA				/ (M[BRD + pos] == 0) ? skip next
		BUN N_CHK_BRD	/ goto N_CHK_BRD
		BUN MY_DM_MOV I	/ return from MY_DM_MOV
N_CHK_BRD,
		ISZ P_BRD		/ ++M[P_BRD]
		BUN L_CHK_BRD	/ goto L_CHK_BRD (loop check board)

MY_MOV,	HEX 0
////////// subroutine (my move) //////////
/ M[BRD_POS] : pos
		CLA				/ AC          <- 0
		STA BRD_POS		/ M[BRD_POS]  <- 0 (pos = 0)
		STA N_P_BRD		/ M[N_P_BRD]  <- 0
		LDA A_BRD		/ AC          <- M[A_BRD] (BRD)
		STA P_BRD		/ M[P_BRD]    <- BRD
		LDA VM9			/ AC          <- -9
		STA CNT_2		/ M[CNT_2]    <- -9
L_CHK_POS, / loop check board : pos = 0, 1, ... 8
		LDA P_BRD I		/ AC          <- M[BRD + pos]
		SZA				/ (M[BRD + pos] == 0) ? skip next
		BUN N_CHK_POS	/ goto N_CHK_POS (++pos) (M[BRD + pos] != 0 : occupied)
		BSA WBF			/ call WBF (WinBlockFork)
		SZA				/ (AC == 0) ? skip next
		BUN MY_MOV I	/ return from MY_MOV
N_CHK_POS, / ++pos
		ISZ BRD_POS		/ ++M[BRD_POS]
		ISZ P_BRD		/ ++M[P_BRD]
		ISZ CNT_2		/ ((++M[CNT_2]) == 0) ? skip next
		BUN L_CHK_POS	/ goto L_CHK_BRD (loop check board)
/ check (M[N_P_BRD] != 0) ? 
		LDA N_P_BRD		/ AC <- M[N_P_BRD]
		SZA				/ (M[N_P_BRDto
		BUN PUT_NXT_POS	/ goto PUT_NXT_POS (M[N_P_BRD] != 0)
		BSA MY_DM_MOV	/ call MY_DM_MOV (my dumb move)
		BUN MY_MOV I	/ return from MY_MOV
PUT_NXT_POS,
		STA P_BRD		/ M[P_BRD]     <- M[N_P_BRD]
		BUN MY_MOV I	/ return from MY_MOV

LD_TBRD, HEX 0
////////// subroutine (load traslated board position) //////////
/ arg 0 (AC) : table-addr (H1, H2, V1, V2, D1, D2, D3, D4)
/ M[BRD_POS] : pos
/ return (AC < 0) : invalid transposition (happens on diagonal translations on edge positions)
/ return (AC >= 0) : translated board position (M[BRD + M[table-addr + pos]])
		ADD BRD_POS		/ AC <- table-addr + pos
		STA TMA			/ M[TMA] <- table-addr + pos
		LDA TMA I		/ AC <- M[table-addr + pos]
		SPA				/ (M[table-addr + pos] >= 0) ? skip next
		BUN LD_TBRD I	/ return from LD_TBRD (AC < 0 : invalid translation)
		ADD A_BRD		/ AC <- BRD + M[table-addr + pos]
		STA TMA			/ M[TMA] <- BRD + M[table-addr + pos]
		LDA TMA I		/ AC <- M[BRD + M[table-addr + pos]]
		BUN LD_TBRD I	/ return from LD_TBRD (AC >= 0 : valid translated board position)

WBF,	HEX 0
////////// subroutine (WinBlockFork) //////////
/ M[BRD_POS] : pos
/ return 0 : not my win...
/ return 1 : my win!!!
		CLA				/ AC <- 0
		STA MY_LN		/ M[MY_LN] <- 0
		STA MY_BLK		/ M[MY_BLK] <- 0
		LDA A_HVD		/ AC <- M[A_HVD] (HVD)
		STA P_HVD		/ M[P_HVD] <- HVD (M[HVD] = {H1, H2, V1, V2, D1, D2, D3, D4})
		LDA VM4			/ AC <- -4
		STA CNT_1		/ M[CNT_1] <- -4
L_WBF,	/ loop WBF
		LDA P_HVD I		/ AC <- M[HVD] ({H1, H2, V1, V2, D1, D2, D3, D4})
		ISZ P_HVD		/ ++M[P_HVD]
/ load BRD_1
		BSA LD_TBRD		/ call LD_TBRD (load translated board position)
		SPA				/ (M[BRD + M[M[HVD] + pos]] >= 0) ? skip next
		BUN WBF_2		/ goto WBF_2 (invalid transposition : exit L_WBF loop)
		STA BRD_1		/ M[BRD_1] <- M[BRD + M[M[HVD] + pos]]
		LDA P_HVD I		/ AC <- M[HVD] ({H1, H2, V1, V2, D1, D2, D3, D4})
		ISZ P_HVD		/ ++M[P_HVD]
/ load BRD_2
		BSA LD_TBRD		/ call LD_TBRD (load translated board position)
		STA BRD_2		/ M[BRD_2] <- M[BRD + M[HVD] + pos]]
/ call WBF_POS (WinBlockForkPos)
		BSA WBF_POS		/ call WBF_POS (WinBlockForkPos)
		SZA				/ (AC == 0) ? skip next
		BUN WBF I		/ return from WBF (return 1 : my win!!!)
		ISZ CNT_1		/ ((++M[CNT_1]) == 0) ? skip next
		BUN L_WBF		/ goto (loop WBF)
WBF_2,	/ (M[MY_BLK] != 0 || M[MY_LN] >= 2 && M[N_P_BRD] == 0) ? M[N_P_BRD] <- M[P_BRD]
		LDA MY_BLK		/ AC <- M[MY_BLK]
		SZA				/ (M[MY_BLK] == 0) ? skip next
		BUN SET_NPOS	/ goto SET_NPOS (M[MY_BLK] > 0 : need to block this position)
/ check my fork (M[MY_LN] >= 2)
		LDA MY_LN		/ AC <- M[MY_LN]
		ADD VM2			/ AC <- M[MY_LN] - 2
		SPA				/ (M[MY_LN] >= 2) ? skip next
		BUN R_WBF_0		/ goto R_WBF_0 (skip SET_NPOS)
/ check (M[N_P_BRD] != 0)
		LDA N_P_BRD		/ AC <- M[N_P_BRD]
		SZA				/ (M[N_P_BRD] != 0) ? skip next
		BUN R_WBF_0		/ goto R_WBF_0 (skip SET_NPOS)
SET_NPOS,
		LDA P_BRD		/ AC <- current BRD pointer
		STA N_P_BRD		/ M[N_P_BRD] <- current BRD pointer (I should take this position if I don't win this turn)
R_WBF_0, / return 0 : not my win...
		CLA				/ AC <- 0
		BUN WBF I		/ return from WBF (not my win...)
		
WBF_POS,	HEX 0
////////// subroutine (WinBlockForkPos) //////////
/ M[BRD_1] : brd[pos1]
/ M[BRD_2] : brd[pos2]
/ return 0 : not my win...
/ return 1 : my win!!!
		LDA BRD_1		/ AC           <- M[BRD_1]
		ADD BRD_2		/ AC           <- M[BRD_1] + M[BRD_2]
		CMA
		INC				/ AC           <- -(M[BRD_1] + M[BRD_2])
		STA BRD_SUM_N	/ M[BRD_SUM_N] <- -(M[BRD_1] + M[BRD_2])
/ check my two-in-a-row : (M[BRD_1] == M[BRD_2] == M[MY_MK])
		ADD MY_MK_2		/ AC <- M[MY_MK_2] - (M[BRD_1] + M[BRD_2]) (M[MY_MK_2] = M[MY_MK] * 2)
		SZA				/ (M[MY_MK] * 2 == M[BRD_1] + M[BRD_2]) ? skip next
		BUN CHK_YL2		/ goto CHK_YL2 (check your two-in-a-row)
		LDA VH1			/ AC           <- 1
		BUN WBF_POS I	/ return from WBF_POS (return AC = 1 : my win!!!)
CHK_YL2, / check your two-in-a-row (M[BRD_1] == M[BRD_2] == M[YR_MK])
		LDA BRD_SUM_N	/ AC           <- -(M[BRD_1] + M[BRD_2])
		ADD YR_MK_2		/ AC           <- M[YR_MK_2] - (M[BRD_1] + M[BRD_2]) (M[YR_MK_2] = M[YR_MK] * 2)
		SZA				/ (M[YR_MK] * 2 == M[BRD_2] + M[BRD_1]) ? skip next
		BUN CHK_ML1		/ goto CHK_MY1 (check myLine)
		ISZ MY_BLK		/ ++M[MY_BLK] (block your two-in-a-row)
		BUN WBF_POS I	/ return from WBF_POS (return AC = 0)
CHK_ML1, / check myLine (M[MY_MK] == M[BRD_2] + M[BRD_1])
		LDA BRD_SUM_N	/ AC           <- -(M[BRD_1] + M[BRD_2])
		ADD MY_MK		/ AC           <- M[MY_MK] - (M[BRD_1] + M[BRD_2])
		SZA				/ (M[MY_MK] == M[BRD_2] + M[BRD_1]) ? skip next
		BUN R_WBF_POS_0	/ goto R_WBR_POS_0 (return 0)
		ISZ MY_LN		/ ++M[MY_LN]
R_WBF_POS_0, / return 0
		CLA				/ AC           <- 0
		BUN WBF_POS I	/ return from WBF_POS (return AC = 0)

SET_MGP,	HEX 0
////////// subroutine (set single message for gpio output) //////////
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
/ output to gpio
		LDA VH2			/ AC <- 2
		IOS				/ IOSEL <- 2 (uart input, gpio output)
		ISZ SET_MGP		/ ++M[SET_MGP]
		BUN SET_MGP I	/ return from SET_MGP

SET_ML,	HEX 0
////////// subroutine (set message list) //////////
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
		BUN IRT			/ goto IRT (return from interrupt handler) --> this should not happen...
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
		LDA VH3			/ AC <- 3
		IOS				/ IOSEL <- 3 (uart input, uart output)
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
		LDA EIM			/ AC         <- M[EIM] (initial value = 1)
		IMK				/ IMASK      <- AC[1:0] (input enabled, output disabled)
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
P_BRD,	HEX 0		/ pointer to BRD
P_MY_LNC, HEX 0		/ pointer to MY_LNC
P_YR_LNC, HEX 0		/ pointer to YR_LNC
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
BRD,	HEX 0 (0)
		HEX 0 (1)
		HEX 0 (2)
		HEX 0 (3)
		HEX 0 (4)
		HEX 0 (5)
		HEX 0 (6)
		HEX 0 (7)
		HEX 0 (8)

/ data (read-only)
EOM,	DEC 1		/ enable output mask (disable input)
EIM,	DEC 2		/ enable input mask (disable output)
IOM,	DEC 3		/ io mask : 11 means UART is enabled at input and output
AMK,	HEX FFF0	/ AMK = FFF0 (and mask)
AMKN,	HEX 000F	/ AMKN = 000F (and mask negated)
VH1,	HEX 1		/ VH1 = 1
VH2,	HEX 2		/ VH2 = 2
VH3,	HEX 3		/ VH3 = 3
VH4,	HEX 4		/ VH4 = 4
VH5,	HEX 5		/ VH5 = 5
VM1,	DEC -1		/ VM1 = -1
VM2,	DEC -2		/ VM2 = -2
VM3,	DEC -3		/ VM2 = -3
VM4,	DEC -4		/ VM2 = -4
VM8,	DEC -8		/ VM2 = -8
VM9,	DEC -9		/ VM2 = -9
VM10,	DEC -10		/ VM10 = -10
CH_0,	CHR 0
CH_1,	CHR 1
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
		CHR T
		CHR I
		CHR C
		CHR -
		CHR T
		CHR A
		CHR C
		CHR -
		CHR T
		CHR O
		CHR E
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
MG_BRD,	DEC 12	/ MG_BRD length
		CHR -
		CHR -
		CHR -
		HEX 0A	/ '\n'
		CHR -
		CHR -
		CHR -
		HEX 0A	/ '\n'
		CHR -
		CHR -
		CHR -
		HEX 0A	/ '\n'

P_CW,	HEX 0	/ CW pointer
A_CW,	SYM CW	/ CW address
CW,		/ array for CHK_3 parameter
		DEC 1	/ offset = 1 (0,1,2)
		DEC 0	/ pos    = 0 (0,1,2)
		DEC 1	/ offset = 1 (3,4,5)
		DEC 3	/ pos    = 3 (3,4,5)
		DEC 1	/ offset = 1 (6,7,8)
		DEC 6	/ pos    = 6 (6,7,8)
		DEC 3	/ offset = 3 (0,3,6)
		DEC 0	/ pos    = 0 (0,3,6)
		DEC 3	/ offset = 3 (1,4,7)
		DEC 1	/ pos    = 1 (1,4,7)
		DEC 3	/ offset = 3 (2,5,8)
		DEC 2	/ pos    = 2 (2,5,8)
		DEC 4	/ offset = 4 (0,4,8)
		DEC 0	/ pos    = 0 (0,4,8)
		DEC 2	/ offset = 2 (2,4,6)
		DEC 2	/ pos    = 2 (2,4,6)

P_HVD,	DEC 0	/ HVD pointer
A_HVD,	SYM A_H1
A_H1,	SYM H1
A_H2,	SYM H2
A_V1,	SYM V1
A_V2,	SYM V2
A_D1,	SYM D1
A_D2,	SYM D2
A_D3,	SYM D3
A_D4,	SYM D4
H1,		DEC  1 / ( H1[0] = 1)
		DEC  2 / ( H1[1] = 2)
		DEC  0 / ( H1[2] = 0)
		DEC  4 / ( H1[3] = 4)
		DEC  5 / ( H1[4] = 5)
		DEC  3 / ( H1[5] = 3)
		DEC  7 / ( H1[6] = 7)
		DEC  8 / ( H1[7] = 8)
		DEC  6 / ( H1[8] = 6)
H2,		DEC  2 / ( H2[0] = 2)
		DEC  0 / ( H2[1] = 0)
		DEC  1 / ( H2[2] = 1)
		DEC  5 / ( H2[3] = 5)
		DEC  3 / ( H2[4] = 3)
		DEC  4 / ( H2[5] = 4)
		DEC  8 / ( H2[6] = 8)
		DEC  6 / ( H2[7] = 6)
		DEC  7 / ( H2[8] = 7)
V1,		DEC  3 / ( V1[0] = 3)
		DEC  4 / ( V1[1] = 4)
		DEC  5 / ( V1[2] = 5)
		DEC  6 / ( V1[3] = 6)
		DEC  7 / ( V1[4] = 7)
		DEC  8 / ( V1[5] = 8)
		DEC  0 / ( V1[6] = 0)
		DEC  1 / ( V1[7] = 1)
		DEC  2 / ( V1[8] = 2)
V2,		DEC  6 / ( V2[0] = 6)
		DEC  7 / ( V2[1] = 7)
		DEC  8 / ( V2[2] = 8)
		DEC  0 / ( V2[3] = 0)
		DEC  1 / ( V2[4] = 1)
		DEC  2 / ( V2[5] = 2)
		DEC  3 / ( V2[6] = 3)
		DEC  4 / ( V2[7] = 4)
		DEC  5 / ( V2[8] = 5)
D1,		DEC  4 / ( D1[0] =  4)
		DEC -1 / ( D1[1] = -1)
		DEC  4 / ( D1[2] =  4)
		DEC -1 / ( D1[3] = -1)
		DEC  0 / ( D1[4] =  0)
		DEC -1 / ( D1[5] = -1)
		DEC  4 / ( D1[6] =  4)
		DEC -1 / ( D1[7] = -1)
		DEC  4 / ( D1[8] =  4)
D2,		DEC  8 / ( D2[0] =  8)
		DEC -1 / ( D2[1] = -1)
		DEC  6 / ( D2[2] =  6)
		DEC -1 / ( D2[3] = -1)
		DEC  8 / ( D2[4] =  8)
		DEC -1 / ( D2[5] = -1)
		DEC  2 / ( D2[6] =  2)
		DEC -1 / ( D2[7] = -1)
		DEC  0 / ( D2[8] =  0)
D3,		DEC -1 / ( D3[0] = -1)
		DEC -1 / ( D3[1] = -1)
		DEC -1 / ( D3[2] = -1)
		DEC -1 / ( D3[3] = -1)
		DEC  2 / ( D3[4] =  2)
D4,		DEC -1 / ( D3[5] = -1)
		DEC -1 / ( D3[6] = -1)
		DEC -1 / ( D3[7] = -1)
		DEC -1 / ( D3[8] = -1)
		DEC  6 / ( D4[4] =  6)

END

