#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>			// for strncmp()
#include <unistd.h>			// for close()
#include <stdbool.h>
#include <ctype.h>
#include <pthread.h>

#include "udpServer.h"
#include "health_monitor.h"
#include "pump_control.h"
#include "moisture_sensor.h"
#include "light_sensor.h"
#include "age.h"
#include "backup.h"

// This should be able to handle max email length (320 characters)
#define MSG_MAX_LEN    1500
#define PORT           12345

// All the text describing the plant's status is no more than
// 11 characters
#define STATUS_MAX_LEN 11

#define HELP_MENU   "Accepted commands:\n\
help -- display all available commands.\n\
email {user_email@address.domain}-- Change the user's email address.\n\
DOB {dd-mm-yyyy} -- Set a DOB for the plant. \n\
sample -- Send an sample email address to the user.\n\
pump [on/off] -- Turn the pump on/off manually.\n\
moist_min {min_threshold} -- Change the minimum safety threshold for moisture.\n\
moist_max {max_threshold} -- Change the maximum safety threshold for moisture.\n\
light_min {min_threshold} -- Change the minimum safety threshold for light.\n\
light_max {max_threshold} -- Change the maximum safety threshold for light.\n\
get [email/moist/light/moist_min/moist_max/light_min/light_max/dob/age] -- Get the specified information from the target.\n\
stop -- cause the server program to end.\n\
<enter> -- repeat last command."

static pthread_t server_t;

static void tolower_str(char *str, const int last_idx) {
    for(int i = 0; i <= last_idx; i++) {
        str[i] = tolower(str[i]);
    }
}

static void send_packet(int *socketDescriptor, char *messageTx, struct sockaddr_in *sinRemote) {
    // Transmit a reply:
    unsigned int sin_len = sizeof(*sinRemote);
    sendto(*socketDescriptor,
        messageTx, strlen(messageTx),
        0,
        (struct sockaddr *) sinRemote, sin_len);
}

static void *startServer(void *input) {
    bool stop = false;

    // Address
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;                   // Connection may be from network
	sin.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
	sin.sin_port = htons(PORT);                 // Host to Network short

    // Create the socket for UDP
	int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

	// Bind the socket to the port (PORT) that we specify
	bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));	

    // Var to record the latest command
    // Initially set to help since it could help user 
    // figure out what to do
    char last_command[MSG_MAX_LEN] = "help\n";

    // Get the data (blocking)
    // Will change sin (the address) to be the address of the client.
    // Note: sin passes information in and out of call!
    struct sockaddr_in sinRemote;
    unsigned int sin_len = sizeof(sinRemote);
    char messageRx[MSG_MAX_LEN];
    char messageTx[MSG_MAX_LEN];

    while (!stop) {
		// Pass buffer size - 1 for max # bytes so room for the null (string data)
		int bytesRx = recvfrom(socketDescriptor,
			messageRx, MSG_MAX_LEN - 1, 0,
			(struct sockaddr *) &sinRemote, &sin_len);

		// Make it null terminated (so string functions work)
		// - recvfrom given max size - 1, so there is always room for the null
		messageRx[bytesRx] = 0;

        // Convert all to lower letters
        tolower_str(messageRx, bytesRx);

        // If only enter was pressed => Changed tok to latest command
        if (!strncmp(messageRx, "\n", MSG_MAX_LEN)) {
            strncpy(messageRx, last_command, MSG_MAX_LEN);
        } 
        else {
            strncpy(last_command, messageRx, MSG_MAX_LEN);
        }

        // printf("MessageRx: %s\n", messageRx);

        // Get the first token of the command
        char *tok = strtok(messageRx, " ");

        // printf("%s\n", tok);

        // Since strtok will replace whitespace by null terminated + 
        // we inserted null terminated at the end of the string
        // => strcmp is perfectly safe to use
        if (!strncmp(tok, "help\n", MSG_MAX_LEN)) {
            snprintf(messageTx, MSG_MAX_LEN, "%s\n", HELP_MENU);
        } 
        else if(!strncmp(tok, "backup", MSG_MAX_LEN)) {
            backup_info();
            snprintf(messageTx, MSG_MAX_LEN, "Done\n");
        } 
        else if(!strncmp(tok, "restore", MSG_MAX_LEN)) {
            restore_info();
            snprintf(messageTx, MSG_MAX_LEN, "Done\n");
        } 
        else if(!strncmp(tok, "email", MSG_MAX_LEN)) {
            // Get the next token
            tok = strtok(NULL, " ");

            // Replace trailing endline with null terminated character
            // Email address should not have endline character
            tok[strcspn(tok, "\n")] = 0;
            Health_setUserEmail(tok);

            snprintf(messageTx, MSG_MAX_LEN, "Done\n");
        } 
        else if(!strncmp(tok, "dob", MSG_MAX_LEN)) {
            if (messageRx[4] == 'r'){
                Age_reset_dob();
            }
            else{
                struct date dob;
                dob.year = atoi(&messageRx[4]);
                dob.month = atoi(&messageRx[9]);
                dob.day_in_month = atoi(&messageRx[12]);
                Age_set_dob(dob);
            }
            snprintf(messageTx, MSG_MAX_LEN, "Done\n");
        }
        else if(!strncmp(tok, "sample\n", MSG_MAX_LEN)) {
            Health_sendSampleEmail();
            snprintf(messageTx, MSG_MAX_LEN, "Done\n");
        } 
        else if(!strncmp(tok, "pump", MSG_MAX_LEN)) {
            // Get the next token
            tok = strtok(NULL, " ");
            
            // On or off has the maximum char of 3
            // + 1 null character
            char status[4];
            
            if (!strncmp(tok, "on\n", MSG_MAX_LEN)) {
                PC_startPump();

                strncpy(status, "on", 4);
            } else if (!strncmp(tok, "off\n", MSG_MAX_LEN)) {
                PC_stopPump();

                
                strncpy(status, "off", 4);
            }

            // Send back the status of the pump to update
            // on the browser
            snprintf(messageTx, MSG_MAX_LEN, "pump:%s\n", status);
        } 
        else if(!strncmp(tok, "moist_min", MSG_MAX_LEN)) {
            // Get the next token
            tok = strtok(NULL, " ");
            
            // Replace trailing endline with null terminated character
            // Email address should not have endline character
            tok[strcspn(tok, "\n")] = 0;

            // Convert the string number to an integer
            int threshold = atoi(tok);
            Moisture_setMinThreshold(threshold);
            threshold = Moisture_getMinThreshold();

            snprintf(messageTx, MSG_MAX_LEN, "moist_min:%d\n", threshold);
        } 
        else if(!strncmp(tok, "moist_max", MSG_MAX_LEN)) {
            // Get the next token
            tok = strtok(NULL, " ");
            
            // Replace trailing endline with null terminated character
            // Email address should not have endline character
            tok[strcspn(tok, "\n")] = 0;

            // Convert the string number to an integer
            int threshold = atoi(tok);
            Moisture_setMaxThreshold(threshold);
            threshold = Moisture_getMaxThreshold();

            snprintf(messageTx, MSG_MAX_LEN, "moist_max:%d\n", threshold);
        } 
        else if (!strncmp(tok, "light_min", MSG_MAX_LEN)) {
            // Get the next token
            tok = strtok(NULL, " ");
            
            // Replace trailing endline with null terminated character
            // Email address should not have endline character
            tok[strcspn(tok, "\n")] = 0;

            // Convert the string number to an integer
            int threshold = atoi(tok);
            Light_setMinThreshold(threshold);
            threshold = Light_getMinThreshold();

            snprintf(messageTx, MSG_MAX_LEN, "light_min:%d\n", threshold);
        } 
        else if (!strncmp(tok, "light_max", MSG_MAX_LEN)) {
            // Get the next token
            tok = strtok(NULL, " ");
            
            // Replace trailing endline with null terminated character
            // Email address should not have endline character
            tok[strcspn(tok, "\n")] = 0;

            // Convert the string number to an integer
            int threshold = atoi(tok);
            Light_setMaxThreshold(threshold);
            threshold = Light_getMaxThreshold();

            snprintf(messageTx, MSG_MAX_LEN, "light_max:%d\n", threshold);
        } 
        else if (!strncmp(tok, "get", MSG_MAX_LEN)) {
            // Get the next token
            tok = strtok(NULL, " ");
            
            if (!strncmp(tok, "email\n", MSG_MAX_LEN)) {
                char *email = Health_getUserEmail();
                snprintf(messageTx, MSG_MAX_LEN, "email:%s\n", email);
            } 
            else if(!strncmp(tok, "dob\n", MSG_MAX_LEN)) {
                struct date dob = Age_get_dob();
                snprintf(messageTx, MSG_MAX_LEN, "dob: %d-%d-%d \n", dob.day_in_month, dob.month, dob.year);
            } 
            else if(!strncmp(tok, "age\n", MSG_MAX_LEN)) {
                struct date age = Age_get_age();
                if (age.year != 0){
                    snprintf(messageTx, MSG_MAX_LEN, "age: %d years \n", age.year);
                }
                else if (age.month != 0){
                    snprintf(messageTx, MSG_MAX_LEN, "age: %d months \n", age.month);
                }
                else{
                    snprintf(messageTx, MSG_MAX_LEN, "age: %d days \n", age.day);
                }
            }
            else if (!strncmp(tok, "moist\n", MSG_MAX_LEN)) {
                // Moisture_readSensor();
                int moist_level = Moisture_getStatus();
                char moist_level_text[STATUS_MAX_LEN];
                if (moist_level < 0) {
                    strncpy(moist_level_text, "Deficient", STATUS_MAX_LEN);
                } else if (moist_level == 0) {
                    strncpy(moist_level_text, "Moderate", STATUS_MAX_LEN);
                } else {
                    strncpy(moist_level_text, "Excessive", STATUS_MAX_LEN);
                }
                snprintf(messageTx, MSG_MAX_LEN, "moist:%s\n", moist_level_text);
            } 
            else if (!strncmp(tok, "light\n", MSG_MAX_LEN)) {
                int light_level = Light_getStatus();
                char light_level_text[STATUS_MAX_LEN];
                if (light_level < 0) {
                    strncpy(light_level_text, "Deficient", STATUS_MAX_LEN);
                } else if (light_level == 0) {
                    strncpy(light_level_text, "Moderate",  STATUS_MAX_LEN);
                } else {
                    strncpy(light_level_text, "Excessive",  STATUS_MAX_LEN);
                }
                snprintf(messageTx, MSG_MAX_LEN, "light:%s\n", light_level_text);
            } 
            else if (!strncmp(tok, "moist_min\n", MSG_MAX_LEN)) {
                int threshold = Moisture_getMinThreshold();
                snprintf(messageTx, MSG_MAX_LEN, "moist_min:%d\n", threshold);
            } 
            else if (!strncmp(tok, "moist_max\n", MSG_MAX_LEN)) {
                int threshold = Moisture_getMaxThreshold();
                snprintf(messageTx, MSG_MAX_LEN, "moist_max:%d\n", threshold);
            } 
            else if (!strncmp(tok, "light_min\n", MSG_MAX_LEN)) {
                int threshold = Light_getMinThreshold();
                snprintf(messageTx, MSG_MAX_LEN, "light_min:%d\n", threshold);
            } 
            else if (!strncmp(tok, "light_max\n", MSG_MAX_LEN)) {
                int threshold = Light_getMaxThreshold();
                snprintf(messageTx, MSG_MAX_LEN, "light_max:%d\n", threshold);
            } 
            else {
                snprintf(messageTx, MSG_MAX_LEN, "Invalid command. Type 'help' for command list\n");
            }
        } 
        else if(!strncmp(tok, "stop\n", MSG_MAX_LEN)) {
            stop = true;

            snprintf(messageTx, MSG_MAX_LEN, "Program terminating\n");
        } 
        else {
            snprintf(messageTx, MSG_MAX_LEN, "Invalid command. Type 'help' for command list\n");
        }
        send_packet(&socketDescriptor, messageTx, &sinRemote);

        // Reset the message send out buffer
        strncpy(messageTx, "", MSG_MAX_LEN);
    }

    close(socketDescriptor);
    return NULL;
}

void UDP_initServer() {
    // Start the thread
    // Shutdown the program if the thread could not be created
    int status = pthread_create(&server_t, NULL, &startServer, NULL);
    if (status) {
        fprintf(stdout, "Cannot start the Server thread!\n");
        exit(-1);
    }
}

void UDP_stopServer() {
    int status = pthread_join(server_t, NULL);
    if (status) {
        fprintf(stdout, "Unable to join Server thread!\n");
        exit(-1);
    }

    printf("Stop Server thread\n");
}