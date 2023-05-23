	ORG 10
	LDA B
	SZA 
	BUN CHK
	HLT / divive by zero
	
CHK,LDA B
	SNA 
	BUN INI /B:AC(15) = 0
	BUN L1 /B:AC(15) = 1
INI, CLA
	LDA B
	CMA
	INC
	STA NB
L0,	CLE
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
	BUN L0
	HLT
L1, LDA A
	SNA 
	BUN A0
	LDA A
	STA TMA
	LDA B
	STA TMB
	BUN A1
A0,	CLA	/A < B, quotient = 0, remainder = A
	STA Q
	LDA A
	STA R
	HLT
A1,	CLE     / compare A and B, if A < B, goto A0, else goto CALC
	LDA TMA
	CIL
	STA TMA
	CLE
	LDA TMB
	CIL 
	STA TMB
	SNA	/ AC(15) = 1 ? skip next
	BSA CHK_BIT0	/ AC(15) = 0
	SZA		/ AC = 0
	BSA CHK_BIT1	/ AC(15) = 1 
	ISZ K2
	BUN A1  / both TMA and TMB
	BUN EQ 
CHK_BIT0, 	HEX 0
	  		LDA TMA
	  		SPA	
	  		BUN CACL	/M[TMA]: AC(15) = 1
	  		CLA
          	BUN CHK_BIT0 I
CHK_BIT1, 	HEX 0
	  		LDA TMA
          	SNA
          	BUN A0	/ A < B, jump to A0
	  		BUN CHK_BIT1 I 
CACL,	CLA
		ADD VH1
		STA Q
		LDA A		/ A > B, convert  most significant bit to 0 
		AND MSK
	　	STA A
		LDA B
		AND MSK
		CMA
		INC
		ADD A
		STA R
		HLT
EQ,		CLA
		STA R
		ADD VH1
		STA Q	
		HLT
A,	HEX f4e6
B,	HEX 7fed
TMA,	HEX 0000
TMB,	HEX 0000
MSK,	HEX 7fff
NB,	HEX 0000
R,	HEX 0000
Q, 	HEX 0000
K,	DEC -16
K2,	DEC -16
VH1,	HEX 0001
	END
	
