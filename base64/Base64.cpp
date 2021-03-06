//#include "stdafx.h"
#include "Base64.h"

using namespace base64;


//1.1) Base64编码
void base64::encode( BYTE *bInput, DWORD dwInputLen, BYTE *bOutput, DWORD *dwOutputLen )
{
	int i,n,pos,max;
	int len_out=0;
	BYTE igroup[3], ogroup[4];
	BYTE dtable[64];	      
	DWORD len_in;


	for (i = 0; i < 26; i++) {
		dtable[i] = 'A' + i;
		dtable[26 + i] = 'a' + i;
	}
	for (i = 0; i < 10; i++) {
		dtable[52 + i] = '0' + i;
	}
	dtable[62] = '+';
	dtable[63] = '/';

	for(len_in = 0; len_in<dwInputLen;len_in+=3)
	{
		pos=len_in;
		igroup[0] = igroup[1] = igroup[2] = 0;
		if(dwInputLen-len_in>=3)
			max=3;
		else 
			max=dwInputLen-len_in;
		for (n = 0; n < max; n++) 
		{
			igroup[n] = bInput[n+pos];
		}
		if (n > 0) 
		{
			ogroup[0] = dtable[igroup[0] >> 2];
			ogroup[1] = dtable[((igroup[0] & 3) << 4) | (igroup[1] >> 4)];
			ogroup[2] = dtable[((igroup[1] & 0xF) << 2) | (igroup[2] >> 6)];
			ogroup[3] = dtable[igroup[2] & 0x3F];

			// Replace characters in output stream with "=" pad
			// characters if fewer than three characters were
			// read from the end of the input stream. 

			if (n < 3) 
			{
				ogroup[3] = '=';
				if (n < 2) 
				{
					ogroup[2] = '=';
				}
			}
			for (i = 0; i < 4; i++) 
			{
				bOutput[len_out++]=ogroup[i];
			}
		}
	}
	*dwOutputLen = len_out;
}

//1.2) Base64解码
void base64::decode(BYTE *bInput, DWORD dwInputLen, BYTE *bOutput, DWORD *dwOutputLen)
{
	int i;
	int pos,step,lastcharnum;
	int len_out=0;
	BYTE a[4], b[4],o[3];
	BYTE dtable[255];
	DWORD len_in;
	for (i = 0; i < 255; i++) 
	{
		dtable[i] = 0x80;
	}
	for (i = 'A'; i <= 'Z'; i++) 
	{
		dtable[i] = 0 + (i - 'A');
	}
	for (i = 'a'; i <= 'z'; i++) 
	{
		dtable[i] = 26 + (i - 'a');
	}
	for (i = '0'; i <= '9'; i++) 
	{
		dtable[i] = 52 + (i - '0');
	}
	dtable['+'] = 62;
	dtable['/'] = 63;
	dtable['='] = 0;

	for(len_in=0;len_in<dwInputLen;len_in=len_in+step)
	{
		pos=len_in;
		step = 0;

		for (i = 0; i < 4; i++) 
		{
			if (dtable[bInput[i+pos]] & 0x80)
			{
				step++;
				pos++;
				i--;
				continue;
			}
			a[i] = bInput[i+pos];
			b[i] = (BYTE) dtable[bInput[i+pos]];
			step++;
		}

		o[0] = (b[0] << 2) | (b[1] >> 4);
		o[1] = (b[1] << 4) | (b[2] >> 2);
		o[2] = (b[2] << 6) | b[3];
		lastcharnum = a[2] == '=' ? 1 : (a[3] == '=' ? 2 : 3);

		for (i = 0; i < lastcharnum; i++) 
		{
			bOutput[len_out++] = o[i];
		}
	}
	*dwOutputLen = len_out;
}
