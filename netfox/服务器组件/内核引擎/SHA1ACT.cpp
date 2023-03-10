/*
 *****************************************************************************
 *
 *  SHA1ACT.c
 *
 *  Copyright (C) 1998, 2009
 *  Paul E. Jones <paulej@packetizer.com>
 *  All Rights Reserved
 *
 *****************************************************************************
 *  $Id: sha1.c 12 2009-06-22 19:34:25Z paulej $
 *****************************************************************************
 *
 *  Description:
 *      This file implements the Secure Hashing Standard as defined
 *      in FIPS PUB 180-1 published April 17, 1995.
 *
 *      The Secure Hashing Standard, which uses the Secure Hashing
 *      Algorithm (SHA), produces a 160-bit message digest for a
 *      given data stream.  In theory, it is highly improbable that
 *      two messages will produce the same message digest.  Therefore,
 *      this algorithm can serve as a means of providing a "fingerprint"
 *      for a message.
 *
 *  Portability Issues:
 *      SHA-1 is defined in terms of 32-bit "words".  This code was
 *      written with the expectation that the processor has at least
 *      a 32-bit machine word size.  If the machine word size is larger,
 *      the code should still function properly.  One caveat to that
 *      is that the input functions taking characters and character
 *      arrays assume that only 8 bits of information are stored in each
 *      character.
 *
 *  Caveats:
 *      SHA-1 is designed to work with messages less than 2^64 bits
 *      long. Although SHA-1 allows a message digest to be generated for
 *      messages of any number of bits less than 2^64, this
 *      implementation only works with messages with a length that is a
 *      multiple of the size of an 8-bit character.
 *
 *****************************************************************************
 */

#include "stdafx.h"
#include "SHA1ACT.h"
 //根据资料，完成此文件的内容
#ifndef _SHA1_H_    /*提示：#ifndef, #define, #endif三者缺一不可*/
#define _SHA1_H_
typedef  unsigned long  uint32_t;
typedef  unsigned char  uint8_t;

/* This structure will hold context information for the hashing operation */
typedef struct SHA1Context
{
	uint32_t Message_Digest[5];         /* Message Digest (output)          */
	uint32_t Length_Low;              /* Message length in bits           */
	uint32_t Length_High;             /* Message length in bits           */
	uint8_t Message_Block[64];    /* 512-bit message blocks      */
	int Message_Block_Index;           /* Index into message block array   */
	int Computed;                     /* Is the digest computed?          */
	int Corrupted;                     /* Is the message digest corruped?  */
} SHA1Context;

/* Function Prototypes */
void SHA1Reset(SHA1Context *);
int SHA1Result(SHA1Context *);
void SHA1Input(SHA1Context *, const char *, unsigned);
//补充自己需要的其它定义、申明等
#endif

/*  Portability Issues:
 *      SHA-1 is defined in terms of 32-bit "words".  This code was
 *      written with the expectation that the processor has at least
 *      a 32-bit machine word size.  If the machine word size is larger,
 *      the code should still function properly.  One caveat to that
 *      is that the input functions taking characters and character
 *      arrays assume that only 8 bits of information are stored in each
 *      character.
 *
 *  Caveats:
 *      SHA-1 is designed to work with messages less than 2^64 bits
 *      long. Although SHA-1 allows a message digest to be generated for
 *      messages of any number of bits less than 2^64, this
 *      implementation only works with messages with a length that is a
 *      multiple of the size of an 8-bit character.
 *
 *****************************************************************************
 */

 /* Define the circular shift macro */
#define SHA1CircularShift(bits,word) \
 ((((word) << (bits)) & 0xFFFFFFFF) | ((word) >> (32-(bits))))

/* Function prototypes */
void SHA1ProcessMessageBlock(SHA1Context *);
void SHA1PadMessage(SHA1Context *);

void SHA1Reset(SHA1Context *context)
{
	context->Length_Low = 0;
	context->Length_High = 0;
	context->Message_Block_Index = 0;


	context->Message_Digest[0] = 0x67452301;
	context->Message_Digest[1] = 0xEFCDAB89;
	context->Message_Digest[2] = 0x98BADCFE;
	context->Message_Digest[3] = 0x10325476;
	context->Message_Digest[4] = 0xC3D2E1F0;

	context->Computed = 0;
	context->Corrupted = 0;
}


int SHA1Result(SHA1Context *context)
{

	if (context->Corrupted)
	{
		return 0;
	}

	if (!context->Computed)
	{
		SHA1PadMessage(context);
		context->Computed = 1;
	}

	return 1;
}

void SHA1Input(SHA1Context  *context, const char *message_array, unsigned length)
{
	if (!length)
	{
		return;
	}

	if (context->Computed || context->Corrupted)
	{
		context->Corrupted = 1;
		return;
	}

	while (length-- && !context->Corrupted)
	{
		context->Message_Block[context->Message_Block_Index++] =
			(*message_array & 0xFF);

		context->Length_Low += 8;
		/* Force it to 32 bits */
		context->Length_Low &= 0xFFFFFFFF;
		if (context->Length_Low == 0)
		{
			context->Length_High++;
			/* Force it to 32 bits */
			context->Length_High &= 0xFFFFFFFF;
			if (context->Length_High == 0)
			{
				/* Message is too long */
				context->Corrupted = 1;
			}
		}

		if (context->Message_Block_Index == 64)
		{
			SHA1ProcessMessageBlock(context);  /*注意经过此函数调用后，context->Message_Block_Index 的变化。*/
		}
		message_array++;
	}
}

/*This function will process the next 512 bits of the message stored in the Message_Block array. */
void SHA1ProcessMessageBlock(SHA1Context *context)
{
	const unsigned K[] =            /* Constants defined in SHA-1   */
	{
		0x5A827999,
		0x6ED9EBA1,
		0x8F1BBCDC,
		0xCA62C1D6
	};
	int         t;                  /* Loop counter                 */
	unsigned    temp;               /* Temporary word value         */
	unsigned    W[80];              /* Word sequence                */
	unsigned    A, B, C, D, E;      /* Word buffers                 */

 /*
 *  Initialize the first 16 words in the array W
 */
	for (t = 0; t < 16; t++)
	{
		W[t] = ((unsigned)context->Message_Block[t * 4]) << 24;
		W[t] |= ((unsigned)context->Message_Block[t * 4 + 1]) << 16;
		W[t] |= ((unsigned)context->Message_Block[t * 4 + 2]) << 8;
		W[t] |= ((unsigned)context->Message_Block[t * 4 + 3]);
	}

	for (t = 16; t < 80; t++)
	{
		W[t] = SHA1CircularShift(1, W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]);
	}

	A = context->Message_Digest[0];
	B = context->Message_Digest[1];
	C = context->Message_Digest[2];
	D = context->Message_Digest[3];
	E = context->Message_Digest[4];

	for (t = 0; t < 20; t++)
	{
		temp = SHA1CircularShift(5, A) +
			((B & C) | ((~B) & D)) + E + W[t] + K[0];
		temp &= 0xFFFFFFFF;
		E = D;
		D = C;
		C = SHA1CircularShift(30, B);
		B = A;
		A = temp;
	}

	for (t = 20; t < 40; t++)
	{
		temp = SHA1CircularShift(5, A) + (B ^ C ^ D) + E + W[t] + K[1];
		temp &= 0xFFFFFFFF;
		E = D;
		D = C;
		C = SHA1CircularShift(30, B);
		B = A;
		A = temp;
	}

	for (t = 40; t < 60; t++)
	{
		temp = SHA1CircularShift(5, A) +
			((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
		temp &= 0xFFFFFFFF;
		E = D;
		D = C;
		C = SHA1CircularShift(30, B);
		B = A;
		A = temp;
	}

	for (t = 60; t < 80; t++)
	{
		temp = SHA1CircularShift(5, A) + (B ^ C ^ D) + E + W[t] + K[3];
		temp &= 0xFFFFFFFF;
		E = D;
		D = C;
		C = SHA1CircularShift(30, B);
		B = A;
		A = temp;
	}

	context->Message_Digest[0] =
		(context->Message_Digest[0] + A) & 0xFFFFFFFF;
	context->Message_Digest[1] =
		(context->Message_Digest[1] + B) & 0xFFFFFFFF;
	context->Message_Digest[2] =
		(context->Message_Digest[2] + C) & 0xFFFFFFFF;
	context->Message_Digest[3] =
		(context->Message_Digest[3] + D) & 0xFFFFFFFF;
	context->Message_Digest[4] =
		(context->Message_Digest[4] + E) & 0xFFFFFFFF;

	context->Message_Block_Index = 0;  /*提示：注意此函数的最后这句。在该函数被调用的地方，注意此语句带来的变化。*/
}

/*
 *  SHA1PadMessage
 *
 *  Description:
 *      According to the standard, the message must be padded to an even
 *      512 bits.  The first padding bit must be a '1'.  The last 64
 *      bits represent the length of the original message.  All bits in
 *      between should be 0.  This function will pad the message
 *      according to those rules by filling the Message_Block array
 *      accordingly.  It will also call SHA1ProcessMessageBlock()
 *      appropriately.  When it returns, it can be assumed that the
 *      message digest has been computed.
 *
 *  Parameters:
 *      context: [in/out] The context to pad
*/
void SHA1PadMessage(SHA1Context *context)
{
	/*
 *  Check to see if the current message block is too small to hold
 *  the initial padding bits and length.  If so, we will pad the
 *  block, process it, and then continue padding into a second
 *  block.
	*/
	if (context->Message_Block_Index > 55)   /* 提示：剩余的长度不够64比特，需要填充1和若干0，使得消息长度满足k*512+448*/
	{
		context->Message_Block[context->Message_Block_Index++] = 0x80;
		while (context->Message_Block_Index < 64)
		{
			context->Message_Block[context->Message_Block_Index++] = 0;
		}

		SHA1ProcessMessageBlock(context);  /*提示：对于第一个512比特进行SHA1处理。注意经过此函数调用后，context->Message_Block_Index 变化为多少。*/

		while (context->Message_Block_Index < 56)
		{
			context->Message_Block[context->Message_Block_Index++] = 0; /*提示：context->Message_Block_Index++，先使用context->Message_Block_Index ，再增加1*/
		}
	}
	else
	{
		context->Message_Block[context->Message_Block_Index++] = 0x80;
		while (context->Message_Block_Index < 56)
		{
			context->Message_Block[context->Message_Block_Index++] = 0;
		}
	}

	/*
 *  Store the message length 8*8bits = 64bits
 */
	context->Message_Block[56] = (context->Length_High >> 24) & 0xFF;
	context->Message_Block[57] = (context->Length_High >> 16) & 0xFF;
	context->Message_Block[58] = (context->Length_High >> 8) & 0xFF;
	context->Message_Block[59] = (context->Length_High) & 0xFF;
	context->Message_Block[60] = (context->Length_Low >> 24) & 0xFF;
	context->Message_Block[61] = (context->Length_Low >> 16) & 0xFF;
	context->Message_Block[62] = (context->Length_Low >> 8) & 0xFF;
	context->Message_Block[63] = (context->Length_Low) & 0xFF;

	SHA1ProcessMessageBlock(context);
}
char* ToHexString(char * input)
{
	char* Result;
	unsigned int nOutSize;
	unsigned int i, r;
	int test;
	int Table[2] = { +1,-1 };

	nOutSize = strlen(input) * 2;          //处理1：在答题纸上写出处理1的作用


	Result = (char *)malloc(nOutSize + 1);   //处理2：在答题纸上写出处理2的作用

	for (i = 0; i < nOutSize; i++)           //处理3：在答题纸上写出处理3，整个for的作用
	{
		r = i + Table[i % 2];

		test = input[i >> 1];
		test = test >> (((i % 2) % 4) << 2);
		test = test & 0xF;
		switch (test)
		{
			case 0:
				Result[r] = '0';
				break;
			case 1:
				Result[r] = '1';
				break;
			case 2:
				Result[r] = '2';
				break;
			case 3:
				Result[r] = '3';
				break;
			case 4:
				Result[r] = '4';
				break;
			case 5:
				Result[r] = '5';
				break;
			case 6:
				Result[r] = '6';
				break;
			case 7:
				Result[r] = '7';
				break;
			case 8:
				Result[r] = '8';
				break;
			case 9:
				Result[r] = '9';
				break;
			case 10:
				Result[r] = 'A';
				break;
			case 11:
				Result[r] = 'B';
				break;
			case 12:
				Result[r] = 'C';
				break;
			case 13:
				Result[r] = 'D';
				break;
			case 14:
				Result[r] = 'E';
				break;
			case 15:
				Result[r] = 'F';
				break;
			default:
				Result[r] = '!';
				break;
		}
	}

	Result[nOutSize] = '\0';                  //处理4：在答题纸上写出处理4的作用
	return Result;

}
char * BitSHA1(CString origin)
{
	char *Message_Digest = (char *)malloc(1024);
	//CString m_abstract;
	// TODO: Add your control notification handler code here
	SHA1Context sha;
	char inputstr[1024];
	int i;
	memset(inputstr, 0, sizeof(inputstr));
	memcpy(inputstr, origin.GetString(), origin.GetLength());//明文
//	extern char Message_Digest[1024];

	//补充代码

	SHA1Reset(&sha);
	SHA1Input(&sha, inputstr, strlen(inputstr));
	i = SHA1Result(&sha);
	//输出结果。注意！！实验报告中要求对以下的for语句完成的功能进行解释。
	for (i = 0; i < 1024; i++)
	{
		Message_Digest[i] = (BYTE)((sha.Message_Digest[i >> 2] >> ((3 - (i & 3)) * 8)) &
			255);
	}
	//修改代码,实现十六进制格式的显示
//	Message_Hex=ToHexString(Message_Digest);
//	m_abstract = Message_Hex;
	//this->SetDlgItemTextA(IDC_RESULT,Message_Digest);
	// TODO: 在此添加控件通知处理程序代码
	return Message_Digest;
}