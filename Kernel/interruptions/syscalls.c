#include <interrupts.h>
#include <keyboard.h>
#include <lib.h>
#include <naiveConsole.h>
#include <process.h>
#include <scheduler.h>
#include <stdint.h>
#include <syscalls.h>
#include <video.h>
#include <sync.h>

typedef uint64_t (*PSysCall)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

typedef struct dateType {
  uint8_t year, month, day;
  uint8_t hour, minute, second;
} dateType;

int64_t sys_read(unsigned int fd, char *buf, size_t count);
uint64_t sys_write(uint8_t fd, char *buffer, uint64_t count);
uint64_t sys_date(dateType *pDate);
uint64_t sys_mem(uint64_t rdi, uint64_t rsi, uint8_t rdx);
void sys_ps(void);
pid_t sys_createPs(uint64_t rip, char *name, int argc, char *argv[], uint8_t mode);
int sys_block(pid_t pid);
int sys_unblock(pid_t pid);
int sys_kill(pid_t pid);
pid_t sys_getpid(void);
int sys_nice(pid_t pid, int priority);
void sys_exit(void);

static PSysCall sysCalls[255] = {
    (PSysCall)&sys_read,   (PSysCall)&sys_write,   (PSysCall)&sys_date,
    (PSysCall)&sys_mem,    (PSysCall)&sys_ps,      (PSysCall)&sys_createPs,
    (PSysCall)&sys_block,  (PSysCall)&sys_unblock, (PSysCall)&sys_kill,
    (PSysCall)&sys_getpid, (PSysCall)&sys_nice,    (PSysCall)&sys_exit,
    (PSysCall)&sem_open, (PSysCall)&sem_post,    (PSysCall)&sem_wait,
    (PSysCall)&sem_close};

uint64_t sysCallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                           uint64_t rcx, uint64_t r8, uint64_t rax) {
  PSysCall sysCall = sysCalls[rax];
  if (sysCall != 0)
    return sysCall(rdi, rsi, rdx, rcx, r8);
  return 0;
}

uint64_t sys_write(uint8_t fd, char *buffer, uint64_t count) {
  if (buffer == NULL || count == 0)
    return -1;

  if (fd > 2)
    return -1;

  Color *fontColor = (fd == STD_ERR) ? &RED : &WHITE;

  for (int i = 0; i < count && buffer[i]; i++)
    ncPrintCharAtt(buffer[i], fontColor, &BLACK);

  return count;
}

int64_t sys_read(unsigned int fd, char *buf, size_t count) {
  long read_count = -1;
  while (read_count == -1) {
    if ((read_count = copy_from_buffer(buf, count)) == -1) {
      _sti();
      _hlt();
      _cli();
    }
  }
  return read_count;
}

uint8_t BCDToDec(uint8_t bcd) {
  return ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
}

uint64_t sys_date(dateType *pDate) {
  if (pDate == 0)
    return 0;
  pDate->year = BCDToDec(getYear());
  pDate->month = BCDToDec(getMonth());
  pDate->day = BCDToDec(getDay());
  pDate->hour = BCDToDec(getHour());
  pDate->minute = BCDToDec(getMinute());
  pDate->second = BCDToDec(getSecond());
  return 1;
}

uint64_t sys_mem(uint64_t rdi, uint64_t rsi, uint8_t rdx) {
  uint8_t *src = (uint8_t *)rdi;
  uint8_t *dst = (uint8_t *)rsi;

  // qemu tiene 64GB mapeados en memoria, asi que en el emulador
  // incluso con sólo 512MB de memoria
  // Podés acceder a todas las direcciones hasta 0x1000000000 - 1
  if (src >= 0x1000000000 || src - 1 + rdx >= 0x1000000000)
    return 1;

  uint8_t i;
  for (i = 0; i < rdx; i++)
    dst[i] = src[i];

  return i;
}

void sys_ps(void) { showAllPs(); }

pid_t sys_createPs(uint64_t rip, char *name, int argc, char *argv[], uint8_t mode) {
  return createProcess(rip, 0, name, argc, argv, mode);
}

int sys_block(pid_t pid) { return block(pid); }

int sys_unblock(pid_t pid) { return unblock(pid); }

int sys_kill(pid_t pid) { return kill(pid); }

pid_t sys_getpid(void) { return getCurrentPid(); }

int sys_nice(pid_t pid, int adjustment) {
  int8_t old = getPriority(pid);
  int8_t new = old + adjustment;
  if (new < 0)
    new = 0;
  return setPriority(pid, new);
}

void sys_exit(void) { kill(getCurrentPid()); }