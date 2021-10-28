OUT=test_mm

all: $(OUT) 

$(OUT): test_mm.c test_util.c test_util.h memoryManager.c memoryManager.h
	gcc -Wall -Wextra test_mm.c test_util.c memoryManager.c -o $@

clean:
	rm -f $(OUT)

.PHONY: all clean
