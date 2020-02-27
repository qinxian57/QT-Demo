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
#define TICKSPERSEC 10000000LL //1s内的100纳秒间隔
#define TICKS_1601_TO_1970 (SECS_1601_TO_1970 * TICKSPERSEC) 

namespace nsData
{
	enum { local = 0, share = 1, ftp = 2, all = 3, amp = 4, imagine = 5 };//文件访问方式 本地，共享，FTP...
};


/*
得到文件的修改时间和创建时间
*/
bool GlobalGetFileTime(std::wstring FileName/*, COleDateTime &ModifyTime, COleDateTime &CreateTime*/);
//bool GlobalGetFileTime(HANDLE hFile, COleDateTime &ModifyTime, COleDateTime &CreateTime);

std::string GetFileVersion(const std::wstring strFilePath, const std::wstring &param);

