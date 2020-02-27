#pragma once
#include "public.h"
#include <algorithm>
#include <functional>
#include <cctype>
//#include <Tlhelp32.h>
//#include <wininet.h>
#include <iomanip>
//#include <Ws2tcpip.h>
#include <winver.h>

//
//
//TCHAR g_ComputerName[_MAX_PATH];
//
//int g_SecondFrameValue = 25;
//IPId
//int g_nMachineId = 0;

bool GlobalGetFileTime(std::wstring FileName/*, COleDateTime &ModifyTime, COleDateTime &CreateTime*/)
{
    HANDLE hFile = ::CreateFile(FileName.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return false;
    FILETIME ftCreate, ftModify, ftAccess;
    if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftModify))
    {
        ::CloseHandle(hFile);
        return false;
    }
    SYSTEMTIME stLocal;
    //CTime LocalTime;
    ZeroMemory(&stLocal, sizeof(SYSTEMTIME));
    FileTimeToSystemTime(&ftCreate, &stLocal);
    //COleDateTime OleCreateTime(stLocal);
    //CreateTime = OleCreateTime;

    ZeroMemory(&stLocal, sizeof(SYSTEMTIME));
    FileTimeToSystemTime(&ftModify, &stLocal);
    //COleDateTime OleModifyTime(stLocal);
    //ModifyTime = OleModifyTime;
    ::CloseHandle(hFile);
    return true;
}
/*
bool GlobalGetFileTime(HANDLE hFile, COleDateTime &ModifyTime, COleDateTime &CreateTime)
{
    if (hFile == INVALID_HANDLE_VALUE) return false;
    FILETIME ftCreate, ftModify, ftAccess;
    if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftModify))
    {
        ::CloseHandle(hFile);
        return false;
    }
    SYSTEMTIME stLocal;
    //CTime LocalTime;
    ZeroMemory(&stLocal, sizeof(SYSTEMTIME));
    FileTimeToSystemTime(&ftCreate, &stLocal);
    //LocalTime.GetAsSystemTime(stLocal);
    COleDateTime OleCreateTime(stLocal);
    CreateTime = OleCreateTime;

    ZeroMemory(&stLocal, sizeof(SYSTEMTIME));
    FileTimeToSystemTime(&ftModify, &stLocal);
    //LocalTime.GetAsSystemTime(stLocal);
    COleDateTime OleModifyTime(stLocal);
    ModifyTime = OleModifyTime;
    ::CloseHandle(hFile);
    return true;
}
*/

std::string GetFileVersion(const std::wstring strFilePath, const std::wstring &param)
{
    DWORD dwSize;
    DWORD dwRtn;
    std::string szVersion;

    //·Ú§³
    dwSize = GetFileVersionInfoSize(strFilePath.c_str(),NULL);
    if (dwSize == 0)
    {
        return "";
    }

    char *pBuf;
    pBuf= new char[dwSize + 1];
    if(pBuf == NULL)
        return "";
    memset(pBuf, 0, dwSize + 1);
    //·Ú
    dwRtn = GetFileVersionInfo(strFilePath.c_str(),NULL, dwSize, pBuf);
    if(dwRtn == 0)
    {
    return "";
    }

    LPVOID lpBuffer = NULL;
    UINT uLen = 0;
    //·Ú§Ý
    dwRtn = VerQueryValue(pBuf, (std::wstring(TEXT("\\StringFileInfo\\080404b0\\")) + param).c_str(), &lpBuffer,&uLen);

                          //0804
    //04b01252,ANSI
    //ResourceView§ÖVersionBlockHeader§á



    if(dwRtn == 0)
    {
    return "";
    }
    szVersion = (char*)lpBuffer;

    delete pBuf;
    return szVersion;
}
