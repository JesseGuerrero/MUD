#include "StdinHandler.h"

using namespace std;

void* StdinHandler(void* thread_arg) {
    /* Count of bytes send */
    int sendcount;

    while(1) {

		/* Data to send */
		string data;

		/* Get a new line from standard input */
		getline (cin,data);
		/* Add additional information */
		data = "Broadcast:"+data+'\n';

		/* Get connection iterator */
		map<string,int>::const_iterator it_con;

		pthread_mutex_lock(&HostInfo::mutex);
		/* Loop over each connection */
		for(it_con = HostInfo::connections.begin() ; it_con != HostInfo::connections.end() ; it_con++) {
			/* Send the data back to the client */
			if((sendcount = send((*it_con).second, data.c_str(), data.size(), 0))== -1){
				string message = "[@] Error sending data to client " + (*it_con).first;
				perror(message.c_str());
				return 0;
			}
		}
		pthread_mutex_unlock(&HostInfo::mutex);

    }

    return 0;
}


