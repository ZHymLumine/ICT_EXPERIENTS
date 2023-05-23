	ORG 10 
CHK,	
		ISZ VM5
		BUN CHK_HEX
		BUN SUM
CHK_HEX,	
			LDA P1 I	
			INC
			ISZ P1
			ISZ C1
			SZA		/ if AC == 0, skip next
			BUN CHK	/ check next bit
			BUN ERR	/ ERROR!
SET_MSG,HEX 0
/ arg0 (AC) : message address
		STA PTR_MG		/ M[PTR_MG] <- arg0 (message address)
		ADD VM1			/ AC <- arg0 - 1
		STA TMA			/ M[TMA] <- arg0 - 1
		LDA TMA I		/ AC <- M[M[TMA]] (M[arg0 - 1] = message count)
		STA CNT			/ M[CNT] <- message count
		BUN SET_MSG I	/ return from SET_MSG
ERR,
		CLA				/ AC <- 0
		LDA A_EMG		/ AC <- M[A_EMG] (EMG)
		BSA SET_MSG		/ call SET_MSG (set message info)
		HLT
SUM,	
			LDA TMA
			CIL
			CIL
			CIL
			CIL
			SZE		/ if E == 0 skip next
			BUN ERR
			ADD P2 I
			ISZ P2
			STA TMA
			ISZ C2
			BUN SUM
			BUN CONVERT
CONVERT,	LDA TMA
			AND MSK
			STA RES 
			ISZ RES
STR_DGT,
			LDA TMA
			CIR
			CIR
			CIR
			CIR
			STA TMA
			ISZ C3
			BUN CONVERT
			HLT
VH3, HEX 3 / M[VH3] = 0x3
C1, DEC -4 / M[C1] = -4
C2, DEC -4 / M[C2] = -4
C3, DEC -4
C4, DEC -4
P1, SYM BUF / 
P2, SYM BUF / 
P3, SYM BUF /
VM1,	DEC -1
MSK,	HEX 000F
BUF, 	HEX 0
		HEX 7
		HEX e
		HEX f
RES, 	DEC 0
		DEC 0
		DEC 0
		DEC 0
TMA,	DEC 0		/ temporal
CNT,	DEC 0
PTR_MG, HEX 0		/ message pointer
A_EMG,	SYM EMG
CNT_EMG,DEC -5		/ CNT_EMG = -6
EMG,	HEX 65		/ 'e'
		HEX 72		/ 'r'
		HEX 72		/ 'r'
		HEX 6F		/ 'o'
		HEX 72		/ 'r'
	END 
