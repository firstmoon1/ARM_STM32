Mem_Addr1 EQU     0x20000040
Mem_Addr2 EQU     0x30000040

main      
          LDR     R0, =Mem_Addr1

          ;Fill   the memory content
          LDR     R1, =0xF0F1F2F3
          STR     R1, [R0]
          ADD     R0, R0, #4
          LDR     R1, =0xF4F5F6F7
          STR     R1, [R0]
          ADD     R0, R0, #4
          LDR     R1, =0xF8F9FAFB
          STR     R1, [R0]
          ADD     R0, R0, #4
          LDR     R1, =0xFCFDFEFF
          STR     R1, [R0]

          ;revisitthe memory content
          LDR     R0, =Mem_Addr1

          ;reset  the total sum
          MOV     R1, #0x00
          LDR     R3, =Mem_Addr2
          STR     R1, [R3]

          MOV     R1, #0x04
Loop1     

          LDR     R3, =0x00000001

          LDR     R2, [R0]

          MOV     R4, R2
          AND     R4, R4, R3

          LSR     R5, R2, #8
          AND     R5, R5, R3

          LSR     R6, R2, #16
          AND     R6, R6, R3

          LSR     R7, R2, #24
          AND     R7, R7, R3

          ADD     R4, R4, R5
          ADD     R4, R4, R6
          ADD     R4, R4, R7

          LDR     R3, =Mem_Addr2
          LDR     R5, [R3]
          ADD     R4, R4, R5
          STR     R4, [R3]

          ADD     R0, R0, #0x04

          SUB     R1, R1, #0x01 ; R1 = R1 - 1
          CMP     R1, #0
          BGT     Loop1



end       B       end

