#ifndef __MINIFLTLIB_H__
#define __MINIFLTLIB_H__

///////////////////////////////////////////////////////////////
// ���μ��� �̸��� �������� �Լ��Դϴ�.
///////////////////////////////////////////////////////////////
NTSTATUS GetProcessImageNameWithAPCLevelFromProcessId(PUNICODE_STRING OutputName, HANDLE ProcessId);
NTSTATUS GetProcessImageNameWithAPCLevel(PUNICODE_STRING OutputName);

// �� ���̺귯���� ����ϱ����� �ݵ�� �� �Լ��� ����ؼ� �ʱ�ȭ�ؾ� �մϴ�.
BOOLEAN InitMiniFltLib();

#endif