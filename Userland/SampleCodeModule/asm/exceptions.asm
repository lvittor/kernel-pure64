GLOBAL throwInvalidOpcodeException
GLOBAL throwDivisionByZeroException

throwInvalidOpcodeException:
    ud2 ; Undefined instruction: raises invalid opcode
    ret

throwDivisionByZeroException:
    mov rax, 0xFF
    mov cl, 0
    div cl
    ret