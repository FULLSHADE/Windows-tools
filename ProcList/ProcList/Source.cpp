#include <windows.h>
#include <iostream>
#include <Psapi.h>
#include <Tlhelp32.h>

#define PROCESS_ARRAY 2048

int main() {

	int i;
	DWORD lpidProcess[PROCESS_ARRAY], lpcbNeeded, cProcesses;

	if (!EnumProcesses(lpidProcess, sizeof(lpidProcess), &lpcbNeeded))
	{
		std::cout << "EnumProcesses failed - " << GetLastError() << std::endl;
		return 0;
	}
	else
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (!hSnapshot) {
			std::cout << "CreateTool failed - " << GetLastError() << std::endl;
		}
		else {

			PROCESSENTRY32 p32;
			p32.dwSize = sizeof(PROCESSENTRY32);

			if (Process32First(hSnapshot, &p32))
			{
				do {
					std::cout << "PID [" << p32.th32ProcessID << "]" << " Process name: " << p32.szExeFile << std::endl;
					std::cout << " \t\t|__ Thread count : " << p32.cntThreads << std::endl;
					std::cout << " \t\t|__ Parent process PID : " << p32.th32ParentProcessID << std::endl;

				} while (Process32Next(hSnapshot, &p32));

				CloseHandle(hSnapshot);
				system("pause");
			}
		}

	}

	return 0;
}