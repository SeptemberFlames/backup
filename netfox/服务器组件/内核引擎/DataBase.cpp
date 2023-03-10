#include "StdAfx.h"
#include "DataBase.h"
#include "TraceServiceManager.h"
#include "ServerPushLog.h"
#include "TraceService.h"

/////////////////////////////////////////////////////////////

//宏定义
_COM_SMARTPTR_TYPEDEF(IADORecordBinding, __uuidof(IADORecordBinding));

//结果校验
#define EfficacyResult(hResult){if (FAILED(hResult)) _com_issue_error(hResult);}

//////////////////////////////////////////////////////////////////////////

//构造函数
CDataBaseException::CDataBaseException()
{
	//错误类型
	m_hResult = S_OK;
	m_SQLException = SQLException_None;

	return;
}

//析构函数
CDataBaseException::~CDataBaseException()
{
}

//接口查询
VOID * CDataBaseException::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseException, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseException, Guid, dwQueryVer);
	return NULL;
}

//设置错误
VOID CDataBaseException::SetExceptionInfo(enSQLException SQLException, LPCTSTR pszItem, CComError * pComError, CString mstr)
{
	//效验参数
	ASSERT(pComError != NULL);
	if (pComError == NULL) return;

	//设置信息
	m_SQLException = SQLException;
	m_hResult = pComError->Error();
	if (pszItem == TEXT(""))
	{
		m_strDescribe.Format(TEXT("数据库异常:%s, 连接:%s"), (LPCTSTR)(pComError->Description()), mstr);
		CTraceService::TraceString(m_strDescribe, TraceLevel_Exception);
	}
	else
	{
		m_strDescribe.Format(TEXT("数据库异常:%s, 参数:%s, 连接:%s"), (LPCTSTR)(pComError->Description()), pszItem, mstr);
		CTraceService::TraceString(m_strDescribe, TraceLevel_Exception);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CDataBase::CDataBase() : m_dwResumeConnectCount(3L), m_dwResumeConnectTime(3L)
{
	//状态变量
	m_dwConnectCount = 0;
	m_dwConnectErrorTime = 0L;

	//创建对象
	m_DBCommand.CreateInstance(__uuidof(Command));
	m_DBRecordset.CreateInstance(__uuidof(Recordset));
	m_DBConnection.CreateInstance(__uuidof(Connection));

	//效验数据
	if (m_DBCommand == NULL) throw TEXT("创建 m_DBCommand 对象失败");
	if (m_DBRecordset == NULL) throw TEXT("创建 m_DBRecordset 对象失败");
	if (m_DBConnection == NULL) throw TEXT("创建 m_DBConnection 对象失败");

	return;
}

//析构函数
CDataBase::~CDataBase()
{
	//关闭连接
	CloseConnection();

	//释放对象
	m_DBCommand.Release();
	m_DBRecordset.Release();
	m_DBConnection.Release();
	return;
}

//接口查询
void * CDataBase::QueryInterface(REFGUID  Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBase, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBase, Guid, dwQueryVer);
	return NULL;
}

//打开连接
VOID CDataBase::OpenConnection()
{
	//连接数据库
	try
	{
		//关闭连接
		CloseConnection();

		//打开连接
		EfficacyResult(m_DBConnection->Open(_bstr_t(m_strConnect), L"", L"", adConnectUnspecified));

		//设置对象
		m_DBConnection->CursorLocation = adUseClient;
		m_DBCommand->ActiveConnection = m_DBConnection;

		//设置变量
		m_dwConnectCount = 0L;
		m_dwConnectErrorTime = 0L;
	}
	catch (CComError & ComError)
	{
		//抛出异常
		OnSQLException(SQLException_Connect, TEXT(""), &ComError);
	}

	return;
}

//关闭连接
VOID CDataBase::CloseConnection()
{
	try
	{
		//关闭连接
		CloseRecordset();
		if ((m_DBConnection != NULL) && (m_DBConnection->GetState() != adStateClosed))
		{
			EfficacyResult(m_DBConnection->Close());
		}
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, TEXT(""), &ComError);
	}

	return;
}

//连接信息
bool CDataBase::SetConnectionInfo(DWORD dwDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword, bool IsSlave)
{
	//效验参数
	ASSERT(szUser != NULL);
	ASSERT(szDBName != NULL);
	ASSERT(szPassword != NULL);

	//变量定义
	CString strUser;
	CString strDBName;
	CString strPassword;

	//字符转换
	ConvertToSQLSyntax(szUser, strUser);
	ConvertToSQLSyntax(szDBName, strDBName);
	ConvertToSQLSyntax(szPassword, strPassword);

	//构造连接
	BYTE * pcbDBAddr = (BYTE *)&dwDBAddr;
	m_strConnect.Format(TEXT("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%d.%d.%d.%d,%ld;"),
		strPassword, strUser, strDBName, pcbDBAddr[0], pcbDBAddr[1], pcbDBAddr[2], pcbDBAddr[3], wPort);

	return true;
}

//连接信息
bool CDataBase::SetConnectionInfo(LPCTSTR szDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword, bool IsSlave)
{
	//效验参数
	ASSERT(szUser != NULL);
	ASSERT(szDBName != NULL);
	ASSERT(szDBAddr != NULL);
	ASSERT(szPassword != NULL);

	//变量定义
	CString strUser;
	CString strDBName;
	CString strPassword;

	//字符转换
	ConvertToSQLSyntax(szUser, strUser);
	ConvertToSQLSyntax(szDBName, strDBName);
	ConvertToSQLSyntax(szPassword, strPassword);

	//构造连接																									
	m_strConnect.Format(TEXT("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%s,%ld;"),
		strPassword, strUser, strDBName, szDBAddr, wPort);

	return true;
}

//清除参数
void  CDataBase::ClearParameters()
{
	try
	{
		long lParameterCount = m_DBCommand->Parameters->Count;
		if (lParameterCount > 0L)
		{
			for (long i = lParameterCount; i > 0; i--)
			{
				m_DBCommand->Parameters->Delete(i - 1);
			}
		}
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, TEXT(""), &ComError);
	}

	return;
}

//获取参数
VOID  CDataBase::GetParameter(LPCTSTR pszParamName, CDBVarValue & DBVarValue)
{
	try
	{
		DBVarValue = m_DBCommand->Parameters->Item[pszParamName]->Value;
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, pszParamName, &ComError);
	}

	return;
}

//插入参数
VOID  CDataBase::AddParameter(LPCTSTR pszName, DataTypeEnum Type, ParameterDirectionEnum Direction, LONG lSize, CDBVarValue & DBVarValue)
{
	try
	{
		_ParameterPtr Parameter = m_DBCommand->CreateParameter(pszName, Type, Direction, lSize, DBVarValue);
		m_DBCommand->Parameters->Append(Parameter);
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, pszName, &ComError);
	}

	return;
}

//切换记录
VOID  CDataBase::NextRecordset()
{
	try
	{
		CDBVarValue DBVarValue(0L);
		m_DBRecordset->NextRecordset(&DBVarValue);
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, TEXT(""), &ComError);
	}

	return;
}

//绑定对象
VOID  CDataBase::BindToRecordset(CADORecordBinding *pBind)
{
	try
	{
		IADORecordBindingPtr RecordBindingPtr(m_DBRecordset);
		RecordBindingPtr->BindToRecordset(pBind);
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, TEXT(""), &ComError);
	}

	return;
}

//往下移动
void  CDataBase::MoveToNext()
{
	try
	{
		m_DBRecordset->MoveNext();
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, TEXT(""), &ComError);
	}

	return;
}

//移到开头
void CDataBase::MoveToFirst()
{
	try
	{
		m_DBRecordset->MoveFirst();
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, TEXT(""), &ComError);
	}

	return;
}

//是否结束
bool CDataBase::IsRecordsetEnd()
{
	bool  RetCode = false;
	try
	{
		RetCode = (m_DBRecordset->EndOfFile == VARIANT_TRUE);
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, TEXT(""), &ComError);
	}

	return RetCode;
}

//获取数目
long CDataBase::GetRecordCount()
{
	long RetCode = 0;
	try
	{
		RetCode = m_DBRecordset->GetRecordCount();
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, TEXT(""), &ComError);
	}

	return RetCode;
}

//获取返回数值
long  CDataBase::GetReturnValue()
{
	try
	{
		//变量定义
		_ParameterPtr Parameter;
		long lParameterCount = m_DBCommand->Parameters->Count;

		//参数搜索
		for (long i = 0; i < lParameterCount; i++)
		{
			Parameter = m_DBCommand->Parameters->Item[i];
			if (Parameter->Direction == adParamReturnValue)
			{
				return Parameter->Value.lVal;
			}
		}

		//错误效验
		ASSERT(FALSE);
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, TEXT(""), &ComError);
	}

	return 0;
}

//关闭记录
VOID CDataBase::CloseRecordset()
{
	try
	{
		if (IsRecordsetOpened() == true) EfficacyResult(m_DBRecordset->Close());
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, TEXT(""), &ComError);
	}


	return;
}

//获取数据
VOID CDataBase::GetRecordsetValue(LPCTSTR pszItem, CDBVarValue & DBVarValue)
{
	try
	{
		FieldsPtr RecordFields = m_DBRecordset->GetFields();
		DBVarValue = RecordFields->GetItem(pszItem)->GetValue();
	}
	catch (CComError & ComError)
	{
		DBVarValue.Clear();
		OnSQLException(SQLException_Syntax, pszItem, &ComError);
	}

	return;
}


/*
//adCmdText 指示提供者应按命令的文本定义（如 SQL 语句）来计算 CommandText
//adAsyncExecute 指示命令应该异步执行, 等待ExecuteComplete回调
//adExecuteNoRecords 表示不需要返回记录

//adLockUnspecified = -1,///未指定
//adLockReadOnly = 1,///只读记录集
//adLockPessimistic = 2, 悲观锁定方式。数据在更新时锁定其它所有动作，这是最安全的锁定机制
//adLockOptimistic = 3, 乐观锁定方式。只有在你调用Update方法时才锁定记录。在此之前仍然可以做数据的更新、插入、删除等动作
//adLockBatchOptimistic = 4，乐观分批更新。编辑时记录不会锁定，更改、插入及删除是在批处理模式下完成。

//adCmdText:表明CommandText是文本命令
//adCmdTable:表明CommandText是一个表名
//adCmdProc : 表明CommandText是一个存储过程
//adCmdUnknown : 未知

//CommandTimeout 0会无限期到等待结果
*/

//存储过程
VOID  CDataBase::ExecuteProcess(LPCTSTR pszSPName, BOOL bRecordset, DWORD dwTimeout)
{
	//执行命令
	do
	{
		try
		{
			//设置名称
			m_DBCommand->CommandType = adCmdStoredProc;
			m_DBCommand->CommandText = pszSPName;

			//执行命令
			if (bRecordset == true)
			{
				//关闭记录
				CloseRecordset();

				//执行过程
				m_DBRecordset->PutRefSource(m_DBCommand);
				m_DBRecordset->CursorLocation = adUseClient;
				EfficacyResult(m_DBRecordset->Open((IDispatch *)m_DBCommand, vtMissing, adOpenForwardOnly, adLockReadOnly, adOptionUnspecified));
			}
			else
			{
				m_DBConnection->CursorLocation = adUseClient;
				EfficacyResult(m_DBCommand->Execute(NULL, NULL, adExecuteNoRecords));
			}
			return;
		}
		catch (CComError & ComError)
		{
			//连接判断
			if (IsConnectError() == true)
			{
				//重新连接
				if (TryConnectAgain(true, &ComError) == false)
				{
					OnSQLException(SQLException_Connect, TEXT(""), &ComError);
				}
			}
			else
			{
				//抛出异常
				OnSQLException(SQLException_Syntax, TEXT(""), &ComError);
			}
		}
	}
	while (TRUE);


	return;
}


/*
//adCmdText 指示提供者应按命令的文本定义（如 SQL 语句）来计算 CommandText
//adAsyncExecute 指示命令应该异步执行, 等待ExecuteComplete回调
//adExecuteNoRecords 表示不需要返回记录

//adLockUnspecified = -1,///未指定
//adLockReadOnly = 1,///只读记录集
//adLockPessimistic = 2, 悲观锁定方式。数据在更新时锁定其它所有动作，这是最安全的锁定机制
//adLockOptimistic = 3, 乐观锁定方式。只有在你调用Update方法时才锁定记录。在此之前仍然可以做数据的更新、插入、删除等动作
//adLockBatchOptimistic = 4，乐观分批更新。编辑时记录不会锁定，更改、插入及删除是在批处理模式下完成。

//adCmdText:表明CommandText是文本命令
//adCmdTable:表明CommandText是一个表名
//adCmdProc : 表明CommandText是一个存储过程
//adCmdUnknown : 未知

//CommandTimeout 0会无限期到等待结果
*/

//执行语句
VOID  CDataBase::ExecuteSentence(LPCTSTR pszCommand, BOOL bRecordset, DWORD dwTimeout)
{
	//执行命令
	do
	{
		try
		{
			//设置名字
			m_DBCommand->CommandType = adCmdText;
			m_DBCommand->CommandText = pszCommand;

			//执行命令
			if (bRecordset == true)
			{
				//关闭记录
				CloseRecordset();

				//执行语句
				m_DBRecordset->PutRefSource(m_DBCommand);
				m_DBRecordset->CursorLocation = adUseClient;
				EfficacyResult(m_DBRecordset->Open((IDispatch *)m_DBCommand, vtMissing, adOpenStatic, adLockOptimistic, adCmdText));
			}
			else
			{
				m_DBConnection->CursorLocation = adUseClient;
				EfficacyResult(m_DBCommand->Execute(NULL, NULL, adCmdText | adExecuteNoRecords));
			}

			return;
		}
		catch (CComError & ComError)
		{
			//连接判断
			if (IsConnectError() == true)
			{
				//重新连接
				if (TryConnectAgain(true, &ComError) == false)
				{
					OnSQLException(SQLException_Connect, TEXT(""), &ComError);
				}
			}
			else
			{
				//抛出异常
				OnSQLException(SQLException_Syntax, TEXT(""), &ComError);
			}
		}
	}
	while (TRUE);

	return;
}

//连接错误
bool CDataBase::IsConnectError()
{
	try
	{
		//执行命令
		if (m_DBConnection == NULL) return true;
		if (m_DBConnection->GetState() == adStateClosed)return true;

		//参数判断
		if (m_DBConnection->Errors->Count > 0L)
		{
			//变量定义
			ErrorPtr pError = NULL;
			LONG lErrorCount = m_DBConnection->Errors->Count;

			//结果搜索
			for (LONG i = 0; i < lErrorCount; i++)
			{
				if (m_DBConnection->Errors->GetItem(i)->Number == E_FAIL)
				{
					//提示消息
					g_TraceServiceManager.TraceString(TEXT("数据库连接中断，稍后将尝试重新连接"), TraceLevel_Warning);

					return true;
				}
			}
		}
		return false;
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, TEXT(""), &ComError);
	}



	return  false;
}

//是否打开
bool  CDataBase::IsRecordsetOpened()
{
	//状态判断
	if (m_DBRecordset == NULL) return false;
	if (m_DBRecordset->GetState() == adStateClosed) return false;

	return true;
}

//重新连接
bool CDataBase::TryConnectAgain(bool bFocusConnect, CComError * pComError)
{
	try
	{
		//保存连接
		bool bReConnect = bFocusConnect;

		//判断连接
		if (bReConnect == false)
		{
			DWORD dwNowTime = (DWORD)time(NULL);
			if ((m_dwConnectErrorTime + m_dwResumeConnectTime) >= dwNowTime)
			{
				bReConnect = true;
			}
		}

		//判断连接
		if (bReConnect == false)
		{
			if (m_dwConnectCount >= m_dwResumeConnectCount)
			{
				bReConnect = true;
			}
		}

		//判断连接
		if (bReConnect == false)
		{
			//设置变量
			m_dwConnectCount++;
			m_dwConnectErrorTime = (DWORD)time(NULL);

			//错误提示
			if (pComError != NULL)
			{
				OnSQLException(SQLException_Connect, TEXT(""), pComError);
			}

			return false;
		}

		//重新连接
		OpenConnection();

		//提示信息
		g_TraceServiceManager.TraceString(TEXT("成功重新连接数据库"), TraceLevel_Warning);

		return true;
	}
	catch (IDataBaseException * pIDataBaseException)
	{
		//重新连接错误
		if (pComError != NULL) OnSQLException(SQLException_Connect, TEXT(""), pComError);
		else throw pIDataBaseException;
	}

	return false;
}

//转换字符
VOID CDataBase::ConvertToSQLSyntax(LPCTSTR pszString, CString & strResult)
{
	//获取缓冲
	INT nSrcLength = lstrlen(pszString);
	LPTSTR pszResult = strResult.GetBufferSetLength(nSrcLength * 2 + 1);

	//替换字符
	while (*pszString != TEXT('\0'))
	{
		if (*pszString == TEXT('\''))
		{
			*pszResult++ = TEXT('\'');
			*pszResult++ = TEXT('\'');
		}
		else *pszResult++ = *pszString++;

	}

	//结束字符
	*pszResult = TEXT('\0');
	strResult.ReleaseBuffer();

	return;
}

//错误处理
VOID CDataBase::OnSQLException(enSQLException SQLException, LPCTSTR pszItem, CComError * pComError)
{
	//设置异常
	m_DataBaseException.SetExceptionInfo(SQLException, pszItem, pComError, m_strConnect);

	//抛出异常
	throw &m_DataBaseException;
	return;
}

bool CDataBase::GetFieldValue(LPCTSTR lpFieldName, BYTE* pData, UINT& uSize)
{
	try
	{
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch (vtFld.vt)
		{
			case VT_ARRAY | VT_UI1:
			{
				for (LONG i = 0; i < (LONG)uSize; i++)
				{
					if (FAILED(SafeArrayGetElement(vtFld.parray, &i, pData + i)))
					{
						uSize = i;
						break;
					}
				}
				break;
			}
			default:
				return false;
		}
		return true;
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, lpFieldName, &ComError);
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////

//组件创建函数
DECLARE_CREATE_MODULE(DataBase);

//////////////////////////////////////////////////////////////////////////