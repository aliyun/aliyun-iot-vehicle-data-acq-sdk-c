//
// Created by root on 2021/4/14.
//

#include "vehicle_property_acq.h"
#include "log.h"
#include "global.h"
#include "pack_util.h"

int PackVehiclePropertyToMsg(int property_id,
                             long timestamp,
                             size_t vehicle_prop_struct_length,
                             const void *vehicle_prop_struct,
                             char **serialized_msg_data,
                             size_t *serialized_msg_data_len) {

  msgpack_sbuffer sbuf;
  msgpack_sbuffer_init(&sbuf);

  msgpack_packer pk;
  msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

  ////#1 header to msgpack operation
  ////1.1 Pack Id
  msgpack_pack_int(&pk, property_id);
  ////1.2 Pack timestamp
  msgpack_pack_long(&pk, timestamp);
  ////1.3 Pack Body Length
  msgpack_pack_int(&pk, vehicle_prop_struct_length);

  ////1.4 Pack Body Data
  char *serialized_struct_data = NULL;
  size_t serialized_struct_len = 0;
  ////If there is a vehicle_prop_struct object, pack the Message Body
  if (vehicle_prop_struct != NULL) {

    switch (property_id) {
      case VEHICLE_PROPERTY_GPS_STATUS: {
        GpsStatus *obj_struct = (GpsStatus *) vehicle_prop_struct;
        PackGpsInfoStructToMsgBody(obj_struct, &serialized_struct_data, &serialized_struct_len);
        break;
      }

      case VEHICLE_PROPERTY_SPEED_STATUS: {
        SpeedState *obj_struct = (SpeedState *) vehicle_prop_struct;
        PackVehicleSpeedToMsgBody(obj_struct, &serialized_struct_data, &serialized_struct_len);
        break;
      }

      case VEHICLE_PROPERTY_IO_STATUS: {
        IOState *obj_struct = (IOState *) vehicle_prop_struct;
        PackVehicleIOToMsgBody(obj_struct, &serialized_struct_data, &serialized_struct_len);
        break;
      }

      case VEHICLE_PROPERTY_TEMPERATURE_STATUS: {
        TemperatureState *obj_struct = (TemperatureState *) vehicle_prop_struct;
        PackVehicleTempStructToMsgBody(obj_struct, &serialized_struct_data, &serialized_struct_len);
        break;
      }

      case VEHICLE_PROPERTY_TTS_INFO: {
        AudioPlayTask *obj_struct = (AudioPlayTask *) vehicle_prop_struct;
        PackAudioPlayStructToMsgBody(obj_struct, &serialized_struct_data, &serialized_struct_len);
        break;
      }

      case VEHICLE_PROPERTY_OSD_INFO: {
        DialogDisplayTask *obj_struct = (DialogDisplayTask *) vehicle_prop_struct;
        PackDialogDisplayToMsgBody(obj_struct, &serialized_struct_data, &serialized_struct_len);
        break;
      }

      case VEHICLE_PROPERTY_JT_PROTOCOL_INFO:{
        JTProtocolExtend *obj_struct = (JTProtocolExtend *) vehicle_prop_struct;
        PackJTProtocolExtendToMsgBody(obj_struct, &serialized_struct_data, &serialized_struct_len);
        break;
      }

      case VEHICLE_PROPERTY_AI_EVENT_STATUS:{
        AIEvent *obj_struct = (AIEvent *) vehicle_prop_struct;
        PackAIEventToMsgBody(obj_struct, &serialized_struct_data, &serialized_struct_len);
        break;
      }
      default: {
        log_e("non-supported message id\n");
        return RESULT_ERROR;
      }
    }
    msgpack_pack_bin_with_body(&pk, serialized_struct_data, serialized_struct_len);
  }

  ////#2 Copy memory buffer
  if ((*serialized_msg_data) == NULL) {
    *serialized_msg_data = malloc(sbuf.size);
    memcpy(*serialized_msg_data, sbuf.data, sbuf.size);
    *serialized_msg_data_len = sbuf.size;

    if (serialized_struct_data != NULL) {
      free(serialized_struct_data);
    }
    msgpack_sbuffer_destroy(&sbuf);
    return RESULT_OK;
  }

  return RESULT_ERROR;
}

/**
 *
 * @param msg_input
 * @param msg_input_len
 * @param unpacked_message_id
 * @param unpacked_message_length
 * @param unpacked_data
 * @return
 */
int UnpackMsgToVehicleProperty(const char *msg_input,
                               size_t msg_input_len,
                               VehicleProperty **vehicle_property_pp) {
  size_t off = 0;
  int property_id = 0;
  long timestamp = 0L;
  size_t struct_size = 0;
  const char *packed_msg_body = NULL;
  int packed_msg_body_length = 0;
  void *struct_data = NULL;


  printf("msg len=%zu\n", msg_input_len);
  msgpack_unpacked result;
  msgpack_unpacked_init(&result);

  ////iterate unpack operation
  ////#1 property_id
  msgpack_unpack_return ret = msgpack_unpack_next(&result, msg_input, msg_input_len, &off);
  if (ret == MSGPACK_UNPACK_SUCCESS) {
    property_id = result.data.via.i64;
    log_i("property_id=0x%x\n", property_id);
  }

  ////#2 timestamp
  ret = msgpack_unpack_next(&result, msg_input, msg_input_len, &off);
  if (ret == MSGPACK_UNPACK_SUCCESS) {
    msgpack_object_print(stdout, result.data);
    puts("");
    timestamp = result.data.via.u64;
  }

  ////#3 struct_size
  ret = msgpack_unpack_next(&result, msg_input, msg_input_len, &off);
  if (ret == MSGPACK_UNPACK_SUCCESS) {
    msgpack_object_print(stdout, result.data);
    puts("");
    struct_size = result.data.via.u64;
  }

  ////#4 struct_data in msg body
  ret = msgpack_unpack_next(&result, msg_input, msg_input_len, &off);
  if (ret == MSGPACK_UNPACK_SUCCESS) {
    msgpack_object_print(stdout, result.data);
    puts("");
    packed_msg_body = result.data.via.str.ptr;
    packed_msg_body_length = result.data.via.str.size;
    switch (property_id) {
      case VEHICLE_PROPERTY_GPS_STATUS: {
        GpsStatus* obj_struct = NULL;
        UnpackMsgBodyToGpsInfoStruct(packed_msg_body, packed_msg_body_length, &obj_struct);
        struct_size = sizeof(GpsStatus);
        struct_data = obj_struct;
        break;
      }

      case VEHICLE_PROPERTY_SPEED_STATUS: {
        SpeedState* obj_struct = NULL;
        UnpackMsgBodyToVehicleSpeedStruct(packed_msg_body, packed_msg_body_length, &obj_struct);
        struct_size = sizeof(SpeedState);
        struct_data = obj_struct;
        break;
      }

      case VEHICLE_PROPERTY_IO_STATUS: {
        IOState* obj_struct = NULL;
        UnpackMsgBodyToVehicleIOStruct(packed_msg_body, packed_msg_body_length, &obj_struct);
        struct_size = sizeof(IOState);
        struct_data = obj_struct;
        break;
      }

      case VEHICLE_PROPERTY_TEMPERATURE_STATUS: {
        TemperatureState* obj_struct = NULL;
        UnpackMsgBodyToVehicleTempStruct(packed_msg_body, packed_msg_body_length, &obj_struct);
        struct_size = sizeof(TemperatureState);
        struct_data = obj_struct;
        break;
      }

      case VEHICLE_PROPERTY_TTS_INFO: {
        AudioPlayTask* obj_struct = NULL;
        UnpackMsgBodyToAudioPlayStruct(packed_msg_body, packed_msg_body_length, &obj_struct);
        struct_size = sizeof(AudioPlayTask);
        struct_data = obj_struct;
        break;
      }

      case VEHICLE_PROPERTY_OSD_INFO: {
        DialogDisplayTask* obj_struct = NULL;
        UnpackMsgBodyToDialogDisplayStruct(packed_msg_body, packed_msg_body_length, &obj_struct);
        struct_size = sizeof(DialogDisplayTask);
        struct_data = obj_struct;
        break;
      }

      case VEHICLE_PROPERTY_JT_PROTOCOL_INFO:{
        JTProtocolExtend* obj_struct = NULL;
        UnpackMsgBodyToJTProtocolExtend(packed_msg_body, packed_msg_body_length, &obj_struct);
        struct_size = sizeof(JTProtocolExtend);
        struct_data = obj_struct;
        break;
      }

      case VEHICLE_PROPERTY_AI_EVENT_STATUS:{
        AIEvent* obj_struct = NULL;
        UnpackMsgBodyToAIEvent(packed_msg_body, packed_msg_body_length, &obj_struct);
        struct_size = sizeof(AIEvent);
        struct_data = obj_struct;
        break;
      }

      default: log_e("non-supported message id\n");
        return RESULT_ERROR;
    }
  }

  if (*vehicle_property_pp == NULL) {
    (*vehicle_property_pp) = malloc(sizeof(VehicleProperty));
    memset((*vehicle_property_pp), 0, sizeof(VehicleProperty));
    (*vehicle_property_pp)->prop_id = property_id;
    (*vehicle_property_pp)->timestamp = timestamp;
    (*vehicle_property_pp)->length = struct_size;
    if (struct_data != NULL && struct_size > 0) {
      (*vehicle_property_pp)->body = malloc(struct_size);
      memcpy((*vehicle_property_pp)->body, struct_data, struct_size);
    }else{
      (*vehicle_property_pp)->body = NULL;
    }
  }

  log_i("unpack body finished\n");
  return RESULT_OK;
}

/**
 *
 * @param msg_input
 * @param len
 * @param unpacked_data
 * @return
 */
int UnpackHttpMsg(const char *msg_input, size_t len, VehicleProperty *unpacked_data) {

  msgpack_unpacked result;
  size_t off = 0;
  msgpack_unpack_return ret;
  int i = 0;
  char unpacked_buffer[UNPACKED_BUFFER_SIZE];
  msgpack_unpacked_init(&result);
  ret = msgpack_unpack_next(&result, msg_input, len, &off);
  while (ret == MSGPACK_UNPACK_SUCCESS) {
    msgpack_object obj = result.data;
    if (obj.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
      unpacked_data->prop_id = obj.via.u64;
    }
    msgpack_object_print(stdout, obj);
    printf("\n");
    msgpack_object_print_buffer(unpacked_buffer, UNPACKED_BUFFER_SIZE, obj);

    ret = msgpack_unpack_next(&result, msg_input, len, &off);
  }
  msgpack_unpacked_destroy(&result);

  if (ret == MSGPACK_UNPACK_CONTINUE) {
    log_i("All msgpack_object in the buffer is consumed.\n");
  } else if (ret == MSGPACK_UNPACK_PARSE_ERROR) {
    log_i("The data in the buf is invalid format.\n");
  }
  return RESULT_OK;
}

/**
 *
 *
 * @param msg_input
 * @param len
 * @param unpacked_message_id
 * @param unpacked_data
 * @return
 */
int UnpackSetPropertyRequestMsg(const char *msg_input,
                                size_t len,
                                int *unpacked_message_id,
                                int *unpacked_message_length,
                                char *unpacked_data) {

  msgpack_unpacked result;
  msgpack_unpack_return ret;
  size_t off = 0;

  char unpacked_buffer[UNPACKED_BUFFER_SIZE];
  msgpack_unpacked_init(&result);

  ////unpack the first element, that is the prop_id
  ret = msgpack_unpack_next(&result, msg_input, len, &off);
  if (ret == MSGPACK_UNPACK_SUCCESS) {
    msgpack_object obj_message_id = result.data;
    if (obj_message_id.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
      *unpacked_message_id = obj_message_id.via.u64;
    }
    msgpack_object_print(stdout, obj_message_id);

    ////unpack the second element, that is the length of msg_input
    ret = msgpack_unpack_next(&result, msg_input, len, &off);
    if (ret == MSGPACK_UNPACK_SUCCESS) {
      msgpack_object obj_message_length = result.data;
      if (obj_message_length.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
        *unpacked_message_length = obj_message_length.via.u64;
      }
      msgpack_object_print(stdout, obj_message_length);

    }
  }
  msgpack_unpacked_destroy(&result);

  if (ret == MSGPACK_UNPACK_CONTINUE) {
    log_i("All msgpack_object in the buffer is consumed.\n");
  } else if (ret == MSGPACK_UNPACK_PARSE_ERROR) {
    log_e("The data in the buf is invalid format.\n");
  }
  return RESULT_OK;
}

/**
 *
 * @param msg_input
 * @param msg_input_len
 * @param unpacked_id
 * @return
 */
int UnpackGetPropertyRequestMsg(const char *msg_input, size_t msg_input_len, int *unpacked_id) {

  msgpack_unpacked result;
  size_t off = 0;
  msgpack_unpack_return ret;
  int i = 0;
  char unpacked_buffer[UNPACKED_BUFFER_SIZE];
  msgpack_unpacked_init(&result);
  ret = msgpack_unpack_next(&result, msg_input, msg_input_len, &off);
  while (ret == MSGPACK_UNPACK_SUCCESS) {
    msgpack_object obj = result.data;
    if (obj.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
      *unpacked_id = obj.via.u64;
    }
    msgpack_object_print(stdout, obj);
    printf("\n");
    msgpack_object_print_buffer(unpacked_buffer, UNPACKED_BUFFER_SIZE, obj);

    ret = msgpack_unpack_next(&result, msg_input, msg_input_len, &off);
  }
  msgpack_unpacked_destroy(&result);

  if (ret == MSGPACK_UNPACK_CONTINUE) {
    log_i("All msgpack_object in the buffer is consumed.\n");
  } else if (ret == MSGPACK_UNPACK_PARSE_ERROR) {
    log_e("The data in the buf is invalid format.\n");
  }
  return RESULT_OK;
}


/**
 *
 * @param obj_struct
 * @param serialized_msg_data
 * @param serialized_msg_len
 * @return
 */
int PackVehicleSpeedToMsgBody(SpeedState *obj_struct, char **serialized_msg_data, size_t *serialized_msg_len) {
  msgpack_sbuffer sbuf;
  msgpack_sbuffer_init(&sbuf);

  msgpack_packer packer;
  msgpack_packer_init(&packer, &sbuf, msgpack_sbuffer_write);
  msgpack_pack_uint32(&packer, obj_struct->sampling_time);
  msgpack_pack_uint32(&packer, obj_struct->speed_kmh);
  msgpack_pack_uint32(&packer, obj_struct->pulse_num);
  msgpack_pack_uint32(&packer, obj_struct->reserved);

  if ((*serialized_msg_data) == NULL) {
    *serialized_msg_data = malloc(sbuf.size);
    memcpy(*serialized_msg_data, sbuf.data, sbuf.size);
    *serialized_msg_len = sbuf.size;

    msgpack_sbuffer_destroy(&sbuf);
    return RESULT_OK;
  }

  return RESULT_ERROR;
}


/**
 *
    uint32_t uiSamplingTime;
    uint32_t uiSpeed;
    uint32_t uiPulseNum;
    uint32_t uiReserved;
 * @param serialized_msg_data
 * @param serialized_msg_len
 * @param obj_struct
 * @return
 */
int UnpackMsgBodyToVehicleSpeedStruct(const char *serialized_msg_data, size_t serialized_msg_len, SpeedState **obj_struct) {

  msgpack_object unpacked_object_array[MAX_UNPACKED_ARRAY_SIZE];
  size_t object_size=0;
  if(UnpackMsgBodyToObject(serialized_msg_data, serialized_msg_len, unpacked_object_array, &object_size) == RESULT_ERROR) {
    log_e("UnpackMsgBodyToObject Failed\n");
    return RESULT_ERROR;
  }

  log_i("unpacked object size is %ld\n", object_size);
  if (*obj_struct == NULL) {
    *obj_struct = malloc(sizeof(SpeedState));
    (*obj_struct)->sampling_time = unpacked_object_array[0].via.u64;
    (*obj_struct)->speed_kmh = unpacked_object_array[1].via.u64;
    (*obj_struct)->pulse_num = unpacked_object_array[2].via.u64;
    (*obj_struct)->reserved = unpacked_object_array[3].via.u64;
    return RESULT_OK;
  }

  return RESULT_ERROR;
}


/**
 * PackVehicleIOToMsgBody and UnpackMsgBodyToVehicleIOStruct
 * @param packer
 * @param vehicle_prop_value
 * @return
 */
int PackVehicleIOToMsgBody(IOState *obj_struct, char **serialized_msg_data, size_t *serialized_msg_len) {
  msgpack_sbuffer sbuf;
  msgpack_sbuffer_init(&sbuf);

  msgpack_packer packer;
  msgpack_packer_init(&packer, &sbuf, msgpack_sbuffer_write);

  msgpack_pack_uint8(&packer, obj_struct->car_key);
  msgpack_pack_uint32(&packer, obj_struct->general_io_state);
  ssize_t ssize = sizeof(obj_struct->general_io_type)/sizeof(obj_struct->general_io_type[0]);
  msgpack_pack_array(&packer, ssize);
  for(int i=0; i<ssize; i++){
    msgpack_pack_uint8(&packer, obj_struct->general_io_type[i]);
  }

  msgpack_pack_uint32(&packer, obj_struct->reserved);

  if ((*serialized_msg_data) == NULL) {
    *serialized_msg_data = malloc(sbuf.size);
    memcpy(*serialized_msg_data, sbuf.data, sbuf.size);
    *serialized_msg_len = sbuf.size;

    msgpack_sbuffer_destroy(&sbuf);
    return RESULT_OK;
  }

  return RESULT_ERROR;
}

int UnpackMsgBodyToVehicleIOStruct(const char *serialized_msg_data,
                                   size_t serialized_msg_len,
                                   IOState **obj_struct) {

  msgpack_object unpacked_object_array[MAX_UNPACKED_ARRAY_SIZE];
  size_t object_size=0;
  if(UnpackMsgBodyToObject(serialized_msg_data, serialized_msg_len, unpacked_object_array, &object_size) == RESULT_ERROR) {
    return RESULT_ERROR;
  }

  log_i("unpacked object size is %ld\n", object_size);
  if (*obj_struct == NULL) {
    *obj_struct = malloc(sizeof(IOState));
    (*obj_struct)->car_key = unpacked_object_array[0].via.u64;
    (*obj_struct)->general_io_state = unpacked_object_array[1].via.u64;
    ssize_t ssize = unpacked_object_array[2].via.array.size;
    for(int i=0; i<ssize; i++) {
      (*obj_struct)->general_io_type[i] = unpacked_object_array[2].via.array.ptr[i].via.u64;
    }
    (*obj_struct)->reserved = unpacked_object_array[3].via.u64;
    return RESULT_OK;
  }

  return 0;
}


/**
 * PackGpsInfoStructToMsgBody and UnpackMsgBodyToGpsInfoStruct
 * @param obj_struct
 * @param serialized_msg_data
 * @param serialized_msg_len
 * @return
 */
int PackGpsInfoStructToMsgBody(GpsStatus *obj_struct, char **serialized_msg_data, size_t *serialized_msg_len) {

  msgpack_sbuffer sbuf;
  msgpack_sbuffer_init(&sbuf);

  msgpack_packer packer;
  msgpack_packer_init(&packer, &sbuf, msgpack_sbuffer_write);

  msgpack_pack_uint8(&packer, obj_struct->gps_status);
  msgpack_pack_uint8(&packer, obj_struct->pps_type);
  msgpack_pack_uint8(&packer, obj_struct->move_status);
  msgpack_pack_uint16(&packer, obj_struct->speed_kmh);

  msgpack_pack_uint8(&packer, obj_struct->latitude_degree);
  msgpack_pack_uint8(&packer, obj_struct->latitude_minute);
  msgpack_pack_uint32(&packer, obj_struct->latitude_second);

  msgpack_pack_uint8(&packer, obj_struct->longitude_degree);
  msgpack_pack_uint8(&packer, obj_struct->longitude_minute);
  msgpack_pack_uint32(&packer, obj_struct->longitude_second);

  msgpack_pack_uint8(&packer, obj_struct->direction_latitude);
  msgpack_pack_uint8(&packer, obj_struct->direction_longitude);
  msgpack_pack_uint16(&packer, obj_struct->gps_angle);
  msgpack_pack_int16(&packer, obj_struct->altitude);


  msgpack_pack_uint8(&packer, obj_struct->year);
  msgpack_pack_uint8(&packer, obj_struct->month);
  msgpack_pack_uint8(&packer, obj_struct->day);
  msgpack_pack_uint8(&packer, obj_struct->hour);
  msgpack_pack_uint8(&packer, obj_struct->minute);
  msgpack_pack_uint8(&packer, obj_struct->second);

  msgpack_pack_int32(&packer, obj_struct->reserved);

  if ((*serialized_msg_data) == NULL) {
    *serialized_msg_data = malloc(sbuf.size);
    memcpy(*serialized_msg_data, sbuf.data, sbuf.size);
    *serialized_msg_len = sbuf.size;

    msgpack_sbuffer_destroy(&sbuf);
    return RESULT_OK;
  }

  return RESULT_OK;
}

int UnpackMsgBodyToGpsInfoStruct(const char *serialized_msg_data, size_t serialized_msg_len, GpsStatus **obj_struct) {
  msgpack_object unpacked_object_array[MAX_UNPACKED_ARRAY_SIZE];
  size_t object_size=0;
  if(UnpackMsgBodyToObject(serialized_msg_data, serialized_msg_len, unpacked_object_array, &object_size) == RESULT_ERROR) {
    log_e("UnpackMsgBodyToObject Failed\n");
    return RESULT_ERROR;
  }

  log_i("unpacked object size is %ld\n", object_size);
  if (*obj_struct == NULL) {
    *obj_struct = malloc(sizeof(GpsStatus));
    (*obj_struct)->gps_status = unpacked_object_array[0].via.u64;
    (*obj_struct)->pps_type = unpacked_object_array[1].via.u64;
    (*obj_struct)->move_status = unpacked_object_array[2].via.u64;
    (*obj_struct)->speed_kmh = unpacked_object_array[3].via.u64;

    (*obj_struct)->latitude_degree = unpacked_object_array[4].via.u64;
    (*obj_struct)->latitude_minute = unpacked_object_array[5].via.u64;
    (*obj_struct)->latitude_second = unpacked_object_array[6].via.u64;

    (*obj_struct)->longitude_degree = unpacked_object_array[7].via.u64;
    (*obj_struct)->longitude_minute = unpacked_object_array[8].via.u64;
    (*obj_struct)->longitude_second = unpacked_object_array[9].via.u64;

    (*obj_struct)->direction_latitude = unpacked_object_array[10].via.u64;
    (*obj_struct)->direction_longitude = unpacked_object_array[11].via.u64;
    (*obj_struct)->gps_angle = unpacked_object_array[12].via.u64;
    (*obj_struct)->altitude = unpacked_object_array[13].via.u64;

    (*obj_struct)->year = unpacked_object_array[14].via.u64;
    (*obj_struct)->month = unpacked_object_array[15].via.u64;
    (*obj_struct)->day = unpacked_object_array[16].via.u64;
    (*obj_struct)->hour = unpacked_object_array[17].via.u64;
    (*obj_struct)->minute = unpacked_object_array[18].via.u64;
    (*obj_struct)->second = unpacked_object_array[19].via.u64;

    (*obj_struct)->reserved = unpacked_object_array[20].via.u64;

    return RESULT_OK;
  }

  return RESULT_ERROR;
}

/**
 * UnpackMsgBodyToDialogDisplayStruct and PackDialogDisplayToMsgBody
 * uint8_t ucsize; //数据长度
   uint8_t uctype;  //  1 警告 ; 0正常提示 ; -- uctime 不能为0
   uint8_t uctime;  //0恒定显示手动退出; 1~255显示的时长，到时间自动退出
   uint32_t uiReserved;
 * @param serialized_msg_data
 * @param serialized_msg_len
 * @param obj_struct
 * @return
 */
int PackDialogDisplayToMsgBody(DialogDisplayTask *obj_struct,
                               char **serialized_msg_data,
                               size_t *serialized_msg_len) {
  msgpack_sbuffer sbuf;
  msgpack_sbuffer_init(&sbuf);

  msgpack_packer packer;
  msgpack_packer_init(&packer, &sbuf, msgpack_sbuffer_write);

  msgpack_pack_uint32(&packer, obj_struct->task_type);
  msgpack_pack_uint8(&packer, obj_struct->delay_time);
  msgpack_pack_uint16(&packer, obj_struct->text_length);
  msgpack_pack_uint32(&packer, obj_struct->reserved);

  if (obj_struct->text_length != 0) {
    msgpack_pack_str_with_body(&packer, obj_struct->text, obj_struct->text_length);
  }

  if ((*serialized_msg_data) == NULL) {
    *serialized_msg_data = malloc(sbuf.size);
    memcpy(*serialized_msg_data, sbuf.data, sbuf.size);
    *serialized_msg_len = sbuf.size;

    msgpack_sbuffer_destroy(&sbuf);
    return RESULT_OK;
  }

  return RESULT_ERROR;
}

int UnpackMsgBodyToDialogDisplayStruct(const char *serialized_msg_data,
                                       size_t serialized_msg_len,
                                       DialogDisplayTask **obj_struct) {

  msgpack_object unpacked_object_array[MAX_UNPACKED_ARRAY_SIZE];
  size_t object_size = 0;
  if (UnpackMsgBodyToObject(serialized_msg_data, serialized_msg_len, unpacked_object_array, &object_size)
      == RESULT_ERROR) {
    log_e("UnpackMsgBodyToObject Failed\n");
    return RESULT_ERROR;
  }

  size_t variable_length_string_length = unpacked_object_array[object_size - 1].via.str.size;
  const char *variable_length_string = unpacked_object_array[object_size - 1].via.str.ptr;

  log_i("unpacked object size is %ld\n", object_size);
  if (*obj_struct == NULL) {
    *obj_struct = malloc(sizeof(DialogDisplayTask) + variable_length_string_length);
    (*obj_struct)->task_type = unpacked_object_array[0].via.u64;
    (*obj_struct)->delay_time = unpacked_object_array[1].via.u64;
    (*obj_struct)->text_length = unpacked_object_array[2].via.u64;
    (*obj_struct)->reserved = unpacked_object_array[3].via.u64;
    if (variable_length_string_length != 0) {
      memcpy(((*obj_struct)->text), variable_length_string, variable_length_string_length);
    } else {
      (*obj_struct)->text[0] = 0;
    }
    return RESULT_OK;
  }
  return RESULT_ERROR;
}
/**
 * PackJTProtocolExtendToMsgBody and UnpackMsgBodyToJTProtocolExtend
 * @param serialized_msg_data
 * @param serialized_msg_len
 * @param obj_struct
 * @return
 */
int PackJTProtocolExtendToMsgBody(JTProtocolExtend *obj_struct,
                                  char **serialized_msg_data,
                                  size_t *serialized_msg_len) {

  msgpack_sbuffer sbuf;
  msgpack_sbuffer_init(&sbuf);

  msgpack_packer packer;
  msgpack_packer_init(&packer, &sbuf, msgpack_sbuffer_write);

  msgpack_pack_uint32(&packer, obj_struct->peripheral_id);
  msgpack_pack_uint8(&packer, obj_struct->text_length);
  msgpack_pack_uint32(&packer, obj_struct->priority);

  if (obj_struct->text_length != 0) {
    msgpack_pack_bin_with_body(&packer, obj_struct->text, obj_struct->text_length);
  }

  if ((*serialized_msg_data) == NULL) {
    *serialized_msg_data = malloc(sbuf.size);
    memcpy(*serialized_msg_data, sbuf.data, sbuf.size);
    *serialized_msg_len = sbuf.size;

    msgpack_sbuffer_destroy(&sbuf);
    return RESULT_OK;
  }

  return RESULT_ERROR;
}

int UnpackMsgBodyToJTProtocolExtend(const char *serialized_msg_data,
                                    size_t serialized_msg_len,
                                    JTProtocolExtend **obj_struct) {

  msgpack_object unpacked_object_array[MAX_UNPACKED_ARRAY_SIZE];
  size_t object_size = 0;
  if (UnpackMsgBodyToObject(serialized_msg_data, serialized_msg_len, unpacked_object_array, &object_size)
      == RESULT_ERROR) {
    log_e("UnpackMsgBodyToObject Failed\n");
    return RESULT_ERROR;
  }

  size_t variable_length_string_length = unpacked_object_array[object_size - 1].via.str.size;
  const char *variable_length_string = unpacked_object_array[object_size - 1].via.str.ptr;

  log_i("unpacked object size is %ld\n", object_size);
  if (*obj_struct == NULL) {
    *obj_struct = malloc(sizeof(JTProtocolExtend)+variable_length_string_length);
    (*obj_struct)->peripheral_id = unpacked_object_array[0].via.u64;
    (*obj_struct)->text_length = unpacked_object_array[1].via.u64;
    (*obj_struct)->priority = unpacked_object_array[2].via.u64;
    if (variable_length_string_length != 0) {
      memcpy(((*obj_struct)->text), variable_length_string, variable_length_string_length);
    } else {
      (*obj_struct)->text[0] = 0;
    }
    return RESULT_OK;
  }
  return RESULT_ERROR;
}


/**
 * PackAudioPlayStructToMsgBody and UnpackMsgBodyToAudioPlayStruct
 * @param obj_struct
 * @param serialized_msg_data
 * @param serialized_msg_len
 * @return
 */
int PackAudioPlayStructToMsgBody(AudioPlayTask *obj_struct,
                                 char **serialized_msg_data,
                                 size_t *serialized_msg_len) {
  msgpack_sbuffer sbuf;
  msgpack_sbuffer_init(&sbuf);

  msgpack_packer packer;
  msgpack_packer_init(&packer, &sbuf, msgpack_sbuffer_write);

  msgpack_pack_uint32(&packer, obj_struct->audio_type);
  msgpack_pack_uint8(&packer, obj_struct->audio_volume);
  msgpack_pack_uint8(&packer, obj_struct->speaker_type);
  msgpack_pack_uint8(&packer, obj_struct->handle_type);
  msgpack_pack_uint8(&packer, obj_struct->execute_type);

  if (strlen(obj_struct->text) != 0) {
    msgpack_pack_str_with_body(&packer, obj_struct->text, strlen(obj_struct->text) + 1);
  }

  if ((*serialized_msg_data) == NULL) {
    *serialized_msg_data = malloc(sbuf.size);
    memcpy(*serialized_msg_data, sbuf.data, sbuf.size);
    *serialized_msg_len = sbuf.size;

    msgpack_sbuffer_destroy(&sbuf);
    return RESULT_OK;
  }

  return RESULT_ERROR;
}

int UnpackMsgBodyToAudioPlayStruct(const char *serialized_msg_body,
                                   size_t serialized_msg_body_len,
                                   AudioPlayTask **obj_struct) {
  size_t off = 0;
  int count = 0;
  printf("msg len=%zu\n", serialized_msg_body_len);
  msgpack_unpacked result;
  msgpack_unpacked_init(&result);

  msgpack_object unpacked_object_array[MAX_UNPACKED_ARRAY_SIZE];

  ////iterate unpack operation
  msgpack_unpack_return ret = msgpack_unpack_next(&result, serialized_msg_body, serialized_msg_body_len, &off);
  while ((ret == MSGPACK_UNPACK_SUCCESS)
      && (count < MAX_UNPACKED_ARRAY_SIZE) && (off <= serialized_msg_body_len)) {
    if (result.data.type != MSGPACK_OBJECT_NIL) {
      unpacked_object_array[count].via = result.data.via;
      unpacked_object_array[count].type = result.data.type;
      count++;
    }
    msgpack_object_print(stdout, result.data);
    printf("\n");
    ret = msgpack_unpack_next(&result, serialized_msg_body, serialized_msg_body_len, &off);
  }
  printf("unpack finished\n");

  size_t variable_length_string_length = unpacked_object_array[count - 1].via.str.size;
  const char *variable_length_string = unpacked_object_array[count - 1].via.str.ptr;
  if (*obj_struct == NULL) {
    *obj_struct = malloc(sizeof(AudioPlayTask) + variable_length_string_length);

    (*obj_struct)->audio_type = unpacked_object_array[0].via.u64;
    (*obj_struct)->audio_volume = unpacked_object_array[1].via.u64;
    (*obj_struct)->speaker_type = unpacked_object_array[2].via.u64;
    (*obj_struct)->handle_type = unpacked_object_array[3].via.u64;
    (*obj_struct)->execute_type = unpacked_object_array[4].via.u64;
    if (variable_length_string_length != 0) {
      memcpy(((*obj_struct)->text), variable_length_string, variable_length_string_length);
    } else {
      (*obj_struct)->text[0] = 0;
    }
  }
  msgpack_unpacked_destroy(&result);
  return RESULT_OK;
}

/**
 * PackVehicleTempStructToMsgBody and UnpackMsgBodyToVehicleTempStruct
 * @param obj_struct
 *          int16_t sRoomCentDegreeInt;
            int16_t sRoomCentDegreeDec;
            int16_t sOilCentDegreeInt;
            int16_t sOilCentDegreeDec;
 * @param serialized_msg_data
 * @param serialized_msg_len
 * @return
 */
int PackVehicleTempStructToMsgBody(TemperatureState *obj_struct,
                                   char **serialized_msg_data,
                                   size_t *serialized_msg_len) {
  msgpack_sbuffer sbuf;
  msgpack_sbuffer_init(&sbuf);

  msgpack_packer packer;
  msgpack_packer_init(&packer, &sbuf, msgpack_sbuffer_write);

  msgpack_pack_float(&packer, obj_struct->cabin_centigrade);
  msgpack_pack_float(&packer, obj_struct->oil_centigrade);
  msgpack_pack_uint32(&packer, obj_struct->reserved);

  if ((*serialized_msg_data) == NULL) {
    *serialized_msg_data = malloc(sbuf.size);
    memcpy(*serialized_msg_data, sbuf.data, sbuf.size);
    *serialized_msg_len = sbuf.size;

    msgpack_sbuffer_destroy(&sbuf);
    return RESULT_OK;
  }

  return RESULT_ERROR;
}

int UnpackMsgBodyToVehicleTempStruct(const char *serialized_msg_body,
                                     size_t serialized_msg_body_len,
                                     TemperatureState **obj_struct) {
  size_t off = 0;
  int count = 0;

  log_i("msg len=%ld\n", serialized_msg_body_len);
  msgpack_unpacked result;
  msgpack_unpacked_init(&result);

  msgpack_object unpacked_object_array[MAX_UNPACKED_ARRAY_SIZE];
  size_t object_size = 0;
  if (UnpackMsgBodyToObject(serialized_msg_body, serialized_msg_body_len, unpacked_object_array, &object_size)
      == RESULT_ERROR) {
    log_e("UnpackMsgBodyToObject Failed\n");
    return RESULT_ERROR;
  }

  log_i("unpacked object size is %ld\n", object_size);

  if (*obj_struct == NULL) {
    *obj_struct = malloc(sizeof(TemperatureState));
    (*obj_struct)->cabin_centigrade = unpacked_object_array[0].via.f64;
    (*obj_struct)->oil_centigrade = unpacked_object_array[1].via.f64;
    (*obj_struct)->reserved = unpacked_object_array[2].via.u64;
  }

  msgpack_unpacked_destroy(&result);
  return RESULT_OK;
}


/**
 *
 * @param obj_struct
 * @param serialized_msg_data
 * @param serialized_msg_len
 * @return
 */
int PackAIEventToMsgBody(AIEvent *obj_struct, char **serialized_msg_data, size_t *serialized_msg_len) {
  msgpack_sbuffer sbuf;
  msgpack_sbuffer_init(&sbuf);

  msgpack_packer packer;
  msgpack_packer_init(&packer, &sbuf, msgpack_sbuffer_write);

  msgpack_pack_str_with_body(&packer, obj_struct->event_name, ITEM_NAME_MAX_LENGTH);
  msgpack_pack_uint8(&packer, obj_struct->sensitivity);

  if ((*serialized_msg_data) == NULL) {
    *serialized_msg_data = malloc(sbuf.size);
    memcpy(*serialized_msg_data, sbuf.data, sbuf.size);
    *serialized_msg_len = sbuf.size;

    msgpack_sbuffer_destroy(&sbuf);
    return RESULT_OK;
  }

  return RESULT_ERROR;
}

int UnpackMsgBodyToAIEvent(const char *serialized_msg_data, size_t serialized_msg_len, AIEvent **obj_struct) {
  msgpack_object unpacked_object_array[MAX_UNPACKED_ARRAY_SIZE];
  size_t object_size = 0;
  if (UnpackMsgBodyToObject(serialized_msg_data, serialized_msg_len, unpacked_object_array, &object_size)
      == RESULT_ERROR) {
    log_e("UnpackMsgToVehicleVoltageStruct Failed\n");
    return RESULT_ERROR;
  }

  log_i("unpacked object size is %ld\n", object_size);
  if (*obj_struct == NULL) {
    *obj_struct = malloc(sizeof(AIEvent));
    if (unpacked_object_array[0].via.str.size != 0) {
      memcpy(((*obj_struct)->event_name), unpacked_object_array[0].via.str.ptr, ITEM_NAME_MAX_LENGTH);
      (*obj_struct)->sensitivity = unpacked_object_array[1].via.u64;
    }
    return RESULT_OK;
  }

  return RESULT_ERROR;
}



/**
 *
 * @param serialized_msg_body
 * @param serialized_msg_body_len
 * @param unpacked_object_array
 * @return
 */
int UnpackMsgBodyToObject(const char *serialized_msg_body,
                          size_t serialized_msg_body_len,
                          msgpack_object *unpacked_object_array,
                          size_t* object_size) {

  log_i("msg len=%ld\n", serialized_msg_body_len);
  if(unpacked_object_array == NULL){
    log_e("unpacked_object_array is NUL\n");
    return RESULT_ERROR;
  }

  size_t off = 0;
  int count = 0;

  msgpack_unpacked result;
  msgpack_unpacked_init(&result);

  msgpack_unpack_return ret = msgpack_unpack_next(&result, serialized_msg_body, serialized_msg_body_len, &off);
  while ((ret == MSGPACK_UNPACK_SUCCESS)
      && (count < MAX_UNPACKED_ARRAY_SIZE) && (off <= serialized_msg_body_len)) {
    if (result.data.type != MSGPACK_OBJECT_NIL) {
      unpacked_object_array[count].via = result.data.via;
      unpacked_object_array[count].type = result.data.type;
      count++;
    }
    msgpack_object_print(stdout, result.data);
    printf("\n");
    ret = msgpack_unpack_next(&result, serialized_msg_body, serialized_msg_body_len, &off);
  }

  log_i("unpack finished\n");
  msgpack_unpacked_destroy(&result);
  *object_size = count;
  return RESULT_OK;
}





