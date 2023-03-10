#ifndef MAP_HEAD_FILE
#define MAP_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

#include <map>
#include <vector>

template<class Key, class ARG_TYPE> class CWHMap
{
	//变量定义
private:
	std::map<Key, ARG_TYPE> m_map;

	//迭代对象
private:
	//迭代对象
	typename std::map<Key, ARG_TYPE>::iterator m_iter;

	//函数定义
public:
	//构造函数
	CWHMap();
	//析构函数
	virtual ~CWHMap();

	//操作接口
public:
	//插入数据
	void Insert(const Key &inputKey, const ARG_TYPE &inputValue);
	//查找数据
	bool Find(const Key &inputKey, ARG_TYPE &outputValue);
	//删除数据
	void Delete(const Key &inputKey);
	//清空数据
	void Clear();

	//成对使用
public:
	//开始迭代
	void LoopBegin();
	//循环获取
	bool LoopGetElement(ARG_TYPE &outputValue);

	//功能接口
public:
	//是否为空
	bool IsEmpty() const;
	//元素数量
	size_t GetCount() const;
};

//////////////////////////////////////////////////////////////////////////////////

//构造函数
template<class Key, class ARG_TYPE>
CWHMap<Key, ARG_TYPE>::CWHMap()
{
	m_map.clear();
	return;
}

//析构函数
template<class Key, class ARG_TYPE>
CWHMap<Key, ARG_TYPE>::~CWHMap()
{
	m_map.clear();
	return;
}

//插入数据
template<class Key, class ARG_TYPE>
AFX_INLINE void CWHMap<Key, ARG_TYPE>::Insert(const Key &inputKey, const ARG_TYPE &inputValue)
{
	typename std::map<Key, ARG_TYPE>::iterator it = m_map.find(inputKey);
	if (m_map.end() == it) 
	{
		m_map.insert(std::pair<Key, ARG_TYPE>(inputKey, inputValue));
	}
	else
	{
		it->second = inputValue;
	}
}

//查找数据
template<class Key, class ARG_TYPE>
AFX_INLINE bool CWHMap<Key, ARG_TYPE>::Find(const Key &inputKey, ARG_TYPE &outputValue)
{
	typename std::map<Key, ARG_TYPE>::iterator it = m_map.find(inputKey);
	if (m_map.end() == it)
	{
		return false;
	}

	outputValue = it->second;
	return true;
}

//删除数据
template<class Key, class ARG_TYPE>
AFX_INLINE void CWHMap<Key, ARG_TYPE>::Delete(const Key &inputKey)
{
	typename std::map<Key, ARG_TYPE>::iterator it = m_map.find(inputKey);
	if (it != m_map.end())
	{
		it = m_map.erase(it);
	}
}

//清空数据
template<class Key, class ARG_TYPE>
AFX_INLINE void CWHMap<Key, ARG_TYPE>::Clear()
{
	std::map<Key, ARG_TYPE> emptyMap;
	m_map.swap(emptyMap);
}

//开始迭代
template<class Key, class ARG_TYPE>
AFX_INLINE void CWHMap<Key, ARG_TYPE>::LoopBegin()
{
	//重置迭代
	m_iter = m_map.begin();
}

//循环获取
template<class Key, class ARG_TYPE>
AFX_INLINE bool CWHMap<Key, ARG_TYPE>::LoopGetElement(ARG_TYPE &outputValue)
{
	if (m_iter != m_map.end())
	{
		//返回数据
		outputValue = m_iter->second;
		//迭代增加
		m_iter++;
		//获取成功
		return true;
	}
	
	//获取结束
	return false;
}

//是否为空
template<class Key, class ARG_TYPE>
AFX_INLINE bool CWHMap<Key, ARG_TYPE>::IsEmpty() const
{
	return m_map.empty();
}

//元素个数
template<class Key, class ARG_TYPE>
AFX_INLINE size_t CWHMap<Key, ARG_TYPE>::GetCount() const
{
	return m_map.size();
}

#endif