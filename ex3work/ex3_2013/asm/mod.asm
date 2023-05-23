    ORG 10
DIVIDE, 
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
        HLT
_M_,
A, HEX 0008 
MOD, HEX 0004 
NB, HEX 0000 
_M_,
R, HEX 0000
_M_,
Q, HEX 0000
K, DEC -16
END