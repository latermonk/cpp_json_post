/*******************************************************
Function : Base64编码解码入口
.
CopyRight: GrgBanking
Author   :
================= Modified list ====================
Version    : // 当前版本 应符合版本管理规定，如v1.0.0
Modify Date: // 修改日期，格式为YYYYMMDD
Mender     : // 修改者，用邮箱前缀，首字母大写
Why        : // 修改原因
How        : // 修改内容
Attention  : // 注意事项
*******************************************************/



#pragma once

#ifndef BYTE
typedef unsigned char BYTE;
#endif
#ifndef byte
typedef BYTE byte;
#endif
#ifndef DWORD
typedef unsigned long       DWORD;
#endif
namespace base64
{
	void encode(BYTE *bInput, DWORD dwInputLen, BYTE *bOutput, DWORD *dwOutputLen);
	void decode(BYTE *bInput, DWORD dwInputLen, BYTE *bOutput, DWORD *dwOutputLen);
};