#pragma once

#include "net_common.hpp"
#include "net_tsqueue.hpp"
#include "net_message.hpp"
#include "net_connection.hpp"

namespace olc
{
	namespace net
	{
		template <typename T>
		class server_interface
		{
			public:

				// second parameter is the address upon which the server will listen to connections
				server_interface(uint16_t port)
					: m_asioAcceptor(m_asioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
				{

				}

				virtual ~server_interface()
				{
					Stop();
				}

				bool Start()
				{
					try
					{
						WaitForClientConnection();

						m_threadContext = std::thread([this]() {m_asioContext.run(); });
					}
					catch (std::exception& e)
					{
						// Something prohibited the server from listening
						std::cerr << "[SERVER] Exception: " << e.what() << "\n";
						return (false);
					}

					std::cout << "[SERVER] Started!\n";
					return (true);
				}

				void Stop()
				{
					// Request the context to close
					m_asioContext.stop();

					// Tidy up the context thread
					if (m_threadContext.joinable()) m_threadContext.join();

					// Inform someone, anybody, if the care...
					std::cout << "[SERVER] Stopped!\n";
				}

				// ASYNC - Instruct asio to wait for connection
				void WaitForClientConnection()
				{
					m_asioAcceptor.async_accept(
						[this](std::error_code ec, boost::asio::ip::tcp::socket socket)
						{
							if (!ec)
							{
								std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";

								std::shared_ptr<connection<T>> newconn =
									std::make_shared<connection<T>>(connection<T>::owner::server,
										m_asioContext, std::move(socket), m_qMessagesIn);

								// Give the user server a chance to deny connection
								if (OnClientConnect(newconn))
								{
									 // Connection allowed, so add to container of new conenctions
									m_deqConnections.push_back(std::move(newconn));

									m_deqConnections.back()->ConnectToClient(nIDCounter++);

									std::cout << "[" << m_deqConnections.back()->GetID() << "] Connection Approved\n";
								}
								else
								{
									std::cout << "[-----] Connection Denied\n";
								}
							}
							else
							{
								// Error has occured during acceptance
								std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
							}

							// Prime the asio context with more work - again simply wait for
							// another connection..
							WaitForClientConnection();
						});
				}

				// Send a message to a specific client
				void MessageClient(std::shared_ptr <connection<T>> client, const message<T> &msg)
				{
					if (client && client->IsConnected())
					{
						client->Send(msg);
					}
					else
					{
						OnClientDisconnect(client);
						client.reset();
						m_deqConnections.erase(
							std::remove(m_deqConnections.begin(), m_deqConnections.end(), client), m_deqConnections.end());
					}
				}

				// Send message to all clients
				void MessageAllClients(const message<T>& msg, std::shared_ptr<connection<T>> pIgnoreClient = nullptr)
				{
					bool bInvalidClientExists = false;

					for (auto& client : m_deqConnections)
					{
						// Check client is connected...
						if (client && client->IsConnected())
						{
							// ..it is!
							if (client != pIgnoreClient)
								client->Send(msg);
						}
						else
						{
							// The client couldn't be contacted, so assume it 
							// has disconnected
							OnClientDisconnect(client);
							client.reset();
							bInvalidClientExists = true;
						}
					}

					if (bInvalidClientExists)
					{
						// TODO: check last parameter
						m_deqConnections.erase(std::remove(m_deqConnections.begin(), m_deqConnections.end(), nullptr), m_deqConnections.end());
					}
				}

				void Update(size_t nMaxMessages = -1)
				{
					size_t nMessageCount = 0;
					while (nMessageCount < nMaxMessages && !m_qMessagesIn.empty())
					{
						// Grab the front message
						auto msg = m_qMessagesIn.pop_front();

						// Pass to message handler
						OnMessage(msg.remote, msg.msg);

						nMessageCount++;
					}
				}

			protected:

				// Called when a client connects, you can veto the connection by returning false
				virtual bool OnClientConnect(std::shared_ptr<connection<T>> client)
				{
					return (false);
				}

				// Called when a client appears to have disconnected
				virtual void OnClientDisconnect(std::shared_ptr<connection<T>> client)
				{
				}

				// Called when a message arrives
				virtual void OnMessage(std::shared_ptr<connection<T>> client, message<T>& msg)
				{

				}

			protected:
				// Thread Safe Queue for incoming message packets
				ocl::net::tsqueue<owned_message<T>>		m_qMessagesIn;

				// Container of active validated connections
				std::deque<std::shared_ptr<connection<T>>> m_deqConnections;

				// Order of declaration is important - it's also the order of initialisation
				boost::asio::io_context			m_asioContext;
				std::thread						m_threadContext;

				// These things need an asio context
				boost::asio::ip::tcp::acceptor	m_asioAcceptor;

				// Clients will be identified in the "wider system" via an ID
				uint32_t nIDCounter = 10000;
		};


		template <typename T>
		class client_interface
		{
			public:

				client_interface() : m_socket(m_context)
				{
					// Initialise the socket with the io context, so it can do stuff
				}

				virtual ~client_interface()
				{
					// If the client is destroyed, always try and disconnect from server
					Disconnect();
				}

			public:
				// Connect to server with hostname/ip-address and port
				bool Connect(const std::string &host, const uint16_t port)
				{
					try
					{
						// Create connection
						m_connection = std::make_unique<connection<T>>(); // TODO

						// Resolve hostname/ip-address into tangible physical address
						boost::asio::ip::tcp::resolver					resolver(m_context);
						boost::asio::ip::tcp::resolver::results_type	endpoints
							= resolver.resolve(host, std::to_string(port));

						// Tell the connection object to connect to server
						m_connection->ConnectToServer(endpoints);

						// Start Context Thread
						thrContext = std::thread([this]() { m_context.run(); });
					}
					catch (std::exception &e)
					{
						std::cerr << "Client Exception: " << e.what() << "\n";
						return (false);
					}

					return (true);
				}

				// Disconnect from server
				void Disconnect()
				{
					// If connection exists, and it's connected then...
					if (IsConnected())
					{
						// ...disconnect from server gracefully
						m_connection->Disconnect();
					}

					// Either way, we're also done with the asio context...
					m_context.stop();
					// ...and its thread
					if (thrContext.joinable())
						thrContext.join();

					// Destroy the connection object
					m_connection.release();
				}

				// Check if client is actually connected to a server
				bool IsConnected()
				{
					if (m_connection)
						return (m_connection->IsConnected());
					else
						return (false);
				}

				// Retrieve queue of messages from server
				ocl::net::tsqueue<owned_message<T>> &Incoming()
				{
					return (m_qMessagesIn);
				}

			protected:
				// asio context handles the data transfer...
				boost::asio::io_context m_context;
				// ...but needs a thread of its own to execute its work commands
				std::thread thrContext;
				// This is the hardware socket that is connected to the server
				boost::asio::ip::tcp::socket m_socket;
				// The client has a single instance of a "connection" object, which handles data transfer
				std::unique_ptr<connection<T>> m_connection;

			private:
				// This is the thread safe queue of incoming messages from server
				ocl::net::tsqueue<owned_message<T>> m_qMessagesIn;
		};
	}
}