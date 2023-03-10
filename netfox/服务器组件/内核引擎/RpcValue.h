
#ifndef RPC_VALUE_H_
#define RPC_VALUE_H_

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
#include <mutex>
#include <queue>
#include <thread>

enum rpc_err_code
{
	RPC_ERR_SUCCESS = 0,
	RPC_ERR_NOT_CONNECTD,
	RPC_ERR_FUNCTIION_NOT_BIND,
	RPC_ERR_RECV_TIMEOUT
};

//返回模板
template<typename T>
struct type_xx { typedef T type; };

template<>
struct type_xx<void> { typedef int8_t type; };

//返回模板
template<typename T>
class value_t {
public:
	typedef typename type_xx<T>::type type;
	typedef std::string msg_type;
	typedef uint16_t code_type;

	value_t() { code_ = 0; msg_.clear(); }
	bool valid() { return (code_ == 0 ? true : false); }
	int error_code() { return code_; }
	std::string error_msg() { return msg_; }
	type val() { return val_; }

	void set_val(const type& val) { val_ = val; }
	void set_code(code_type code) { code_ = code; }
	void set_msg(msg_type msg) { msg_ = msg; }

	friend Serializer& operator >> (Serializer& in, value_t<T>& d) {
		in >> d.code_ >> d.msg_;
		if (d.code_ == 0) {
			in >> d.val_;
		}
		return in;
	}
	friend Serializer& operator << (Serializer& out, value_t<T> d) {
		out << d.code_ << d.msg_ << d.val_;
		return out;
	}
private:
	code_type code_;
	msg_type msg_;
	type val_;
};

#endif
