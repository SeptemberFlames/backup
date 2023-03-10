#include "stdafx.h"
#include "mybase64.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "ctype.h"
const char gBase[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=" };
static union
{//为了方便操作
	struct
	{
		unsigned long d : 6;
		unsigned long c : 6;
		unsigned long b : 6;
		unsigned long a : 6;
	} s;
	unsigned char c[3];
} u;

//编码
char *gEncBase64(char* data, unsigned long inlen, char *outdata, unsigned long *outlen)
{
	char *p = NULL, *p1 = NULL;
	int tlen = 0, i = 0, j = 0;
	if (data == NULL || inlen == 0)
		return NULL;//传入参数不对 
	tlen = inlen / 3;
	if (inlen % 3 != 0)
		tlen = tlen + 1;
	tlen = tlen * 4;
	*outlen = tlen;//编码后的数据长度

	p = data;
	p1 = outdata;
	tlen = inlen;

	while (tlen > 0)
	{
		//循环编码
		u.c[0] = u.c[1] = u.c[2] = 0;
		for (i = 0; i < 3; i++)
		{
			//取3字节
			if (tlen <= 0) break;
			u.c[2 - i] = (char)*p;
			tlen--; p++;
		}
		if (i == 0)
		{
			break;//没取到
		}
		switch (i)
		{
			//根据取到的字节数分情况
			case 1:
				//取到1字节
				*p1 = gBase[u.s.a]; p1++;
				*p1 = gBase[u.s.b]; p1++;
				*p1 = gBase[64]; p1++;//'=';
				*p1 = gBase[64]; p1++;//'=';
				break;
			case 2:
				//取到2字节
				*p1 = gBase[u.s.a]; p1++;
				*p1 = gBase[u.s.b]; p1++;
				*p1 = gBase[u.s.c]; p1++;
				*p1 = gBase[64]; p1++;//'=';
				break;
			case 3:
				//取到3字节 
				*p1 = gBase[u.s.a]; p1++;
				*p1 = gBase[u.s.b]; p1++;
				*p1 = gBase[u.s.c]; p1++;
				*p1 = gBase[u.s.d]; p1++;
				break;
			default:
				break;
		}
	}
	return outdata;
}

char* gDecBase64(char* data, unsigned long inlen, char *outdata, unsigned long *outlen)
{
	//解码
	char ch1 = 0, ch2 = 0, ch3 = 0, ch4 = 0, *p = NULL;
	char *p1 = NULL, *p2 = NULL, *p3 = NULL, *p4 = NULL;
	int tlen = 0, rlen = 0;

	if (data == NULL || inlen == 0)
	{
		return NULL;
	}

	tlen = inlen * 3 / 4;
	p = data;
	tlen = inlen;
	rlen = 0;

	while (tlen > 0)
	{
		//循环解码
		if (tlen > 0)
		{//取第1位
			ch1 = *p; p++; tlen--;
			if ((p1 = (char*)strchr(gBase, ch1)) == NULL)//不在表中
				return 0;
		}
		if (tlen > 0)
		{//取第2位
			ch2 = *p; p++; tlen--;
			if ((p2 = (char*)strchr(gBase, ch2)) == NULL)//不在表中
				return 0;
		}
		if (tlen > 0)
		{//取第3位
			ch3 = *p; p++; tlen--;
			if ((p3 = (char*)strchr(gBase, ch3)) == NULL)//不在表中
				return 0;
		}
		if (tlen > 0)
		{//取第4位
			ch4 = *p; p++; tlen--;
			if ((p4 = (char*)strchr(gBase, ch4)) == NULL)//不在表中
				return 0;
		}
		if ((ch1 == gBase[64]) || (ch2 == gBase[64])
			|| (ch3 == gBase[64] && ch4 != gBase[64]))
			return 0;//编码有错误
		u.s.a = p1 - gBase;
		u.s.b = p2 - gBase;
		if (ch3 == gBase[64])
		{
			//原数据只有1个字节
			*(outdata + rlen) = (char)u.c[2]; rlen++;
		}
		else if (ch4 == gBase[64])
		{
			//原数据只有2个字节
			u.s.c = p3 - gBase;
			*(outdata + rlen) = (char)u.c[2]; rlen++;
			*(outdata + rlen) = (char)u.c[1]; rlen++;
		}
		else
		{//原数据有3个字节
			u.s.c = p3 - gBase;
			u.s.d = p4 - gBase;
			*(outdata + rlen) = (char)u.c[2]; rlen++;
			*(outdata + rlen) = (char)u.c[1]; rlen++;
			*(outdata + rlen) = (char)u.c[0]; rlen++;
		}
	}

	*outlen = rlen;

	return outdata;
}
