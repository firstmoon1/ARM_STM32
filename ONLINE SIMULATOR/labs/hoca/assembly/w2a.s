Mem_Addr1 EQU     0x3040
Mem_Addr2 EQU     0x4030

main      ;Fill   the memory content.
          LDR     R0, =Mem_Addr1
          LDR     R1, =0x0F1F2F3F
          STR     R1, [R0]
          ADD     R0, R0, #4
          LDR     R1, =0x4F5F6F7F
          STR     R1, [R0]
          ADD     R0, R0, #4
          LDR     R1, =0x8F9FAFBF
          STR     R1, [R0]
          ADD     R0, R0, #4
          LDR     R1, =0xCFDFEFFF
          STR     R1, [R0]

          ;revisitthe memory content, find their average
          LDR     R0, =Mem_Addr1

          ;reset  the total sum
          MOV     R1, #0x00
          LDR     R3, =Mem_Addr2
          STR     R1, [R3]

          MOV     R1, #0x04 ; counter

Loop1     
          LDR     R3, =0x000000FF ; mask
          LDR     R2, [R0]

          MOV     R4, R2
          AND     R4, R4, R3

          LSR     R5, R2, #8
          AND     R5, R5, R3

          LSR     R6, R2, #16
          AND     R6, R6, R3

          LSR     R7, R2, #24
          AND     R7, R7, R3

          ADD     R8, R4, R5
          ADD     R8, R8, R6
          ADD     R8, R8, R7

          LDR     R3, =Mem_Addr2
          LDR     R5, [R3]
          ADD     R8, R8, R5
          STR     R8, [R3]

          ADD     R0, R0, #0x04

          SUB     R1, R1, #0x01 ; R1 = R1 - 1
          CMP     R1, #0

          BGT     Loop1

          ;take   the average value
          LDR     R3, =Mem_Addr2
          LDR     R8, [R3]
          LSR     R8, R8, #0x04
          STR     R8, [R3]

end       B       end

