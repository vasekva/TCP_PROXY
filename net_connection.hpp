#pragma once

#include "net_common.hpp"
#include "net_tsqueue.hpp"
#include "net_message.hpp"

namespace olc
{
	namespace net
	{
		template <typename T>
		class connection : public std::enable_shared_from_this<connection<T>>
		{
			public:
				connection() {}
				virtual ~connection() {}

			public:
				bool ConnectToServer();
				bool Disconnect();
				bool IsConnected() const;

			public:
				bool  Send(const message<T> &msg);

			protected:
				// Each connection has a unique socket to a remote
				boost::asio::ip::tcp::socket m_socket;

				// This context is shared with the whole asio instance
				boost::asio::io_context &m_asioContext;

				// This queue holds all messages to be sent to the remote side
				// of this connection
				ocl::net::tsqueue<message<T>> m_qMessagesOut;

				// This queue holds all messages that have been received from
				// the remote side of this connection. Note it is a reference
				// as the "owner" of this connection is expected to provide a queue
				ocl::net::tsqueue<owned_message> &m_qMessagesIn;
		};
	}
}