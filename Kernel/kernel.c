// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <idtLoader.h>
#include <sched.h>
#include <keyboard.h>
#include <video.h>
#include <mm.h>
#include <smh.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void *const sampleCodeModuleAddress = (void *)0x400000;
static void *const sampleDataModuleAddress = (void *)0x500000;

typedef int (*EntryPoint)();

void clearBSS(void *bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void *getStackBase()
{
	return (void *)((uint64_t)&endOfKernel +
			PageSize * 8 //The size of the stack itself, 32KiB
			- sizeof(uint64_t) //Begin at the top of the stack
	);
}

void *initializeKernelBinary()
{
	void *moduleAddresses[] = { sampleCodeModuleAddress,
				    sampleDataModuleAddress };

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

int main()
{
	load_idt();
	init_screen();
	heap_init();
	init_keyboard();
	init_pipes();
	init_shem(); // DEFENSA

	_process_prototype_t shell = {
		.function_address = (void *)sampleCodeModuleAddress,
		.name = "sh",
		.priority = 0,
		.state = READY,
		.fds = { 0, 1, 2 },
		.foreground = 1,
	};
	init_sched(&shell, 0, (char *[]){ NULL });

	ncNewline();
	ncNewline();

	ncPrint("  Sample data module at 0x");
	ncPrintHex((uint64_t)sampleDataModuleAddress);

	ncNewline();
	ncPrint("  Sample data module contents: ");
	ncPrint((char *)sampleDataModuleAddress);
	ncNewline();

	ncPrint("[Finished]");
	ncNewline();

	while (1)
		;

	return 0;
}
