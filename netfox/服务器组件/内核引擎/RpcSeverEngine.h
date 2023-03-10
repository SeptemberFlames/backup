/**
*
*	Copyright 2018-04-28 Button
*
*/

#ifndef RPC_SERVER_FILE_H_
#define RPC_SERVER_FILE_H_

#pragma once

#include <string>
#include <map>
#include <string>
#include <sstream>
#include <functional>
#include <memory>
#include "zmq.hpp"
#include "Serializer.hpp"
#include "TraceServiceManager.h"
#include "TraceService.h"
#include "ServerPushLog.h"
#include "RpcValue.h"
#include <mutex>
#include <queue>
#include <thread>

#include "KernelEngineHead.h"

// rpc 类定义
class KERNEL_ENGINE_CLASS CRpcServerEngine
{
public:
	enum rpc_err_code
	{
		RPC_ERR_SUCCESS = 0,
		RPC_ERR_FUNCTIION_NOT_BIND,
		RPC_ERR_RECV_TIMEOUT
	};

public:
	// 打包帮助模板
	template<typename Tuple, std::size_t... Is>
	void package_params_impl(Serializer& ds, const Tuple& t, std::index_sequence<Is...>)
	{
		initializer_list<int>{((ds << std::get<Is>(t)), 0)...};
	}

	template<typename... Args>
	void package_params(Serializer& ds, const std::tuple<Args...>& t)
	{
		package_params_impl(ds, t, std::index_sequence_for<Args...>{});
	}

	// 用tuple做参数调用函数模板类
	template<typename Function, typename Tuple, std::size_t... Index>
	decltype(auto) invoke_impl(Function&& func, Tuple&& t, std::index_sequence<Index...>)
	{
		return func(std::get<Index>(std::forward<Tuple>(t))...);
	}

	template<typename Function, typename Tuple>
	decltype(auto) invoke(Function&& func, Tuple&& t)
	{
		constexpr auto size = std::tuple_size<typename std::decay<Tuple>::type>::value;
		return invoke_impl(std::forward<Function>(func), std::forward<Tuple>(t), std::make_index_sequence<size>{});
	}

	// 调用帮助类，主要用于返回是否void的情况
	template<typename R, typename F, typename ArgsTuple>
	typename std::enable_if<std::is_same<R, void>::value, typename type_xx<R>::type >::type
		call_helper(F f, ArgsTuple args) {
		invoke(f, args);
		return 0;
	}

	template<typename R, typename F, typename ArgsTuple>
	typename std::enable_if<!std::is_same<R, void>::value, typename type_xx<R>::type >::type
		call_helper(F f, ArgsTuple args) {
		return invoke(f, args);
	}

	//构造函数
public:
	CRpcServerEngine();
	 ~CRpcServerEngine();
	
	//转换接口
public:
	string CStingToString(CString src)
	{
		USES_CONVERSION;
		char* p = T2A(src.GetBuffer(0));
		string dst(p);
		src.ReleaseBuffer();
		return dst;
	}
	CString StringToCString(string src)
	{
		return CString(src.c_str());
	}

	//内部调用
private:
	template<typename R, typename C, typename S, typename... Params>
	void callproxy_(R(C::* func)(Params...), S* s, Serializer* pr, const char* data, int len) 
	{
		using args_type = std::tuple<typename std::decay<Params>::type...>;

		Serializer ds(StreamBuffer(data, len));
		constexpr auto N = std::tuple_size<typename std::decay<args_type>::type>::value;
		args_type args = ds.get_tuple < args_type >(std::make_index_sequence<N>{});

		auto ff = [=](Params... ps)->R 
		{
			return (s->*func)(ps...);
		};

		typename type_xx<R>::type r = call_helper<R>(ff, args);

		value_t<R> val;
		val.set_code(RPC_ERR_SUCCESS);
		val.set_val(r);
		(*pr) << val;
	}

	template<typename R, typename... Params>
	void callproxy_(std::function<R(Params... ps)> func, Serializer* pr, const char* data, int len) 
	{
		using args_type = std::tuple<typename std::decay<Params>::type...>;

		Serializer ds(StreamBuffer(data, len));
		constexpr auto N = std::tuple_size<typename std::decay<args_type>::type>::value;
		args_type args = ds.get_tuple < args_type >(std::make_index_sequence<N>{});

		typename type_xx<R>::type r = call_helper<R>(func, args);

		value_t<R> val;
		val.set_code(RPC_ERR_SUCCESS);
		val.set_val(r);
		(*pr) << val;
	}

	//内部调用
private:
	template<typename F>
	inline void callproxy(F fun, Serializer* pr, const char* data, int len)
	{
		callproxy_(fun, pr, data, len);
	}

	template<typename F, typename S>
	inline void callproxy(F fun, S * s, Serializer * pr, const char * data, int len)
	{
		callproxy_(fun, s, pr, data, len);
	}

	//绑定接口
public:
	template<typename F>
	void bind(std::string name, F func)
	{
		m_handlers[name] = std::bind(&CRpcServerEngine::callproxy<F>, this, func, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}

	template<typename F, typename S>
	void bind(std::string name, F func, S* s)
	{
		m_handlers[name] = std::bind(&CRpcServerEngine::callproxy<F, S>, this, func, s, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}

	//网络收发
public:
	//连接状态
	bool zmqConnected();
	//网络发送
	bool zmqSend(zmq::message_t& data);
	//网路接收
	bool zmqRecv(zmq::message_t& data);

	//调度接口
public:
	//调用回复
	void callAndReply(std::string name, const char* data, int len);

	//线程函数
public:
	//接收函数
	void OnZmqMsgFunction();

	//服务接口
public:
	//启动服务
	virtual bool StartService();
	//停止服务
	virtual bool ConcludeService();
	
	//获取方法
public:
	auto GetBindName(string name)
	{
		return m_handlers[name];
	}

	//绑定相关
private: 
	std::map<std::string, std::function<void(Serializer*, const char*, int)>> m_handlers;

	//网络相关
private:
	zmq::context_t m_context;
	std::unique_ptr<zmq::socket_t, std::function<void(zmq::socket_t*)>> m_socket;
	rpc_err_code m_error_code;

	//状态变量
protected:
	bool							m_bService;							//服务标志

	//线程对象
protected:
	std::thread *					m_zmqMsgThread;						//接收线程
};

#endif