section .text

global FastStrcpy

FastStrcpy:
    movdqa xmm0, [rsi]
    movdqa xmm1, [rsi + 16]
    movdqa [rdi], xmm0
    movdqa [rdi + 16], xmm1
    mov rax, rdi
    ret