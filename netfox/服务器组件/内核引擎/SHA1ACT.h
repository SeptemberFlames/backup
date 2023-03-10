/*****************************************************************************
 *
 *  SHA1ACT.h : main header file for the SHA1ACT application
 *
 *  Copyright (C) 1998, 2009
 *  Paul E. Jones <paulej@packetizer.com>
 *  All Rights Reserved
 *
 *****************************************************************************
 *  $Id: sha1.h 12 2009-06-22 19:34:25Z paulej $
 *****************************************************************************
 *
 *  Description:
 *      This class implements the Secure Hashing Standard as defined
 *      in FIPS PUB 180-1 published April 17, 1995.
 *
 *      Many of the variable names in the SHA1Context, especially the
 *      single character names, were used because those were the names
 *      used in the publication.
 *
 */

#ifndef _SHA1_H_
#define _SHA1_H_

 /*
  *  This structure will hold context information for the hashing
  *  operation
  */
typedef  unsigned int  uint32_t;
typedef  unsigned char  uint8_t;
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
//补充必要的内容
#endif

void SHA1Reset(SHA1Context* context);
void SHA1Input(SHA1Context *, const char *, unsigned);
void SHA1ProcessMessageBlock(SHA1Context *);
void SHA1PadMessage(SHA1Context *);
char* ToHexString(char * input);
int SHA1Result(SHA1Context *context);
char * BitSHA1(CString);