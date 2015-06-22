#ifndef __MINIFLTLIB_H__
#define __MINIFLTLIB_H__

#define MINIFLT_LIB_TAG 'tlfM'

typedef struct
{
	UNICODE_STRING uniName;
	ANSI_STRING ansiString;
} NAME_INFORMATION, *PNAME_INFORMATION;

///////////////////////////////////////////////////////////////
// ���� �̸��� �������� ���� �Լ����� ����Ǿ����ϴ�.
///////////////////////////////////////////////////////////////
PVOID AllocatePoolWithFillZero( 
	ULONG dwType, 
	ULONG dwSize 
	);

NTSTATUS CreateNameInformationMemory( 
	PNAME_INFORMATION * ppObjNameInfo 
	);

NTSTATUS DeleteNameInformationMemory( 
	PNAME_INFORMATION pObjNameInfo 
	);

NTSTATUS QueryFileName(
	PFILE_OBJECT pFileObj, 
	PNAME_INFORMATION pObjNameInfo
	);

BOOLEAN QueryFileNameEx(__in PFLT_CALLBACK_DATA Data,
								__in PCFLT_RELATED_OBJECTS FltObjects,
								__out PFLT_FILE_NAME_INFORMATION *nameInfo,
								__out PNAME_INFORMATION *pNameInfo,
								__out PUNICODE_STRING *nameToUse);
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
// ���μ��� �̸��� �������� �Լ��Դϴ�.
///////////////////////////////////////////////////////////////
NTSTATUS GetProcessImageNameWithAPCLevelFromProcessId(PUNICODE_STRING OutputName, HANDLE ProcessId);
NTSTATUS GetProcessImageNameWithAPCLevel(PUNICODE_STRING OutputName);

// �� ���̺귯���� ����ϱ����� �ݵ�� �� �Լ��� ����ؼ� �ʱ�ȭ�ؾ� �մϴ�.
BOOLEAN InitMiniFltLib();

#endif