	.file	1 "/theyer/projects/theyergfx/jetracer2/library/binary/binary.c"
gcc2_compiled.:
__gnu_compiled_c:
	.text
	.p2align 3
	.globl	binReadByte
	.text
	.ent	binReadByte
binReadByte:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	j	$31
	lbu	$2,0($4)
	.set	macro
	.set	reorder

	.end	binReadByte
	.p2align 3
	.globl	binWriteByte
	.text
	.ent	binWriteByte
binWriteByte:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	sb	$4,0($5)
	.set	noreorder
	.set	nomacro
	j	$31
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

	.end	binWriteByte
	.p2align 3
	.globl	binWriteString
	.text
	.ent	binWriteString
binWriteString:
	.frame	$sp,64,$31		# vars= 0, regs= 8/0, args= 0, extra= 0
	.mask	0x80070000,-16
	.fmask	0x00000000,0
	subu	$sp,$sp,64
	sd	$18,32($sp)
	sd	$17,16($sp)
	move	$18,$6
	sd	$16,0($sp)
	move	$17,$5
	move	$16,$4
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L5
	sd	$31,48($sp)
	.set	macro
	.set	reorder

	jal	strlen
	.set	noreorder
	.set	nomacro
	b	$L6
	move	$6,$2
	.set	macro
	.set	reorder

	.p2align 2
$L5:
	move	$6,$0
$L6:
	.set	noreorder
	.set	nomacro
	blez	$17,$L14
	move	$5,$0
	.set	macro
	.set	reorder

	move	$4,$16
	.p2align 3
$L10:
	slt	$2,$5,$6
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L11
	move	$2,$0
	.set	macro
	.set	reorder

	lb	$2,0($4)
$L11:
	addu	$3,$18,$5
	addu	$5,$5,1
	sb	$2,0($3)
	slt	$2,$5,$17
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L10
	addu	$4,$4,1
	.set	macro
	.set	reorder

$L14:
	move	$2,$17
	ld	$31,48($sp)
	ld	$18,32($sp)
	ld	$17,16($sp)
	ld	$16,0($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,64
	.set	macro
	.set	reorder

	.end	binWriteString
	.section	.bss
str.12:
	.align	3
	.space	256
	.previous
	.p2align 3
	.globl	binReadString
	.text
	.ent	binReadString
binReadString:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	lbu	$2,0($5)
	move	$7,$4
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L22
	move	$6,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	blez	$7,$L22
	lui	$8,%hi(str.12) # high
	.set	macro
	.set	reorder

	move	$4,$5
	addiu	$3,$8,%lo(str.12) # low
	.p2align 3
$L24:
	lbu	$4,0($4)
	addu	$3,$6,$3
	addu	$6,$6,1
	sb	$4,0($3)
	addu	$2,$5,$6
	move	$4,$2
	lbu	$3,0($4)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L17
	slt	$2,$6,$7
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L17
	slt	$2,$6,255
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L24
	addiu	$3,$8,%lo(str.12) # low
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	b	$L23
	addiu	$2,$8,%lo(str.12) # low
	.set	macro
	.set	reorder

	.p2align 2
$L22:
	lui	$8,%hi(str.12) # high
$L17:
	addiu	$2,$8,%lo(str.12) # low
$L23:
	addu	$3,$6,$2
	.set	noreorder
	.set	nomacro
	j	$31
	sb	$0,0($3)
	.set	macro
	.set	reorder

	.end	binReadString
	.p2align 3
	.globl	binReadInt
	.text
	.ent	binReadInt
binReadInt:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	lbu	$6,3($4)
	subu	$sp,$sp,16
	lbu	$3,1($4)
	lbu	$5,2($4)
	lbu	$2,0($4)
	sb	$3,1($sp)
	sb	$2,0($sp)
	sb	$5,2($sp)
	sb	$6,3($sp)
	lw	$2,0($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,16
	.set	macro
	.set	reorder

	.end	binReadInt
	.p2align 3
	.globl	binReadShort
	.text
	.ent	binReadShort
binReadShort:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	lbu	$2,1($4)
	subu	$sp,$sp,16
	lbu	$3,0($4)
	sb	$2,1($sp)
	sb	$3,0($sp)
	lh	$2,0($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,16
	.set	macro
	.set	reorder

	.end	binReadShort
	.p2align 3
	.globl	binReadUShort
	.text
	.ent	binReadUShort
binReadUShort:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	lbu	$2,1($4)
	lbu	$3,0($4)
	sll	$2,$2,8
	.set	noreorder
	.set	nomacro
	j	$31
	or	$2,$3,$2
	.set	macro
	.set	reorder

	.end	binReadUShort
	.p2align 3
	.globl	binReadFloat
	.text
	.ent	binReadFloat
binReadFloat:
	.frame	$sp,16,$31		# vars= 0, regs= 2/0, args= 0, extra= 0
	.mask	0x80000000,-16
	.fmask	0x00000000,0
	subu	$sp,$sp,16
	sd	$31,0($sp)
	jal	binReadInt
	mtc1	$2,$f1
	cvt.s.w	$f1,$f1
	li.s	$f0,1.00000000000000000000e3
	ld	$31,0($sp)
	.set	noreorder
	nop
	nop
	div.s	$f0,$f1,$f0
	.set	reorder
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,16
	.set	macro
	.set	reorder

	.end	binReadFloat
	.p2align 3
	.globl	binReadPs2Float
	.text
	.ent	binReadPs2Float
binReadPs2Float:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	j	$31
	l.s	$f0,0($4)
	.set	macro
	.set	reorder

	.end	binReadPs2Float
	.p2align 3
	.globl	binWriteShort
	.text
	.ent	binWriteShort
binWriteShort:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	srl	$2,$4,8
	sb	$4,0($5)
	sb	$2,1($5)
	.set	noreorder
	.set	nomacro
	j	$31
	li	$2,2			# 0x2
	.set	macro
	.set	reorder

	.end	binWriteShort
	.p2align 3
	.globl	binWriteUShort
	.text
	.ent	binWriteUShort
binWriteUShort:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	andi	$4,$4,0xffff
	srl	$2,$4,8
	sb	$4,0($5)
	sb	$2,1($5)
	.set	noreorder
	.set	nomacro
	j	$31
	li	$2,2			# 0x2
	.set	macro
	.set	reorder

	.end	binWriteUShort
	.p2align 3
	.globl	binWriteInt
	.text
	.ent	binWriteInt
binWriteInt:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	sra	$2,$4,24
	srl	$3,$4,8
	srl	$6,$4,16
	sb	$2,3($5)
	sb	$3,1($5)
	li	$2,4			# 0x4
	sb	$6,2($5)
	.set	noreorder
	.set	nomacro
	j	$31
	sb	$4,0($5)
	.set	macro
	.set	reorder

	.end	binWriteInt
	.p2align 3
	.globl	binWriteFloat
	.text
	.ent	binWriteFloat
binWriteFloat:
	.frame	$sp,16,$31		# vars= 0, regs= 2/0, args= 0, extra= 0
	.mask	0x80000000,-16
	.fmask	0x00000000,0
	li.s	$f0,1.00000000000000000000e3
	move	$5,$4
	subu	$sp,$sp,16
	mul.s	$f12,$f12,$f0
	sd	$31,0($sp)
	cvt.w.s $f0,$f12
	mfc1	$4,$f0
	jal	binWriteInt
	ld	$31,0($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,16
	.set	macro
	.set	reorder

	.end	binWriteFloat
