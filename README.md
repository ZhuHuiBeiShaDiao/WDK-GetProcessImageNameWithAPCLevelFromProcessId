# WDK-GetProcessImageNameWithAPCLevelFromProcessId
WDK-GetProcessImageNameWithAPCLevelFromProcessId

커널 드라이버에서 InitMiniFltLib 함수를 호출 후 아래 두 함수를 호출하여 PASSIVE_LEVEL 및 APC_LEVEL에서 프로세스 이름을 가져올 수 있습니다.

GetProcessImageNameWithAPCLevel, GetProcessImageNameWithAPCLevelFromProcessId

Here's an example:

```
FLT_PREOP_CALLBACK_STATUS
SpyPreOperationCallback(
__inout PFLT_CALLBACK_DATA Data,
__in PCFLT_RELATED_OBJECTS FltObjects,
__deref_out_opt PVOID *CompletionContext
)
{
	if (KeGetCurrentIrql() <= APC_LEVEL)
	{
		UNICODE_STRING OutputName;
		OutputName.Length = 0;
		OutputName.MaximumLength = 2048 * sizeof(WCHAR);
		OutputName.Buffer = ExAllocatePoolWithTag(PagedPool, OutputName.MaximumLength, 0);
		if (OutputName.Buffer != NULL)
		{
			// APC_LEVEL일 경우가 있기 때문에 APC_LEVEL에서도 프로세스 이름을 가져오는 함수를 호출합니다.
			status = GetProcessImageNameWithAPCLevel(&OutputName);
			if (NT_SUCCESS(status) == TRUE)
			{
			}
			ExFreePoolWithTag(OutputName.Buffer, 0);
		}
	}
}

NTSTATUS
DriverEntry(
__in PDRIVER_OBJECT DriverObject,
__in PUNICODE_STRING RegistryPath
)
{
	if (InitMiniFltLib() == FALSE)
	{
		KdPrint(("InitMiniFltLib failed\n"));

		return STATUS_UNSUCCESSFUL;
	}


	..................
}
```
