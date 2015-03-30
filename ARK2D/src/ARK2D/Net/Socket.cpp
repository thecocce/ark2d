/*
 * Socket.cpp
 *
 *  Created on: 28 March 2015
 *      Author: Ashley
 */

#include "Includes.h"
#include "Socket.h"
#include "Address.h"
#include "../ARK2D.h"
#include "../Util/Log.h"

namespace ARK {
	namespace Net { 

		bool Socket::initializeSockets()
		{
			#if defined(ARK2D_WINDOWS)
		    	WSADATA WsaData;
				return WSAStartup( MAKEWORD(2,2), &WsaData ) != NO_ERROR;
			#else
				return true;
			#endif
		}

		void Socket::shutdownSockets()
		{
			#if defined(ARK2D_WINDOWS)
				WSACleanup();
			#endif
		}

		Socket::Socket():
			m_socket(0)
		{

		}
	
		bool Socket::open( unsigned short port )
		{
			assert( !isOpen() );
		
			// create socket
			m_socket = ::socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

			if ( m_socket <= 0 )
			{
				ARK2D::getLog()->e( "failed to create socket" );
				m_socket = 0;
				return false;
			}

			// bind to port
			sockaddr_in address;
			address.sin_family = AF_INET;
			address.sin_addr.s_addr = INADDR_ANY;
			address.sin_port = htons( (unsigned short) port );
		
			if ( ::bind( (unsigned int) m_socket, (const sockaddr*) &address, sizeof(sockaddr_in) ) < 0 )
			{
				ARK2D::getLog()->e( "failed to bind socket" );
				close();
				return false;
			}

			// set non-blocking io
			#if defined(ARK2D_MACINTOSH) || defined(ARK2D_UBUNTU_LINUX)
		
				int nonBlocking = 1;
				if ( fcntl( m_socket, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 )
				{
					ARK2D::getLog()->e( "failed to set non-blocking socket" );
					close();
					return false;
				}
			
			#elif defined(ARK2D_WINDOWS)
		
				DWORD nonBlocking = 1;
				if ( ioctlsocket( m_socket, FIONBIO, &nonBlocking ) != 0 )
				{
					ARK2D::getLog()->e( "failed to set non-blocking socket" );
					close();
					return false;
				}

			#endif
		
			return true;
		}
	
		void Socket::close()
		{
			if (m_socket != 0)
			{
				#if defined(ARK2D_MACINTOSH) || defined(ARK2D_UBUNTU_LINUX)
					::close( m_socket );
				#elif defined(ARK2D_WINDOWS)
					closesocket( m_socket );
				#endif
				m_socket = 0;
			}
		}
	
		bool Socket::isOpen() const {
			return m_socket != 0;
		}
	
		bool Socket::send( const Address& destination, const void* data, int size )
		{
			assert( data );
			assert( size > 0 );
		
			if ( m_socket == 0 ) {
				return false;
			}
		
			assert( destination.getAddress() != 0 );
			assert( destination.getPort() != 0 );
		
			sockaddr_in address;
			address.sin_family = AF_INET;
			address.sin_addr.s_addr = htonl( destination.getAddress() );
			address.sin_port = htons( (unsigned short) destination.getPort() );

			int sent_bytes = sendto( m_socket, (const char*)data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in) );

			return sent_bytes == size;
		}
	
		int Socket::receive( Address & sender, void * data, int size )
		{
			assert( data );
			assert( size > 0 );
		
			if ( socket == 0 ) { 
				return false;
			}
			
			#if defined(ARK2D_WINDOWS)
			typedef int socklen_t;
			#endif
			
			sockaddr_in from;
			socklen_t fromLength = sizeof( from );

			int received_bytes = recvfrom( m_socket, (char*)data, size, 0, (sockaddr*)&from, &fromLength );

			if ( received_bytes <= 0 )
				return 0;

			unsigned int address = ntohl( from.sin_addr.s_addr );
			unsigned short port = ntohs( from.sin_port );

			sender = Address( address, port );

			return received_bytes;
		}

		Socket::~Socket() 
		{
			close();
		}
	}
}
