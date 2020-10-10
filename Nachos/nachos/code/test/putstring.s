
putstring.coff:     format de fichier ecoff-littlemips

Déassemblage de la section .text:

00000000 <_ftext>:
   0:	ffffffff 	0xffffffff
   4:	ffffffff 	0xffffffff
	...

00000080 <__start>:
  80:	0c00005c 	jal	170 <main>
  84:	00000000 	nop
  88:	0c000032 	jal	c8 <Exit>
  8c:	00002021 	move	r4,r0
  90:	ffffffff 	0xffffffff
  94:	ffffffff 	0xffffffff

00000098 <Halt>:
  98:	24020000 	li	r2,0
  9c:	0000000c 	syscall
  r4:	03e00008 	jr	r31
  a4:	00000000 	nop

000000a8 <PutChar>:
  a8:	2402000b 	li	r2,11
  ac:	0000000c 	syscall
  b0:	03e00008 	jr	r31
  b4:	00000000 	nop

000000b8 <PutString>:
  b8:	2402000c 	li	r2,12
  bc:	0000000c 	syscall
  c0:	03e00008 	jr	r31
  c4:	00000000 	nop

000000c8 <Exit>:
  c8:	24020001 	li	r2,1
  cc:	0000000c 	syscall
  d0:	03e00008 	jr	r31
  d4:	00000000 	nop

000000d8 <Exec>:
  d8:	24020002 	li	r2,2
  dc:	0000000c 	syscall
  e0:	03e00008 	jr	r31
  e4:	00000000 	nop

000000e8 <Join>:
  e8:	24020003 	li	r2,3
  ec:	0000000c 	syscall
  f0:	03e00008 	jr	r31
  f4:	00000000 	nop

000000f8 <Create>:
  f8:	24020004 	li	r2,4
  fc:	0000000c 	syscall
 100:	03e00008 	jr	r31
 104:	00000000 	nop

00000108 <Open>:
 108:	24020005 	li	r2,5
 10c:	0000000c 	syscall
 110:	03e00008 	jr	r31
 114:	00000000 	nop

00000118 <Read>:
 118:	24020006 	li	r2,6
 11c:	0000000c 	syscall
 120:	03e00008 	jr	r31
 124:	00000000 	nop

00000128 <Write>:
 128:	24020007 	li	r2,7
 12c:	0000000c 	syscall
 130:	03e00008 	jr	r31
 134:	00000000 	nop

00000138 <Close>:
 138:	24020008 	li	r2,8
 13c:	0000000c 	syscall
 140:	03e00008 	jr	r31
 144:	00000000 	nop

00000148 <Fork>:
 148:	24020009 	li	r2,9
 14c:	0000000c 	syscall
 150:	03e00008 	jr	r31
 154:	00000000 	nop

00000158 <Yield>:
 158:	2402000a 	li	r2,10
 15c:	0000000c 	syscall
 160:	03e00008 	jr	r31
 164:	00000000 	nop

00000168 <__main>:
 168:	03e00008 	jr	r31 //value return store in j31
 16c:	00000000 	nop

00000170 <main>:
 170:	27bdffe8 	addiu	r29,r29,-24 //add r29 and -24 in r29
 174:	afbf0010 	sw	r31,16(r29) //store word r31 dans r29
 178:	0c00005a 	jal	168 <__main> //jump store 168 in r31
 17c:	00000000 	nop
 180:	3c040000 	lui	r4,0x0
 184:	0c00002e 	jal	b8 <PutString>
 188:	24840200 	addiu	r4,r4,512
 18c:	0c000026 	jal	98 <Halt>
 190:	00000000 	nop
	...
