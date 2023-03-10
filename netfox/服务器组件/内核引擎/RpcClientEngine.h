/**
*
*	Copyright 2018-04-28 Button
*
*/

#ifndef RPC_CLIENT_FILE_H_
#define RPC_CLIENT_FILE_H_

#pragma once

#include <string>
#include <map>
#include <string>
#include <sstream>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#include "zmq.hpp"
#include "Serializer.hpp"
#include "RpcValue.h"

#include "KernelEngineHead.h"

//////////////////////////////////////////////////////////////////////////

// rpc 类定义
class KERNEL_ENGINE_CLASS CRpcClientEngine
{
protected:
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

	//构造析构
public:
	CRpcClientEngine();
	~CRpcClientEngine();

	//网络函数
public:
	//开始连接
	void connect(std::string ip, int port);
	//设置超时
	void set_timeout(int option, int ms);

public:
	//同步调用
	template<typename R, typename... Params>
	value_t<R> syncCall(std::string name, Params... ps) {
		using args_type = std::tuple<typename std::decay<Params>::type...>;
		args_type args = std::make_tuple(ps...);

		Serializer ds;
		ds << name;

		package_params(ds, args);

		return netSyncCall<R>(ds);
	}

	template<typename R>
	value_t<R> syncCall(std::string name) {
		Serializer ds;
		ds << name;

		return netSyncCall<R>(ds);
	}

	//网络收发
protected:
	//连接状态
	bool zmqConnected();
	//网络发送
	bool zmqSend(zmq::message_t& data);
	//网路接收
	bool zmqRecv(zmq::message_t& data);

	//远程调用
private:
	//同步调用
	template<typename R>
	value_t<R> netSyncCall(Serializer& ds)
	{
		std::lock_guard<std::mutex> lk(m_mutex);

		zmq::message_t request(ds.size() + 1);
		memcpy(request.data(), ds.data(), ds.size());

		value_t<R> val;
		if (!zmqConnected()) {
			m_error_code = RPC_ERR_NOT_CONNECTD;
			val.set_code(RPC_ERR_NOT_CONNECTD);
			val.set_msg("recv not connected");
			return val;
		}

		if (!zmqSend(request)) {
			m_error_code = RPC_ERR_RECV_TIMEOUT;
			val.set_code(RPC_ERR_RECV_TIMEOUT);
			val.set_msg("recv timeout");
			return val;
		}

		zmq::message_t reply;
		zmqRecv(reply);
		if (reply.size() == 0) {
			// timeout
			m_error_code = RPC_ERR_RECV_TIMEOUT;
			val.set_code(RPC_ERR_RECV_TIMEOUT);
			val.set_msg("recv timeout");
			return val;
		}
		else {
			m_error_code = RPC_ERR_SUCCESS;
			ds.clear();
			ds.write_raw_data((char*)reply.data(), reply.size());
			ds.reset();

			ds >> val;
			return val;
		}
	}

private:
	std::unique_ptr<zmq::socket_t, std::function<void(zmq::socket_t*)>> m_socket;
	rpc_err_code	m_error_code;

private:
	zmq::context_t	m_context;

private:
	std::mutex		m_mutex;
};

#endif