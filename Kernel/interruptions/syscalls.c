#include <syscalls.h>
#include <naiveConsole.h>
#include <stdint.h>

void sys_write(uint8_t fd, char * buffer, uint64_t count) {
    for (int i = 0; i < count && buffer[i]; i++)
		ncPrintChar(buffer[i]);
    	
    // struct fd f = fdget_pos(fd);
	// ssize_t ret = -EBADF;

	// if (f.file) {
	// 	loff_t pos, *ppos = file_ppos(f.file);
	// 	if (ppos) {
	// 		pos = *ppos;
	// 		ppos = &pos;
	// 	}
	// 	ret = vfs_write(f.file, buf, count, ppos);
	// 	if (ret >= 0 && ppos)
	// 		f.file->f_pos = pos;
	// 	fdput_pos(f);
	// }

	// return ret;
	return 0;
}