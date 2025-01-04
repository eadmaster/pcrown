!
! Sega Saturn start screen & bootloader
! By cafe-alpha, 2013
!
! From Sega Saturn cartridge flash tool
! by Anders Montonen, 2012
!
! Original software by ExCyber
! Graphics routines by Charles MacDonald
!
! Creative Commons Attribution-ShareAlike 3.0 Unported (CC BY-SA 3.0)
!
.section .data

.global _CopyStubData
.global _CopyStubDataLen

    .align 4
_CopyStubData:
    .incbin "obj/copystub.bin"
_CopyStubEnd:

    .align 4
_CopyStubDataLen:   .long _CopyStubEnd - _CopyStubData
