#
# (c) 2002  Charles MacDonald
#

CC	=	sh-$(KPITOUTPUT)-gcc
AS	=	sh-$(KPITOUTPUT)-as
LD	=	sh-$(KPITOUTPUT)-ld

#FLAGS	=	-Wall -m2 -small
FLAGS	=	-Wall -m2 -mb -Os -fomit-frame-pointer -nostdlib -std=c99 -DSC_COMP_DISABLE

#LDFLAGS	=	-nostartfiles -small --script bart.lk -Map obj/main.map
LDFLAGS	=	-nostartfiles --script bart.lk -Map obj/main.map
COPYSTUB_LDFLAGS	=	-nostartfiles --script copystub/copystub_bart.lk -Map obj/copystub.map
	
LIBS	=	"$(PPATH)\\KPIT\GNUSH$(KPITVER)-$(KPITOUTPUT)\Sh-$(KPITOUTPUT)\sh-$(KPITOUTPUT)\lib\libc.a" \
		"$(PPATH)\\KPIT\GNUSH$(KPITVER)-$(KPITOUTPUT)\Sh-$(KPITOUTPUT)\lib\gcc\sh-$(KPITOUTPUT)\$(KPITVER2)-GNUSH_$(KPITVER)\libgcc.a"

EXE = ../splash_loader.bin
COPYSTUB_EXE = obj/copystub.bin

OBJ =	obj/main.o	\
		obj/misc.o \
		obj/compress.o \
		obj/vdp.o \
		obj/copystubwrap.o

COPYSTUB_OBJ =	obj/copystub.o



all	:	$(EXE)

$(EXE)	:	$(OBJ)
		$(LD) $(LDFLAGS) $(OBJ) $(LIBS) -o $(EXE)

obj/%.o : 	%.c
		$(CC) -c $< -o $@ $(FLAGS)

obj/%.o : 	../%.c
		$(CC) -c $< -o $@ $(FLAGS)

obj/%.o :	%.s $(COPYSTUB_EXE)
		$(AS) $< -o $@


$(COPYSTUB_EXE)	:	$(COPYSTUB_OBJ)
		$(LD) $(COPYSTUB_LDFLAGS) $(COPYSTUB_OBJ) $(LIBS) -o $(COPYSTUB_EXE)

obj/%.o : 	copystub/%.c
		$(CC) -c $< -o $@ $(FLAGS)


pack	:
		sh-$(KPITOUTPUT)-strip $(EXE) --strip-all

clean	:
		rm -f obj/*.*

makedir :
		mkdir obj
        
#
# end of makefile
#
