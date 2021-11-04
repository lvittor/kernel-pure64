
rebuild: clean all

all:  bootloader kernel userland image

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all

userland:
	cd Userland; make all

format:
	find Kernel/ Userland/ -iname *.h -o -iname *.c | xargs clang-format -i

cppcheck:
	cppcheck --quiet --enable=all --force --inconclusive .

pvs-studio: # docker run --security-opt seccomp:unconfined -v "${PWD}:/root" -ti -w /root agodio/itba-so:1.0
	pvs-studio-analyzer trace -- make
	pvs-studio-analyzer analyze
	plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o report.tasks PVS-Studio.log

image: kernel bootloader userland
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean

.PHONY: bootloader image collections kernel userland all clean
