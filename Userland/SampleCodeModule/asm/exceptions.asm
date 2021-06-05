GLOBAL throwInvalidOpcodeException
GLOBAL throwDivisionByZeroException

throwInvalidOpcodeException:
    mov rax, 0xFEDCBA9876543210
    push rax
    ud2 ; Undefined instruction: raises invalid opcode
    ret ; no debería llegar acá, porque se reinicia la shell

throwDivisionByZeroException:
    mov rax, 0xFF
    mov cl, 0
    div cl
    ret