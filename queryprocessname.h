NTSTATUS GetProcessImageNameWithAPCLevelFromProcessId(PUNICODE_STRING OutputName, HANDLE ProcessId);
// ���μ��� �̸��� PEB�� ���� ��������, APC_LEVEL������ ��� �� �� �ֽ��ϴ�.
NTSTATUS GetProcessImageNameWithAPCLevel(PUNICODE_STRING OutputName);

BOOLEAN InitGetProcessImageNameWithAPCLevel();