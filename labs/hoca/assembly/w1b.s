value  DCD     0xF7F6F5F4
even   DCD     0
odd    DCD     0


main   
       LDR     R0, =value
       ;       LDR R1, [R0]
       LDR     R2, [R0]

       LDR     R3, =0x00000001

       MOV     R4, R2
       AND     R4, R4, R3

       LSR     R5, R2, #8
       AND     R5, R5, R3

       LSR     R6, R2, #16
       AND     R6, R6, R3

       LSR     R7, R2, #24
       AND     R7, R7, R3


       MOV     R1, #00;
       ADD     R1, R1, R4
       ADD     R1, R1, R5
       ADD     R1, R1, R6
       ADD     R1, R1, R7

       MOV     R3, #04
       STR     R1, [R0, R3]

       MOV     R2, #04
       SUB     R2, R2, R1

       ADD     R3, R3, #0x04
       STR     R2, [R0, R3]
       end


