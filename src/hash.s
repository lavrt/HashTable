section .text
global djb2_asm

djb2_asm:
    xor   eax, eax              ; hash = 0
.next_char:
    movzx edx, byte [rdi]       ; load byte
    test  edx, edx              ; \0?
    jz    .done
    lea   eax, [eax * 8]        ; hash << 3
    lea   eax, [eax + eax * 4]  ; hash = (hash << 3) * 5 = hash * 33
    add   eax, edx              ; + current char
    inc   rdi                   ; s++
    jmp   .next_char
.done:
    ret

