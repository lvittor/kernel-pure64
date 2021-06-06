GLOBAL supports_cpuid
GLOBAL supports_vaesni
GLOBAL supports_vpclmulqdq
GLOBAL supports_f16c
GLOBAL supports_fma
GLOBAL supports_avx2

//https://www.youtube.com/watch?v=p5X1Sf5ejCc
supports_cpuid:
    push rbx            
    pushfq
    pop rax
    mov rbx, rax

    xor rax, 200000h   
    push rax            
    popfq               
                       
    pushfq              
    pop rax            
    cmp rax, rbx        
    jz no_cpuid         
    pop rbx             
    mov rax, 1          
    ret
no_cpuid:
    pop rbx
    mov rax, 0
    ret

supports_vaesni:
    mov eax, 1
    cpuid
    and ecx, 01A000000H
    cmp ecx, 01A000000H; check OSXSAVE AVX and AESNI feature flags
    jne not_supported
    ; processor supports AVX and VEX-encoded AESNI and XGETBV is enabled by OS
    mov ecx, 0; specify 0 for XCR0 register
    XGETBV ; result in EDX:EAX
    and eax, 06H
    cmp eax, 06H; check OS has enabled both XMM and YMM state support
    jne not_supported
    mov eax, 1
    jmp done

supports_vpclmulqdq:
    mov eax, 1
    cpuid
    and ecx, 018000002H
    cmp ecx, 018000002H; check OSXSAVE AVX and PCLMULQDQ feature flags
    jne not_supported
    ; processor supports AVX and VEX-encoded PCLMULQDQ and XGETBV is enabled by OS
    mov ecx, 0; specify 0 for XCR0 register
    XGETBV ; result in EDX:EAX
    and eax, 06H
    cmp eax, 06H; check OS has enabled both XMM and YMM state support
    jne not_supported
    mov eax, 1
    jmp done

supports_f16c:
    mov eax, 1
    cpuid
    and ecx, 038000000H
    cmp ecx, 038000000H; check OSXSAVE, AVX, F16C feature flags
    jne not_supported
; processor supports AVX,F16C instructions and XGETBV is enabled by OS
    mov ecx, 0; specify 0 for XCR0 register
    XGETBV; result in EDX:EAX
    and eax, 06H
    cmp eax, 06H; check OS has enabled both XMM and YMM state support
    jne not_supported
    mov eax, 1
    jmp done

supports_fma:
    mov eax, 1
    cpuid
    and ecx, 018001000H
    cmp ecx, 018001000H; check OSXSAVE, AVX, FMA feature flags
    jne not_supported
    ; processor supports AVX,FMA instructions and XGETBV is enabled by OS
    mov ecx, 0; specify 0 for XCR0 register
    XGETBV; result in EDX:EAX
    and eax, 06H
    cmp eax, 06H; check OS has enabled both XMM and YMM state support
    jne not_supported
    mov eax, 1
    jmp done

supports_avx2:
    mov eax, 1
    cpuid
    and ecx, 018000000H
    cmp ecx, 018000000H; check both OSXSAVE and AVX feature flags
    jne not_supported
; processor supports AVX instructions and XGETBV is enabled by OS
    mov eax, 7
    mov ecx, 0
    cpuid
    and ebx, 20H
    cmp ebx, 20H; check AVX2 feature flags
    jne not_supported
    mov ecx, 0; specify 0 for XCR0 register
    XGETBV; result in EDX:EAX
    and eax, 06H
    cmp eax, 06H; check OS has enabled both XMM and YMM state support
    jne not_supported
    mov eax, 1
    jmp done
not_supported:
    mov eax, 0
done:
    ret