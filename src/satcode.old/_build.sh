 
# PRE: install into $HOME/opt/sh2-elf-gcc  https://github.com/kentosama/sh2-elf-gcc

export PATH="${PATH}:$HOME/opt/sh2-elf-gcc/sh2-toolchain/bin"
export KPITOUTPUT=elf

sh2-elf-gcc -o satcode_patch.elf patch.s -O2 -Wall -m2  -nostartfiles -Wl,--script,bart.lk -Wl,-Map,main.map
# --small 

# Generating stripped patch.bin
sh2-elf-strip satcode_patch.elf --strip-all --output-target=binary -o satcode_patch.bin
