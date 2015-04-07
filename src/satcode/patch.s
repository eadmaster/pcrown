num_functions:       .long 4
func1_offset:        .long 0x6E640
func1_max_size:      .long 0x2B4
func1_patch_offset:  .long _DrawDialogText
func1_patch_size:    .long off_60728F0-_DrawDialogText+4
func2_offset:        .long 0x6E95C
func2_max_size:      .long 0x74
func2_patch_offset:  .long CalcNextCharacterDrawPosition
func2_patch_size:    .long dword_60729CC-CalcNextCharacterDrawPosition+4
func3_offset:        .long 0x6FF5C
func3_max_size:      .long 0x17C
func3_patch_offset:  .long QueryChoice
func3_patch_size:    .long off_60740D4-QueryChoice+4
func4_offset:        .long 0x7020C
func4_max_size:      .long 0xF0
func4_patch_offset:  .long _DrawChoiceSelection
func4_patch_size:    .long off_60742F8-_DrawChoiceSelection+4

filename:            .ascii "0.BIN"

.align 2

! Should be no larger than 0x2B4 bytes. (0x060728F4-0x06072640)

_DrawDialogText:                        ! CODE XREF: sub_60723C4+CCp
                                        ! sub_60731D0+2CCp
                                        ! sub_6073F5C+DEp
                                        ! sub_6074300+2BAp
                                        ! DATA XREF: RAM:off_6072530o
                                        ! RAM:off_60734D0o
                                        ! RAM:off_60740C4o
                                        ! RAM:off_6074638o
                mov.l   r8, @-r15
                mov.l   r9, @-r15
                mov.l   r10, @-r15
                mov.l   r11, @-r15
                mov.l   r12, @-r15
                mov.l   r13, @-r15
                mov.l   r14, @-r15
                sts.l   pr, @-r15
                ! Set stack position(new stack usage is 0x14 bytes)
                !add     #-0x10, r15
                add     #-0x14, r15
                mov     r15, r14
                mov     #0, r0
                mov.l   r0, @(4,r14)
                mov     #0, r12
                mov     #0, r6
                mov.l   off_60727E8, r1 ! _queue_function_ptr
                mov.l   @r1, r1
                mov     #0x5F, r7
                mov.l   off_60727EC, r3 ! unk_60DA4A0
                mov     #0, r2
                mov.l   r1, @(8,r14)    ! ?
                mov     r1, r0
                add     #88, r0         ! ?
                mov.l   r0, @(8,r14)
                mov     #0, r0
                mov.l   r0, @r14

loc_6072672:                            ! CODE XREF: sub_6072640+40j
                mov.l   @r14, r1
                add     r3, r1
                mov.b   r2, @r1
                mov.l   @r14, r1
                add     #1, r1
                !extu.b  r1, r1
                cmp/hi  r7, r1
                bf/s    loc_6072672
                mov.l   r1, @r14
                mov.l   @(8,r14), r0
                mov.w   @r0, r2         ! read character from text string
                mov     #0x12, r1

                mov.l   dword_60727F0, r13 ! 0x25E6B800 ! font address                
                
                ! Copy width entry to stack for now
                mov     r2, r3
                add     r13, r3
                mov.b   @r3, r3
                shlr    r3
                mov.l   r3, @(0x10,r14)
                
                mulu    r1, r2          ! Calculate font offset from character                
                sts     macl, r1
                add     r1, r13
                mov     r0, r1          ! Add address to offset
                add     #0x1E, r1
                mov.b   @r1, r1
                movt    r2
                add     #-2, r1
                extu.b  r1, r1
                cmp/hi  r2, r1
                bt/s    loc_60726C8
                mov     #0, r0
                mov.l   r0, @r14
                mov     #7, r7
                mov.l   off_60727EC, r3 ! unk_60DA4A0
                mov     #0, r2

loc_60726AC:                            ! CODE XREF: sub_6072640+80j
                mov     r6, r5
                mov     r6, r1
                add     #1, r1
                extu.b  r1, r6
                add     r3, r5
                mov.b   r2, @r5
                mov.l   @r14, r1
                add     #1, r1
                extu.b  r1, r1
                cmp/hi  r7, r1
                bf/s    loc_60726AC
                mov.l   r1, @r14
                bra     loc_60726CA
                mov     #0, r0
! ---------------------------------------------------------------------------

loc_60726C8:                            ! CODE XREF: sub_6072640+60j
                mov     #0, r6

loc_60726CA:                            ! CODE XREF: sub_6072640+84j
                mov.l   r0, @r14
                mov.l   @(8,r14), r11
                mov.l   off_60727EC, r10 ! unk_60DA4A0
                add     #0x1E, r11

loc_60726D2:                            ! CODE XREF: sub_6072640+222j
                mov     #0, r9
                mov     r9, r5

loc_60726D6:                            ! CODE XREF: sub_6072640+20Cj
                mov     r5, r1
                add     #1, r1
                extu.b  r1, r9
                mov.b   @r13, r4
                mov.l   off_60727F4, r0 ! ___ashlsi3
                extu.b  r4, r4
                jsr     @r0 ! ___ashlsi3
                mov.l   r6, @(0xC,r14)
                mov     r0, r1
                mov     r1, r4
                mov.l   off_60727F8, r1 ! ___ashiftrt_r4_7
                jsr     @r1 ! ___ashiftrt_r4_7
                mov     #1, r0
                mov     r4, r3
                and     r0, r3
                shll2   r3
                shll2   r3
                mov.l   @(0xC,r14), r6
                mov     #0x5F, r1
                cmp/hi  r1, r6          ! ???
                bf      loc_607270A
                mov.b   @r11, r1
                extu.b  r1, r1
                tst     r1, r1
                bt/s    loc_6072756
                tst     r3, r3

loc_607270A:                            ! CODE XREF: sub_6072640+BEj
                mov.b   @r11, r1
                !extu.b  r1, r1
                mov     r1, r0
                cmp/eq  #1, r0
                bt/s    loc_607271A
                cmp/eq  #3, r0
                bf/s    loc_607273A
                cmp/eq  #2, r0

loc_607271A:                            ! CODE XREF: sub_6072640+D2j
                mov     r3, r2
                shlr2   r2
                shlr2   r2
                mov     r2, r0
                cmp/eq  #1, r0
                bf      loc_6072728
                mov     #2, r2

loc_6072728:                            ! CODE XREF: sub_6072640+E4j
                mov     r6, r0
                mov     r6, r1
                add     #1, r1
                extu.b  r1, r6
                add     r10, r0
                mov.b   @r0, r1
                or      r1, r2
                bra     loc_6072754
                mov.b   r2, @r0
! ---------------------------------------------------------------------------

loc_607273A:                            ! CODE XREF: sub_6072640+D6j
                bf/s    loc_6072756
                tst     r3, r3
                mov     r3, r2
                shlr2   r2
                shlr2   r2
                mov     r2, r0
                cmp/eq  #1, r0
                bf      loc_607274C
                mov     #2, r2

loc_607274C:                            ! CODE XREF: sub_6072640+108j
                shll2   r2
                shll2   r2
                mov     r6, r0
                mov.b   r2, @(r0,r10)

loc_6072754:                            ! CODE XREF: sub_6072640+F6j
                tst     r3, r3

loc_6072756:                            ! CODE XREF: sub_6072640+C6j
                                        ! sub_6072640:loc_607273Aj
                bf/s    loc_6072762
                mov     r12, r0
                mov.b   @(r0,r10), r1
                mov.w   word_60727E4, r8 ! 0xF0
                bra     loc_6072764
                and     r1, r8
! ---------------------------------------------------------------------------

loc_6072762:                            ! CODE XREF: sub_6072640:loc_6072756j
                mov     r3, r8

loc_6072764:                            ! CODE XREF: sub_6072640+11Ej
                mov     r9, r5
                mov.b   @r13, r4
                mov.l   off_60727F4, r0 ! ___ashlsi3
                extu.b  r4, r4
                jsr     @r0 ! ___ashlsi3
                mov.l   r6, @(0xC,r14)
                mov     r0, r1
                mov     r1, r4
                mov.l   off_60727F8, r1 ! ___ashiftrt_r4_7
                jsr     @r1 ! ___ashiftrt_r4_7
                mov     #1, r0
                mov     r4, r3
                mov.l   @(0xC,r14), r6
                mov     #0x5F, r1
                cmp/hi  r1, r6
                bf/s    loc_6072790
                and     r0, r3
                mov.b   @r11, r1
                extu.b  r1, r1
                tst     r1, r1
                bt/s    loc_60727D4
                tst     r3, r3

loc_6072790:                            ! CODE XREF: sub_6072640+142j
                mov.b   @r11, r1
                !extu.b  r1, r1          ! change this to "and     #h'F, r0"
                mov     r1, r0
                and     #0xF, r0
                cmp/eq  #1, r0
                bt/s    loc_60727A0
                cmp/eq  #3, r0
                bf/s    loc_60727B4
                cmp/eq  #2, r0

loc_60727A0:                            ! CODE XREF: sub_6072640+158j
                mov     r3, r2
                mov     r2, r0
                cmp/eq  #1, r0
                bf      loc_60727AA
                mov     #2, r2

loc_60727AA:                            ! CODE XREF: sub_6072640+166j
                shll2   r2
                shll2   r2
                mov     r6, r0
                bra     loc_60727D2
                mov.b   r2, @(r0,r10)
! ---------------------------------------------------------------------------

loc_60727B4:                            ! CODE XREF: sub_6072640+15Cj
                bf/s    loc_60727D4
                tst     r3, r3
                mov     r3, r2
                mov     r2, r0
                cmp/eq  #1, r0
                bf      loc_60727C2
                mov     #2, r2

loc_60727C2:                            ! CODE XREF: sub_6072640+17Ej
                mov     r6, r0
                mov     r6, r1
                add     #1, r1
                extu.b  r1, r6
                add     r10, r0
                mov.b   @r0, r1
                or      r1, r2
                mov.b   r2, @r0

loc_60727D2:                            ! CODE XREF: sub_6072640+170j
                tst     r3, r3

loc_60727D4:                            ! CODE XREF: sub_6072640+14Cj
                                        ! sub_6072640:loc_60727B4j
                bf/s    loc_60727FC
                mov     r12, r0
                mov.b   @(r0,r10), r1
                mov     #0xF, r0
                and     r0, r1
                bra     loc_60727FE
                or      r1, r8
! ---------------------------------------------------------------------------
                !.align 2
word_60727E4:   .word 0xF0            ! DATA XREF: sub_6072640+11Cr
                .align 2
off_60727E8:    .long 0x060B74D4      ! DATA XREF: sub_6072640+1Cr
off_60727EC:    .long 0x060DA4A0      ! DATA XREF: sub_6072640+22r
                                        ! sub_6072640+68r
                                        ! sub_6072640+8Er
dword_60727F0:  .long 0x25E6B800      ! DATA XREF: sub_6072640+4Cr
off_60727F4:    .long 0x06099AF4      ! DATA XREF: sub_6072640+9Er
                                        ! sub_6072640+128r
off_60727F8:    .long 0x06099A3E      ! DATA XREF: sub_6072640+AAr
                                        ! sub_6072640+134r
! ---------------------------------------------------------------------------

loc_60727FC:                            ! CODE XREF: sub_6072640:loc_60727D4j
                or      r3, r8

loc_60727FE:                            ! CODE XREF: sub_6072640+19Ej
                mov     r12, r2
                mov     r12, r1
                add     #1, r1
                extu.b  r1, r12
                mov     r2, r0
                mov.b   r8, @(r0,r10)
                mov.l   @(4,r14), r1
                add     #2, r1
                extu.b  r1, r1
                mov     r1, r0
                cmp/eq  #0xC, r0
                bf/s    loc_607283E
                mov.l   r1, @(4,r14)
                mov     #0, r0
                mov.l   r0, @(4,r14)
                mov     r6, r1
                add     #2, r1
                extu.b  r1, r6
                mov     #0, r2
                mov     #0, r7
                mov     #1, r3

loc_6072828:                            ! CODE XREF: sub_6072640+1FAj
                mov     r12, r0
                mov     r12, r1
                add     #1, r1
                extu.b  r1, r12
                add     r10, r0
                mov     r2, r1
                add     #1, r1
                extu.b  r1, r2
                cmp/hi  r3, r2
                bf/s    loc_6072828
                mov.b   r7, @r0

loc_607283E:                            ! CODE XREF: sub_6072640+1D4j
                mov     r9, r1
                add     #1, r1
                extu.b  r1, r9
                mov     #7, r1
                cmp/hi  r1, r9
                bt/s    loc_6072850
                mov     r9, r5
                bra     loc_60726D6
                nop
! ---------------------------------------------------------------------------

loc_6072850:                            ! CODE XREF: sub_6072640+208j
                mov.l   @r14, r1
                add     #1, r1
                !extu.b  r1, r1         ! REMOVEME: is this necessary?
                mov.l   r1, @r14
                mov.l   @r14, r0
                mov     #0x11, r1
                cmp/hi  r1, r0
                bt/s    loc_6072866
                add     #1, r13
                bra     loc_60726D2
                nop
! ---------------------------------------------------------------------------

loc_6072866:                            ! CODE XREF: sub_6072640+21Ej
                mov     #0, r0
                mov.l   r0, @(4,r14)
                mov.l   off_60728E4, r0 ! CalcNextCharacterDrawPosition
                jsr     @r0 ! CalcNextCharacterDrawPosition
                nop
                mov     #0, r0
                mov.l   r0, @r14
                mov.l   off_60728E8, r9 ! sub_60728F4
                mov.l   off_60728EC, r8 ! character_draw_offset
                mov.l   @(4,r14), r1
                mov.l   @r14, r0

loc_607287C:                            ! CODE XREF: sub_6072640+27Cj
                add     #1, r1
                extu.b  r1, r1
                mov.l   r1, @(4,r14)
                mov.l   off_60728F0, r1 ! unk_60DA4A0
                mov.b   @(r0,r1), r4
                jsr     @r9 ! sub_60728F4
                extu.b  r4, r4
                mov.l   @r8, r1
                mov     r0, r2          ! Take 2 4BPP pixels and write them to buffer
                mov.b   r2, @r1
                mov.l   @r8, r2
                add     #1, r2
                mov.l   r2, @r8
                mov.l   @(4,r14), r0
                cmp/eq  #7, r0
                bf/s    loc_60728B0
                mov.l   @r14, r1        ! we've finished copying over a row of a character
                mov     #0, r0
                mov.l   r0, @(4,r14)    ! set x to 0?
                add     #1, r1
                !extu.b  r1, r1         ! REMOVEME: is this necessary?
                mov.l   r1, @r14        
                mov.w   word_60728E0, r1 ! 0x85
                add     r1, r2
                mov.l   r2, @r8         ! adjust buffer to the next line
                mov.l   @r14, r1

loc_60728B0:                            ! CODE XREF: sub_6072640+25Aj
                add     #1, r1
                !extu.b  r1, r1          ! REMOVEME: is this necessary?
                mov.l   r1, @r14
                mov.l   @r14, r0
                mov     #0x5F, r1
                cmp/hi  r1, r0
                bf/s    loc_607287C
                mov.l   @(4,r14), r1    ! character is finished
                mov.l   @(8,r14), r2
                add     #0x1D, r2       ! ?

                mov.b   @r2, r1
                mov.l   @(0x10,r14), r0 ! fetch character width
                add     r0, r1          ! increment character x offset by width
                mov.b   r1, @r2         ! increment character x offset
                add     r0, r0          ! double the width(eventually, this shouldn't be necessary)

                ! Restore stack position(new stack usage is 0x14 bytes)
                !add     #0x10, r14
                add     #0x14, r14                
                mov     r14, r15
                lds.l   @r15+, pr
                mov.l   @r15+, r14
                mov.l   @r15+, r13
                mov.l   @r15+, r12
                mov.l   @r15+, r11
                mov.l   @r15+, r10
                mov.l   @r15+, r9
                rts
                mov.l   @r15+, r8
! End of function _DrawDialogText

! ---------------------------------------------------------------------------
word_60728E0:   .word 0x85            ! DATA XREF: sub_6072640+268r
                .align 2
off_60728E4:    .long 0x0607295C      ! DATA XREF: sub_6072640+22Ar
off_60728E8:    .long 0x060728F4      ! DATA XREF: sub_6072640+234r
off_60728EC:    .long 0x060DA534      ! DATA XREF: sub_6072640+236r
off_60728F0:    .long 0x060DA4A0      ! DATA XREF: sub_6072640+242r

! Should be no larger than 0x74 bytes. (0x060729D0-0607295C)
CalcNextCharacterDrawPosition:          ! CODE XREF: sub_6072640+22Cp
                                         ! sub_6073F5C+1Ep
                                        ! DATA XREF: RAM:off_60728E4o
                                        ! RAM:off_60740B4o
                mov.l   r14, @-r15
                mov.l   off_60729C4, r1 ! _queue_function_ptr
                mov.l   off_60729C8, r7 ! character_draw_offset
                mov.l   @r1, r2
                mov     r2, r1
                add     #0x77, r1
                mov.b   @r1, r1         ! y offset(in characters)
                mov     r1, r0
                and     #0xF, r0
                mov     #0x80, r3
                extu.b  r3, r3
                and     r3, r1
                shll    r1
                mov.l   dword_60729CC, r3 ! 0x5C08800
                mov.l   r3, @r7
                tst     r0, r0
                bf/s    loc_6072988
                mov     r15, r14        ! row 1
                add     #0x75, r2
                mov.b   @r2, r2
                extu.b  r2, r2
                add     r2, r1
                add     r3, r1
                bra     loc_60729B6
                mov.l   r1, @r7
! ---------------------------------------------------------------------------

loc_6072988:                            ! CODE XREF: CalcNextCharacterDrawPosition+16j
                cmp/eq  #1, r0
                bf/s    loc_607299E
                cmp/eq  #2, r0          ! row 2
                add     #117, r2
                mov.b   @r2, r2
                extu.b  r2, r2
                add     r1, r2
                mov.w   word_60729C0, r1 ! 0x7A8
                bra     loc_60729B0
                nop
! ---------------------------------------------------------------------------

loc_607299E:                            ! CODE XREF: CalcNextCharacterDrawPosition+2Ej
                bf/s    loc_60729B8
                mov     #0, r0          ! row 3
                add     #117, r2
                mov.b   @r2, r2
                extu.b  r2, r2
                add     r1, r2
                mov.w   word_60729C2, r1 ! 0xF50

loc_60729B0:                            ! CODE XREF: CalcNextCharacterDrawPosition+3Ej
                add     r1, r2
                add     r3, r2
                mov.l   r2, @r7

loc_60729B6:                            ! CODE XREF: CalcNextCharacterDrawPosition+28j
                mov     #-1, r0
loc_60729B8:                            ! CODE XREF: CalcNextCharacterDrawPosition:loc_607299Ej
                mov     r14, r15
                rts
                mov.l   @r15+, r14
! End of function CalcNextCharacterDrawPosition

! ---------------------------------------------------------------------------
word_60729C0:   .word 0x7A8             ! DATA XREF: CalcNextCharacterDrawPosition+3Cr
word_60729C2:   .word 0xF50             ! DATA XREF: CalcNextCharacterDrawPosition+52r
.align 2
off_60729C4:    .long 0x060B74D4        ! DATA XREF: CalcNextCharacterDrawPosition+2r
off_60729C8:    .long 0x060DA534        ! DATA XREF: CalcNextCharacterDrawPosition+4r
dword_60729CC:  .long 0x05C08800        ! DATA XREF: CalcNextCharacterDrawPosition+8r


! Should be no larger than 0x17C bytes. (0x06073F5C-0x060740D8)
QueryChoice:                            ! CODE XREF: _HandleTextControlCodes+31Cp
                                        ! DATA XREF: _HandleTextControlCodes:off_6072D4Co
                mov.l   r8, @-r15
                mov.l   r9, @-r15
                mov.l   r10, @-r15
                mov.l   r11, @-r15
                mov.l   r12, @-r15
                mov.l   r13, @-r15
                mov.l   r14, @-r15
                sts.l   pr, @-r15
                add     #-4, r15
                mov     r15, r14
                mov     #0, r13
                mov     #0, r12
                mov.l   off_60740B0, r1 ! _queue_function_ptr
                mov.l   @r1, r10
                mov.l   off_60740B4, r0 ! CalcNextCharacterDrawPosition
                jsr     @r0 ! CalcNextCharacterDrawPosition
                add     #0x58, r10
                mov.l   off_60740B8, r1 ! character_draw_offset
                mov.l   @r1, r1
                mov.l   off_60740BC, r2 ! unk_60DA510
                mov.l   r1, @(0,r2)     ! character_draw_offset

loc_6073F86:                            ! CODE XREF: QueryChoice+FAj
                tst     r13, r13
                bt/s    loc_6073F96
                mov     r13, r2
                shll2   r2
                add     r2, r2
                mov     #0, r1
                mov.l   off_60740BC, r0 ! unk_60DA510
                mov.l   r1, @(r0,r2)

loc_6073F96:                            ! CODE XREF: QueryChoice+2Cj
                mov     r13, r2
                shll2   r2
                add     r2, r2
                mov.l   off_60740BC, r3 ! unk_60DA510
                add     r3, r2
                add     #4, r2
                mov     #0, r1
                mov.l   off_60740C0, r0 ! _DecompressTextWord
                jsr     @r0 ! _DecompressTextWord
                mov.w   r1, @r2
                mov.w   @r10, r1
                mov.w   word_60740A8, r2 ! 0x3FF
                extu.w  r1, r1
                cmp/eq  r2, r1          ! Check if it's a control code
                bf/s    loc_6073FCA
                tst     r12, r12
                mov.l   off_60740C0, r3 ! _DecompressTextWord
                jsr     @r3 ! _DecompressTextWord
                nop
                mov.w   @r10, r1        ! which control code is it?
                mov.w   word_60740AA, r2 ! 0x3F6
                extu.w  r1, r1
                cmp/eq  r2, r1          ! Is it an end code?
                bf      loc_6073FC8
                movt    r12

loc_6073FC8:                            ! CODE XREF: QueryChoice+68j
                tst     r12, r12

loc_6073FCA:                            ! CODE XREF: QueryChoice+56j
                bf/s    loc_607405A
                mov     #0, r8
                mov     r13, r0
                shll2   r0
                add     r0, r0
                mov.l   r0, @r14
                mov     r0, r9
                mov.l   off_60740BC, r3 ! unk_60DA510
                add     r3, r9
                mov     r9, r11
                add     #4, r11
                mov.w   @r10, r1

loc_6073FE2:                            ! CODE XREF: QueryChoice+ECj
                mov.w   word_60740A8, r2 ! 0x3FF
                extu.w  r1, r1
                cmp/eq  r2, r1          ! if character isn't a control code, bail
                bf/s    loc_6074000
                tst     r12, r12
                mov.l   off_60740C0, r0 ! _DecompressTextWord
                jsr     @r0 ! _DecompressTextWord
                nop
                mov.w   @r10, r1
                mov.w   word_60740AC, r2 ! 0x3E8
                extu.w  r1, r1
                cmp/eq  r2, r1          ! is it end of line code?
                bf      loc_6073FFE
                movt    r12

loc_6073FFE:                            ! CODE XREF: QueryChoice+9Ej
                tst     r12, r12

loc_6074000:                            ! CODE XREF: QueryChoice+8Cj
                bf/s    loc_607404C     ! option finished, time to move onto next one?
                tst     r13, r13
                bt/s    loc_6074038
                tst     r8, r8
                bf/s    loc_6074038
                mov     r13, r1         ! option_num
                add     #-1, r1         ! Only seems to come here after one option parsed and drawn
                shll2   r1
                add     r1, r1
                !mov.l   off_60740BC, r3 ! unk_60DA510
                mov.l   off_60740BC, r1 ! unk_60DA510
                !add     r3, r1          ! this is no longer needed
                nop
                mov.l   @r1+, r2        ! pointer to current posion of vdp1 draw buffer for text                
                mov.l   @r14, r0        ! width of option text?
                add     #4, r2          ! modify this to either 4 or 8
                mov.l   r2, @(r0,r3)    ! adjust the x position of the black background bar for selected option
                mov     r10, r1
                add     #0x1D, r1
                mov.b   @r1, r1                
                mov.l   @r9, r2
                add     r1, r2          ! adjust the x position of the black background bar for selected option
                mov.l   r2, @r9
                mov     r10, r2
                add     #0x1D, r2
                mov.b   @r2, r1         ! increment x position
                mov     #1, r8
                add     #4, r1          ! modify this to either 4 or 8
                mov.b   r1, @r2
                nop

loc_6074038:                            ! CODE XREF: QueryChoice+A8j
                                        ! QueryChoice+ACj
                mov.l   off_60740C4, r1 ! _DrawDialogText
                jsr     @r1 ! _DrawDialogText ! Do the text rendering
                nop
                mov.w   @r11, r1
                mov.l   off_60740C0, r3 ! _DecompressTextWord
                !add character width from DrawDialogText return
                !add     #1, r1
                add     r0, r1
                jsr     @r3 ! _DecompressTextWord
                mov.w   r1, @r11
                bra     loc_6073FE2
                mov.w   @r10, r1
! ---------------------------------------------------------------------------

loc_607404C:                            ! CODE XREF: QueryChoice:loc_6074000j
                mov     r13, r1         ! option finished, time to move onto next one?
                add     #1, r1
                extu.w  r1, r13         ! increment option_num
                mov     r13, r0
                cmp/eq  #4, r0          ! I guess the maximum is 3?
                bf/s    loc_6073F86
                mov     #0, r12

loc_607405A:                            ! CODE XREF: QueryChoice:loc_6073FCAj
                mov.l   off_60740B0, r1 ! _queue_function_ptr
                mov.l   @r1, r2
                mov     r2, r1
                add     #0x4E, r1
                mov.w   @r1, r1
                extu.w  r1, r1
                tst     r1, r1
                bt/s    loc_6074074
                mov     r2, r1
                add     #0x48, r1
                mov.w   @r1, r0
                or      #5, r0
                mov.w   r0, @r1

loc_6074074:                            ! CODE XREF: QueryChoice+10Cj
                mov     r10, r1
                add     #0x23, r1
                add     #-1, r13
                mov.b   r13, @r1
                add     #0x22, r10
                mov.l   off_60740C8, r1 ! unk_60DA5B2
                mov.b   @r1, r1
                mov.b   r1, @r10
                mov.l   off_60740CC, r1 ! _DrawChoiceSelection
                jsr     @r1 ! _DrawChoiceSelection
                mov     #1, r4
                mov.l   off_60740D0, r4 ! ChangeQuerySelection
                mov.l   off_60740D4, r1 ! _QueueFunctionExec
                jsr     @r1 ! _QueueFunctionExec
                nop
                add     #4, r14
                mov     r14, r15
                lds.l   @r15+, pr
                mov.l   @r15+, r14
                mov.l   @r15+, r13
                mov.l   @r15+, r12
                mov.l   @r15+, r11
                mov.l   @r15+, r10
                mov.l   @r15+, r9
                rts
                mov.l   @r15+, r8
! End of function QueryChoice

word_60740A8:   .word 0x3FF             ! DATA XREF: QueryChoice+50r
                                        ! QueryChoice:loc_6073FE2r
word_60740AA:   .word 0x3F6             ! DATA XREF: QueryChoice+62r
word_60740AC:   .word 0x3E8             ! DATA XREF: QueryChoice+98r
.align 2
off_60740B0:    .long 0x060B74D4        ! DATA XREF: QueryChoice+18r
                                        ! QueryChoice:loc_607405Ar
off_60740B4:    .long 0x0607295C        ! DATA XREF: QueryChoice+1Cr
off_60740B8:    .long 0x060DA534        ! DATA XREF: QueryChoice+22r
off_60740BC:    .long 0x060DA510        ! DATA XREF: QueryChoice+26r
                                        ! QueryChoice+36r
                                        ! QueryChoice+40r
                                        ! QueryChoice+7Cr
                                        ! QueryChoice+B6r
off_60740C0:    .long 0x060720F0        ! DATA XREF: QueryChoice+48r
                                        ! QueryChoice+5Ar
                                        ! QueryChoice+90r
                                        ! QueryChoice+E4r
off_60740C4:    .long 0x06072640        ! DATA XREF: QueryChoice:loc_6074038r
off_60740C8:    .long 0x060DA5B2        ! DATA XREF: QueryChoice+122r
off_60740CC:    .long 0x0607420C        ! DATA XREF: QueryChoice+128r
off_60740D0:    .long 0x060740D8        ! DATA XREF: QueryChoice+12Er
off_60740D4:    .long 0x060111E4        ! DATA XREF: QueryChoice+130r


! Should be no larger than 0xF0 bytes. (0x0607420C-0x060742FC)
_DrawChoiceSelection:                   ! CODE XREF: QueryChoice+12Ap
                                        ! ChangeQuerySelection+B8p
                                        ! ChangeQuerySelection+EAp
                                        ! ChangeQuerySelection+F6p
                                        ! DATA XREF: RAM:off_60740CCo
                                        ! ChangeQuerySelection+B6o
                                        ! ChangeQuerySelection+E8o
                                        ! ChangeQuerySelection+F4o
                                        ! RAM:off_6074204o
                mov.l   r8, @-r15
                mov.l   r9, @-r15
                mov.l   r14, @-r15
                mov     r15, r14
                mov.l   off_60742F0, r1 ! _queue_function_ptr
                mov.l   @r1, r3
                mov     r3, r1
                add     #0x7A, r1
                mov.b   @r1, r1
                mov.l   off_60742F4, r2 ! dword_60DA510
                extu.b  r1, r1
                shll2   r1
                add     r1, r1
                add     r2, r1
                add     #4, r1
                mov.w   @r1, r1         ! width in characters
                mov     #0, r5
                extu.w  r1, r1
                cmp/hs  r1, r5
                bt/s    loc_60742E0     ! if width in character is less or equal to 0, bail
                extu.b  r4, r9
                mov.l   off_60742F8, r4 ! character_draw_offset
                mov     r3, r8
                add     #0x7A, r8
                mov     r1, r6

loc_607423E:                            ! CODE XREF: _DrawChoiceSelection+D0j
                mov.b   @r8, r1
                mov     r6, r7
                extu.b  r1, r0
                shll2   r0
                add     r0, r0
                mov.w   word_60742EE, r1 ! 140
                mulu    r1, r5
                mov.l   @(r0,r2), r2
                sts     macl, r1
                add     r1, r2
                mov.l   r2, @r4

loc_6074254:                            ! CODE XREF: _DrawChoiceSelection+B2j
                tst     r9, r9
                bt      loc_607427E
                
                ! Select
                mov.l   @r4, r1
                mov.b   @r1, r1
                extu.b  r1, r3
                mov.w   word_60742EC, r1 ! 0xF0
                and     r3, r1
                tst     r1, r1
                bf/s    loc_6074270
                mov     #0x10, r1
                mov     #0x30, r1

loc_6074270:                            ! CODE XREF: _DrawChoiceSelection+5Ej
                dt      r7
                bt      skippixel2      ! skip second pixel if odd width
                dt      r7
                mov     #0xF, r0
                and     r0, r3
                tst     r3, r3
                bt/s    loc_607429A
                mov     #1, r0
                bra     loc_607429E
                or      r0, r1
skippixel2:
                mov     #0xF, r0
                and     r0, r3
                bra     loc_607429E
                or      r3, r1
                
! ---------------------------------------------------------------------------

loc_607427E:                            ! CODE XREF: _DrawChoiceSelection+4Ej
                ! Deselect
                mov.l   @r4, r1
                mov.b   @r1, r1
                extu.b  r1, r3
                mov     r3, r0
                and     #0xF0, r0
                cmp/eq  #0x30, r0
                bf/s    loc_6074290
                mov     #0x30, r1
                mov     #0, r1

loc_6074290:                            ! CODE XREF: _DrawChoiceSelection+7Ej
                dt      r7
                bf      dopixel2       ! skip second pixel if odd width

                mov     #0xF, r0
                and     r0, r3
                bra     loc_607429E
                or      r3, r1

dopixel2:                
                dt      r7

                mov     r3, r0
                cmp/eq  #3, r0
                bt      loc_607429E

loc_607429A:                            ! CODE XREF: _DrawChoiceSelection+6Aj
                mov     #3, r0
                or      r0, r1

loc_607429E:                            ! CODE XREF: _DrawChoiceSelection+6Ej
                                        ! _DrawChoiceSelection+8Cj
                mov.b   r1, @r2
                add     #1, r2

!loc_60742B4:                            ! CODE XREF: _DrawChoiceSelection+98j
                tst     r7, r7           ! Any more lines remaining?
                bf/s    loc_6074254
                mov.l   r2, @r4
                mov     r5, r1
                add     #1, r1
                extu.w  r1, r5
                mov.l   off_60742F4, r2 ! dword_60DA510
                mov     #0xC, r1
                cmp/hs  r1, r5
                bf/s    loc_607423E
                nop

loc_60742E0:                            ! CODE XREF: _DrawChoiceSelection+26j
                mov     r14, r15
                mov.l   @r15+, r14
                mov.l   @r15+, r9
                rts
                mov.l   @r15+, r8
                nop
                nop
                nop
                nop
                nop
                nop
                nop
                nop
                nop
                nop
! End of function _DrawChoiceSelection
word_60742EC:   .word 0xF0              ! DATA XREF: _DrawChoiceSelection+58r
!CHANGED
!word_60742EE:   .word 0x85              ! DATA XREF: _DrawChoiceSelection+A4r
word_60742EE:   .word 140              ! DATA XREF: _DrawChoiceSelection+A4r
.align 2
off_60742F0:    .long 0x060B74D4        ! DATA XREF: _DrawChoiceSelection+8r
off_60742F4:    .long 0x060DA510        ! DATA XREF: _DrawChoiceSelection+12r
                                        ! _DrawChoiceSelection+BEr
off_60742F8:    .long 0x060DA534
                                        ! DATA XREF: _DrawChoiceSelection+2Ar



_main:
rts
