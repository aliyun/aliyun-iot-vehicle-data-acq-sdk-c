#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "vehicle_property_acq.h"
#include <memory.h>
#include <zconf.h>
#include "vehicle_property_acq.h"
#include "pack_util.h"
#include "global.h"

int VehicleSetCallback_(const VehicleProperty *data){
  printf("VehicleSetCallback_ was called\n");
  switch (data->prop_id) {
    case VEHICLE_PROPERTY_OSD_INFO: {
      AudioPlayTask *obj_struct = malloc(sizeof(AudioPlayTask));
      UnpackMsgBodyToAudioPlayStruct(data->body, data->length, &obj_struct);
      printf("VEHICLE_PROPERTY_SET_OSD_INFO set uiAudioType=%d\n", obj_struct->audio_type);
      printf("VEHICLE_PROPERTY_SET_OSD_INFO set body=%s\n", obj_struct->text);
      break;
    }

    case VEHICLE_PROPERTY_TTS_INFO: {
      printf("VEHICLE_PROPERTY_SET_OSD_INFO was called\n");
      break;
    }

  }
  return RESULT_OK;
}

/**
 *
 * @param property_data
 * property_data->id is determined by caller,
 * but the property_data->body needs to be filled.
 * @return
 */
int VehiclePropertyGetCallback_(VehicleProperty *property_data){
  printf("VehiclePropertyGetCallback_ was called\n");
  char *data_buffer = NULL;
  size_t data_buffer_length = 0;

  property_data->timestamp = 0L;
  if(property_data->body != NULL) {
    free(property_data->body);
  }

  switch (property_data->prop_id) {
    case VEHICLE_PROPERTY_GPS_STATUS: {
      GpsStatus gps_status;
      memset(&gps_status, 0, sizeof(gps_status));
      gps_status.gps_status = 1;
      gps_status.pps_type = 2;
      gps_status.move_status = 3;
      gps_status.speed_kmh = 4;
      property_data->body = malloc(sizeof(GpsStatus));
      property_data->length = sizeof(GpsStatus);
      memcpy(property_data->body, &gps_status, sizeof(GpsStatus));
      break;
    }

    case VEHICLE_PROPERTY_SPEED_STATUS:{
      SpeedState speed_state;
      memset(&speed_state, 0, sizeof(speed_state));
      speed_state.sampling_time = 1;
      speed_state.speed_kmh = 2;
      speed_state.pulse_num = 3;
      speed_state.reserved = 4;
      property_data->body = malloc(sizeof(SpeedState));
      property_data->length = sizeof(SpeedState);
      memcpy(property_data->body, &speed_state, sizeof(SpeedState));
      break;
    }

    case VEHICLE_PROPERTY_IO_STATUS:{
      IOState io_state;
      memset(&io_state, 0, sizeof(io_state));
      io_state.car_key = 1;
      io_state.general_io_state = 2;
      for(int i=0;i<32;i++) {
        io_state.general_io_type[i] = i;
      }
      io_state.reserved = 5;
      property_data->body = malloc(sizeof(IOState));
      property_data->length = sizeof(IOState);
      memcpy(property_data->body, &io_state, sizeof(IOState));
      break;
    }
    case VEHICLE_PROPERTY_TEMPERATURE_STATUS:{
      TemperatureState temperature_state;
      temperature_state.cabin_centigrade = 1;
      temperature_state.oil_centigrade = 2;
      temperature_state.reserved = 2;
      property_data->body = malloc(sizeof(TemperatureState));
      property_data->length = sizeof(TemperatureState);
      memcpy(property_data->body, &temperature_state, sizeof(temperature_state));
      break;
    }

  }
  return RESULT_OK;
}

/**
 *
 * @param data
 */
void VehicleReleaseMemoryFromGetCallback_(VehicleProperty *data){
  printf("VehicleReleaseMemoryFromGetCallback_ was called\n");
  if(data->body != NULL){
    free(data->body);
  }
}

static VehicleCallbacks customer_callback;

/**
 *
 * @return
 */
int main() {
  customer_callback.vehicle_get_cb = VehiclePropertyGetCallback_;
  customer_callback.vehicle_set_cb = VehicleSetCallback_;
  customer_callback.vehicle_release_memory_from_get_cb = VehicleReleaseMemoryFromGetCallback_;
  customer_callback.size = sizeof(VehicleCallbacks);
  VehicleInit(customer_callback);

  AudioPlayTask * tts_info = malloc(sizeof(AudioPlayTask)+10);
  memset(tts_info, 0, sizeof(AudioPlayTask));
  tts_info->audio_type = 1;
  tts_info->audio_volume = 2;
  tts_info->speaker_type = 3;
  tts_info->handle_type = 4;
  tts_info->execute_type = 5;
  memcpy(tts_info->text, "TEST", 10);

  GpsStatus  *gps_status = malloc(sizeof(GpsStatus));
  gps_status->gps_status = 1;
  gps_status->pps_type = 2;
  gps_status->move_status = 3;
  gps_status->speed_kmh = 4;

  SpeedState *speed_state = malloc(sizeof(SpeedState));
  speed_state->sampling_time = 1;
  speed_state->speed_kmh = 2;
  speed_state->pulse_num = 3;
  speed_state->reserved = 4;


  IOState *io_state = malloc(sizeof(IOState));
  io_state->car_key = 1;
  io_state->general_io_state = 99;
  for(int i=0;i<32;i++){
    io_state->general_io_type[i]=i;
  }

  TemperatureState* temperature_state = malloc(sizeof(TemperatureState));
  temperature_state->cabin_centigrade = 1.1;
  temperature_state->oil_centigrade = 2.2;
  temperature_state->reserved = 3;

  AIEvent* ai_event = malloc(sizeof(AIEvent));
  strcpy(ai_event->event_name, "COLL");
  ai_event->sensitivity = 1;

  while(true){
    VehiclePropChanged(VEHICLE_PROPERTY_GPS_STATUS, gps_status, sizeof(GpsStatus));
    sleep(2);

    VehiclePropChanged(VEHICLE_PROPERTY_SPEED_STATUS, speed_state, sizeof(SpeedState));
    sleep(2);

    VehiclePropChanged(VEHICLE_PROPERTY_IO_STATUS, io_state, sizeof(IOState));
    sleep(2);

    VehiclePropChanged(VEHICLE_PROPERTY_TEMPERATURE_STATUS, temperature_state, sizeof(temperature_state));
    sleep(2);

    VehiclePropChanged(VEHICLE_PROPERTY_AI_EVENT_STATUS, ai_event, sizeof(AIEvent));
    sleep(2);
  }

  VehicleRelease();
}
