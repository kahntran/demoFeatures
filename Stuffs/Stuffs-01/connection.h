
#ifndef _CONNECTION_H
#define _CONNECTION_H


#define DLL_EXPORT
#ifdef DLL_EXPORT
#define DllExport   __declspec( dllexport )
#else
#define DllExport
#endif

//INCLUDE
#include<list>
#include<string.h>
#include<stdlib.h>

#include<sstream>
#include<iostream>

#include<WinSock2.h>
#include<Windows.h>

#pragma comment(lib,"ws2_32.lib")


//DEFINE
#define DEFAULT_PORT		3000
#define DEFAULT_DATA		"\\serverdata"
#define DEFAULT_PROC_COUNT	2


/* This structure contains infomation
 * about SOCKET and name of client
 * socket_
 * hten_
 */
struct SOCKET_CLIENT
{
	char* hten_;
	SOCKET socket_;

	bool friend operator == (const SOCKET_CLIENT& socket1, const SOCKET_CLIENT& socket2)
	{
		return socket1.hten_ == socket2.hten_ && socket1.socket_ == socket2.socket_;
	}
};



/* Contains requier from client 
 *
 */
enum Handle_Requier
{
	ADD_BIRD = 1000,
	ADD_SIGHTING,
	LIST_BIRD,
	LIST_SIGHTING,
	SERVER_QUIT,
	REMOVE_BIRD,
	HANDLE_ERRO,
	HANDLE_SUCCESS
};


/* Contain infomation of a bird
 *
 */
struct Bird
{
	char	name_[30];
	char	color_[30];
	float	weight_;
	float	height_;

	void print_Bird (char* title = "Title")
	{
		printf ("%s Name bird: %s\n", title, name_);
		printf ("%s Color bird: %s\n", title, color_);
		printf ("%s Weight bird: %.2f\n", title, weight_);
		printf ("%s Height bird: %.2f\n", title, height_);
	}
	bool friend operator == (const Bird& bird1, const Bird& bird2)
	{
		return	bird1.name_ == bird2.name_ && bird1.color_ == bird2.color_ &&
				bird1.weight_ == bird2.weight_ && bird1.height_ == bird2.height_;
	}
};


/* Contain infomation of a date
 * include date, month, year
 */
struct Dates
{
	unsigned short	date_;
	unsigned short	month_;
	unsigned int	year_;

	bool friend operator == (const Dates& date1, const Dates& date2) 
	{
		return date1.date_ == date2.date_ && date1.month_ == date2.month_ && date1.year_ == date2.year_;
	}
};


/* contain infomation of a hour
 * include hour and minute
 */
struct Hour
{
	unsigned short	hour_;
	unsigned short	minute_;

	bool friend operator == (const Hour& hour1, const Hour& hour2) 
	{
		return hour1.hour_ == hour2.hour_ && hour1.minute_ == hour2.minute_;
	}
};


/* Contain infomation of time
 * include Date and Time
 */
struct Time
{
	Dates	date_;
	Hour	hour_;

	void printf_time (char* title)
	{
		printf ("%s Time %u/%u/%u - %uh:%u\n", title, date_.date_, date_.month_, date_.year_, hour_.hour_, hour_.minute_);
	}

	friend std::ostringstream&  operator<<(std::ostringstream& os, const Time& time)
	{
		os << time.date_.date_ << "/" << time.date_.month_ << "/" << time.date_.year_ << " - " << time.hour_.hour_ << ":" << time.hour_.minute_;
		return os;
	}

	bool friend operator == (const Time& time1, const Time& time2)
	{
		return time1.date_ == time2.date_ && time1.hour_ == time2.hour_;
	}
};


/* Contain info mation of a bird and 
 * location which sighing a bird at what time
 */
struct Sighting
{
	char name_[30];
	char location_[30];
	Time time_;
	void print_sighting (char* title = "Title")
	{
		printf ("%s Name: %s\n",		title, name_);
		printf ("%s Location: %s\n",	title, location_);
		time_.printf_time(title);
	}
	bool friend operator==(const Sighting& sighting1, const Sighting& sighting2)
	{
		return	sighting1.name_ == sighting2.name_ && sighting1.location_ == sighting2.location_ &&
				sighting1.time_ == sighting2.time_;
	}
};


struct Re_Expression_Sighting
{
	char	name_[30];
	Dates	date_start_;
	Dates	date_end_;
};

/* Contain reequier from client and messege  
 * 
 */
struct RectRequire
{
	int			requier_;
	Bird		bird_;
	Sighting	sighting_;
	Re_Expression_Sighting re_expression_;
};

/* Initialize Connection 
 * 
 */
class DllExport Connection
{
public:
	Connection();
	~Connection();

	/*
	 *
	 */
	bool	initializeConnection(void);

	/*
	 *
	 */
	int		sendMessagePort(SOCKET, char*, int);

	/*
	 *
	 */
	int		rectMessagePort(SOCKET_CLIENT*, RectRequire&);

	/*
	 *
	*/
	void	quit(void);

	/*
	 *
	*/
	char*			get_IP();

	/*
	 *
	*/
	void			set_IP(char*);

	/*
	 *
	*/
	unsigned short	get_Server_Port(void);

	/*
	 *
	*/
	void			set_Server_Port(unsigned short);

	/*
	 *
	*/
	std::string		get_Data(void);

	/*
	 *
	*/
	void			set_Data(std::string);

	/*
	 *
	*/
	unsigned short	get_Proc_Count(void);

	/*
	 *
	*/
	void			set_Proc_Count(unsigned short);

	SOCKET			get_Listen_Socket();
	SOCKET			server_Socket_;
	std::list<SOCKET_CLIENT> ClientList_;
protected:

private:	
	char*			buffer_[1024];
	char*			ip_address_;
	char*			server_name_;
	unsigned short	server_port_;
	unsigned short	proc_count_;

	std::string		data_location_;

	WORD			version_;
	SOCKET			client_Socket_;
	
	WSADATA			wsaData_;
	sockaddr_in		local_,  from_;
};
#endif //_CONNECTION_H