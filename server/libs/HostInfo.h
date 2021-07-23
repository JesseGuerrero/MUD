#ifndef HOSTINFO_H_
#define HOSTINFO_H_

#include <iostream>
#include <sstream>
#include <map>
#include <pthread.h>

struct HostInfo {
	/* Map of connections to the server */
	static std::map<std::string,int> connections;
	/* Mutex variable */
	static pthread_mutex_t mutex;

	/* Socket descriptor of remote host */
	int remote_sock;
	/* Remote host IP address */
	std::string remote_ip_address;
	/* Remote host Port */
	unsigned short int remote_port;
	/* Local host IP address */
	std::string local_ip_address;
	/* Local host Port */
	unsigned short int local_port;

	/* Remote host information (IP+port) */
	std::string remote_info;
	/* Local host information (IP+port) */
	std::string local_info;

	HostInfo(int remote_sock, const std::string& remote_ip_address, unsigned short int remote_port,
			const std::string& local_ip_address, unsigned short int local_port);

	~HostInfo();
};


#endif /* HOSTINFO_H_ */
