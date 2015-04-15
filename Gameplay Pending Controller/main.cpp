#include <windows.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <tchar.h>
#include <thread>
#include "GetProcessID.h"

using namespace std;

DWORD GetPID(LPCTSTR processName);
bool WriteMem(LPCTSTR processName, LPVOID location, byte value);
int ReadMem(LPCTSTR processName, LPVOID location);
void GameplayPending(bool status);

int main(void) {
	cout << "Renegade Gameplay Pending Controller!\n\n";
	char ans;
	while (true) {
		cout << "Gameplay Pending (Y/N): ";
		cin >> ans;

		if (ans == 'Y') {
			GameplayPending(true);
		}
		else {
			GameplayPending(false);
		}

	}
	cin.get();
	return 0;
}

void GameplayPending(bool status) {
	if (status) {
		WriteMem("rp2server2", (LPVOID)0x478A93, 0x7E);
	}
	else {
		WriteMem("rp2server2", (LPVOID)0x478A93, 0xEB);
	}
}
bool WriteMem(LPCTSTR processName, LPVOID location, byte value) {
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, false, GetPID(processName));
	if (!hProc) {
		cerr << "Cannot write memory" << endl;
		return false;
	}
	else {
		int isSuccessful = WriteProcessMemory(hProc, location, &value, 1, NULL);
		CloseHandle(hProc);
		if (isSuccessful > 0) {
			return true;
		}
		else {
			return false;
		}
	}
}
int ReadMem(LPCTSTR processName, LPVOID location) {
	int output;
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, false, GetPID(processName));
	if (!hProc) {
		cerr << "Cannot read memory" << endl;
		return 0;
	}
	else {
		int MemRd = ReadProcessMemory(hProc, location, &output, sizeof(&output), NULL);
		return output;
		CloseHandle(hProc);
	}
}
DWORD GetPID(LPCTSTR processName) {
	std::vector<DWORD> SetOfPID;
	GetProcessID(processName, SetOfPID);
	if (SetOfPID.empty()) {
		return 0;
	}
	return SetOfPID[0];
}