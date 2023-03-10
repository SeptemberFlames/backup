#ifndef QUEUE_HEAD_FILE
#define QUEUE_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

#include <map>
#include <vector>
#include <queue>

template<class ARG_TYPE> class CWHQueue
{
	//变量定义
private:
	std::queue<ARG_TYPE>		m_queue;

	//函数定义
public:
	//构造函数
	CWHQueue();
	//析构函数
	virtual ~CWHQueue();

	//操作接口
public:
	//保存数据
	void Push(const ARG_TYPE &inputValue);
	//获取队头
	ARG_TYPE Front();
	//删除队头
	void Pop();
	//删除数据
	void Delete(const ARG_TYPE &inputKey);
	//清空数据
	void Clear();

	//功能接口
public:
	//是否为空
	bool IsEmpty() const;
	//元素数量
	size_t GetCount() const;
};

//////////////////////////////////////////////////////////////////////////////////

//构造函数
template<class ARG_TYPE>
CWHQueue<ARG_TYPE>::CWHQueue()
{
	std::queue<ARG_TYPE> empty;
	m_queue.swap(empty);
	return;
}

//析构函数
template<class ARG_TYPE>
CWHQueue<ARG_TYPE>::~CWHQueue()
{
	std::queue<ARG_TYPE> empty;
	m_queue.swap(empty);
	return;
}

//插入数据
template<class ARG_TYPE>
AFX_INLINE void CWHQueue<ARG_TYPE>::Push(const ARG_TYPE &inputValue)
{
	m_queue.push(inputValue);
}

//查找数据
template<class ARG_TYPE>
AFX_INLINE ARG_TYPE CWHQueue<ARG_TYPE>::Front()
{
	return m_queue.front();
}

//删除数据
template<class ARG_TYPE>
AFX_INLINE void CWHQueue<ARG_TYPE>::Pop()
{
	m_queue.pop();
}

//删除数据
template<class ARG_TYPE>
AFX_INLINE void CWHQueue<ARG_TYPE>::Delete(const ARG_TYPE &inputValue)
{
	m_map.erase(inputKey);
	std::queue<ARG_TYPE>::iterator it = m_queue.begin();
	while (it != m_key.end())
	{
		if (*it == inputValue)
		{
			it = m_queue.erase(it);
			break;
		}
		it++;
	}
}

//清空数据
template<class ARG_TYPE>
AFX_INLINE void CWHQueue<ARG_TYPE>::Clear()
{
	std::queue<ARG_TYPE> emptyMap;
	m_queue.swap(emptyMap);
}

//是否为空
template<class ARG_TYPE>
AFX_INLINE bool CWHQueue<ARG_TYPE>::IsEmpty() const
{
	return m_queue.empty();
}

//元素个数
template<class ARG_TYPE>
AFX_INLINE size_t CWHQueue<ARG_TYPE>::GetCount() const
{
	return m_queue.size();
}

#endif