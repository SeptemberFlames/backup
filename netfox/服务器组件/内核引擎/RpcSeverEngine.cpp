#include "Stdafx.h"
#include "RpcSeverEngine.h"

//////////////////////////////////////////////////////////////////////////

inline CRpcServerEngine::CRpcServerEngine() : m_context(1) {

	//状态变量
	m_bService = false;

	//线程变量
	m_zmqMsgThread = NULL;

	//错误编码
	m_error_code = RPC_ERR_SUCCESS;
}

//析构函数
CRpcServerEngine::~CRpcServerEngine()
{
	//终止服务
	ConcludeService();

	return;
}

//启动服务
//ZMQ_REQ	 用于客户端发送请求消息，此类型的socket必须严格遵循先发送后接收的顺序
//ZMQ_REP	 用于服务端接收消息，此类型的socket必须严格遵循先接收后发送的顺序
//ZMQ_DEALER 是一种用于请求/答应模式的更高级的扩展Socket，它可以自由的收发消息，没有ZMQ_REP/ZMQ_REQ那样的限制
bool CRpcServerEngine::StartService()
{
	//状态变量
	m_bService = true;

	//通信句柄
	m_socket = std::unique_ptr<zmq::socket_t, std::function<void(zmq::socket_t*)>>(new zmq::socket_t(m_context, ZMQ_REP), [](zmq::socket_t* sock) { sock->close(); delete sock; sock = nullptr; });

	//监听端口
	ostringstream os;
	os << "tcp://*:" << PORT_RPC;
	m_socket->bind(os.str());

	//接收超时
	m_socket->setsockopt(ZMQ_RCVTIMEO, 2000);

	//接收线程
	m_zmqMsgThread = new std::thread(std::bind(&CRpcServerEngine::OnZmqMsgFunction, this));
	m_zmqMsgThread->detach();

	return true;
}

//停止服务
bool CRpcServerEngine::ConcludeService()
{
	//状态变量
	m_bService = false;
	return true;
}

//连接状态
bool CRpcServerEngine::zmqConnected()
{
	try
	{
		return m_socket->connected();
	}
	catch (...)
	{
		return false;
	}
}

//网络发送
bool CRpcServerEngine::zmqSend(zmq::message_t& data)
{
	try
	{
		return m_socket->send(data);
	}
	catch (...)
	{
		return false;
	}
}

//网路接收
bool CRpcServerEngine::zmqRecv(zmq::message_t& data)
{
	try
	{
		return m_socket->recv(&data);
	}
	catch (...)
	{
		return false;
	}
}

//调用回复
void CRpcServerEngine::callAndReply(std::string name, const char* data, int len)
{
	try
	{
		Serializer ds;
		if (m_handlers.find(name) == m_handlers.end()) {
			(ds) << value_t<int>::code_type(RPC_ERR_FUNCTIION_NOT_BIND);
			(ds) << value_t<int>::msg_type("function not bind: " + name);
		}
		else
		{
			auto fun = m_handlers[name];
			fun(&ds, data, len);
			ds.reset();
		}

		zmq::message_t retmsg(ds.size());
		memcpy(retmsg.data(), ds.data(), ds.size());
		zmqSend(retmsg);
	}
	catch (...)
	{

	}
}

//接收函数
void CRpcServerEngine::OnZmqMsgFunction()
{
	while (true)
	{
		//处理完毕
		if (!m_bService) {
			break;
		}

		try
		{
			//读取数据
			zmq::message_t data;

			//接收数据
			if (zmqRecv(data))
			{
				//数据入队
				StreamBuffer iodev((char*)data.data(), data.size());
				if (iodev.size() > 0)
				{
					//变量准备
					std::string funname;

					//数据解析
					Serializer ds(iodev);
					ds >> funname;

					//函数判断
					if (funname != "")
					{
						//调用函数
						callAndReply(funname, ds.current(), ds.size() - funname.size());
					}
					else
					{
						CTraceService::TraceString(TEXT("远程调用服务器接收到空函数"), TraceLevel_Exception);
					}
				}
			}
		}
		catch (...)
		{

		}
	}

	//关闭网络
	if (m_socket) {
		m_socket->close();
	}

	//调试信息
	//CTraceService::TraceString(TEXT("消息接收线程退出"), TraceLevel_Normal);
}