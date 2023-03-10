#ifndef ARRAY_HEAD_FILE
#define ARRAY_HEAD_FILE

#pragma once

#include <mutex>
#include <Afxmt.h>

//////////////////////////////////////////////////////////////////////////////////

//数组模板类
template <class TYPE, class ARG_TYPE = const TYPE &> class CWHArray
{
	//变量定义
private:
	TYPE *							m_pData;							//数组指针
	INT_PTR							m_nMaxCount;						//缓冲数目
	INT_PTR							m_nGrowCount;						//增长数目
	INT_PTR							m_nElementCount;					//元素数目

	//互斥对象
public:
	CMutex *						m_pMutex;							//互斥指针

	//函数定义
public:
	//构造函数
	CWHArray();
	//析构函数
	virtual ~CWHArray();

	//信息函数
public:
	//是否空组
	bool IsEmpty() const;
	//获取数目
	INT_PTR GetCount() const;

	//功能函数
public:
	//获取缓冲
	TYPE * GetData();
	//获取缓冲
	const TYPE * GetData() const;
	//增加元素
	INT_PTR Add(ARG_TYPE newElement);
	//查找元素
	INT_PTR Find(ARG_TYPE newElement);
	//拷贝数组
	VOID Copy(const CWHArray & Src);
	//追加数组
	INT_PTR Append(const CWHArray & Src);
	//获取元素
	TYPE & GetAt(INT_PTR nIndex);
	//获取元素
	const TYPE & GetAt(INT_PTR nIndex) const;
	//获取元素
	TYPE & ElementAt(INT_PTR nIndex);
	//获取元素
	const TYPE & ElementAt(INT_PTR nIndex) const;
	//插入数据
	VOID InsertAt(INT_PTR nIndex, const CWHArray & Src);
	//插入数据
	VOID InsertAt(INT_PTR nIndex, ARG_TYPE newElement, INT_PTR nCount = 1);
	//删除数据
	VOID RemoveAt(INT_PTR nIndex, INT_PTR nCount = 1);
	//删除元素
	VOID RemoveAll();

	//基本接口
public:
	//设置大小
	VOID SetSize(INT_PTR nNewSize);
	//设置元素
	VOID SetAt(INT_PTR nIndex, ARG_TYPE newElement);
	//设置元素
	VOID SetAtGrow(INT_PTR nIndex, ARG_TYPE newElement);

	//操作重载
public:
	//操作重载
	TYPE & operator[](INT_PTR nIndex);
	//操作重载
	const TYPE & operator[](INT_PTR nIndex) const;

	//内存函数
public:
	//释放内存
	VOID FreeMemory();
	//申请内存
	VOID AllocMemory(INT_PTR nNewCount);
};

//////////////////////////////////////////////////////////////////////////////////
// CWHArray<TYPE, ARG_TYPE> 内联函数

//是否空组
template<class TYPE, class ARG_TYPE>
AFX_INLINE bool CWHArray<TYPE, ARG_TYPE>::IsEmpty() const
{
	CSingleLock SingleLock(m_pMutex);
	
	return (m_nElementCount == 0);
}

//获取数目
template<class TYPE, class ARG_TYPE>
AFX_INLINE INT_PTR CWHArray<TYPE, ARG_TYPE>::GetCount() const
{
	CSingleLock SingleLock(m_pMutex);

	return m_nElementCount;
}

//增加元素
template<class TYPE, class ARG_TYPE>
AFX_INLINE INT_PTR CWHArray<TYPE, ARG_TYPE>::Add(ARG_TYPE newElement)
{
	CSingleLock SingleLock(m_pMutex);

	INT_PTR nIndex = m_nElementCount;
	SetAtGrow(nIndex, newElement);
	return nIndex;
}

//查找元素
template<class TYPE, class ARG_TYPE>
INT_PTR CWHArray<TYPE, ARG_TYPE>::Find(ARG_TYPE newElement)
{
	CSingleLock SingleLock(m_pMutex);

	INT_PTR nFindIndex = -1;
	for (INT_PTR nIndex = 0; nIndex < m_nElementCount; nIndex++)
	{
		if (ElementAt(nIndex) == newElement)
		{
			nFindIndex = nIndex;
			break;
		}
	}

	return nFindIndex;
}

//操作重载
template<class TYPE, class ARG_TYPE>
AFX_INLINE TYPE & CWHArray<TYPE, ARG_TYPE>::operator[](INT_PTR nIndex)
{
	return ElementAt(nIndex);
}

//操作重载
template<class TYPE, class ARG_TYPE>
AFX_INLINE const TYPE & CWHArray<TYPE, ARG_TYPE>::operator[](INT_PTR nIndex) const
{
	return GetAt(nIndex);
}

//////////////////////////////////////////////////////////////////////////////////
// CWHArray<TYPE, ARG_TYPE> 外联函数

//构造函数
template<class TYPE, class ARG_TYPE>
CWHArray<TYPE, ARG_TYPE>::CWHArray()
{
	//成员变量
	m_pData = NULL;
	m_nMaxCount = 0;
	m_nGrowCount = 0;
	m_nElementCount = 0;

	//互斥对象
	m_pMutex = new CMutex;

	return;
}

//构造函数
template<class TYPE, class ARG_TYPE>
CWHArray<TYPE, ARG_TYPE>::~CWHArray()
{
	if (m_pData != NULL)
	{
		for (INT_PTR i = 0;i < m_nElementCount;i++)	(m_pData + i)->~TYPE();
		delete[](BYTE *)m_pData;
		m_pData = NULL;

		
	}

	if (m_pMutex != NULL)
	{
		delete m_pMutex;
		m_pMutex = NULL;
	}

	return;
}

//获取缓冲
template<class TYPE, class ARG_TYPE>
TYPE * CWHArray<TYPE, ARG_TYPE>::GetData()
{
	CSingleLock SingleLock(m_pMutex);

	return m_pData;
}

//获取缓冲
template<class TYPE, class ARG_TYPE>
const TYPE * CWHArray<TYPE, ARG_TYPE>::GetData() const
{
	CSingleLock SingleLock(m_pMutex);

	return m_pData;
}

//拷贝数组
template<class TYPE, class ARG_TYPE>
VOID CWHArray<TYPE, ARG_TYPE>::Copy(const CWHArray & Src)
{
	CSingleLock SingleLock(m_pMutex);

	//效验参数
	ASSERT(this != &Src);
	if (this == &Src) return;

	//拷贝数组
	AllocMemory(Src.m_nElementCount);
	if (m_nElementCount > 0)
	{
		for (INT_PTR i = 0;i < m_nElementCount;i++) (m_pData + i)->~TYPE();
		memset(m_pData, 0, m_nElementCount * sizeof(TYPE));
	}
	for (INT_PTR i = 0;i < Src.m_nElementCount;i++)	m_pData[i] = Src.m_pData[i];
	m_nElementCount = Src.m_nElementCount;

	return;
}

//追加数组
template<class TYPE, class ARG_TYPE>
INT_PTR CWHArray<TYPE, ARG_TYPE>::Append(const CWHArray & Src)
{
	CSingleLock SingleLock(m_pMutex);

	//效验参数
	ASSERT(this != &Src);
	if (this == &Src) AfxThrowInvalidArgException();

	//拷贝数组
	if (Src.m_nElementCount > 0)
	{
		INT_PTR nOldCount = m_nElementCount;
		AllocMemory(m_nElementCount + Src.m_nElementCount);
		for (INT_PTR i = 0;i < Src.m_nElementCount;i++)	m_pData[m_nElementCount + i] = Src.m_pData[i];
		m_nElementCount += Src.m_nElementCount;
	}

	return m_nElementCount;
}

//获取元素
template<class TYPE, class ARG_TYPE>
TYPE & CWHArray<TYPE, ARG_TYPE>::GetAt(INT_PTR nIndex)
{
	CSingleLock SingleLock(m_pMutex);

	ASSERT((nIndex >= 0) && (nIndex < m_nElementCount));
	if ((nIndex < 0) || (nIndex >= m_nElementCount)) AfxThrowInvalidArgException();

	return m_pData[nIndex];
}

//获取元素
template<class TYPE, class ARG_TYPE>
const TYPE & CWHArray<TYPE, ARG_TYPE>::GetAt(INT_PTR nIndex) const
{
	CSingleLock SingleLock(m_pMutex);

	ASSERT((nIndex >= 0) && (nIndex < m_nElementCount));
	if ((nIndex < 0) || (nIndex >= m_nElementCount)) AfxThrowInvalidArgException();

	return m_pData[nIndex];
}

//获取元素
template<class TYPE, class ARG_TYPE>
TYPE & CWHArray<TYPE, ARG_TYPE>::ElementAt(INT_PTR nIndex)
{
	CSingleLock SingleLock(m_pMutex);

	ASSERT((nIndex >= 0) && (nIndex < m_nElementCount));
	if ((nIndex < 0) && (nIndex >= m_nElementCount)) AfxThrowInvalidArgException();

	return m_pData[nIndex];
}

//获取元素
template<class TYPE, class ARG_TYPE>
const TYPE & CWHArray<TYPE, ARG_TYPE>::ElementAt(INT_PTR nIndex) const
{
	CSingleLock SingleLock(m_pMutex);

	ASSERT((nIndex >= 0) && (nIndex < m_nElementCount));
	if ((nIndex < 0) && (nIndex >= m_nElementCount)) AfxThrowInvalidArgException();

	return m_pData[nIndex];
}

//设置大小
template<class TYPE, class ARG_TYPE>
VOID CWHArray<TYPE, ARG_TYPE>::SetSize(INT_PTR nNewSize)
{
	//效验参数
	ASSERT(nNewSize >= 0);
	if (nNewSize < 0)	AfxThrowInvalidArgException();

	//设置大小
	AllocMemory(nNewSize);
	if (nNewSize > m_nElementCount)
	{
		for (INT_PTR i = m_nElementCount;i < nNewSize;i++) new ((VOID *)(m_pData + i)) TYPE;
	}
	else if (nNewSize < m_nElementCount)
	{
		for (INT_PTR i = nNewSize;i < m_nElementCount;i++) (m_pData + i)->~TYPE();
		memset(m_pData + nNewSize, 0, (m_nElementCount - nNewSize) * sizeof(TYPE));
	}
	m_nElementCount = nNewSize;

	return;
}

//设置元素
template<class TYPE, class ARG_TYPE>
VOID CWHArray<TYPE, ARG_TYPE>::SetAt(INT_PTR nIndex, ARG_TYPE newElement)
{
	ASSERT((nIndex >= 0) && (nIndex < m_nElementCount));
	if ((nIndex >= 0) && (nIndex < m_nElementCount)) m_pData[nIndex] = newElement;
	else AfxThrowInvalidArgException();

	return;
}

//设置元素
template<class TYPE, class ARG_TYPE>
VOID CWHArray<TYPE, ARG_TYPE>::SetAtGrow(INT_PTR nIndex, ARG_TYPE newElement)
{
	//效验参数
	ASSERT(nIndex >= 0);
	if (nIndex < 0) AfxThrowInvalidArgException();

	//设置元素
	if (nIndex >= m_nElementCount) SetSize(m_nElementCount + 1);
	m_pData[nIndex] = newElement;

	return;
}

//插入数据
template<class TYPE, class ARG_TYPE>
VOID CWHArray<TYPE, ARG_TYPE>::InsertAt(INT_PTR nIndex, const CWHArray & Src)
{
	CSingleLock SingleLock(m_pMutex);

	//效验参数
	ASSERT(nStartIndex >= 0);
	if (nStartIndex < 0) AfxThrowInvalidArgException();

	if (Src.m_nElementCount > 0)
	{
		//申请数组
		if (nIndex < m_nElementCount)
		{
			INT_PTR nOldCount = m_nElementCount;
			SetSize(m_nElementCount + Src.m_nElementCount);
			for (INT_PTR i = 0;i < nCount;i++) (m_pData + nOldCount + i)->~TYPE();
			memmove(m_pData + nIndex + nCount, m_pData + nIndex, (nOldCount - nIndex) * sizeof(TYPE));
			memset(m_pData + nIndex, 0, Src.m_nElementCount * sizeof(TYPE));
			for (INT_PTR i = 0;i < Src.m_nElementCount;i++) new (m_pData + nIndex + i) TYPE();
		}
		else
		{
			SetSize(nIndex + nCount);
		}

		//拷贝数组
		ASSERT((nIndex + Src.m_nElementCount) <= m_nElementCount);
		while (nCount--) m_pData[nIndex++] = newElement;
	}

	return;
}

//插入数据
template<class TYPE, class ARG_TYPE>
VOID CWHArray<TYPE, ARG_TYPE>::InsertAt(INT_PTR nIndex, ARG_TYPE newElement, INT_PTR nCount)
{
	CSingleLock SingleLock(m_pMutex);

	//效验参数
	ASSERT(nIndex >= 0);
	ASSERT(nCount > 0);
	if ((nIndex < 0) || (nCount <= 0)) AfxThrowInvalidArgException();

	//申请数组
	if (nIndex < m_nElementCount)
	{
		INT_PTR nOldCount = m_nElementCount;
		SetSize(m_nElementCount + nCount);
		for (INT_PTR i = 0;i < nCount;i++) (m_pData + nOldCount + i)->~TYPE();
		memmove(m_pData + nIndex + nCount, m_pData + nIndex, (nOldCount - nIndex) * sizeof(TYPE));
		memset(m_pData + nIndex, 0, nCount * sizeof(TYPE));
		for (INT_PTR i = 0;i < nCount;i++) new (m_pData + nIndex + i) TYPE();
	}
	else SetSize(nIndex + nCount);

	//拷贝数组
	ASSERT((nIndex + nCount) <= m_nElementCount);
	while (nCount--) m_pData[nIndex++] = newElement;

	return;
}

//删除数据
template<class TYPE, class ARG_TYPE>
VOID CWHArray<TYPE, ARG_TYPE>::RemoveAt(INT_PTR nIndex, INT_PTR nCount)
{
	CSingleLock SingleLock(m_pMutex);

	//效验参数
	ASSERT(nIndex >= 0);
	ASSERT(nCount >= 0);
	ASSERT(nIndex + nCount <= m_nElementCount);
	if ((nIndex < 0) || (nCount < 0) || ((nIndex + nCount > m_nElementCount))) AfxThrowInvalidArgException();

	//删除数据
	INT_PTR nMoveCount = m_nElementCount - (nIndex + nCount);
	for (INT_PTR i = 0;i < nCount;i++) (m_pData + nIndex + i)->~TYPE();
	if (nMoveCount > 0) memmove(m_pData + nIndex, m_pData + nIndex + nCount, nMoveCount * sizeof(TYPE));
	m_nElementCount -= nCount;

	return;
}

//删除元素
template<class TYPE, class ARG_TYPE>
VOID CWHArray<TYPE, ARG_TYPE>::RemoveAll()
{
	CSingleLock SingleLock(m_pMutex);

	if (m_nElementCount > 0)
	{
		for (INT_PTR i = 0;i < m_nElementCount;i++) (m_pData + i)->~TYPE();
		memset(m_pData, 0, m_nElementCount * sizeof(TYPE));
		m_nElementCount = 0;
	}

	return;
}

//释放内存
template<class TYPE, class ARG_TYPE>
VOID CWHArray<TYPE, ARG_TYPE>::FreeMemory()
{
	if (m_nElementCount != m_nMaxCount)
	{
		TYPE * pNewData = NULL;
		if (m_nElementCount != 0)
		{
			pNewData = (TYPE *) new BYTE[m_nElementCount * sizeof(TYPE)];
			memcpy(pNewData, m_pData, m_nElementCount * sizeof(TYPE));
		}
		delete[](BYTE *)m_pData;
		m_pData = pNewData;
		m_nMaxCount = m_nElementCount;
	}

	return;
}

//申请内存
template<class TYPE, class ARG_TYPE>
VOID CWHArray<TYPE, ARG_TYPE>::AllocMemory(INT_PTR nNewCount)
{
	//效验参数
	ASSERT(nNewCount >= 0);

	if (nNewCount > m_nMaxCount)
	{
		//计算数目
		INT_PTR nGrowCount = m_nGrowCount;
		if (nGrowCount == 0)
		{
			nGrowCount = m_nElementCount / 8;
			nGrowCount = (nGrowCount < 4) ? 4 : ((nGrowCount > 1024) ? 1024 : nGrowCount);
		}
		nNewCount += nGrowCount;

		//申请内存
		TYPE * pNewData = (TYPE *) new BYTE[nNewCount * sizeof(TYPE)];
		memcpy(pNewData, m_pData, m_nElementCount * sizeof(TYPE));
		memset(pNewData + m_nElementCount, 0, (nNewCount - m_nElementCount) * sizeof(TYPE));
		delete[](BYTE *)m_pData;

		//设置变量
		m_pData = pNewData;
		m_nMaxCount = nNewCount;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////

#endif