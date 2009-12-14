.globl _start
.globl _str

_strdat:
    .string "Hello, world!\n"

_str:
    .long _strdat

_start:
    mov $4, %eax
    mov $1, %ebx
    mov _str, %ecx
    mov $14, %edx
    int $0x80
    _loop:
    jmp _loop
