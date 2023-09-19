CHECK:   prn -5
mcro ti
        sub @r5, StEaM
        inc K
endmcro
clr @r3
red   @r5
jsr  CHECK
ti
PUMP :  cmp @r2, 50
ti
lea PUMP , @r4

StEaM: .data 33, 51

