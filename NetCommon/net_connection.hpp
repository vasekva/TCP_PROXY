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
				enum class owner
				{
					server,
					client
				};

				connection(owner parent, boost::asio::io_context &asioContext, boost::asio::ip::tcp::socket socket, ocl::net::tsqueue<owned_message<T>> &qIn)
					: m_asioContext(asioContext), m_socket(std::move(socket)), m_qMessagesIn(qIn)
				{
					m_nOwnerType = parent;
				}

				virtual ~connection() {}

				uint32_t GetID() const
				{
					return (id);
				}

			public:
				void ConnectToClient(uint32_t uid = 0)
				{
					if (m_nOwnerType == owner::server)
					{
						if (m_socket.is_open())
						{
							id = uid;
							ReadHeader();
						}
					}
				}

				void ConnectToServer(const boost::asio::ip::tcp::resolver::results_type &endpoints);
				void Disconnect()
				{
					if (IsConnected())
						boost::asio::post(m_asioContext, [this]() { m_socket.close(); });
				}
				bool IsConnected() const
				{
					return (m_socket.is_open());
				}

			public:
				void  Send(const message<T> &msg)
				{
					boost::asio::post(m_asioContext,
						[this, msg]()
						{
							bool bWritingMessage = !m_qMessagesOut.empty();
							m_qMessagesOut.push_back(msg);
							if (!bWritingMessage)
							{
								WriteHeader();
							}
						});
				}

			private:
				// ASYNC - Prime context ready to read a message header
				void ReadHeader()
				{
					boost::asio::async_read(m_socket, boost::asio::buffer(&m_msgTemporaryIn.header, sizeof(message_header<T>)),
						[this](std::error_code ec, std::size_t length)
						{
							if (!ec)
							{
								if (m_msgTemporaryIn.header.size > 0)
								{
									m_msgTemporaryIn.body.resize(m_msgTemporaryIn.header.size);
									ReadBody();
								}
								else
								{
									AddToIncomingMessageQueue();
								}
							}
							else
							{
								std::cout << "[" << id << "] Read Header Fail.\n";
								m_socket.close();
							}
						});
				}

				// ASYNC - Prime context ready to read a message body
				void ReadBody()
				{
					boost::asio::async_read(m_socket, boost::asio::buffer(m_msgTemporaryIn.body.data(), m_msgTemporaryIn.body.size()),
						[this](std::error_code ec, std::size_t length)
						{
							if (!ec)
							{
								AddToIncomingMessageQueue();
							}
							else
							{
								// As above!
								std::cout << "[" << id << "] Read body Fail.\n";
								m_socket.close();
							}
						});
				}

				// ASYNC - Prime context to write a message header
				void WriteHeader()
				{
					boost::asio::async_write(m_socket, boost::asio::buffer(&m_qMessagesOut.front().header, sizeof(message_header<T>)),
						[this](std::error_code ec, std::size_t length)
						{
							if (!ec)
							{
								if (m_qMessagesOut.front().body.size() > 0)
								{
									WriteBody();
								}
								else
								{
									m_qMessagesOut.pop_front();

									if (!m_qMessagesOut.empty())
									{
										WriteHeader();
									}
								}
							}
							else
							{
								std::cout << "[" << id << "] Write Header Fail.\n";
								m_socket.close();
							}
						});

				}

				// ASYNC - Prime context to write a message body
				void WriteBody()
				{
					boost::asio::async_write(m_socket, boost::asio::buffer(m_qMessagesOut.front().body.data(), m_msgTemporaryIn.body.size()),
						[this](std::error_code ec, std::size_t length)
						{
							if (!ec)
							{
								m_qMessagesOut.pop_front();

								if (!m_qMessagesOut.empty())
								{
									WriteHeader();
								}
							}
							else
							{
								std::cout << "[" << id << "] Write Body Fail.\n";
								m_socket.close();
							}
						});
				}

				void AddToIncomingMessageQueue()
				{
					if (m_nOwnerType == owner::server)
						m_qMessagesIn.push_back({ this->shared_from_this(), m_msgTemporaryIn });
					else
						m_qMessagesIn.push_back({ nullptr, m_msgTemporaryIn });

					ReadHeader();
				}

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
				ocl::net::tsqueue<owned_message<T>> &m_qMessagesIn;
				message<T> m_msgTemporaryIn;

				// The "owner" decides how some of the connection behaves
				owner m_nOwnerType = owner::server;
				uint32_t id = 0;
		};
	}
}