#include <utils.h>
#include <lib.h>
#include <cpuid.h>
#include <cpuidflags.h>

#define BUFFER_SIZE 16

#define ISHEXA(x)                                                              \
	(((x) >= 'a' && (x) <= 'f') || ((x) >= 'A' && (x) <= 'F') || ISDIGIT(x))

#define ISVOWEL(c)                                                             \
	((c) == 'a' || (c) == 'A' || (c) == 'e' || (c) == 'E' || (c) == 'i' || \
	 (c) == 'I' || (c) == 'o' || (c) == 'O' || (c) == 'u' || (c) == 'U')

uint8_t counter = 0;

void sleep(int seconds)
{
	int start = _seconds_elapsed();
	while (1) {
		int currentSeconds = _seconds_elapsed();
		if (currentSeconds - start >= seconds)
			return;
	}
}

void loop(int argc, char *argv[])
{
	pid_t pid = _get_pid();
	while (1) {
		print_f(STDOUT, "\nMi PID es: %d\n", pid);
		sleep(3);
	}
	_exit();
}

void cat(int argc, char *argv[])
{
	int c;
	while ((c = _get_char(STDIN)) != -1)
		print_f(STDOUT, "%c", c);
	_exit();
}

void wc(int argc, char *argv[])
{
	int lines = 0;
	int c;
	while ((c = _get_char(STDIN)) != -1)
		if (c == '\n')
			lines++;
	print_f(1, "\n");
	print_f(1, "\nLa cantidad de lineas que recibi son: %d\n", lines);
	_exit();
}

void filter(int argc, char *argv[])
{
	int c;
	while ((c = _get_char(STDIN)) != -1)
		if (ISVOWEL(c))
			print_f(STDOUT, "%c", c);
	print_f(STDOUT, "\n");
	_exit();
}
