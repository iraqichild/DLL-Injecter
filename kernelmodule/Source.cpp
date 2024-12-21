#include "ioctl.h"

extern "C"
NTSTATUS
DriverEntry(int* a, int* b)
{
	return IoCreateDriver(NULL, IO::V_DriverEntry);
}