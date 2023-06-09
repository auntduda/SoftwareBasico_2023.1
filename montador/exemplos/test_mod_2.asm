MOD_2:  BEGIN
EXTERN: MOD_3
        PUBLIC MOD_2
        PUBLIC N2
        SECTION TEXT
        INPUT N2
        JMP MOD_3
        SECTION DATA
N2:     SPACE
        END
