GLOBAL print
GLOBAL setReg
GLOBAL fillDate

print:
    mov rax, 1
    int 80h
    ret

fillDate:
    mov rax, 3
    int 80h
    ret

setReg:
    mov rax, 0xFFFF
    mov rbx, 0xFFF0
    mov rcx, 0xFF00
    mov rdx, 0xF000
    mov r8,  0xFF00
    mov r9,  0xFFF0
    mov r10, 0xFFFF
    mov r11, 0xFFF0
    mov r12, 0xFF00
    mov r13, 0xF000
    mov r14, 0xFF00
    mov r15, 0xFFF0
    div cl
    mov ax, cx
    ret