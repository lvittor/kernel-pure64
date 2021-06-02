/* sampleCodeModule.c */
#include <stdint.h>
#include <lib.h>
#include <utils.h>

char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;

int main() {
	printDate();
	//All the following code may be removed 
	*v = 'X';
	*(v+1) = 0x74; // 0b0111.0100

	print_f(1, "HOLA1: %d\n HOLA2: %s\n %c \%\n", 100000, "STRING", 'a');
	return 0;
	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;
}