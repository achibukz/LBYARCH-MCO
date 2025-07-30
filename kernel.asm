bits 64
default rel
global asmKernel
section .text

asmKernel:
    ; RCX = x1
    ; RDX = x2
    ; R8  = y1
    ; R9  = y2

    extern z_asm
    lea r10, [rel z_asm]    ; r10 = z

    xor eax, eax            ; i = 0

eucl:
    cmp eax, 4
    jge end

    movss xmm0, [rdx + rax * 4]
    subss xmm0, [rcx + rax * 4]
    movaps xmm1, xmm0
    mulss xmm1, xmm0

    movss xmm2, [r9 + rax * 4]
    subss xmm2, [r8 + rax * 4]
    movaps xmm3, xmm2
    mulss xmm3, xmm2

    addss xmm1, xmm3
    sqrtss xmm1, xmm1

    movss [r10 + rax * 4], xmm1

    inc eax
    jmp eucl

end:
    mov rax, r10        ; return z
    ret
