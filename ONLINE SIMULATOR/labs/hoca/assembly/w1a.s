value  DCD     0xF0F1F3F4
result DCD     0

main   
       LDR     R0, =value
       LDR     R1, [R0]

       MOV     R2, #0x000000FF

       MOV     R6, R1
       AND     R6, R6, R2
       LSR     R5, R1, #8
       AND     R5, R5, R2
       LSR     R4, R1, #16
       AND     R4, R4, R2
       LSR     R3, R1, #24
       AND     R3, R3, R2

       MOV     R1, #00;
       ADD     R1, R1, R3
       ADD     R1, R1, R4
       ADD     R1, R1, R5
       ADD     R1, R1, R6

       LSR     R1, R1, #2
       ADR     R0, result
       STR     R1, [R0]

       end


