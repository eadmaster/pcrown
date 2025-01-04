 
# PRE: install into $HOME/opt/sh2-elf-gcc  https://github.com/kentosama/sh2-elf-gcc

export PATH="${PATH}:$HOME/opt/sh2-elf-gcc/sh2-toolchain/bin"
export KPITOUTPUT=elf

make
