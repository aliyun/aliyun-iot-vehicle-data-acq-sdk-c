#include <microhttpd.h>
#include "global.h"


// Structure that stores connection information for MHD
int HttpServerInit(int port);
int SetHttpCallback(VehicleCallbacks driver_callback);

struct connection_info_struct
{
    int method_type;
    char *answer_data;
    size_t answer_data_length;
    char *request_path;
    struct MHD_PostProcessor *postprocessor;
};

// Creates and sends MHD response, I could come up with a better function name
static enum MHD_Result SendResponse(struct MHD_Connection *connection, const char *response_details, size_t response_data_length);

// Iteration function is used by MHD post processor to iterate through POSTed chunks of data
static enum MHD_Result IteratePost(void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
                                   const char *filename, const char *content_type,
                                   const char *transfer_encoding, const char *data, uint64_t off,
                                   size_t size);

// Request completed does clean up after request
static void RequestCompleted (void *cls, struct MHD_Connection *connection,
                              void **con_cls, enum MHD_RequestTerminationCode toe);

// Function that serves particular connection and prepares the reply
// It contains the logic to serve POST and GET requests and responces
static enum MHD_Result answer_to_connection (void *cls, struct MHD_Connection *connection,
                                 const char *url, const char *method,
                                 const char *version, const char *upload_data,
                                 size_t *upload_data_size, void **con_cls);

static enum MHD_Result RouteHandlerToConnection (void *cls, struct MHD_Connection *connection,
                                                 const char *url, const char *method,
                                                 const char *version, const char *upload_data,
                                                 size_t *upload_data_size, void **con_cls);
