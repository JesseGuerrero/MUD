#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <getopt.h>

#include <fcntl.h>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "libs/ConnectHandler.h"
#include "libs/StdinHandler.h"
#include "libs/IPv4Parse.h"

using namespace std;

/* How many pending connections will be hold */
const int backlog = 10;

/* Some constant */
const int noargument = 0;
const int requiredargument = 1;

static void print_usage (ostream& stream, const string& program_name, int exit_code) {
	stream << "Usage: " << program_name << " -p <port> [-a <ip>]" << endl;
	stream << "  -a  --allow <ip>   Allow the IPs to send data to the server" << endl;
	stream << "  -p  --port  <port> Listening port" << endl;
	stream << "Examples: " << endl;
	stream << program_name << " -p 1234 " << endl;
	stream << program_name << " -p 1234 -a 192.168.1.1-8 " << endl;
	stream << program_name << " -p 1234 -a 192.168.1.* " << endl;
	stream << program_name << " -p 1234 -a 192.168.1.1,4,19 " << endl;
	exit (exit_code);
}

int main(int argc, char** argv){
	/* Flag is 1 if the listening port is set */
	int listen_port = 0;
	/* Listening port */
    int host_port;
    /* Allowed IP addresses */
    string ip_allowed = "All";
    /* Get the program name */
    string program_name = string(argv[0]);

    while (1) {
		static struct option long_options[] =
		  {
			/* These options don't set a flag.
			   We distinguish them by their indices. */
			{"allow",   requiredargument, 0, 'a'},
			{"port",    requiredargument, 0, 'p'},
			{0, 0, 0, 0}
		  };

		/* Getopt_long stores the option index here. */
		int option_index = 0;

		int c = getopt_long (argc, argv, "a:p:",
							 long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
		  break;

		switch (c) {

		  case 'a':
			ip_allowed = string(optarg);
			break;

		  case 'p':
		    host_port=atoi(optarg);
		    listen_port = 1;
			break;

		  default:
			print_usage(cerr,program_name,1);
			break;
		  }
      }

    if(!listen_port) {
    	cerr << "[@] ERROR: Missing port number" << endl;
		print_usage(cerr,program_name,1);
    }

    /* Print some general information */
    cout << "[%] SIMPLE ECHO SERVER" << endl;
    cout << "[%] Listening on port : " << host_port << endl;
    cout << "[%] Set of allowed IPs : " << ip_allowed << endl;

    /* Get the set of allowed IPs */
    vector<string>* allowed_ips = ParseIP(ip_allowed);

    /* Local listening socket */
    int local_sock = socket(AF_INET, SOCK_STREAM, 0);

    if(local_sock == -1){
    	perror("[@] Error initializing server socket: ");
    	exit(1);
    }

    /* Generic integer */
    int *p_int = new int;
    *p_int = 1;

    /* Set sockets options */
    if( (setsockopt(local_sock, SOL_SOCKET, SO_REUSEADDR, p_int, sizeof(int)) == -1 )||
        (setsockopt(local_sock, SOL_SOCKET, SO_KEEPALIVE, p_int, sizeof(int)) == -1 ) ){
    	perror("[@] Error setting server socket options: ");
        delete p_int;
    	exit(1);
    }

    delete p_int;

    /* Local Address structure */
    sockaddr_in my_addr;

    my_addr.sin_family = AF_INET;                           /* Set socket family */
    my_addr.sin_port = htons(host_port);                    /* Set port */
    my_addr.sin_addr.s_addr = INADDR_ANY ;                  /* Set local IP address */
    memset(my_addr.sin_zero, 0, sizeof(my_addr.sin_zero));  /* Fill with zeros */

    /* Bind socket */
    if(bind(local_sock, (sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
    	perror("[@] Error binding socket : ");
    	exit(1);
    }

    /* Listen */
    if(listen(local_sock, backlog) == -1 ){
    	perror("[@] Error listening socket : ");
    	exit(1);
    }

    /* Initialize mutex variable */
    pthread_mutex_init(&HostInfo::mutex, NULL);

    /* Spawn thread for handle stdin events */
    pthread_t thread_stdin;
    pthread_create(&thread_stdin, 0, &StdinHandler, 0);

    /* Now lets do the server stuff */
    while(true) {

        cout << "[#] Waiting for a connection..." << endl;

        socklen_t addr_size = sizeof(sockaddr_in);
        /* Remote socket descriptor */
        int remote_sock;
        /* Remote Address structure */
        sockaddr_in remote_addr;
        /* Thread ID */
        pthread_t thread_id;

        if(( remote_sock = accept(local_sock, (sockaddr*)&remote_addr, &addr_size))!= -1 ) {
        	/* Get IP address of the host connected */
        	string remote_ip_address(inet_ntoa(remote_addr.sin_addr));
        	/* Get the port of the remote host */
        	unsigned short int remote_port = ntohs(remote_addr.sin_port);
        	/* Get IP address of the local host */
        	string local_ip_address(inet_ntoa(my_addr.sin_addr));
        	/* Get the port of the local host */
        	unsigned short int local_port = ntohs(my_addr.sin_port);
        	/* Print some information */
        	cout << "[#] Received connection from: " << remote_ip_address << ":" << remote_port << endl;

        	/* Check if the IP is allowed to connect */
        	vector<string>::iterator it_ip = find(allowed_ips->begin(), allowed_ips->end(), remote_ip_address);

        	/* If allowed... */
        	if(true/*it_ip != allowed_ips->end()*/) {
				/* New host info structure */
				HostInfo *host_info = new HostInfo(remote_sock, remote_ip_address, remote_port, local_ip_address, local_port);
				/* Cast to void */
				void* thread_arg = static_cast<void*>(host_info);

				/* Spawn thread */
				pthread_create(&thread_id, 0, &ConnectHandler, thread_arg);

				/* Detach thread */
				pthread_detach(thread_id);
        	} /* If not... */ else {
        		/* Number of bytes sent */
        		int sendcount;
        		/* Message */
        		string data = "Server:Your IP is " + remote_ip_address + " and is not allowed to connect here. Goodbye.\n";
        		/* Send the data back to the client */
        		if((sendcount = send(remote_sock, data.c_str(), data.size(), 0))== -1){
        			string message = "[@] Error sending data to client " + remote_ip_address;
        			perror(message.c_str());
        		}
        		close(remote_sock);
        	}
        }
        else
            perror("[@] Error accepting connection: ");

    }

}
