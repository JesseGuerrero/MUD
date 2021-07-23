#include "HostInfo.h"

using namespace std;

/* Global map were the connections are saved */
std::map<std::string,int> HostInfo::connections;
/* Mutex variable */
pthread_mutex_t HostInfo::mutex;

static string IntToString(int number) {
   stringstream ss;
   ss << number;
   return ss.str();
}

HostInfo::HostInfo(int remote_sock, const string& remote_ip_address, unsigned short int remote_port,
		const string& local_ip_address, unsigned short int local_port) :
         remote_sock(remote_sock),
         remote_ip_address(remote_ip_address),
         remote_port(remote_port),
         local_ip_address(local_ip_address),
         local_port(local_port) {

	/* Grab information of the host connected */
	string sremote_port = IntToString(this->remote_port);
	/* Grab information of the local host */
	string slocal_port = IntToString(this->local_port);

	/* Create a string of remote host information */
	remote_info = this->remote_ip_address + ":" + sremote_port;
	/* Create a string of local host information */
	local_info = "Server:" + slocal_port;

	/* Add the new connection information into the static map */
	pthread_mutex_lock(&mutex);
	connections[remote_info] = this->remote_sock;
	pthread_mutex_unlock(&mutex);
}

HostInfo::~HostInfo() {
	/* Remove the connection information from the static map */
	pthread_mutex_lock(&mutex);
	connections.erase(remote_info);
	pthread_mutex_unlock(&mutex);
}

