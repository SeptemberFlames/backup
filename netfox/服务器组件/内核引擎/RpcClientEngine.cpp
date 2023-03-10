#include "Stdafx.h"
#include "RpcClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//构造函数
CRpcClientEngine::CRpcClientEngine() : m_context(1)
{
	m_error_code = RPC_ERR_SUCCESS;
}

//析构函数
CRpcClientEngine::~CRpcClientEngine()
{
	//关闭网络
	if (m_socket) m_socket->close();

	//关闭关联
	m_context.close();
}

//开始连接
//ZMQ_REQ	 用于客户端发送请求消息，此类型的socket必须严格遵循先发送后接收的顺序
//ZMQ_REP	 用于服务端接收消息，此类型的socket必须严格遵循先接收后发送的顺序
//ZMQ_DEALER 是一种用于请求/答应模式的更高级的扩展Socket，它可以自由的收发消息，没有ZMQ_REP/ZMQ_REQ那样的限制
void CRpcClientEngine::connect(std::string ip, int port)
{
	m_socket = std::unique_ptr<zmq::socket_t, std::function<void(zmq::socket_t*)>>(new zmq::socket_t(m_context, ZMQ_REQ), [](zmq::socket_t* sock) { sock->close(); delete sock; sock = nullptr; });
	ostringstream os;
	os << "tcp://" << ip << ":" << port;
	m_socket->connect(os.str());
}

//设置超时, ZMQ_RCVTIMEO, ZMQ_SNDTIMEO
void CRpcClientEngine::set_timeout(int option, int ms)
{
	m_socket->setsockopt(option, ms);
}

//连接状态
bool CRpcClientEngine::zmqConnected()
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
bool CRpcClientEngine::zmqSend(zmq::message_t& data)
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
bool CRpcClientEngine::zmqRecv(zmq::message_t& data)
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
