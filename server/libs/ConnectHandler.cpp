#include "ConnectHandler.h"
#include <stdio.h>
#include <string.h>
#include "../World.h"
#include "../Player.h"

using namespace std;

void decode(char *packetID, HostInfo* host_info, string data_rcv);

void* ConnectHandler(void* thread_arg) {
	/* Get information of the proxy connection */
	HostInfo* host_info = static_cast<HostInfo*>(thread_arg);

	/* Get the socket descriptor of the remote host */
	int remote_sock = host_info->remote_sock;

	/* Grab information of the local and remote hosts */
	string remote_info = host_info->remote_info;
	string local_info = host_info->local_info;

    /* Buffer for receiving data */
    const int buffer_len = 65536;
    char* buffer = new char[buffer_len];

    /* Set the buffer to zero */
    memset(buffer, 0, buffer_len);

    /* Count of bytes received */
    int bytecount;


    /* Additional */
    string info = local_info + "->" + remote_info + ":";
    /* History of commands */
    string history = "";

    while(1) {

		/* Set the buffer to zero */
		memset(buffer, 0, buffer_len);

		/* Receive data */
		bytecount = recv(remote_sock, buffer, buffer_len, 0);
		if(bytecount == -1){
			string message = "[@] Error receiving data from host " + remote_info;
			perror(message.c_str());
			delete [] buffer;
			delete host_info;
			return 0;
		} else if( bytecount == 0) {
			cout << "[@] Connection closed from " + remote_info << endl;
			delete [] buffer;
			delete host_info;
			return 0;
		}

		/* Data received */
		string data_rcv = string(buffer);

		//Get Packet ID
        char * buf = strdup(data_rcv.c_str());
		char *packetID = strtok(buf, "-");
        cout << "" + (string)packetID << endl;
		data_rcv.erase (data_rcv.begin(), data_rcv.begin() + strlen(packetID) + 1);

		decode(packetID, host_info, data_rcv);
    }

    return 0;
}

void decode(char *packetID, HostInfo* host_info, string data_rcv) {
    switch(atoi(packetID)) {
        case 0:
            World::addPlayer(data_rcv);
            break;
        case 1:
            break;
    }

    /* Get the socket descriptor of the remote host */
    int remote_sock = host_info->remote_sock;

    /* Grab information of the local and remote hosts */
    string remote_info = host_info->remote_info;
    string local_info = host_info->local_info;

    /* Count of bytes send */
    int sendcount;

    /* Send the data back to the client */
    if((sendcount = send(remote_sock, data_rcv.c_str(), data_rcv.size(), 0))== -1){
        string message = "[@] Error sending data to client " + remote_info;
        perror(message.c_str());
        delete host_info;
    }
}
