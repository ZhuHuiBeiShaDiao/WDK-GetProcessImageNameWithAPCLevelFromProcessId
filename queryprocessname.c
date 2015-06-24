#include <ntifs.h>
#include <ntddk.h>

#define SYSTEM_PROCESS_NAME L"System"

ULONG g_OSMajorVersion;
ULONG g_OSMinorVersion;

typedef NTSTATUS (*QUERY_INFO_PROCESS) (
    __in HANDLE ProcessHandle,
    __in PROCESSINFOCLASS ProcessInformationClass,
    __out_bcount(ProcessInformationLength) PVOID ProcessInformation,
    __in ULONG ProcessInformationLength,
    __out_opt PULONG ReturnLength
    );

typedef struct _RTL_USER_PROCESS_PARAMETERS {
  unsigned char           Reserved1[16];
  PVOID          Reserved2[10];
  UNICODE_STRING ImagePathName;
  UNICODE_STRING CommandLine;
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef struct _PEB {
  unsigned char                          Reserved1[2];
  unsigned char                          BeingDebugged;
  unsigned char                          Reserved2[1];
  PVOID                         Reserved3[2];
  PVOID                 Ldr;
  PRTL_USER_PROCESS_PARAMETERS  ProcessParameters;
  unsigned char                          Reserved4[104];
  PVOID                         Reserved5[52];
  PVOID PostProcessInitRoutine;
  unsigned char                          Reserved6[128];
  PVOID                         Reserved7[1];
  ULONG                         SessionId;
} PEB, *PPEB;

QUERY_INFO_PROCESS ZwQueryInformationProcess;

#define EPROCESS_PEB_WINDOWSXP_OFFSET_32BIT 0x1B0

#define EPROCESS_PEB_WINDOWS2003_OFFSET_32BIT 0x1A0
#define EPROCESS_PEB_WINDOWS2003_OFFSET_64BIT 0x2C0

#define EPROCESS_PEB_WINDOWS_VISTA_OFFSET_32BIT 0x188
#define EPROCESS_PEB_WINDOWS_VISTA_OFFSET_64BIT 0x290

#define EPROCESS_PEB_WINDOWS7_OFFSET_32BIT 0x1A8
#define EPROCESS_PEB_WINDOWS7_OFFSET_64BIT 0x338

#define EPROCESS_PEB_WINDOWS8_OFFSET_32BIT 0x140
#define EPROCESS_PEB_WINDOWS8_OFFSET_64BIT 0x3E8

#define EPROCESS_PEB_WINDOWS10_OFFSET_32BIT 0x144
#define EPROCESS_PEB_WINDOWS10_OFFSET_64BIT 0x3F8

char *GetPEBOffset(PEPROCESS Process)
{
	char *Offset = NULL;

	if ((g_OSMajorVersion == 10) && (g_OSMinorVersion == 0))
	{
		// Windows 10 Insider preview 입니다.
#ifdef  _WIN64
		Offset = (char *)Process + EPROCESS_PEB_WINDOWS10_OFFSET_64BIT;
#else
		Offset = (char *)Process + EPROCESS_PEB_WINDOWS10_OFFSET_32BIT;
#endif
	}
	else if ((g_OSMajorVersion == 6) && 
		((g_OSMinorVersion == 2) || g_OSMinorVersion == 3))
	{
		// Windows 8, Windows 8.1 입니다. 
#ifdef  _WIN64
		Offset = (char *)Process + EPROCESS_PEB_WINDOWS8_OFFSET_64BIT;
#else
		Offset = (char *)Process + EPROCESS_PEB_WINDOWS8_OFFSET_32BIT;
#endif
	}
	else if ((g_OSMajorVersion == 6) && (g_OSMinorVersion == 1))
	{
		// Windows 7입니다. ( SP1 포함 )
#ifdef  _WIN64
		Offset = (char *)Process + EPROCESS_PEB_WINDOWS7_OFFSET_64BIT;
#else
		Offset = (char *)Process + EPROCESS_PEB_WINDOWS7_OFFSET_32BIT;
#endif
	}
	else if ((g_OSMajorVersion == 6) && (g_OSMinorVersion == 0))
	{
		// Windows Vista 이상 또는 Windows 2008 Server 입니다. ( 모든 서비스팩 )
#ifdef  _WIN64
		Offset = (char *)Process + EPROCESS_PEB_WINDOWS_VISTA_OFFSET_64BIT;
#else
		Offset = (char *)Process + EPROCESS_PEB_WINDOWS_VISTA_OFFSET_32BIT;
#endif
	}
	else if ((g_OSMajorVersion == 5) && (g_OSMinorVersion == 2))
	{
#ifdef  _WIN64
	    // Windows 2003 Server SP1 x64 ( R2포함 ) 이상 또는 Windows XP Professional x64 Edition SP1 이상입니다.
		Offset = (char *)Process + EPROCESS_PEB_WINDOWS2003_OFFSET_64BIT;
#else
		// Windows 2003 Server SP1 ( R2포함 ) 이상입니다.
		Offset = (char *)Process + EPROCESS_PEB_WINDOWS2003_OFFSET_32BIT;
#endif
	}
	else if ((g_OSMajorVersion == 5) && (g_OSMinorVersion == 1))
	{
		// Windows XP 인 경우입니다.
		Offset = (char *)Process + EPROCESS_PEB_WINDOWSXP_OFFSET_32BIT;
	}
	
	return Offset;
}

NTSTATUS GetProcessImageNameWithAPCLevelFromProcessId(PUNICODE_STRING OutputName, HANDLE ProcessId)
{
	NTSTATUS status;
	PEPROCESS Process;

	if (ProcessId == (HANDLE)0x04)
	{
		// 시스템 프로세스인 경우에는 "System"으로 바로 리턴시킵니다.
		return RtlAppendUnicodeToString(OutputName, SYSTEM_PROCESS_NAME);
	}

	status = PsLookupProcessByProcessId(ProcessId, &Process);
	if (NT_SUCCESS(status) == TRUE)
	{
		PPEB Peb;
		char *Offset;
		
		Offset = GetPEBOffset(Process);
		if (Offset != NULL)
		{
			Peb = (PPEB)*(ULONG_PTR *)Offset;
			if (Peb != NULL)
			{
				KAPC_STATE ApcState;
				
				KeStackAttachProcess(Process, &ApcState);
				try
				{
					ProbeForRead(Peb, sizeof(PEB), sizeof(UCHAR));

					if (Peb->ProcessParameters != NULL)
					{
						ProbeForRead(Peb->ProcessParameters, sizeof(RTL_USER_PROCESS_PARAMETERS), sizeof(UCHAR));

						if ((Peb->ProcessParameters->ImagePathName.Buffer != NULL) 
							&& (Peb->ProcessParameters->ImagePathName.Length > 0))
						{
							ProbeForRead(Peb->ProcessParameters->ImagePathName.Buffer,
								Peb->ProcessParameters->ImagePathName.Length,
								sizeof(UCHAR));

							status = RtlAppendUnicodeStringToString(OutputName, &(Peb->ProcessParameters->ImagePathName));
							if (NT_SUCCESS(status) == TRUE)
							{
								NOTHING

								// 테스트용도외에는 주석을 해제하지마세요.
								// Windows Vista 미만에 운영체제에서는 이 루틴이 APC 레벨에서 호출 될 수 있습니다.
								if (KeGetCurrentIrql() == PASSIVE_LEVEL)
								{
									KdPrint(("Unicode string: %wZ\n", OutputName));
								}
							}
						}
						else
						{
							KdPrint(("Peb->ProcessParameters->ImagePathName is invalid\n"));
							status = STATUS_UNSUCCESSFUL;
						}
					}
					else
					{
						KdPrint(("Peb->ProcessParameters is NULL\n"));
						status = STATUS_UNSUCCESSFUL;
					}
				}
				except( EXCEPTION_EXECUTE_HANDLER )
				{
					KdPrint(("EXCEPTION_EXECUTE_HANDLER\n"));
					status = STATUS_ACCESS_VIOLATION;
				}
				KeUnstackDetachProcess(&ApcState);
			}
			else
			{
				KdPrint(("Peb is NULL\n"));
				status = STATUS_UNSUCCESSFUL;
			}
		}
		else
		{
			KdPrint(("GetPEBOffset failed.\n"));
			status = STATUS_UNSUCCESSFUL;
		}
		ObDereferenceObject(Process);
	}
	else
	{
		KdPrint(("PsLookupProcessByProcessId failed\n"));
	}
	return status;
}

NTSTATUS GetProcessImageNameWithAPCLevel(PUNICODE_STRING OutputName)
{
	NTSTATUS status;

	HANDLE ProcessId;
	PEPROCESS Process;

	ProcessId = PsGetCurrentProcessId();

	status = GetProcessImageNameWithAPCLevelFromProcessId(OutputName, ProcessId);
	return status;
}

BOOLEAN InitGetProcessImageNameWithAPCLevel()
{
	NTSTATUS ntStatus;
	RTL_OSVERSIONINFOW OSVersionInfo;

	OSVersionInfo.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);
	ntStatus = RtlGetVersion(&OSVersionInfo);
	if (NT_SUCCESS(ntStatus) == FALSE)
	{
		KdPrint(("RtlGetVersion failed\n"));

		return FALSE;
	}
	g_OSMajorVersion = OSVersionInfo.dwMajorVersion;
	g_OSMinorVersion = OSVersionInfo.dwMinorVersion;
	KdPrint(("major : %u, minor: %u", g_OSMajorVersion, g_OSMinorVersion));
}