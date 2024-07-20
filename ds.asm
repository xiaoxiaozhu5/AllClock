DSSP  SEGMENT  CODE    

PUBLIC  DSS			  

RSEG  DSSP			  



IN EQU P1.6
FLAG1     EQU   05H   
TTL       EQU   07H   

INIT_1820:
SETB IN
NOP
CLR IN

MOV R1,#18;#3
TSR1:MOV R0,#107
DJNZ R0,$
DJNZ R1,TSR1
SETB IN
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
MOV R0,#0DEH;#25H
TSR2:
JNB IN,TSR3;wait
DJNZ R0,TSR2
LJMP TSR4 ;delay
TSR3:
SETB FLAG1 
LJMP TSR5
TSR4:
CLR FLAG1 
LJMP TSR7
TSR5:
MOV R7,#6
TSR6:
MOV R0,#117
DJNZ R0,$ 
DJNZ R7,TSR6
TSR7:
SETB IN
RET

DSS:
DS1820PRO:

SETB IN
LCALL INIT_1820
JB FLAG1,TSS2
jmp BCD33 
TSS2:
MOV A,#0CCH 
LCALL WRITE_1820
MOV A,#44H 
LCALL WRITE_1820
CALL  DL1MS 
SETB IN
LCALL INIT_1820
MOV A,#0CCH 
LCALL WRITE_1820
MOV A,#0BEH 
LCALL WRITE_1820
LCALL READ_18200
MOV A,TTL
MOV B,TTL-1
MOV C,B.0
RRC A
MOV C,B.1
RRC A
MOV C,B.2
RRC A
MOV C,B.3
RRC A
MOV TTL,A 
BCD33: 
JB FLAG1,BCD44
mov R7,#0FFH
JMP TORET
BCD44:
MOV R7,TTL
TORET:
RET

WRITE_1820:
MOV R2,#8
CLR C
WR1:
CLR IN
MOV R3,#36;#6
DJNZ R3,$
RRC A
MOV IN,C
MOV R3,#192;#23
DJNZ R3,$
SETB IN
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
DJNZ R2,WR1
SETB IN
RET

READ_18200:
MOV R4,#2 
MOV R1,#TTL 
RE00:
MOV R2,#8
RE01:
CLR C
SETB IN
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
CLR IN
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
SETB IN
MOV R3,#54;#9
RE10: 
DJNZ R3,RE10
MOV C,IN
MOV R3,#138;#23
RE20: 
DJNZ R3,RE20
RRC A
DJNZ R2,RE01
MOV @R1,A
DEC R1
DJNZ R4,RE00
RET

DL1MS:
	MOV R7,#6
DL1MS2:
    MOV R6,#255
	 DJNZ R6,$
	 DJNZ R7,DL1MS2
    RET 

END
