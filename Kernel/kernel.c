#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <idtLoader.h>
#include <multitasking.h>
#include <video.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	init_screen();

	return getStackBase();
}



int main() {	
	drawShellBorder(&WHITE);

	load_idt();

	prompt_info leftPrompt = {	.x = 0,
								.y = 0,
							  	.baseX = 0,
							  	.baseY = 0,
							  	.windowWidth = getScreenWidth()/2 - 4,
							  	.windowHeight = getScreenHeight()};

	prompt_info rightPrompt = {	.x = 0,
								.y = 0,
								.baseX = getScreenWidth() / 2 + 4,
								.baseY = 0,
								.windowWidth = getScreenWidth()/2 - 4, 
								.windowHeight = getScreenHeight()};

	loadTask(0, (uint64_t)sampleCodeModuleAddress, 0x600000, leftPrompt);
	loadTask(1, (uint64_t)sampleCodeModuleAddress, 0x700000, rightPrompt);
	initCurrentTask();
	
	ncNewline();
	ncNewline();

	ncPrint("  Sample data module at 0x");
	ncPrintHex((uint64_t)sampleDataModuleAddress);
	
	ncNewline();
	ncPrint("  Sample data module contents: ");
	ncPrint((char*)sampleDataModuleAddress);
	ncNewline();
	
	ncPrint("[Finished]");
	ncNewline();

	while(1);

	return 0;
}
