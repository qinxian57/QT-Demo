#pragma once
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
//#include <atltime.h>
#include <ATLComTime.h>

#define SECSPERDAY 86400LL /* one day seconds */
#define SECS_1601_TO_1970 ((369LL * 365LL + 89LL) * (ULONGLONG)SECSPERDAY)/* 1601 to 1970 is 369 years plus 89 leap days */
#define CUR_LEAPSECONDS_FROM_1970  37LL  //from 1970.1.1 to 2017-9-29 LeapSeconds
#define TICKSPERSEC 10000000LL //1s�ڵ�100������
#define TICKS_1601_TO_1970 (SECS_1601_TO_1970 * TICKSPERSEC) 

namespace nsData
{
	enum { local = 0, share = 1, ftp = 2, all = 3, amp = 4, imagine = 5 };//�ļ����ʷ�ʽ ���أ�����FTP...
};


/*
�õ��ļ����޸�ʱ��ʹ���ʱ��
*/
bool GlobalGetFileTime(std::wstring FileName/*, COleDateTime &ModifyTime, COleDateTime &CreateTime*/);
//bool GlobalGetFileTime(HANDLE hFile, COleDateTime &ModifyTime, COleDateTime &CreateTime);

std::string GetFileVersion(const std::wstring strFilePath, const std::wstring &param);

