
test.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <.text>:
   0:	50                   	push   %rax
   1:	53                   	push   %rbx
   2:	51                   	push   %rcx
   3:	83 c3 10             	add    $0x10,%ebx
   6:	48 87 e6             	xchg   %rsp,%rsi
   9:	48 01 c7             	add    %rax,%rdi
