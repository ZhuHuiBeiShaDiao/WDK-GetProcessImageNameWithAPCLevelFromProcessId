NTSTATUS GetProcessImageNameWithAPCLevelFromProcessId(PUNICODE_STRING OutputName, HANDLE ProcessId);
// 프로세스 이름을 PEB로 부터 가져오며, APC_LEVEL에서도 사용 할 수 있습니다.
NTSTATUS GetProcessImageNameWithAPCLevel(PUNICODE_STRING OutputName);

BOOLEAN InitGetProcessImageNameWithAPCLevel();