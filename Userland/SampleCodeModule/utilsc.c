#include <utils.h>
#include <lib.h>

void printDate() {
	dateType currDate;
	fillDate(&currDate);
	print_f(1, "Date: %d/%d/%d\nTime: %d:%d:%d (UTC)\n", currDate.day, currDate.month, currDate.year + 2000, currDate.hour, currDate.minute, currDate.second);
}