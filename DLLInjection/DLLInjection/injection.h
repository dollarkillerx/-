#pragma once
#include <exception>
#include <Windows.h>
#include <TlHelp32.h>

// ͨ���������ƻ�ȡ����ID   
// Params: ��������
// Return: ����ID  (�����ڷ���0)
DWORD ProcessNameFindPID(char* ProcessName)
{
	// ��ȡ��ǰϵͳ���̿��� 
	HANDLE ProcessAll = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	// �ҵ�������ͬ��
	PROCESSENTRY32 processInfo = { 0 };
	processInfo.dwSize = sizeof(PROCESSENTRY32);
	do
	{
		// th32ProcessID ����ID
		// szExeFile ��������
		if (strcmp(ProcessName, processInfo.szExeFile) == 0)
		{
			return processInfo.th32ProcessID;
		}
	} while (Process32Next(ProcessAll, &processInfo));

	return 0;
}

// ע��
// Params: ����ID,dll��ַ
bool InjectDLL(DWORD dwProcessId, char* dllPath)
{
	try
	{
		// ��ȡ���̾��
		// params: ���̷���Ȩ��(PROCESS_ALL_ACCESSȫ����),�Ƿ�̳о��FALSE������ID,
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);

		// Զ������һ���ڴ�
		// params: Զ�̵�ַ,�ڴ���ʼ��ַ,��С(+1��ֹ���),�ڴ��������,����ҳ��������ڴ汣��
		// ����ɹ��������ڴ��ַ
		SIZE_T paramsLen = strlen(dllPath) + 1;
		LPVOID paramsAddress = VirtualAllocEx(hProcess, NULL, paramsLen, MEM_COMMIT, PAGE_READWRITE);
		if (paramsAddress == nullptr)
		{
			throw "�����ڴ�ʧ��";
		}

		// д�ڴ�
		// params�� ���̵�ַ,������ַ,����,����,NULL
		WriteProcessMemory(hProcess, paramsAddress, dllPath, paramsLen, NULL);

		// ��ȡLoadLibrary ������ַ
		HMODULE load_addr = LoadLibrary(TEXT("Kernel32.dll"));
		LPTHREAD_START_ROUTINE load_libaray_addr = (LPTHREAD_START_ROUTINE)GetProcAddress(load_addr, "LoadLibraryA");


		// params: ���̾��,ΪNULLĬ�ϰ�ȫ������,��ջ��ʼ��С0Ĭ��,����ָ�룬����������ָ�룬����״̬0�������У�Զ��ָ��������ָ��
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, load_libaray_addr, paramsAddress, 0, NULL);
		if (hThread == nullptr)
		{
			throw "�̴߳���ʧ��";
		}
		// �ȴ��߳��¼�����
		WaitForSingleObject(hThread, 2000);

		// �ͷ�����������ڴ�ռ�
/*
	 ���ͷ�����������ڴ�ռ䡿
  ����1��Ŀ����̵ľ�����þ������ӵ�� PROCESS_VM_OPERATION Ȩ��
  ����2��ָ��Ҫ�ͷŵ������ڴ�ռ��׵�ַ��ָ��
  ����3�������ڴ�ռ���ֽ���
  ����4��MEM_DECOMMIT����ʾ�ڴ�ռ䲻���ã��ڴ�ҳ�������ڡ�
		 MEM_RELEASE���ַ�ʽ�ܳ��ף���ȫ���ա�
  */
		VirtualFreeEx(hProcess, paramsAddress, paramsLen, MEM_DECOMMIT);
		// ������Դ
		CloseHandle(hThread);
		CloseHandle(hProcess);

	}
	catch (std::exception& e)
	{
		MessageBox(NULL, TEXT("DLLע���쳣"), TEXT("ERRO"), MB_OK);
		return false;
	}

	return true;
}

// ж��DLL
// params: ����ID��dllpath
bool UnInjectDll(DWORD dwProcessId, char* dllPath)
{
	try
	{
		// get process handle
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
		if (hProcess == nullptr)
		{
			throw "Open Process Error";
		}

		HMODULE freeLibrary = LoadLibrary(TEXT("Kernel32.dll"));
		LPTHREAD_START_ROUTINE freeLibraryAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(freeLibrary, "FreeLibrary");
		// params: ���̾��,ΪNULLĬ�ϰ�ȫ������,��ջ��ʼ��С0Ĭ��,����ָ�룬����������ָ�룬����״̬0�������У�Զ��ָ��������ָ��
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, freeLibraryAddr, NULL, 0, NULL);
		if (hThread == nullptr)
		{
			throw "�̴߳���ʧ��";
		}
		// �ȴ��߳��¼�����
		WaitForSingleObject(hThread, 2000);
		// ������Դ
		CloseHandle(hThread);
		CloseHandle(hProcess);
	}
	catch (std::exception& e)
	{
		MessageBox(NULL, TEXT("DLLж���쳣"), TEXT("ERRO"), MB_OK);
		return false;
	}

	return true;
}
