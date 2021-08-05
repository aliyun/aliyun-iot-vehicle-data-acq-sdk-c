//
// Created by Eason on 2021/4/1.
//
#include "vehicle_property_acq.h"
#include "http_client/http_client_internal.h"
#include "curl/curl.h"
#include "util/log.h"
#include "http_server/http_server_internal.h"
#include <microhttpd.h>
#include "util/pack_util.h"

static char *module_name = "httpd";
static char *protocol = "http";

static char *http_client_request_address = "localhost";
static int  http_client_request_port = 5002;
static char http_client_request_url[50]={0};

struct      MHD_Daemon *http_serverd;
static int  http_server_port = 5001;

/**
 *
 * @param callbacks
 * @return
 */
int VehicleInit(VehicleCallbacks callbacks) {
  log_init(module_name, LOG_STDOUT, LOG_LEVEL_DEBUG, LOG_MOD_BRIEF);

  ////Init Http Client
  snprintf(http_client_request_url, sizeof(http_client_request_url), "%s://%s:%d", protocol, http_client_request_address, http_client_request_port);
  log_i("Request Address %s\n", http_client_request_url);

  ////Init Http Server
  if(HttpServerInit(http_server_port) != RESULT_ERROR){
    log_i("Listen on Port %d successfully\n", http_server_port);
    SetHttpCallback(callbacks);
    return RESULT_OK;
  }
  return RESULT_ERROR;
}

/**
 *
 * @return
 */
int VehicleRelease() {
  MHD_stop_daemon(http_serverd);
  return RESULT_OK;
}

/**
 *
 * @param property_data_struct
 * @return
 */
int VehiclePropChanged(int property_id, const void *property_data_struct, size_t property_data_struct_length) {
  log_i("prop_id=0x%x\n", property_id);
  char* serialized_data_buf = NULL;
  size_t serialized_data_size = 0;

  PackVehiclePropertyToMsg(property_id, 0, property_data_struct_length, property_data_struct,
                           &serialized_data_buf, &serialized_data_size);

  if (CURLE_OK != curl_post_req(http_client_request_url, serialized_data_buf, serialized_data_size)){
    log_e("curl_post_req post request\n");
    return RESULT_ERROR;
  }else{
    log_i("curl_post_req successfully\n");
  }

  ////release buffer
  if(serialized_data_buf != NULL){
    free(serialized_data_buf);
  }

  return RESULT_OK;
}