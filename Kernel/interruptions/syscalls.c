#include <syscalls.h>
#include <naiveConsole.h>
#include <stdint.h>

void sys_write(uint8_t fd, char * buffer, uint64_t count) {
	if (buffer == 0 || count <= 0)
		return;
		
	if (fd > 2)
		return;

	char attribute = (fd == STD_ERR) ? RED_ON_BLACK : WHITE_ON_BLACK;
    
	for (int i = 0; i < count && buffer[i]; i++)
		ncPrintCharAtt(buffer[i], attribute);
}

	// return ret;
	return 0;
}