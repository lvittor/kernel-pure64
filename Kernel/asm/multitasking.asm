GLOBAL create_task, init_task
GLOBAL setRSP, getRSP

; uint64_t create_task(uint64_t id, uint64_t baseRSP, uint64_t function) 
create_task:
    ; TODO: Elegir dónde pushear el stackframe inicial. Importa?
    push 0x0    ; SS
    push rsi    ; RSP
    push 0x206  ; RFLAGS
    push 0x8    ; CS
    push rdx    ; RIP

    ; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

    iretq

; void init_task(uint64_t RSP)
init_task:
    mov rsp, rdi
    iretq

; void setRSP(uint64_t newRSP)
setRSP:
    mov rsp, rdi
    ret

getRSP:
    mov rax, rsp
    ret