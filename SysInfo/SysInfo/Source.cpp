#include <windows.h>
#include <iostream>
#include <Lmcons.h>
#include <Slpublic.h>
#include <Lmwksta.h>
#include <lm.h>
#include <string>

#pragma comment(lib, "netapi32.lib")

void usageMsg() {

	std::cout << "SysInfo v1.0 - Local system information gathering tool\n\n";
}

int main() {

	usageMsg();

	TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD computerNameSize = sizeof(computerName);

	if (!GetComputerNameA(computerName, &computerNameSize)){
		std::cout << "\tFailed to get computer name\n";
		return 0;
	}

	std::cout << "\tComputer Name \t\t: " << computerName << std::endl;

	TCHAR usernameBuffer[UNLEN + 1];
	DWORD usernameBufferSize = UNLEN + 1;

	if (!GetUserNameA(usernameBuffer, &usernameBufferSize)) {
		std::cout << "\tFailed to get username\n";
	}

	std::cout << "\tCurrent Username \t: " << usernameBuffer << std::endl;

	SYSTEM_INFO lpSystemInfo;
	GetSystemInfo(&lpSystemInfo);

	std::cout << "\tCPU Processor count \t: " << lpSystemInfo.dwNumberOfProcessors << std::endl;
	int cpuType = lpSystemInfo.wProcessorArchitecture;

	if (cpuType == 0){
		std::cout << "\tCPU architecture \t: x86 CPU" << std::endl;
	}

	else if (cpuType == 9){
		std::cout << "\tCPU architecture \t: x64 CPU\n";
	}
	std::cout << "\tLowest memory address   : 0x" << lpSystemInfo.lpMinimumApplicationAddress << std::endl;
	std::cout << "\tHighest memory address  : 0x" << lpSystemInfo.lpMaximumApplicationAddress << std::endl;

	SYSTEM_INFO sysinfo;
	::GetNativeSystemInfo(&sysinfo);

	std::cout << "\tConfigured Processors   : " << sysinfo.dwActiveProcessorMask << std::endl;
	std::cout << "\tLogical processors      : " << sysinfo.dwNumberOfProcessors << std::endl;

	/*
	int deviceIndex = 0;
	DISPLAY_DEVICE displayDevice;
	displayDevice.cb = sizeof(displayDevice);
	while (EnumDisplayDevicesA(NULL, deviceIndex, &displayDevice, 0)) {
		int monitorIndex = 0;
		std::cout << "\tMonitor devices \t: " << displayDevice.DeviceName << std::endl;
		std::cout << "\tDevice descriptions\t: " << displayDevice.DeviceString << std::endl;
		deviceIndex++;
	}*/

	static MEMORYSTATUSEX lpBuffer;
	lpBuffer.dwLength = sizeof(lpBuffer);
	if (!GlobalMemoryStatusEx(&lpBuffer)) {
		std::cout << "GlobalMemoryStatusEx failed\n";
		return 0;
	}
	else {
		std::cout << "\tTotal Physical RAM \t: " << lpBuffer.ullTotalPhys / (1024 * 1024) << " MB" << std::endl;
		std::cout << "\tTotal Available RAM\t: " << lpBuffer.ullAvailPhys / (1024 * 1024) << " MB"  << std::endl;
		std::cout << "\tTotal Virtual RAM \t: " << lpBuffer.ullTotalVirtual / (1024 * 1024) << " MB" << std::endl;
	}

	DEP_SYSTEM_POLICY_TYPE depStatus = GetSystemDEPPolicy();
	if (depStatus) {
		switch (depStatus) {

		case 0:
			std::cout << "\tDep Policy \t\t: DEP is set to always off\n";
		case 1:
			std::cout << "\tDep Policy \t\t: DEP is set to always on\n";
		case 2:
			std::cout << "\tDep Policy \t\t: DEP is set to opt-in\n";
		}
	}
	else {
		std::cout << "\tDep Policy \t\t: DEP is set to opt-out]";

	}

	LPWKSTA_INFO_102 pBuf = NULL;
	NET_API_STATUS nStatus;
	LPWSTR pszServerName = NULL;

	nStatus = NetWkstaGetInfo(pszServerName, 102, (LPBYTE*)&pBuf);
	if (nStatus == NERR_Success) {

		std::cout << "\tLogged on users \t: " << pBuf->wki102_logged_on_users << std::endl;
		wprintf(L"\tDomain\t\t\t: %s\n", pBuf->wki102_langroup);
		std::cout << "\tMajor Version \t\t: " << pBuf->wki102_ver_major << ".0" << std::endl;

	}

	return 0;
}