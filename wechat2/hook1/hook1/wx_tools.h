#pragma once
#include "Windows.h"
#include <stdio.h>


//username         ��ַ : 647040CC        ƫ���� : 17740CC
//phone_number     ��ַ : 64704100        ƫ���� : 1774100
//wx_id            ��ַ : 64704230        ƫ���� : 1774230
//os               ��ַ : 64704520        ƫ���� : 1774520
//avatar           ��ַ : 64704394        ƫ���� : 1774394 *

class Wx
{
	DWORD winAdd;
public:
	Wx()
	{
		HMODULE winAddH = LoadLibrary(TEXT("WeChatWin.dll"));
		winAdd = (DWORD)winAddH;
	}
	CHAR* GetWxID()
	{
		return readWechatData(0x1774230);
	}
	CHAR * GetPhoneNumber()
	{
		return readWechatData(0x1774100);
	}
	CHAR * GetUserName()
	{
		return readWechatData(0x17740CC);
	}
	CHAR * GetOS()
	{
		return readWechatData(0x1774520);
	}
	CHAR * GetAvatar()  // avatar����һ��ָ��
	{
		DWORD avatarPit = getPointerAddress(0x1774394);
		// (DWORD*)avatarPit;   ��������תΪָ��
		*(DWORD*)avatarPit;
		CHAR data[0x100] = { 0 };
		sprintf_s(data, "%s", *(DWORD*)avatarPit);
		return data;
	}
	VOID UpdataWxID(char * wx_id)
	{
		// ����΢��ID
		// GetCurrentProcess ��ȡ��ǰ���̾��
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)(winAdd + 0x1774230),wx_id,sizeof(wx_id),NULL);
	}
private:
	CHAR * readWechatData(DWORD addr)
	{
		CHAR data[0x100] = { 0 };
		sprintf_s(data,"%s",winAdd + addr);
		return data;
	}
	DWORD getPointerAddress(DWORD addr)
	{
		return winAdd + addr;
	}
};