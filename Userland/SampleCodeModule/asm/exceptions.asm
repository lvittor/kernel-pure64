GLOBAL throwInvalidOpcodeException
GLOBAL throwDivisionByZeroException

throwInvalidOpcodeException:
    ud2
    ret ; no debería llegar acá, porque se reinicia la shell

throwDivisionByZeroException:
    mov rax, 0xFF
    mov cl, 0
    div cl
    ret