/*******************************************************
Function : Base64����������
.
CopyRight: GrgBanking
Author   :
================= Modified list ====================
Version    : // ��ǰ�汾 Ӧ���ϰ汾����涨����v1.0.0
Modify Date: // �޸����ڣ���ʽΪYYYYMMDD
Mender     : // �޸��ߣ�������ǰ׺������ĸ��д
Why        : // �޸�ԭ��
How        : // �޸�����
Attention  : // ע������
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