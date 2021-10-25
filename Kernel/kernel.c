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
  while(1) {
    if(seconds_elapsed() % 3 == 0){
      if(ticks_elapsed() % 18 == 0)
        ncPrint("A  ");
      if(seconds_elapsed() > 5)
        kill(0);
    }
  }
}

void printB(){
  while(1) {
    if(seconds_elapsed() % 3 == 1){
      if(ticks_elapsed() % 18 == 0)
        ncPrint("B  ");
    }
  }
}

void printC(){
  while(1) {
    if(seconds_elapsed() % 3 == 2){
      ncPrint("C  ");
    }
  }
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

  //run_process(init_process(200 * 1024 * 1024, sampleCodeModuleAddress));
  //addToReady((uint64_t) sampleCodeModuleAddress, 5);
  addToReady((uint64_t) &printA, 0);
  addToReady((uint64_t) &printB, 0);
  addToReady((uint64_t) &printC, 0);
  
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
