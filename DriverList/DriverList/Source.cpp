#include <windows.h>
#include <iostream>
#include <Psapi.h>

#define ARRAY_SIZE 1024

void usageMsg() {

	std::cout << "\t__ DriverList ___\n\n";

	std::cout << "A mini Windows tool to view loaded modules,\nand their associated kernel base address\n\n";

}

int main(int argc, char* argv[])
{

	usageMsg();
	system("pause");

	std::cout << "\n\n[---------------------------------------- loaded drivers----------------------------------------]\n\n";

	LPVOID lpImageBase[ARRAY_SIZE];
	DWORD lpcbNeeded;
	int i, cd;

	if (!EnumDeviceDrivers(lpImageBase, sizeof(lpImageBase), &lpcbNeeded)) 
	{
		std::cout << "EnumeDeviceDrivers failed " << GetLastError << std::endl;
		return 0;
	}
	else 
	{
		cd = lpcbNeeded / sizeof(lpImageBase[0]);
		std::cout << "There are " << cd << " drivers loaded." << std::endl;

		TCHAR szDriver[ARRAY_SIZE];

		for (i = 0; i < cd; i++) {

			if (!GetDeviceDriverBaseNameA(lpImageBase[i], szDriver, sizeof(szDriver))) 
			{
				std::cout << "[!] Failed to get loaded driver names" << " Erorr code - " << GetLastError() << std::endl;
			}
			else 
			{

				LPVOID drivers[1024];
				DWORD cbNeeded;

				EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded);
				PVOID KernelBaseAddress = { 0 };
				KernelBaseAddress = drivers[i];
				std::cout << "[" << i + 1 << "] " << "Kernel base address -  0x" << KernelBaseAddress;

				std::cout << " - Driver name: " << szDriver << std::endl;

			}
		}
		system("pause");
	}

}
