read -p "asm file name without ext: " filename
filename_S=${filename}.S 
filename_o=${filename}.o
as --32 ${filename_S} -o ${filename_o}
ld ${filename_o} -o ${filename} -melf_i386
