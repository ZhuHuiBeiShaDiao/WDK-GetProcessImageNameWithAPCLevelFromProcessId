#include <Ntddk.h>

ULONG g_OSMajorVersion;
ULONG g_OSMinorVersion;

BOOLEAN InitMiniFltLib()
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

	return TRUE;
}