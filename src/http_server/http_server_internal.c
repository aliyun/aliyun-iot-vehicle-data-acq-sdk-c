#include "vehicle_property_acq.h"
#include <log.h>
#include <pack_util.h>
#include "global.h"
#include "http_server_internal.h"

struct MHD_Daemon *http_d;
static VehicleCallbacks driver_callback_;
static char empty_response[]="";

/**
 *
 * @param driver_callback
 * @return
 */
int SetHttpCallback(VehicleCallbacks driver_callback) {
  memset(&driver_callback_, 0, sizeof(driver_callback_));
  driver_callback_.vehicle_get_cb = driver_callback.vehicle_get_cb;
  driver_callback_.vehicle_set_cb = driver_callback.vehicle_set_cb;
  driver_callback_.vehicle_release_memory_from_get_cb = driver_callback.vehicle_release_memory_from_get_cb;
  driver_callback_.size = sizeof(driver_callback_);
  return RESULT_OK;
}

/**
 *
 * @param connection
 * @param response_details
 * @return
 */
static enum MHD_Result SendResponse(struct MHD_Connection *connection, const char *response_details, size_t response_data_length) {
  enum MHD_Result ret;
  struct MHD_Response *response;
  //MHD_RESPMEM_PERSISTENT
  response = MHD_create_response_from_buffer(response_data_length,
                                             (void *) response_details,
                                             MHD_RESPMEM_MUST_COPY);
  if (!response) {
    return MHD_NO;
  }
  ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
  MHD_destroy_response(response);

  return ret;
}

/**
 *
 * @param coninfo_cls
 * @param request_data:
 * @param request_data_length
 * @return
 */
static int RequestParse(struct connection_info_struct *coninfo_cls,
                        const char *request_data,
                        size_t request_data_length,
                        char **response_data,
                        size_t* response_data_length){

  log_i("RequestParse %d %s", request_data_length, request_data);
  if(strcmp(coninfo_cls->request_path, GET_PROPERTY_API_PATH)==0) {

    VehicleProperty* vehicle_prop = NULL;
    UnpackMsgToVehicleProperty(request_data, request_data_length, &vehicle_prop);
    if(vehicle_prop != NULL) {
      ////Feedback Function
      driver_callback_.vehicle_get_cb(vehicle_prop);

      if (vehicle_prop->length != 0 && vehicle_prop->length < MAX_ANSWER_SIZE) {
        char *serialized_msg_data = NULL;
        size_t serialized_msg_data_len = 0;
        PackVehiclePropertyToMsg(vehicle_prop->prop_id, 0,
            vehicle_prop->length, vehicle_prop->body,
            &serialized_msg_data, &serialized_msg_data_len);

        driver_callback_.vehicle_release_memory_from_get_cb(vehicle_prop);

        *response_data = serialized_msg_data;
        *response_data_length = serialized_msg_data_len;
        return RESULT_OK;
      }
    }
    return RESULT_ERROR;


  }else if(strcmp(coninfo_cls->request_path, SET_PROPERTY_API_PATH)==0){
    VehicleProperty *vehicle_prop = NULL;
    UnpackMsgToVehicleProperty(request_data, request_data_length, &vehicle_prop);

    driver_callback_.vehicle_set_cb(vehicle_prop);
    *response_data = empty_response;
    *response_data_length = strlen(empty_response)+1;
    return RESULT_OK;

  }else{
    *response_data = empty_response;
    *response_data_length = strlen(empty_response)+1;
    return RESULT_ERROR;
  }
}

/**
 *
 * @param cls
 * @param connection
 * @param con_cls
 * @param toe
 */
static void RequestCompleted(void *cls, struct MHD_Connection *connection,
                             void **con_cls, enum MHD_RequestTerminationCode toe) {
  struct connection_info_struct *con_info = *con_cls;

  if (NULL == con_info) {
    return;
  }

  if (con_info->method_type == POST) {
    log_i("clear post \n");
    if (con_info->answer_data) {
      free(con_info->answer_data);
    }

    if (con_info->request_path) {
      free(con_info->request_path);
    }
  }
  free(con_info);
  *con_cls = NULL;
}

/**
 *
 * @param url
 * @return
 */
static int IsRoutedPath(const char *url) {
  if (!strncasecmp(url, GET_PROPERTY_API_PATH, GET_PROPERTY_API_PATH_LENGTH)) {
    return 1;
  } else if (!strncasecmp(url, GET_PROPERTY_API_PATH, GET_PROPERTY_API_PATH_LENGTH)) {
    return 1;
  } else if (!strncasecmp(url, NOTIFY_PROPERTY_CHANGE_API_PATH, NOTIFY_PROPERTY_CHANGE_API_PATH_LENGTH)){
    return 1;
  }
  return 0;
}

/**
 *
 * @param cls
 * @param connection
 * @param url
 * @param method
 * @param version
 * @param upload_data
 * @param upload_data_size
 * @param con_cls :
 * @return
 */
static enum MHD_Result RouteHandlerToConnection(void *cls, struct MHD_Connection *connection,
                                                const char *url, const char *method,
                                                const char *version, const char *upload_data,
                                                size_t *upload_data_size, void **con_cls)
{
  if (NULL == *con_cls){ // if it is first iteration of the new request
    // set up connection structure
    struct connection_info_struct *con_info;
    con_info = malloc(sizeof(struct connection_info_struct));
    if (NULL == con_info) {
      return MHD_NO;
    }

    con_info->answer_data = NULL;
    con_info->request_path = NULL;

    if (0 == strcmp(method, "POST")) {
      ////only support POST request
      log_i("%s\n", "Received POST request");

      if (IsRoutedPath(url)) {
        log_i("url %s checked ok\n", url);
        con_info->method_type = POST;
        con_info->request_path = malloc(strlen(url) + 1);
        memset(con_info->request_path, 0, strlen(url) + 1);
        strcpy(con_info->request_path, url);

      }
    }

    *con_cls = (void *) con_info;
    return MHD_YES;
  }

  if (0 == strcmp(method, "POST")) {
    // for second and following iterations of POST request
    log_i("%s\n", "working on POST request\n");
    struct connection_info_struct *con_info = *con_cls;

    if (IsRoutedPath(url)) {
      if (*upload_data_size != 0) {
        char *response_data = NULL;
        size_t response_data_length = 0;
        if (RequestParse(con_info, upload_data, *upload_data_size, &response_data, &response_data_length)
            == RESULT_OK) {
          if (response_data_length != 0) {
            con_info->answer_data = malloc(response_data_length);
            memset(con_info->answer_data, 0, response_data_length);
            memcpy(con_info->answer_data, response_data, response_data_length);

            con_info->answer_data_length = response_data_length;
          }
        }
        *upload_data_size = 0;
        return MHD_YES;
      } else if (NULL != con_info->answer_data) {
        return SendResponse(connection, con_info->answer_data, con_info->answer_data_length);
      }
      ////must return MHD_NO
      ////return MHD_NO;
    }
  }
  ////return MHD_NO;
}

/**
 *
 * @param port
 * @return
 */
int HttpServerInit(int port) {
  log_i("Start server ");
  http_d = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, port,
                        NULL, NULL,
                            &RouteHandlerToConnection, NULL,
                            MHD_OPTION_NOTIFY_COMPLETED, RequestCompleted,
                            NULL, MHD_OPTION_END);
  if (NULL == http_d) {
    log_i("failure in MHD_start_daemon().\n");
    return RESULT_ERROR;
  }
  return RESULT_OK;
}
