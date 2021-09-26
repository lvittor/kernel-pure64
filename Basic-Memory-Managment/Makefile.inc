GCC=gcc
LD=ld
AR=ar
ASM=nasm

GCCFLAGS=-m64 -std=c99 -Wall
ARFLAGS=rvs
ASMFLAGS=-felf64
LDFLAGS=--warn-common -z max-page-size=0x1000
