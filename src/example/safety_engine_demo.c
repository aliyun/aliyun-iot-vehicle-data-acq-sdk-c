//
// Created by Eason on 2021/4/22.
//

#include "stdlib.h"
#include "stdio.h"
#include <microhttpd.h>
#include <pack_util.h>
#include <http_client/http_client_internal.h>
#include "vehicle_property_acq.h"
#include "curl/curl.h"
#include <string.h>
#include <stdbool.h>

void PrintData(const char* upload_data, size_t upload_data_size);
static char *http_client_request_url = "http://localhost:5001/notifypropertieschanged";
static char *http_client_request_get_url = "http://localhost:5001/getproperty";
extern ResponseMemory chunk;

const char response_string[] = " ";
static enum MHD_Result
ahc_echo(void *cls,
         struct MHD_Connection *connection,
         const char *url,
         const char *method,
         const char *version,
         const char *upload_data,
         size_t *upload_data_size,
         void **ptr) {
  static int aptr;
  struct MHD_Response *response;
  enum MHD_Result ret;

  (void) cls;               /* Unused. Silent compiler warning. */
  (void) url;               /* Unused. Silent compiler warning. */
  (void) version;           /* Unused. Silent compiler warning. */
  (void) upload_data;       /* Unused. Silent compiler warning. */
  (void) upload_data_size;  /* Unused. Silent compiler warning. */

  ////
  if (&aptr != *ptr) {
    /* do never respond on first call */
    *ptr = &aptr;
    return MHD_YES;
  }

  if (*upload_data_size != 0) {
    printf("data length %zu\n", *upload_data_size);
    PrintData(upload_data, *upload_data_size);
    *upload_data_size = 0;
    ////必须返回MHD_YES，否则libmicrohttpserver会 will report Application reported internal error, closing connection.
    //This callback return MHD_YES if connection is allowed, MHD_NO if not.
    return MHD_YES;
  }

  //*ptr = NULL;                  /* reset when done */
  response = MHD_create_response_from_buffer(strlen(response_string) + 1,
                                             (void *) response_string,
                                             MHD_RESPMEM_PERSISTENT);
  ret = MHD_queue_response(connection,
                           MHD_HTTP_OK,
                           response);
  MHD_destroy_response(response);
  return ret;
}


int vehicle_get_request(char *serialized_data_buf, size_t serialized_data_size) {
  if (CURLE_OK != curl_post_req(http_client_request_get_url, serialized_data_buf, serialized_data_size)) {
    printf("curl_post_req post request failed\n");
    return 0;

  } else {
    printf("curl_post_req successfully\n\n");
    if(chunk.size != 0 && (chunk.response != NULL)){
      PrintData(chunk.response, chunk.size);

    }
  }

  return 0;
}


/**
 *
 * @param upload_data
 * @param upload_data_size
 */
void PrintData(const char* upload_data, size_t upload_data_size){

  VehicleProperty *vehicle_property = NULL;
  UnpackMsgToVehicleProperty(upload_data, upload_data_size, &vehicle_property);

  if (vehicle_property != NULL) {
    printf("prop_id=0x%x, length=%zu\n", vehicle_property->prop_id, vehicle_property->length);
    switch (vehicle_property->prop_id) {
      case VEHICLE_PROPERTY_TTS_INFO : {
        AudioPlayTask *audio_play_task = (AudioPlayTask *) vehicle_property->body;
        printf("obj_struct audio_type=%d\n", audio_play_task->audio_type);
        printf("obj_struct audio_volume=%d\n", audio_play_task->audio_volume);
        printf("obj_struct speaker_type=%d\n", audio_play_task->speaker_type);
        if (strlen(audio_play_task->text) != 0) {
          printf("obj_struct %s\n", audio_play_task->text);
        }
        break;
      }

      case VEHICLE_PROPERTY_SPEED_STATUS : {
        SpeedState *speed_state = (SpeedState *) vehicle_property->body;
        printf("obj_struct sampling_time=%d\n", speed_state->sampling_time);
        printf("obj_struct speed_kmh=%d\n", speed_state->speed_kmh);
        printf("obj_struct pulse_num=%d\n", speed_state->pulse_num);
        printf("obj_struct reserved=%d\n", speed_state->reserved);
        break;
      }

      case VEHICLE_PROPERTY_GPS_STATUS : {
        GpsStatus *gps_status = (GpsStatus *) vehicle_property->body;
        printf("obj_struct gps_status=%d\n", gps_status->gps_status);
        printf("obj_struct pps_type=%d\n", gps_status->pps_type);
        printf("obj_struct move_status=%d\n", gps_status->move_status);
        printf("obj_struct speed_kmh=%d\n", gps_status->speed_kmh);
        break;
      }
    }
    free(vehicle_property);
    vehicle_property = NULL;
  }
}


////when MHD_USE_ERROR_LOG is ON : prompt "Internal application error, closing connection"

int
main(int argc,
     char *const *argv) {
  struct MHD_Daemon *d;
  d = MHD_start_daemon(MHD_USE_AUTO | MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG,
                       5002,
                       NULL, NULL,
                       &ahc_echo, NULL,
                       MHD_OPTION_CONNECTION_TIMEOUT, (unsigned int) 120,
                       MHD_OPTION_STRICT_FOR_CLIENT, (int) 1,
                       MHD_OPTION_END);

  char *serialized_msg_data = NULL;
  size_t serialized_msg_len = 0;

  int MESSAGE_ID[] = {VEHICLE_PROPERTY_AI_EVENT_STATUS,
                      VEHICLE_PROPERTY_GPS_STATUS,
                      VEHICLE_PROPERTY_SPEED_STATUS,
                      VEHICLE_PROPERTY_IO_STATUS,
                      VEHICLE_PROPERTY_TEMPERATURE_STATUS,
                      VEHICLE_PROPERTY_VOLTAGE_STATUS,
                      VEHICLE_PROPERTY_TTS_INFO,
                      VEHICLE_PROPERTY_OSD_INFO,
                      VEHICLE_PROPERTY_JT_PROTOCOL_INFO
  };

  size_t i =0;
  int number = sizeof(MESSAGE_ID)/sizeof(MESSAGE_ID[0]);
  while (true) {
    size_t index = (i++) % number;
    PackVehiclePropertyToMsg(MESSAGE_ID[index], 0, 0, NULL,
                             &serialized_msg_data, &serialized_msg_len);
    vehicle_get_request(serialized_msg_data, serialized_msg_len);

    if(serialized_msg_data != NULL){
      free(serialized_msg_data);
      serialized_msg_data = NULL;
    }
    sleep(2);
  }

  MHD_stop_daemon(d);
  return 0;
}
