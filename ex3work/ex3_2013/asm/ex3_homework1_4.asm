ORG 10
    LDA N
    INC
    STA DECENT
L0, LDA DECENT /decrease number
    ADD MONE
    STA DECENT
    BUN DEV /go to DEV
 
DEV, LDA D /Inclement divider and go to sub routine to execute dividing
     INC
     STA D
     BSA SH1
     LDA R    	/If R = 0, skip and back to L0
     SZA
     BUN CHK  	/If R != 0, back to DEV(necessary to check DECENT>=D*D)
     LDA ONE
     STA D
     BUN L0
 
CHK, LDA D
     STA MD1
     STA MD2
     BSA MUL
     LDA MD3
     ADD MONE
     CMA
     ADD DECENT
     SZE
     BUN DEV
     HLT

SH1, HEX 0 /sub routine, zyouyozan
     LDA Z /-initialization-
     STA ND
     STA Q
     STA R
     LDA KB
     STA K /----------------
     LDA DECENT
     STA A
     LDA D
     CMA
     INC
     STA ND
SH2, CLE
     LDA A
     CIL
     STA A
     LDA R
     CIL
     STA R
     ADD ND
	 SNA
     STA R
     LDA Q
     CIL
     STA Q
     ISZ K  
     BUN SH2
     BUN SH1 I
 
MUL, HEX 0 /multiply
         CLE
         LDA MK2
         STA MK
         LDA Z
         STA MD3
MUL0,LDA MD2
          CIR
          STA MD2
          SZE
          BUN MUL1
          BUN MUL2
MUL1,LDA MD3
          ADD MD1
          STA MD3
          CLE
MUL2,LDA MD1
          CIL
          STA MD1
          ISZ MK
          BUN MUL0
          BUN MUL I    	 
 	
N, DEC 65535
D, DEC 1 /devider
DECENT, DEC 10 /descent, max prime
 
MD1, DEC 0/--multiply--
MD2, DEC 0
MD3, DEC 0/
MK, DEC -16
MK2, DEC -16/--------------------
 
A, HEX 0 /--zyouyozan--
ND, HEX 0
Q, HEX 0
R, HEX 0
K, DEC -16
KB, DEC -16/-----------
Z, HEX 0/zero
ONE, DEC 1/one
MONE, DEC -1/minus one
	END


