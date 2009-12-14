.globl _start

_start:
    mov $4, %eax
    mov $1, %ebx
    movl $_str, %ecx
    mov $14, %edx
    int $0x80
    _loop:
    jmp _loop
