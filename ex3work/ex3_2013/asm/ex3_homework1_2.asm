	ORG 10
	LDA B
	SZA 
	BUN MOD
	HLT / divive by zero
	
MOD,	
	CLA
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
A,	HEX 0456
B,	HEX 0000
NB,	HEX 0000
R,	HEX 0000
Q, 	HEX 0000
K,	DEC -16
	END
	
