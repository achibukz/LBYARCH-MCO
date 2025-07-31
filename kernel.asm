bits 64
default rel
global asmKernel

section .text
asmKernel:
    ; rcx = x1, rdx = x2
    ; r8  = y1, r9  = y2
    ; [rsp+8]  = z
    ; [rsp+16] = n (multiple of 4)

    mov r10, [rsp + 40]      ; z
    mov r11d, [rsp + 48]     ; n
    xor eax, eax             ; i = 0

eucl:
    cmp eax, r11d
    jge end

    movaps xmm0, [rdx + rax * 4]
    subps  xmm0, [rcx + rax * 4]
    movaps xmm1, xmm0
    mulps  xmm1, xmm0

    movaps xmm2, [r9 + rax * 4]
    subps  xmm2, [r8 + rax * 4]
    movaps xmm3, xmm2
    mulps  xmm3, xmm2

    addps  xmm1, xmm3
    sqrtps xmm1, xmm1

    movaps [r10 + rax * 4], xmm1

    add eax, 4
    jmp eucl

end:
    ret
