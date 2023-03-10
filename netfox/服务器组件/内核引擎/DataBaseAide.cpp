#include "Stdafx.h"
#include "DataBaseAide.h"
#include "TraceService.h"
#include "DataBase.h"
#include "ServerPushLog.h"
//////////////////////////////////////////////////////////////////////////   

//构造函数
CDataBaseAide::CDataBaseAide(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIDataBase = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IDataBase);

	return;
}

//析构函数
CDataBaseAide::~CDataBaseAide()
{
}

//设置对象
bool CDataBaseAide::SetDataBase(IUnknownEx * pIUnknownEx)
{
	//设置接口
	if (pIUnknownEx != NULL)
	{
		//查询接口
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IDataBase) != NULL);
		m_pIDataBase = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IDataBase);

		//成功判断
		if (m_pIDataBase == NULL) return false;
	}
	else
	{
		m_pIDataBase = NULL;
	}

	return true;
}

//获取对象
VOID * CDataBaseAide::GetDataBase(REFGUID Guid, DWORD dwQueryVer)
{
	if (m_pIDataBase == NULL) return NULL;
	return m_pIDataBase->QueryInterface(Guid, dwQueryVer);
}

//获取数据
INT CDataBaseAide::GetValue_INT(LPCTSTR pszItem)
{
	//校验参数
	ASSERT(m_pIDataBase != NULL);

	//读取变量
	CDBVarValue DBVarValue;
	m_pIDataBase->GetRecordsetValue(pszItem, DBVarValue);

	return DBVarValue;
}

//获取数据
UINT CDataBaseAide::GetValue_UINT(LPCTSTR pszItem)
{
	//校验参数
	ASSERT(m_pIDataBase != NULL);

	//读取变量
	CDBVarValue DBVarValue;
	m_pIDataBase->GetRecordsetValue(pszItem, DBVarValue);

	return DBVarValue;
}

//获取数据
LONG CDataBaseAide::GetValue_LONG(LPCTSTR pszItem)
{
	//校验参数
	ASSERT(m_pIDataBase != NULL);

	//读取变量
	CDBVarValue DBVarValue;
	m_pIDataBase->GetRecordsetValue(pszItem, DBVarValue);

	return DBVarValue;
}

//获取数据
BYTE CDataBaseAide::GetValue_BYTE(LPCTSTR pszItem)
{
	//校验参数
	ASSERT(m_pIDataBase != NULL);

	//读取变量
	CDBVarValue DBVarValue;
	m_pIDataBase->GetRecordsetValue(pszItem, DBVarValue);

	return DBVarValue;
}

//获取数据
WORD CDataBaseAide::GetValue_WORD(LPCTSTR pszItem)
{
	//校验参数
	ASSERT(m_pIDataBase != NULL);

	//读取变量
	CDBVarValue DBVarValue;
	m_pIDataBase->GetRecordsetValue(pszItem, DBVarValue);

	return DBVarValue;
}

//获取数据
DWORD CDataBaseAide::GetValue_DWORD(LPCTSTR pszItem)
{
	//校验参数
	ASSERT(m_pIDataBase != NULL);

	//读取变量
	CDBVarValue DBVarValue;
	m_pIDataBase->GetRecordsetValue(pszItem, DBVarValue);

	return DBVarValue;
}

//获取数据
FLOAT CDataBaseAide::GetValue_FLOAT(LPCTSTR pszItem)
{
	//校验参数
	ASSERT(m_pIDataBase != NULL);

	//读取变量
	CDBVarValue DBVarValue;
	m_pIDataBase->GetRecordsetValue(pszItem, DBVarValue);

	return DBVarValue;
}

//获取数据
DOUBLE CDataBaseAide::GetValue_DOUBLE(LPCTSTR pszItem)
{
	//校验参数
	ASSERT(m_pIDataBase != NULL);

	//读取变量
	CDBVarValue DBVarValue;
	m_pIDataBase->GetRecordsetValue(pszItem, DBVarValue);

	return DBVarValue;
}

//获取数据
LONGLONG CDataBaseAide::GetValue_LONGLONG(LPCTSTR pszItem)
{
	//校验参数
	ASSERT(m_pIDataBase != NULL);

	//读取变量
	CDBVarValue DBVarValue;
	m_pIDataBase->GetRecordsetValue(pszItem, DBVarValue);

	return DBVarValue;
}

//获取数据
VOID CDataBaseAide::GetValue_VarValue(LPCTSTR pszItem, CDBVarValue & DBVarValue)
{
	//校验参数
	ASSERT(m_pIDataBase != NULL);

	//读取变量
	m_pIDataBase->GetRecordsetValue(pszItem, DBVarValue);

	return;
}

//获取数据
VOID CDataBaseAide::GetValue_SystemTime(LPCTSTR pszItem, SYSTEMTIME & SystemTime)
{
	//校验参数
	ASSERT(m_pIDataBase != NULL);

	//读取变量
	CDBVarValue DBVarValue;
	ZeroMemory(&SystemTime, sizeof(SystemTime));
	m_pIDataBase->GetRecordsetValue(pszItem, DBVarValue);

	//转换变量
	COleDateTime OleDataTime = DBVarValue;
	SystemTime.wYear = OleDataTime.GetYear();
	SystemTime.wMonth = OleDataTime.GetMonth();
	SystemTime.wDayOfWeek = OleDataTime.GetDayOfWeek();
	SystemTime.wDay = OleDataTime.GetDay();
	SystemTime.wHour = OleDataTime.GetHour();
	SystemTime.wMinute = OleDataTime.GetMinute();
	SystemTime.wSecond = OleDataTime.GetSecond();

	return;
}

//获取字符
VOID CDataBaseAide::GetValue_String(LPCTSTR pszItem, LPSTR pszString, UINT uMaxCount)
{
	//校验参数
	ASSERT(pszString != NULL);
	ASSERT(m_pIDataBase != NULL);

	//读取变量
	CDBVarValue DBVarValue;
	m_pIDataBase->GetRecordsetValue(pszItem, DBVarValue);

	//转换变量
	switch (DBVarValue.vt)
	{
	case VT_BSTR:
		{
			CW2A DBString(DBVarValue.bstrVal);
			lstrcpynA(pszString, DBString, uMaxCount);
			return;
		}
	default:
		{
			pszString[0] = 0;
			return;
		}
	}

	return;
}

//获取字符
VOID CDataBaseAide::GetValue_String(LPCTSTR pszItem, LPWSTR pszString, UINT uMaxCount)
{
	//校验参数
	ASSERT(pszString != NULL);
	ASSERT(m_pIDataBase != NULL);

	//读取变量
	CDBVarValue DBVarValue;
	m_pIDataBase->GetRecordsetValue(pszItem, DBVarValue);

	//转换变量
	switch (DBVarValue.vt)
	{
	case VT_BSTR:
		{
			lstrcpynW(pszString, DBVarValue.bstrVal, uMaxCount);
			return;
		}
	default:
		{
			pszString[0] = 0;
			return;
		}
	}

	return;
}

//重置参数
VOID CDataBaseAide::ResetParameter()
{
	//校验参数
	ASSERT(m_pIDataBase != NULL);

	//设置参数
	m_pIDataBase->ClearParameters();
	m_pIDataBase->AddParameter(TEXT("RETURN_VALUE"), adInteger, adParamReturnValue, sizeof(LONG), CDBVarValue((LONG)0L));

	return;
}

//获取参数
void  CDataBaseAide::GetParameter(LPCTSTR pszItem, BYTE* szBuffer, UINT& uSize)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, szBuffer, uSize);
	}
}

VOID CDataBaseAide::GetParameter(LPCTSTR lpFieldName, datastream& kDataStream)
{
	static BYTE s_TempData[SOCKET_TCP_PACKET];
	UINT	uGetSize = SOCKET_TCP_PACKET;
	GetParameter(lpFieldName, s_TempData, uGetSize);
	if (uGetSize != SOCKET_TCP_PACKET && uGetSize > 0)
	{
		kDataStream.pushValue((char*)s_TempData, uGetSize);
	}
}
//获取参数
VOID CDataBaseAide::GetParameter(LPCTSTR pszItem, CDBVarValue & DBVarValue)
{
	ASSERT(m_pIDataBase != NULL);
	m_pIDataBase->GetParameter(pszItem, DBVarValue);
	return;
}

//获取参数
VOID CDataBaseAide::GetParameter(LPCTSTR pszItem, LPSTR pszString, UINT uSize)
{
	//变量定义
	CDBVarValue DBVarValue;

	//获取参数
	ASSERT(m_pIDataBase != NULL);
	m_pIDataBase->GetParameter(pszItem, DBVarValue);

	//设置结果
	lstrcpynA(pszString, (LPCSTR)(CW2A(DBVarValue.bstrVal)), uSize);

	return;
}

//获取参数
VOID CDataBaseAide::GetParameter(LPCTSTR pszItem, LPWSTR pszString, UINT uSize)
{
	//变量定义
	CDBVarValue DBVarValue;

	//获取参数
	ASSERT(m_pIDataBase != NULL);
	m_pIDataBase->GetParameter(pszItem, DBVarValue);

	//设置结果
	lstrcpynW(pszString, (LPCWSTR)(DBVarValue.bstrVal), uSize);

	return;
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, INT nValue, ParameterDirectionEnum ParameterDirection)
{
	ASSERT(m_pIDataBase != NULL);
	m_pIDataBase->AddParameter(pszItem, adInteger, ParameterDirection, sizeof(LONG), CDBVarValue((LONG)nValue));
	return;
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, UINT uValue, ParameterDirectionEnum ParameterDirection)
{
	ASSERT(m_pIDataBase != NULL);
	m_pIDataBase->AddParameter(pszItem, adInteger, ParameterDirection, sizeof(LONG), CDBVarValue((LONG)uValue));
	return;
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, LONG lValue, ParameterDirectionEnum ParameterDirection)
{
	ASSERT(m_pIDataBase != NULL);
	m_pIDataBase->AddParameter(pszItem, adInteger, ParameterDirection, sizeof(LONG), CDBVarValue((LONG)lValue));
	return;
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, LONGLONG lValue, ParameterDirectionEnum ParameterDirection)
{
	ASSERT(m_pIDataBase != NULL);
	m_pIDataBase->AddParameter(pszItem, adBigInt, ParameterDirection, sizeof(LONGLONG), CDBVarValue((LONGLONG)lValue));
	return;
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, BYTE cbValue, ParameterDirectionEnum ParameterDirection)
{
	ASSERT(m_pIDataBase != NULL);
	m_pIDataBase->AddParameter(pszItem, adTinyInt, ParameterDirection, sizeof(BYTE), CDBVarValue((LONG)cbValue));
	return;
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, WORD wValue, ParameterDirectionEnum ParameterDirection)
{
	ASSERT(m_pIDataBase != NULL);
	m_pIDataBase->AddParameter(pszItem, adInteger, ParameterDirection, sizeof(LONG), CDBVarValue((LONG)wValue));
	return;
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, DWORD dwValue, ParameterDirectionEnum ParameterDirection)
{
	ASSERT(m_pIDataBase != NULL);
	m_pIDataBase->AddParameter(pszItem, adInteger, ParameterDirection, sizeof(LONG), CDBVarValue((LONG)dwValue));
	return;
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, FLOAT fValue, ParameterDirectionEnum ParameterDirection)
{
	ASSERT(m_pIDataBase != NULL);
	m_pIDataBase->AddParameter(pszItem, adDouble, ParameterDirection, sizeof(DOUBLE), CDBVarValue((DOUBLE)fValue));
	return;
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, DOUBLE dValue, ParameterDirectionEnum ParameterDirection)
{
	ASSERT(m_pIDataBase != NULL);
	m_pIDataBase->AddParameter(pszItem, adDouble, ParameterDirection, sizeof(DOUBLE), CDBVarValue((DOUBLE)dValue));
	return;
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, LPCSTR pszString, ParameterDirectionEnum ParameterDirection)
{
	ASSERT(m_pIDataBase != NULL);
	m_pIDataBase->AddParameter(pszItem, adVarChar, ParameterDirection, CountStringBufferA(pszString), CDBVarValue(pszString));
	return;
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, LPCWSTR pszString, ParameterDirectionEnum ParameterDirection)
{
	ASSERT(m_pIDataBase != NULL);
	m_pIDataBase->AddParameter(pszItem, adVarWChar, ParameterDirection, CountStringBufferW(pszString), CDBVarValue(pszString));
	return;
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, SYSTEMTIME & SystemTime, ParameterDirectionEnum ParameterDirection)
{
	ASSERT(m_pIDataBase != NULL);

	DOUBLE dValue = 0.0;
	SystemTimeToVariantTime(&SystemTime, &dValue);

	m_pIDataBase->AddParameter(pszItem, adDBDate, ParameterDirection, sizeof(DOUBLE), CDBVarValue((DOUBLE)dValue));
}

//插入参数
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, BYTE * pData, ULONG lSize, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		//变量定义
		SAFEARRAYBOUND SafeArrayBound;
		ZeroMemory(&SafeArrayBound, sizeof(SafeArrayBound));

		//设置变量
		SafeArrayBound.lLbound = 0L;
		SafeArrayBound.cElements = lSize;
		//变量定义
		SAFEARRAY * pSafeArray = SafeArrayCreate(VT_UI1, 1, &SafeArrayBound);

		//创建数组
		for (LONG nIndex = 0; nIndex < (LONG)lSize; nIndex++)
		{
			SafeArrayPutElement(pSafeArray, &nIndex, pData + nIndex);
		}

		//设置变量
		VARIANT VarChunk;
		VariantClear(&VarChunk);
		VarChunk.parray = pSafeArray;
		VarChunk.vt = VT_ARRAY | VT_UI1;

		m_pIDataBase->AddParameter(pszItem, adLongVarBinary, adParamInput, lSize, CDBVarValue(VarChunk));
	}
}

//插入参数
VOID CDataBaseAide::AddParameterOutput(LPCTSTR pszItem, LPSTR pszString, UINT uSize, ParameterDirectionEnum ParameterDirection)
{
	ASSERT(m_pIDataBase != NULL);
	m_pIDataBase->AddParameter(pszItem, adVarChar, ParameterDirection, uSize, CDBVarValue(pszString));
	return;
}

//插入参数
VOID CDataBaseAide::AddParameterOutput(LPCTSTR pszItem, LPWSTR pszString, UINT uSize, ParameterDirectionEnum ParameterDirection)
{
	ASSERT(m_pIDataBase != NULL);
	m_pIDataBase->AddParameter(pszItem, adVarWChar, ParameterDirection, uSize, CDBVarValue(pszString));
	return;
}

//返回数值
LONG CDataBaseAide::GetReturnValue()
{
	ASSERT(m_pIDataBase != NULL);
	return m_pIDataBase->GetReturnValue();
}

//执行过程
LONG CDataBaseAide::ExecuteProcess(LPCTSTR pszSPName, BOOL bRecordset, DWORD dwTimeout)
{
	ASSERT(m_pIDataBase != NULL);
	m_pIDataBase->ExecuteProcess(pszSPName, bRecordset, dwTimeout);
	if (!bRecordset) return DB_SUCCESS;

	LONG RetValue = m_pIDataBase->GetReturnValue();
	return RetValue;
}

//执行语句
LONG CDataBaseAide::ExecuteSentence(LPCTSTR pszCommand, BOOL bRecordset, DWORD dwTimeout)
{
	ASSERT(m_pIDataBase != NULL);
	m_pIDataBase->ExecuteSentence(pszCommand, bRecordset, dwTimeout);
	if (!bRecordset) return DB_SUCCESS;

	LONG RetValue = m_pIDataBase->GetReturnValue();
	return RetValue;
}

// 获取系统毫秒级时间
unsigned long long CDataBaseAide::get_systen_milliseconds()
{
	//cpu时钟频率 次/秒 目前 一般最大 4G 2^32   
	static LARGE_INTEGER tick_rate = { 0 };
	//记录cpu从开机起，运行的时钟次数 1秒 4G 可以运行约 100多年  
	LARGE_INTEGER ticks = { 0 };
	//获取cpu时钟频率  
	QueryPerformanceFrequency(&tick_rate);
	//获取从cpu开机起，运行的时钟次数  
	QueryPerformanceCounter(&ticks);
	//用ticks/rate == 从开机器到现在经过的秒数  
	unsigned long long seconds = ticks.QuadPart / tick_rate.QuadPart;
	//总ticks - 秒*rate == 两秒钟之间的ticks  
	unsigned long long left_part = ticks.QuadPart - (tick_rate.QuadPart*seconds);
	//在两秒之间的ticks可以用来求出两秒之间的毫秒数  
	unsigned long long mseconds = left_part * 1000 / tick_rate.QuadPart;
	//求出总毫秒数 如果直接用ticks * 1000 / rate 则有可能在1年以后溢出  
	unsigned long long ret = seconds * 1000 + mseconds;
	return ret;
}
//////////////////////////////////////////////////////////////////////////