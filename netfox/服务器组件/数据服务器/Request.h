//******************************************

//纯C++的socket访问Http封装类，Neeao修改

 //http://neeao.com

//2009-08-25

 //******************************************



 #if !defined(AFX_REQUEST_H__9F2C9BB6_CBA7_40AF_80A4_09A1CE1CE220__INCLUDED_)

 #define AFX_REQUEST_H__9F2C9BB6_CBA7_40AF_80A4_09A1CE1CE220__INCLUDED_



 #if _MSC_VER > 1000

 #pragma once

 #endif // _MSC_VER > 1000





#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <winsock2.h>

#pragma comment(lib, "WS2_32")



 using namespace std;

 #define MEM_BUFFER_SIZE 10



 /*

     HTTPRequest: Structure that returns the HTTP headers and message

                    from the request

 */

 typedef struct
 {

    LPSTR headerSend;                               // Pointer to HTTP header Send

    LPSTR headerReceive;                            // Pointer to HTTP headers Receive

    LPSTR message;                                  // Pointer to the HTTP message

    long messageLength;                             // Length of the message

 } HTTPRequest;



/*

     MemBuffer:  Structure used to implement a memory buffer, which is a

                 buffer of memory that will grow to hold variable sized

                 parts of the HTTP message.

 */

 typedef struct
 {

    unsigned    char *buffer;

    unsigned    char *position;

    size_t      size;

 } MemBuffer;





 class Request
 {

 public:

    Request();

    virtual ~Request();



 private:

     void        MemBufferCreate(MemBuffer *b);

     void        MemBufferGrow(MemBuffer *b);

    void        MemBufferAddByte(MemBuffer *b, unsigned char byt);

    void        MemBufferAddBuffer(MemBuffer *b, unsigned char *buffer, size_t size);

     DWORD       GetHostAddress(LPCSTR host);

    void        SendString(SOCKET sock,LPCSTR str);

     BOOL        ValidHostChar(char ch);

	 void        ParseURL(string url,LPSTR protocol,int lprotocol, LPSTR host,int lhost,LPSTR request,int lrequest,int *port);

     int         SendHTTP(string url,LPCSTR headerReceive,BYTE *post, DWORD postLength,HTTPRequest *req);



 public:
    int     SendRequest(bool IsPost, string url, string& psHeaderSend, string& pszHeaderReceive,string& pszMessage);
	char*  WC2UT(CString unicode);

	// GB2312 转换成　Unicode
	void Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer);
	//GB2312 转为 UTF-8
	void GB2312ToUTF_8(string& pOut,char *pText, int pLen);
	// Unicode 转换成UTF-8 
	void UnicodeToUTF_8(char* pOut,WCHAR* pText);
	//UTF-8 转为 GB2312
	void UTF_8ToGB2312(string &pOut, char *pText, int pLen);
	// 把UTF-8转换成Unicode
	void UTF_8ToUnicode(WCHAR* pOut,char *pText);
	// 把Unicode 转换成 GB2312 
	void UnicodeToGB2312(char* pOut,unsigned short uData);








};



 #endif // !defined(AFX_REQUEST_H__9F2C9BB6_CBA7_40AF_80A4_09A1CE1CE220__INCLUDED_)


