#include <idtLoader.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <string.h>
#include <userland.h>
#include <video.h>
#include <scheduler.h>
#include <process.h>
#include <time.h>
#include <mmgr.h>
#include <interrupts.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

void clearBSS(void *bssAddress, uint64_t bssSize) {
  memset(bssAddress, 0, bssSize);
}

void *getStackBase() {
  return (void *)((uint64_t)&endOfKernel +
                  PageSize * 8       // The size of the stack itself, 32KiB
                  - sizeof(uint64_t) // Begin at the top of the stack
  );
}

void printA(){
  int blocked = 0;
  while(1) {
    ncPrint("A");
    _hlt();
  }
}

void printB(){
  while(1) {
    ncPrint("B");
    _hlt();
  }
}

void printC(){
  while(1) {
    ncPrint("C");
    _hlt();
  }
}

void processControl() {
  while(seconds_elapsed() < 1);
  block(1);
  while(seconds_elapsed() < 2);
  block(2);
	unblock(1);
	while(seconds_elapsed() < 3);
	unblock(2);
	setPriority(1, 3);
	setPriority(2, 2);
	setPriority(3, 1);
	while(seconds_elapsed() < 4);
	block(1);
	block(2);
	block(3);
	while(seconds_elapsed() < 6);
	unblock(1);
	unblock(2);
	unblock(3);
	kill(4);
}


void *initializeKernelBinary() {
  // char buffer[10];

  // ncPrint("[x64BareBones]");
  // ncNewline();

  // ncPrint("CPU Vendor:");
  // ncPrint(cpuVendor(buffer));
  // ncNewline();

  // ncPrint("[Loading modules]");
  // ncNewline();
  void *moduleAddresses[] = {sampleCodeModuleAddress, sampleDataModuleAddress};

  loadModules(&endOfKernelBinary, moduleAddresses);
  // ncPrint("[Done]");
  // ncNewline();
  // ncNewline();

  // ncPrint("[Initializing kernel's binary]");
  // ncNewline();

  clearBSS(&bss, &endOfKernel - &bss);

  // ncPrint("  text: 0x");
  // ncPrintHex((uint64_t)&text);
  // ncNewline();
  // ncPrint("  rodata: 0x");
  // ncPrintHex((uint64_t)&rodata);
  // ncNewline();
  // ncPrint("  data: 0x");
  // ncPrintHex((uint64_t)&data);
  // ncNewline();
  // ncPrint("  bss: 0x");
  // ncPrintHex((uint64_t)&bss);
  // ncNewline();

  // ncPrint("[Done]");
  // ncNewline();
  // ncNewline();

  //init_screen();

  return getStackBase();
}

int main() {
  init_screen();
  prompt_info prompt = {.x = 0,
                        .y = 0,
                        .baseX = 0,
                        .baseY = 0,
                        .windowWidth = getScreenWidth(),
                        .windowHeight = getScreenHeight()};
  declarePrompt(&prompt);

  initMgr();
  initScheduler();

	createProcess((uint64_t) &printA, 0);
	createProcess((uint64_t) &printB, 0);
	createProcess((uint64_t) &printC, 0);
	createProcess((uint64_t) &processControl, 0);


  //showAllPs();

  load_idt();
  
  //(((EntryPoint)sampleCodeModuleAddress)());

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

  return 0;
}
