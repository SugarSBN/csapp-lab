
test:     file format elf64-x86-64


Disassembly of section .init:

0000000000001000 <_init>:
    1000:	f3 0f 1e fa          	endbr64 
    1004:	48 83 ec 08          	sub    $0x8,%rsp
    1008:	48 8b 05 d9 2f 00 00 	mov    0x2fd9(%rip),%rax        # 3fe8 <__gmon_start__>
    100f:	48 85 c0             	test   %rax,%rax
    1012:	74 02                	je     1016 <_init+0x16>
    1014:	ff d0                	call   *%rax
    1016:	48 83 c4 08          	add    $0x8,%rsp
    101a:	c3                   	ret    

Disassembly of section .plt:

0000000000001020 <.plt>:
    1020:	ff 35 a2 2f 00 00    	push   0x2fa2(%rip)        # 3fc8 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:	f2 ff 25 a3 2f 00 00 	bnd jmp *0x2fa3(%rip)        # 3fd0 <_GLOBAL_OFFSET_TABLE_+0x10>
    102d:	0f 1f 00             	nopl   (%rax)

Disassembly of section .plt.got:

0000000000001030 <__cxa_finalize@plt>:
    1030:	f3 0f 1e fa          	endbr64 
    1034:	f2 ff 25 bd 2f 00 00 	bnd jmp *0x2fbd(%rip)        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    103b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

Disassembly of section .text:

0000000000001040 <_start>:
    1040:	f3 0f 1e fa          	endbr64 
    1044:	31 ed                	xor    %ebp,%ebp
    1046:	49 89 d1             	mov    %rdx,%r9
    1049:	5e                   	pop    %rsi
    104a:	48 89 e2             	mov    %rsp,%rdx
    104d:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
    1051:	50                   	push   %rax
    1052:	54                   	push   %rsp
    1053:	4c 8d 05 66 01 00 00 	lea    0x166(%rip),%r8        # 11c0 <__libc_csu_fini>
    105a:	48 8d 0d ef 00 00 00 	lea    0xef(%rip),%rcx        # 1150 <__libc_csu_init>
    1061:	48 8d 3d da 00 00 00 	lea    0xda(%rip),%rdi        # 1142 <main>
    1068:	ff 15 72 2f 00 00    	call   *0x2f72(%rip)        # 3fe0 <__libc_start_main@GLIBC_2.2.5>
    106e:	f4                   	hlt    
    106f:	90                   	nop

0000000000001070 <deregister_tm_clones>:
    1070:	48 8d 3d 99 2f 00 00 	lea    0x2f99(%rip),%rdi        # 4010 <__TMC_END__>
    1077:	48 8d 05 92 2f 00 00 	lea    0x2f92(%rip),%rax        # 4010 <__TMC_END__>
    107e:	48 39 f8             	cmp    %rdi,%rax
    1081:	74 15                	je     1098 <deregister_tm_clones+0x28>
    1083:	48 8b 05 4e 2f 00 00 	mov    0x2f4e(%rip),%rax        # 3fd8 <_ITM_deregisterTMCloneTable>
    108a:	48 85 c0             	test   %rax,%rax
    108d:	74 09                	je     1098 <deregister_tm_clones+0x28>
    108f:	ff e0                	jmp    *%rax
    1091:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    1098:	c3                   	ret    
    1099:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000000010a0 <register_tm_clones>:
    10a0:	48 8d 3d 69 2f 00 00 	lea    0x2f69(%rip),%rdi        # 4010 <__TMC_END__>
    10a7:	48 8d 35 62 2f 00 00 	lea    0x2f62(%rip),%rsi        # 4010 <__TMC_END__>
    10ae:	48 29 fe             	sub    %rdi,%rsi
    10b1:	48 89 f0             	mov    %rsi,%rax
    10b4:	48 c1 ee 3f          	shr    $0x3f,%rsi
    10b8:	48 c1 f8 03          	sar    $0x3,%rax
    10bc:	48 01 c6             	add    %rax,%rsi
    10bf:	48 d1 fe             	sar    %rsi
    10c2:	74 14                	je     10d8 <register_tm_clones+0x38>
    10c4:	48 8b 05 25 2f 00 00 	mov    0x2f25(%rip),%rax        # 3ff0 <_ITM_registerTMCloneTable>
    10cb:	48 85 c0             	test   %rax,%rax
    10ce:	74 08                	je     10d8 <register_tm_clones+0x38>
    10d0:	ff e0                	jmp    *%rax
    10d2:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    10d8:	c3                   	ret    
    10d9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000000010e0 <__do_global_dtors_aux>:
    10e0:	f3 0f 1e fa          	endbr64 
    10e4:	80 3d 25 2f 00 00 00 	cmpb   $0x0,0x2f25(%rip)        # 4010 <__TMC_END__>
    10eb:	75 2b                	jne    1118 <__do_global_dtors_aux+0x38>
    10ed:	55                   	push   %rbp
    10ee:	48 83 3d 02 2f 00 00 	cmpq   $0x0,0x2f02(%rip)        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    10f5:	00 
    10f6:	48 89 e5             	mov    %rsp,%rbp
    10f9:	74 0c                	je     1107 <__do_global_dtors_aux+0x27>
    10fb:	48 8b 3d 06 2f 00 00 	mov    0x2f06(%rip),%rdi        # 4008 <__dso_handle>
    1102:	e8 29 ff ff ff       	call   1030 <__cxa_finalize@plt>
    1107:	e8 64 ff ff ff       	call   1070 <deregister_tm_clones>
    110c:	c6 05 fd 2e 00 00 01 	movb   $0x1,0x2efd(%rip)        # 4010 <__TMC_END__>
    1113:	5d                   	pop    %rbp
    1114:	c3                   	ret    
    1115:	0f 1f 00             	nopl   (%rax)
    1118:	c3                   	ret    
    1119:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001120 <frame_dummy>:
    1120:	f3 0f 1e fa          	endbr64 
    1124:	e9 77 ff ff ff       	jmp    10a0 <register_tm_clones>

0000000000001129 <_Z1Qi>:
    1129:	f3 0f 1e fa          	endbr64 
    112d:	8d 04 3f             	lea    (%rdi,%rdi,1),%eax
    1130:	c3                   	ret    

0000000000001131 <_Z1Pi>:
    1131:	f3 0f 1e fa          	endbr64 
    1135:	53                   	push   %rbx
    1136:	8d 5f 01             	lea    0x1(%rdi),%ebx
    1139:	e8 eb ff ff ff       	call   1129 <_Z1Qi>
    113e:	01 d8                	add    %ebx,%eax
    1140:	5b                   	pop    %rbx
    1141:	c3                   	ret    

0000000000001142 <main>:
    1142:	f3 0f 1e fa          	endbr64 
    1146:	b8 00 00 00 00       	mov    $0x0,%eax
    114b:	c3                   	ret    
    114c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000001150 <__libc_csu_init>:
    1150:	f3 0f 1e fa          	endbr64 
    1154:	41 57                	push   %r15
    1156:	4c 8d 3d 93 2c 00 00 	lea    0x2c93(%rip),%r15        # 3df0 <__frame_dummy_init_array_entry>
    115d:	41 56                	push   %r14
    115f:	49 89 d6             	mov    %rdx,%r14
    1162:	41 55                	push   %r13
    1164:	49 89 f5             	mov    %rsi,%r13
    1167:	41 54                	push   %r12
    1169:	41 89 fc             	mov    %edi,%r12d
    116c:	55                   	push   %rbp
    116d:	48 8d 2d 84 2c 00 00 	lea    0x2c84(%rip),%rbp        # 3df8 <__do_global_dtors_aux_fini_array_entry>
    1174:	53                   	push   %rbx
    1175:	4c 29 fd             	sub    %r15,%rbp
    1178:	48 83 ec 08          	sub    $0x8,%rsp
    117c:	e8 7f fe ff ff       	call   1000 <_init>
    1181:	48 c1 fd 03          	sar    $0x3,%rbp
    1185:	74 1f                	je     11a6 <__libc_csu_init+0x56>
    1187:	31 db                	xor    %ebx,%ebx
    1189:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    1190:	4c 89 f2             	mov    %r14,%rdx
    1193:	4c 89 ee             	mov    %r13,%rsi
    1196:	44 89 e7             	mov    %r12d,%edi
    1199:	41 ff 14 df          	call   *(%r15,%rbx,8)
    119d:	48 83 c3 01          	add    $0x1,%rbx
    11a1:	48 39 dd             	cmp    %rbx,%rbp
    11a4:	75 ea                	jne    1190 <__libc_csu_init+0x40>
    11a6:	48 83 c4 08          	add    $0x8,%rsp
    11aa:	5b                   	pop    %rbx
    11ab:	5d                   	pop    %rbp
    11ac:	41 5c                	pop    %r12
    11ae:	41 5d                	pop    %r13
    11b0:	41 5e                	pop    %r14
    11b2:	41 5f                	pop    %r15
    11b4:	c3                   	ret    
    11b5:	66 66 2e 0f 1f 84 00 	data16 nopw %cs:0x0(%rax,%rax,1)
    11bc:	00 00 00 00 

00000000000011c0 <__libc_csu_fini>:
    11c0:	f3 0f 1e fa          	endbr64 
    11c4:	c3                   	ret    

Disassembly of section .fini:

00000000000011c8 <_fini>:
    11c8:	f3 0f 1e fa          	endbr64 
    11cc:	48 83 ec 08          	sub    $0x8,%rsp
    11d0:	48 83 c4 08          	add    $0x8,%rsp
    11d4:	c3                   	ret    
