// Module that taking care of setting up and tearing down the server thread
#ifndef _UDP_SERVER_
#define _UDP_SERVER_

// Initialize the server and start listening for request on a new thread
void UDP_initServer(void);

// Stop the server and clean up data
void UDP_stopServer(void);

#endif 