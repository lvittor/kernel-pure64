GLOBAL cpuVendor
GLOBAL getYear, getMonth, getDay
GLOBAL getHour, getMinute, getSecond
GLOBAL exchange
GLOBAL getKey
section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

; RTC functions
getYear:
    mov al, 0x09
    out 70h, al
    in al, 71h
	ret

getMonth:
    mov al, 0x08
    out 70h, al
    in al, 71h
	ret

getDay:
    mov al, 0x07
    out 70h, al
    in al, 71h
	ret

getHour:
    mov al, 0x04
    out 70h, al
    in al, 71h
	ret

getMinute:
    mov al, 0x02
    out 70h, al
    in al, 71h
	ret

getSecond:
    mov al, 0x00
    out 70h, al
    in al, 71h
	ret

getKey:
	xor rax, rax
.loop:
	in al, 0x64
	and al, 0x01
    cmp al, 0
	je .loop

	in al, 0x60

	ret

exchange:  ;; int64_t exchange(int64_t mutex, int64_t value);
    mov rax, rsi
    xchg [rdi], eax
    ret