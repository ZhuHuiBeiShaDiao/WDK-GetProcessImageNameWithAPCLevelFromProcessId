#ifndef __MINIFLTLIB_H__
#define __MINIFLTLIB_H__

#define MINIFLT_LIB_TAG 'tlfM'

typedef struct
{
	UNICODE_STRING uniName;
	ANSI_STRING ansiString;
} NAME_INFORMATION, *PNAME_INFORMATION;

///////////////////////////////////////////////////////////////
// 파일 이름을 가져오기 위한 함수들이 선언되었습니다.
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
// 프로세스 이름을 가져오는 함수입니다.
///////////////////////////////////////////////////////////////
NTSTATUS GetProcessImageNameWithAPCLevelFromProcessId(PUNICODE_STRING OutputName, HANDLE ProcessId);
NTSTATUS GetProcessImageNameWithAPCLevel(PUNICODE_STRING OutputName);

// 이 라이브러리를 사용하기전에 반드시 이 함수를 사용해서 초기화해야 합니다.
BOOLEAN InitMiniFltLib();

#endif