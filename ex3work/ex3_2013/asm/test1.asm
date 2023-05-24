	ORG 10		/ program entry point
/ check (M[Y] = 0) ?
L0, 	CLE			/ E <- 0
	LDA Y		/ AC <- M[Y]
	SZA			/ (AC == 0) ? skip next
	BUN LY		/ goto LY
	HLT
/ M[Y] >>= 1
LY,	CIR			/ {AC[15:0], E} <- {E, AC[15:0]}
	STA Y		/ M[Y] <- AC
	SZE			/ (E == 0) ? skip next
	BUN LP		/ goto LP
/ M[X] <<= 1
LX,	LDA X		/ AC <- M[X]
	CIL			/ {E, AC[15:0]} <- {AC[15:0], E}
	STA X		/ M[X] <- AC
	BUN L0		/ goto LO
/ M[P] += M[X]
LP, LDA X		/ AC <- M[X]
	ADD P		/ AC <- AC + M[P]
	STA P		/ M[P} <- AC
	CLE			/ E <- 0
	BUN LX		/ goto LX
/ data
_M_,
X,	DEC 0		/ X = -70 --> X:init
_M_,
Y,	DEC 5		/ Y = 111 --> Y:init
P,	DEC 0		/ (init : 0) P = X * Y : result
VM2,	DEC -2
END

