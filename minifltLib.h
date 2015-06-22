#ifndef __MINIFLTLIB_H__
#define __MINIFLTLIB_H__

///////////////////////////////////////////////////////////////
// 프로세스 이름을 가져오는 함수입니다.
///////////////////////////////////////////////////////////////
NTSTATUS GetProcessImageNameWithAPCLevelFromProcessId(PUNICODE_STRING OutputName, HANDLE ProcessId);
NTSTATUS GetProcessImageNameWithAPCLevel(PUNICODE_STRING OutputName);

// 이 라이브러리를 사용하기전에 반드시 이 함수를 사용해서 초기화해야 합니다.
BOOLEAN InitMiniFltLib();

#endif