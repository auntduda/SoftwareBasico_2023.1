MOD_B:  BEGIN
EXTERN: N1
EXTERN: FINISH
        PUBLIC MOD_B
        SECTION TEXT
        INPUT N2
        LOAD N1
        ADD N2
        STORE N2
        OUTPUT N2
        JMP FINISH
        SECTION DATA
N2:     SPACE
        END
