1 //******************************************

 //纯C++的Socket访问Http封装类，Neeao修改

 //http://neeao.com

//2009-08-25

 //******************************************





 #include "stdafx.h"

#include "Request.h"

 #include <string>

 #ifdef _DEBUG

#undef THIS_FILE

 static char THIS_FILE[]=__FILE__;

 #define new DEBUG_NEW

 #endif





 //////////////////////////////////////////////////////////////////////

 // Construction/Destruction

 //////////////////////////////////////////////////////////////////////



 Request::Request()
{

 }



 Request::~Request()
 {


 }



 //*******************************************************************************************************

 //MemBufferCreate:

//                  Passed a MemBuffer structure, will allocate a memory buffer

 //                   of MEM_BUFFER_SIZE.  This buffer can then grow as needed.

 //*******************************************************************************************************

 void Request::MemBufferCreate(MemBuffer *b)
{

    b->size = MEM_BUFFER_SIZE;

     b->buffer =(unsigned    char *) malloc( b->size );

     b->position = b->buffer;

 }



 //*******************************************************************************************************

 // MemBufferGrow:

 //                  Double the size of the buffer that was passed to this function.

 //*******************************************************************************************************

 void Request::MemBufferGrow(MemBuffer *b)
 {

     size_t sz;

    sz = b->position - b->buffer;

     b->size = b->size *2;

    b->buffer =(unsigned    char *) realloc(b->buffer,b->size);

    b->position = b->buffer + sz;   // readjust current position

}



 //*******************************************************************************************************

 // MemBufferAddByte:

 //                  Add a single byte to the memory buffer, grow if needed.

 //*******************************************************************************************************

 void Request::MemBufferAddByte(MemBuffer *b,unsigned char byt)
 {

    if( (size_t)(b->position-b->buffer) >= b->size )

        MemBufferGrow(b);

     *(b->position++) = byt;

}



//*******************************************************************************************************

 // MemBufferAddBuffer:

 //                  Add a range of bytes to the memory buffer, grow if needed.

 //*******************************************************************************************************

 void Request::MemBufferAddBuffer(MemBuffer *b,

                                  unsigned char *buffer, size_t size)
 {

     while( ((size_t)(b->position-b->buffer)+size) >= b->size )
         MemBufferGrow(b);


    memcpy(b->position,buffer,size);

     b->position+=size;

 }



 //*******************************************************************************************************

 // GetHostAddress:

 //                  Resolve using DNS or similar(WINS,etc) the IP

 //                   address for a domain name such as www.wdj.com.

 //*******************************************************************************************************

 DWORD Request::GetHostAddress(LPCSTR host)
{

    struct hostent *phe;
    char *p;



    phe = gethostbyname( host );

     if(phe==NULL)
        return 0;

    p = *phe->h_addr_list;
     return *((DWORD*)p);
 }



//*******************************************************************************************************

 // SendString:

//                  Send a string(null terminated) over the specified socket.

 //*******************************************************************************************************

 void Request::SendString(SOCKET sock,LPCSTR str)
 {
    send(sock,str,strlen(str),0);//向一个已连接的套接口发送数据

 }



 //*******************************************************************************************************

 // ValidHostChar:

 //                  Return TRUE if the specified character is valid

 //                      for a host name, i.e. A-Z or 0-9 or -.:

 //*******************************************************************************************************

 BOOL Request::ValidHostChar(char ch)
 {

     return( isalpha(ch) || isdigit(ch)					//isalpha判断字符ch是否为英文字母//isdigit判断字符c是否为数字
         || ch=='-' || ch=='.' || ch==':' );

 }





 //*******************************************************************************************************

 // ParseURL:

 //                  Used to break apart a URL such as

 //                      http://www.localhost.com:80/TestPost.htm into protocol, port, host and request.

 //*******************************************************************************************************

 void Request::ParseURL(string url,LPSTR protocol,int lprotocol,LPSTR host,int lhost,LPSTR request,int lrequest,int *port)
 {

    char *work,*ptr,*ptr2;



    *protocol = *host = *request = 0;

     *port=80;



    work = strdup(url.c_str());///复制字符串

    strupr(work);///将字符串s转换为大写形式



     ptr = strchr(work,':');                         // 查找字符串work中首次出现字符:的位置

    if(ptr!=NULL)
     {
        *(ptr++) = 0;

         lstrcpyn(protocol,work,lprotocol);//复制前20位
     }
     else
     {

         lstrcpyn(protocol,"HTTP",lprotocol);
        ptr = work;

    }



     if( (*ptr=='/') && (*(ptr+1)=='/') )           //跳过“//”
        ptr+=2;



     ptr2 = ptr;                                     // ptr2指向网站的首地址

     while( ValidHostChar(*ptr2) && *ptr2 )			///跳过网站的首地址

        ptr2++;



    *ptr2=0;

    lstrcpyn(host,ptr,lhost);///复制从首地址开始的内容

    lstrcpyn(request,url.c_str() + (ptr2-work),lrequest);   // 复制首地址后面的内容

    ptr = strchr(host,':');                         // find the port number, if any

     if(ptr!=NULL)
    {
        *ptr=0;

         *port = 80;//atoi(ptr+1);

     }
     free(work);
 }



 //*******************************************************************************************************

 // SendHTTP:

 //                  Main entry point for this code.

 //                    url           - The URL to GET/POST to/from.

 //                    headerSend        - Headers to be sent to the server.

 //                    post          - Data to be posted to the server, NULL if GET.

 //                    postLength    - Length of data to post.

 //                    req           - Contains the message and headerSend sent by the server.

 //

 //                    returns 1 on failure, 0 on success.

 //*******************************************************************************************************

 int Request::SendHTTP(string url,LPCSTR headerReceive,BYTE *post,

                      DWORD postLength,HTTPRequest *req)
 {

     WSADATA         WsaData;

     SOCKADDR_IN     sin;

     SOCKET          sock;

     char            buffer[512];

     char            protocol[20],host[256],request[1024];

    int             l,port,chars,err;

     MemBuffer       headersBuffer,messageBuffer;

     char            headerSend[1024];

     BOOL            done;



     ParseURL(url,protocol,sizeof(protocol),host,sizeof(host),       // Parse the URL

         request,sizeof(request),&port);

     if(strcmp(protocol,"HTTP"))///strcmp比较字符串s1和s
	 {
         return 1;
	 }



     err = WSAStartup (0x0101, &WsaData);                            // Init Winsock

     if(err!=0)
	 {
        return 1;
	 }



     sock = socket (AF_INET, SOCK_STREAM, 0);

     //if (socket == INVALID_SOCKET)

     if (sock == INVALID_SOCKET)
     {
         return 1;
     }



     sin.sin_family = AF_INET;                                       //Connect to web sever

     sin.sin_port = htons( (unsigned short)port );				//htons的功能：将一个无符号短整型数值转换为网络字节序，即大端模式(big-endian)

     sin.sin_addr.s_addr = GetHostAddress(host);				//用getHostAddress方法获得IP地址



     if( connect (sock,(LPSOCKADDR)&sin, sizeof(SOCKADDR_IN) ) )
     {
         return 1;
     }





     if( !*request )
         lstrcpyn(request,"/",sizeof(request));



     if( post == NULL )
     {

         SendString(sock,"GET ");

         strcpy(headerSend, "GET ");

     }
     else
     {

         SendString(sock,"POST ");

         strcpy(headerSend, "POST ");

     }

     SendString(sock,request);

     strcat(headerSend, request);//strcat把src所指字符串添加到dest结尾处(覆盖dest结尾处的'\0')并添加'\0'



     SendString(sock," HTTP/1.0\r\n");

     strcat(headerSend, " HTTP/1.0\r\n");



     SendString(sock,"Accept: image/gif, image/x-xbitmap,"						//????

         " image/jpeg, image/pjpeg, application/vnd.ms-excel,"

        " application/msword, application/vnd.ms-powerpoint,"

         " */*\r\n");

    strcat(headerSend, "Accept: image/gif, image/x-xbitmap,"

         " image/jpeg, image/pjpeg, application/vnd.ms-excel,"

        " application/msword, application/vnd.ms-powerpoint,"

        " */*\r\n");



    SendString(sock,"Accept-Language: en-us\r\n");

    strcat(headerSend, "Accept-Language: en-us\r\n");



    SendString(sock,"Accept-Encoding: gzip, default\r\n");

    strcat(headerSend, "Accept-Encoding: gzip, default\r\n");



	SendString(sock,"User-Agent: Neeao/4.0\r\n");

    strcat(headerSend, "User-Agent: Neeao/4.0\r\n");



     if(postLength)
     {
	    sprintf(buffer,"Content-Length: %ld\r\n",postLength);
		SendString(sock,buffer);
        strcat(headerSend, buffer);
    }

     //SendString(sock,"Cookie: mycookie=blablabla\r\n");

     //  printf("Cookie: mycookie=blablabla\r\n");

     SendString(sock,"Host: ");
     strcat(headerSend, "Host: ");



     SendString(sock,host);
     strcat(headerSend, host);

     SendString(sock,"\r\n");
    strcat(headerSend, "\r\n");



     if( (headerReceive!=NULL) && *headerReceive )
     {
         SendString(sock,headerReceive);
         strcat(headerSend, headerReceive);

     }



     SendString(sock,"\r\n");                                // Send a blank line to signal end of HTTP headerReceive
     strcat(headerSend, "\r\n");



     if( (post!=NULL) && postLength )
     {
        send(sock,(const char*)post,postLength,0);
         post[postLength]    = '\0';
        strcat(headerSend, (const char*)post);
     }



     //strcpy(req->headerSend, headerSend);
    req->headerSend     = (char*) malloc( sizeof(char*) * strlen(headerSend));
    strcpy(req->headerSend, (char*) headerSend );


    MemBufferCreate(&headersBuffer );
     chars = 0;
     done = FALSE;

     while(!done)
     {

         l = recv(sock,buffer,1,0);	///从一个套接口接收数据

         if(l<0)

             done=TRUE;
         switch(*buffer)

         {

         case '\r':

             break;

         case '\n':

             if(chars==0)

                 done = TRUE;

             chars=0;

             break;

         default:

             chars++;

             break;

         }
         MemBufferAddByte(&headersBuffer,*buffer);

     }

    req->headerReceive  = (char*) headersBuffer.buffer;

	*(headersBuffer.position) = 0;


     MemBufferCreate(&messageBuffer);                            // Now read the HTTP body

     do
     {

         l = recv(sock,buffer,sizeof(buffer)-1,0);
         if(l<0)
             break;

         *(buffer+l)=0;

         MemBufferAddBuffer(&messageBuffer, (unsigned char*)&buffer, l);

     } while(l>0);

     *messageBuffer.position = 0;

     req->message = (char*) messageBuffer.buffer;

     req->messageLength = (messageBuffer.position - messageBuffer.buffer);

     closesocket(sock);                                          // Cleanup
     return 0;

 }





 //*******************************************************************************************************
 // SendRequest
 //
 //*******************************************************************************************************

 int Request::SendRequest(bool IsPost, string url, string& psHeaderSend, string& psHeaderReceive, string& psMessage)
 {

     HTTPRequest         req;
     int                 i,rtn;
     LPSTR               buffer;



     req.headerSend       = NULL;

     req.headerReceive    = NULL;

    req.message           = NULL;



     //Read in arguments

     if(IsPost)
     {
		 /* POST */

         i  = psHeaderSend.length();

         buffer  = (char*) malloc(i+1);

         strcpy(buffer, psHeaderSend.c_str());

         rtn   = SendHTTP( url,"Content-Type: application/x-www-form-urlencoded\r\n",(unsigned char*)buffer,i, &req);

         free(buffer);

     }
     else/* GET */
     {
         rtn = SendHTTP(url,NULL,NULL,0,&req);
     }


     if(!rtn)    //Output message and/or headerSend
     {

         psHeaderSend        = req.headerSend;

         psHeaderReceive     = req.headerReceive;

         psMessage           = req.message;


         free(req.headerSend);

         free(req.headerReceive);

         free(req.message);

         return 1;

     }
     else
     {

         return 0;

     }

 }

 char* Request::WC2UT(CString unicode)  
 {   
	 int len;  
	 len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)&unicode, -1, NULL, 0, NULL, NULL);  
	 char *szUtf8=new char[len + 1];
	 memset(szUtf8, 0, len * 2 + 2);
	 WideCharToMultiByte (CP_UTF8, 0, (LPCWSTR)&unicode, -1, szUtf8, len, NULL,NULL);
	 return szUtf8;


	 //int len = WideCharToMultiByte(CP_UTF8, 0, buf, -1, NULL, 0, NULL, NULL);
	 //std::vector <char> utf8(len);
	 //WideCharToMultiByte(CP_UTF8, 0, buf, -1, &utf8[0], len, NULL, NULL);
	 //return std::string(&utf8[0]);
 }
 // GB2312 转换成　Unicode
 void Request::Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer)
 {
	 ::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
	 return;
 }
 //GB2312 转为 UTF-8
 void Request::GB2312ToUTF_8(string& pOut,char *pText, int pLen)
 {
	 char buf[4];
	 char* rst = new char[pLen + (pLen >> 2) + 2];

	 memset(buf,0,4);
	 memset(rst,0,pLen + (pLen >> 2) + 2);

	 int i = 0;
	 int j = 0;      
	 while(i < pLen)
	 {
		 //如果是英文直接复制就可以
		 if( *(pText + i) >= 0)
		 {
			 rst[j++] = pText[i++];
		 }
		 else
		 {
			 WCHAR pbuffer;
			 Gb2312ToUnicode(&pbuffer,pText+i);

			 UnicodeToUTF_8(buf,&pbuffer);

			 unsigned short int tmp = 0;
			 tmp = rst[j] = buf[0];
			 tmp = rst[j+1] = buf[1];
			 tmp = rst[j+2] = buf[2];

			 j += 3;
			 i += 2;
		 }
	 }
	 rst[j] = '\0';

	 //返回结果
	 pOut = rst;             
	 delete []rst;   

	 return;
 }


 // Unicode 转换成UTF-8 
 void Request::UnicodeToUTF_8(char* pOut,WCHAR* pText)
 {
	 // 注意 WCHAR高低字的顺序,低字节在前，高字节在后
	 char* pchar = (char *)pText;

	 pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
	 pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
	 pOut[2] = (0x80 | (pchar[0] & 0x3F));

	 return;
 }
 //UTF-8 转为 GB2312
 void  Request::UTF_8ToGB2312(string &pOut, char *pText, int pLen)
 {
	 char * newBuf = new char[pLen];
	 char Ctemp[4];
	 memset(Ctemp,0,4);

	 int i =0;
	 int j = 0;

	 while(i < pLen)
	 {
		 if(pText[i] > 0)
		 {
			 newBuf[j++] = pText[i++];                       
		 }
		 else                 
		 {
			 WCHAR Wtemp;
			 UTF_8ToUnicode(&Wtemp,pText + i);

			 UnicodeToGB2312(Ctemp,Wtemp);

			 newBuf[j] = Ctemp[0];
			 newBuf[j + 1] = Ctemp[1];

			 i += 3;    
			 j += 2;   
		 }
	 }
	 newBuf[j] = '\0';

	 pOut = newBuf;
	 delete [] newBuf;

	 return; 
 }
 // 把UTF-8转换成Unicode
 void Request::UTF_8ToUnicode(WCHAR* pOut,char *pText)
 {
	 char* uchar = (char *)pOut;

	 uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	 uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);

	 return;
 }
 // 把Unicode 转换成 GB2312 
 void Request::UnicodeToGB2312(char* pOut,unsigned short uData)
 {
	 WideCharToMultiByte(CP_ACP,NULL,(LPCWSTR)&uData,1,pOut,sizeof(WCHAR),NULL,NULL);
	 return;
 }