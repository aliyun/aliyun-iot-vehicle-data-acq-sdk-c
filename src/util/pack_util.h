//
// Created by root on 2021/4/14.
//

#ifndef TBOX_DRIVER_MSGPACK_MSGPACK_UTIL_H_
#define TBOX_DRIVER_MSGPACK_MSGPACK_UTIL_H_

#include <stdio.h>
#include <vehicle_property_acq.h>
#include <msgpack.h>


#define UNPACKED_BUFFER_SIZE            256
#define MAX_UNPACKED_ARRAY_SIZE         40

/**
 * PackVehiclePropertyToMsg and UnpackMsgToVehicleProperty
 * @param msg_input
 * @param msg_input_len
 * @param vehicle_property_pp
 * @return
 */
int PackVehiclePropertyToMsg(int property_id,
                             long timestamp,
                             size_t vehicle_prop_struct_length,
                             const void *vehicle_prop_struct,
                             char **serialized_msg_data,
                             size_t *serialized_msg_data_len);
int UnpackMsgToVehicleProperty(const char *msg_input,
                               size_t msg_input_len,
                               VehicleProperty **vehicle_property_pp);


/**
 * 0x1002
 * PackAIEventToMsgBody and UnpackMsgBodyToAIEvent
 * @param obj_struct
 * @param serialized_msg_data
 * @param serialized_msg_len
 * @return
*/
int PackAIEventToMsgBody(AIEvent *obj_struct,
                         char **serialized_msg_data,
                         size_t *serialized_msg_len);
int UnpackMsgBodyToAIEvent(const char *serialized_msg_data,
                           size_t serialized_msg_len,
                           AIEvent **obj_struct);


/**
 * 0x00001015 VEHICLE_PROPERTY_VEHICLE_SPEED_INFO
 */
int PackVehicleSpeedToMsgBody(SpeedState *obj_struct,
                              char **serialized_msg_data,
                              size_t *serialized_msg_len);
int UnpackMsgBodyToVehicleSpeedStruct(const char *serialized_msg_data,
                                      size_t serialized_msg_len,
                                      SpeedState **obj_struct);

/**
 * 0x00001016 VEHICLE_PROPERTY_VEHICLE_IO_INFO
 * @param obj_struct
 * @param serialized_msg_data
 * @param serialized_msg_len
 * @return
 */
int PackVehicleIOToMsgBody(IOState *obj_struct,
                           char **serialized_msg_data,
                           size_t *serialized_msg_len);
int UnpackMsgBodyToVehicleIOStruct(const char *serialized_msg_data,
                                   size_t serialized_msg_len,
                                   IOState **obj_struct);

/**
 * 0x00001017 VEHICLE_PROPERTY_VEHICLE_TEMP_INFO
 * PackVehicleTempStructToMsgBody and UnpackMsgBodyToVehicleTempStruct
 * @param obj_struct
 * @param serialized_msg_data
 * @param serialized_msg_len
 */
int PackVehicleTempStructToMsgBody(TemperatureState *obj_struct,
                                   char **serialized_msg_data,
                                   size_t *serialized_msg_len);
int UnpackMsgBodyToVehicleTempStruct(const char *serialized_msg_data,
                                     size_t serialized_msg_len,
                                     TemperatureState **obj_struct);

/**
 * PackGpsInfoStructToMsgBody and UnpackMsgBodyToGpsInfoStruct
 * 0x00001005 VEHICLE_PROPERTY_GPS_INFO
 */
int PackGpsInfoStructToMsgBody(GpsStatus *obj_struct,
                               char **serialized_msg_data,
                               size_t *serialized_msg_len);

int UnpackMsgBodyToGpsInfoStruct(const char *serialized_msg_data,
                                 size_t serialized_msg_len,
                                 GpsStatus **obj_struct);

/**
 * Property_ID = 0x1101
 * UnpackMsgBodyToAudioPlayStruct and PackAudioPlayStructToMsgBody
 *
 */
int PackAudioPlayStructToMsgBody(AudioPlayTask *obj_struct,
                                 char **serialized_msg_data,
                                 size_t *serialized_msg_len);
int UnpackMsgBodyToAudioPlayStruct(const char *serialized_msg_body,
                                   size_t serialized_msg_body_len,
                                   AudioPlayTask **obj_struct);

/**
 * PackDialogDisplayToMsgBody and UnpackMsgBodyToDialogDisplayStruct
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
                               size_t *serialized_msg_len);
int UnpackMsgBodyToDialogDisplayStruct(const char *serialized_msg_data,
                                       size_t serialized_msg_len,
                                       DialogDisplayTask **obj_struct);

/**
 * UnpackMsgBodyToJTProtocolExtend and
 * @param serialized_msg_data
 * @param serialized_msg_len
 * @param obj_struct
 * @return
 */
int PackJTProtocolExtendToMsgBody(JTProtocolExtend *obj_struct,
                                  char **serialized_msg_data,
                                  size_t *serialized_msg_len);

int UnpackMsgBodyToJTProtocolExtend(const char *serialized_msg_data,
                                    size_t serialized_msg_len,
                                    JTProtocolExtend **obj_struct);


/**
 *
 * @param serialized_msg_body
 * @param serialized_msg_body_len
 * @param unpacked_object_array
 * @return
 */
int UnpackMsgBodyToObject(const char *serialized_msg_body,
                          size_t serialized_msg_body_len,
                          msgpack_object* unpacked_object_array,
                          size_t* object_size);

#endif //TBOX_DRIVER_MSGPACK_MSGPACK_UTIL_H_
