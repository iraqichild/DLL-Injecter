#include "ioctl.h"

NTSTATUS IO::V_DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegPat)
{
	NTSTATUS Status;

	Status = STATUS_UNSUCCESSFUL;

	PDEVICE_OBJECT obj;

	Status = IoCreateDevice(DriverObject, 0, &IO::DriverName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &obj);
	if (!NT_SUCCESS(Status))
		return Status;


	Status = IoCreateSymbolicLink(&IO::SymbolicName, &IO::DriverName);
	if (!NT_SUCCESS(Status))
	{
		::IoDeleteDevice(obj);
	     return Status;
	}
	


	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = &IO::DriverDispatch;

	obj->Flags |= DO_BUFFERED_IO;
	obj->Flags &= ~DO_DEVICE_INITIALIZING;


	return Status;
}

#include <ntddk.h>

_KTHREAD* Get_KTHREAD(int ProcessPid, int ThreadPid)
{
	PEPROCESS CurrentProcess;
	PETHREAD CurrentThread;
	KAPC_STATE CurrentApc;
	PLIST_ENTRY ListHead, ListEntry;

	NTSTATUS Status = PsLookupProcessByProcessId((HANDLE)ProcessPid, &CurrentProcess);
	if (!NT_SUCCESS(Status)) {
		return NULL;
	}

	KeStackAttachProcess(CurrentProcess, &CurrentApc);

	ListHead = &CurrentProcess->ThreadListHead;
	for (ListEntry = ListHead->Flink; ListEntry != ListHead; ListEntry = ListEntry->Flink) {
		CurrentThread = CONTAINING_RECORD(ListEntry, _KTHREAD, ThreadListEntry);



		
		
		
	}

	KeUnstackDetachProcess(&CurrentApc);

	return NULL;
}


NTSTATUS IO::DriverDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	NTSTATUS Status = {};
	ULONG BytesReturned = {};
	PIO_STACK_LOCATION Stack = IoGetCurrentIrpStackLocation(Irp);

	ULONG IoControlCode = Stack->Parameters.DeviceIoControl.IoControlCode;

	switch (IoControlCode)
	{
	case EX_GET_KTHREAD:

	default:
		break;
	}


}