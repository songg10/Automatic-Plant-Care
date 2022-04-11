#include "backup.h"
#define MAX_LENGTH  1024


int backup_info(void)
{
    // get the values we want to store
    char* email = Health_getUserEmail();
    int min_light = Light_getMinThreshold();
    int max_light = Light_getMaxThreshold();
    int min_moisture = Moisture_getMinThreshold();
    int max_moisture = Moisture_getMaxThreshold();
    struct date dob = Age_get_dob();
    // concatenate values into one string
    char buffer[MAX_LENGTH];
    snprintf(buffer, MAX_LENGTH, "%s %d %d %d %d %d %d %d %d\n", 
             email, min_light, max_light, min_moisture, max_moisture, 
             dob.year, dob.month, dob.day_in_month, dob.day);
    printf("%s\n", buffer);
    // write the string to the backup file
    return Utils_write_bin(BACKUP_FILE, buffer, strlen(buffer)+1);
}

int restore_info(void)
{
    // read from file
    char* buffer = malloc(MAX_LENGTH);
    memset(buffer, 0, MAX_LENGTH);
    if (Utils_read_bin(BACKUP_FILE, buffer) == -1){
        printf("Error reading the backup file.\n");
        return -1;
    }
    // tokenize the buffer we have read into
    char attributes[NUM_ATTRIBUTES][MAX_LENGTH];
    int buff_ptr = 0;
    int attr_ptr = 0;
    int attr_value_ptr = 0;
    for (int i = 0; i < strlen(buffer)+1; i++){
        if (buffer[buff_ptr] == '\0'){
            break;
        }
        if (buffer[buff_ptr] != ' '){
            attributes[attr_ptr][attr_value_ptr] = buffer[buff_ptr];
            attr_value_ptr++;
        }
        else if (buffer[buff_ptr] == ' '){
            if (attr_ptr == 0){
                attributes[attr_ptr][attr_value_ptr] = '\0';
            }
            attr_ptr++;
            attr_value_ptr = 0;
        }
        buff_ptr++;
    }
    free(buffer);
    // format the tokens appropriately
    char* email = malloc(strlen(attributes[0] + 1));
    memset(email, 0, strlen(attributes[0])+1);
    strcpy(email, attributes[0]);
    int min_light = atoi(attributes[1]);
    int max_light = atoi(attributes[2]);
    int min_moisture = atoi(attributes[3]);
    int max_moisture = atoi(attributes[4]);
    struct date day;
    day.year = atoi(attributes[5]);
    day.month = atoi(attributes[6]);
    day.day_in_month = atoi(attributes[7]);
    day.day = atoi(attributes[8]);
    // set the values
    Health_setUserEmail(email);
    Light_setMinThreshold(min_light);
    Light_setMaxThreshold(max_light);
    Moisture_setMinThreshold(min_moisture);
    Moisture_setMaxThreshold(max_moisture);
    Age_set_dob(day);
    free(email);
    return 0;
}

