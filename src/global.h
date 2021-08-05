#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <pthread.h>


#define GET 0
#define POST 1


#define RESULT_OK 1
#define RESULT_ERROR -1


#define MAX_ANSWER_SIZE            1024
#define ITEM_NAME_MAX_LENGTH       15


#define GET_PROPERTY_API_PATH "/getproperty"
#define GET_PROPERTY_API_PATH_LENGTH sizeof(GET_PROPERTY_API_PATH)

#define SET_PROPERTY_API_PATH "/setproperty"
#define SET_PROPERTY_API_PATH_LENGTH sizeof(SET_PROPERTY_API_PATH)

#define NOTIFY_PROPERTY_CHANGE_API_PATH "/notifypropertieschanged"
#define NOTIFY_PROPERTY_CHANGE_API_PATH_LENGTH sizeof(NOTIFY_PROPERTY_CHANGE_API_PATH)


