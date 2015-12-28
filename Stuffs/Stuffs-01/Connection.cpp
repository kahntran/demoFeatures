/////////INCLUDE/////////////////////
#include"connection.h"
#include"data.h"


////////SETUP////////////////////////
Connection::Connection()
{
	WCHAR path[MAX_PATH];
	char* str = new char[(wcslen(path))];
	SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, path));
	wcstombs(str, path, sizeof(path));
	data_location_ = str;
	free(str);
	server_port_ = DEFAULT_PORT;
	proc_count_ = DEFAULT_PROC_COUNT;

	server_name_ = "localhost";
	ip_address_ = "127.0.0.1";
	version_ = MAKEWORD(2,2);

}

Connection::~Connection()
{
	if (server_Socket_ != INVALID_SOCKET)
		closesocket (server_Socket_);
	WSACleanup();
}

char*			Connection::get_IP()
{
	return ip_address_;
}

void			Connection:: set_IP(char* ip_address)
{
	ip_address_ = ip_address;
}

unsigned short	Connection::get_Server_Port(void)
{
	return server_port_;
}
void			Connection::set_Server_Port(unsigned short port)
{
	server_port_ = port;
}

std::string		Connection::get_Data(void)
{
	return data_location_;
}

void			Connection::set_Data(std::string data)
{
	data_location_ = data;
}

unsigned short	Connection::get_Proc_Count(void)
{
	return proc_count_;
}
void			Connection::set_Proc_Count(unsigned short proc_count)
{
	proc_count_ = proc_count;
}

SOCKET			Connection::get_Listen_Socket()
{
	return server_Socket_;
}

bool			Connection::initializeConnection (void)
{
	int wsaerr;
	int result;
	printf ("Server: Begin startup connection.........\n");
	wsaerr = WSAStartup(version_, &wsaData_);
	if (wsaerr != 0)
	{
		printf ("Server: WSAStartup() failed with error %d\n", wsaerr);
		WSACleanup();
		return false;
	}
	else
		printf ("Server: WSAStartup() is ---- OK\n");
	server_Socket_ = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_Socket_ == INVALID_SOCKET)
	{
		printf ("Server: Socket() failed with erro %d\n", WSAGetLastError());
		WSACleanup();
		return false;
	}
	else
	{
		printf ("Server: Socket() is  ---- OK\n");
	}
	local_.sin_family = AF_INET;
	local_.sin_addr.s_addr = INADDR_ANY;//inet_addr(ip_address);
	local_.sin_port = htons(server_port_);
	result = bind (server_Socket_, (SOCKADDR*)&local_, sizeof(local_));
	if (result == SOCKET_ERROR)
	{
		printf ("Server: Bind() failed with erro %d\n", WSAGetLastError());
		closesocket (server_Socket_);
		return false;
	}
	else
	{
		printf ("Server: Bind() is ---- OK\n");
	}
	result = listen (server_Socket_, 5);
	if (result == SOCKET_ERROR)
	{
		printf ("Server: listen() failed with erro %d\n", WSAGetLastError());
		WSACleanup();
		return false;
	}
	else
	{
		printf ("Server: listen() is ---- OK\n");
	}
	return true;
}

int				Connection::sendMessagePort(SOCKET send_socket,char* message,int lenght)
{
	//std::list<Bird> b;
	//memcpy(&b, message, lenght);
	int iStat = 0;
	if (send_socket == NULL)
		return 0;
	iStat = send(send_socket, message, lenght, 0);
	if (iStat == -1)
		return 1;
	return 0;
}
int				Connection::rectMessagePort(SOCKET_CLIENT* socket, RectRequire& requier)
{
	char	temp[4096];
	int		iStat;
	iStat = recv(socket->socket_, temp, 4096, 0);
	send(socket->socket_, "OK", sizeof("OK"), 0);
 	if (iStat == -1)
	{
		closesocket(socket->socket_);
		return 1;
	}
	else
	{
		int sequier = atoi(temp);
		requier.requier_ = sequier;
		if (sequier == ADD_BIRD)
		{
			char *rect_client = new char[sizeof(Bird)];
			iStat = recv(socket->socket_, rect_client, sizeof(Bird), 0);
			if (iStat == -1)
			{
				closesocket(socket->socket_);
				return 1;
			}
			else
			{
				memcpy(&requier.bird_, rect_client, sizeof(Bird));
				
			}
			
		}
		else if (sequier == ADD_SIGHTING)
		{
			char *rect_client = new char[sizeof(Sighting)];
			iStat = recv(socket->socket_, rect_client, sizeof(Sighting), 0);
			if (iStat == -1)
			{
				closesocket(socket->socket_);
				return 1;
			}
			else
			{
				memcpy(&requier.sighting_, rect_client, sizeof(Sighting));
				
			}
		}
		else if (sequier == REMOVE_BIRD)
		{
			char *str = new char[200];
			iStat = recv(socket->socket_, str, 200, 0);
			if (iStat == -1)
			{
				closesocket(socket->socket_);
				return 1;
			}
			else
			{
				strcpy(requier.bird_.name_, str);
				
			}
		}
		else if (sequier == LIST_BIRD)
		{
			char *str = new char[200];
			iStat = recv(socket->socket_, str, 200, 0);
			if (iStat == -1)
			{
				closesocket(socket->socket_);
				return 1;
			}
			if (str != 0)
				free(str);
		}
		else if (sequier == LIST_SIGHTING)
		{
			char *str = new char[200];
			iStat = recv(socket->socket_, str, 200, 0);
			if (iStat == -1)
			{
				closesocket(socket->socket_);
				return 1;
			}
			else
			{
				memcpy(&requier.re_expression_, str, sizeof(Re_Expression_Sighting));
			}
		}
	}
	return 0;
}
