//
// Created by Eason on 2021/4/1.
//
#ifndef TBOX_DRIVER__VEHICLE_PROPERTY_ACQ_H_
#define TBOX_DRIVER__VEHICLE_PROPERTY_ACQ_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "global.h"


// Property ID definitions.
/**
 * VIN of vehicle.
 */
#define VEHICLE_PROPERTY_INFO_VIN                                 (0x00000100)

/**
 * Maker name of vehicle.
 */
#define VEHICLE_PROPERTY_INFO_MAKE                                (0x00000101)

/**
 * Model of vehicle.
 */
#define VEHICLE_PROPERTY_INFO_MODEL                                 (0x00000102)

#define VEHICLE_PROPERTY_AI_EVENT_STATUS                            (0x00001002)
#define VEHICLE_PROPERTY_GPS_STATUS                                 (0x00001005)
#define VEHICLE_PROPERTY_SPEED_STATUS                               (0x00001015)
#define VEHICLE_PROPERTY_IO_STATUS                                  (0x00001016)
#define VEHICLE_PROPERTY_TEMPERATURE_STATUS                         (0x00001017)
#define VEHICLE_PROPERTY_VOLTAGE_STATUS                             (0x00001018)
#define VEHICLE_PROPERTY_TTS_INFO                                   (0x00001101)
#define VEHICLE_PROPERTY_OSD_INFO                                   (0x00001103)
#define VEHICLE_PROPERTY_JT_PROTOCOL_INFO                           (0x00001120)


/**
 * Encapsulates the property name and the associated value. It
 * is used across various API calls to set values, get values or
 * to notify values changed.
 */
typedef struct tag_VehicleProperty {
  /** Property identifier */
  int prop_id;

  /** Time is elapsed nanoseconds since boot */
  long timestamp;

  /** The length of the property body */
  size_t length;

  /** The property body */
  char* body;
} VehicleProperty;

/**
 * Get a vehicle property value immediately. data should be allocated
 * properly. The caller of the API OWNS the data field.
 * Caller will set data->prop, but the implementation needs to fill all entries
 * properly when returning.For data->value, the implementation should allocate
 * necessary memory and caller is responsible for calling release_memory_from_get,
 * which allows the implementation to release allocated memory.
 */
typedef int (*VehiclePropertySetCallback)(const VehicleProperty *data);
typedef int (*VehiclePropertyGetCallback)(VehicleProperty *data);
typedef void (*VehicleReleaseMemoryFromGetCallback)(VehicleProperty *data);

typedef struct tag_VehicleCallbacks {
  /** set to sizeof(vehicle_callbacks_t) */
  size_t size;
  VehiclePropertyGetCallback vehicle_get_cb;
  VehiclePropertySetCallback vehicle_set_cb;
  VehicleReleaseMemoryFromGetCallback vehicle_release_memory_from_get_cb;
} VehicleCallbacks;


/**
 * Init the vehicle client.
 */
int VehicleInit(VehicleCallbacks callbacks);

/**
 * Releases resources that created in VehicleInit().
 */
int VehicleRelease();

/**
 * Suggests that an vehicle property has changed.
 *
 * data is OWNED by the caller, who is responsible for releasing
 * the memory after the method returns.
 */
int VehiclePropChanged(int property_id, const void *property_data_struct, size_t property_data_struct_length);

/**
 * GPS Message
 */
typedef struct struct_GpsStatus{
  uint8_t gps_status;/* status: 0: valid; 1: invalid; 2: none */
  uint8_t gps_type;/* GPS data source 0:GPS 1:BEIDOU 2:GLONASS 3:Galileo */
  uint8_t move_status;/*0: normal; 1: drift; 2: invalid*/
  uint16_t speed_kmh;				/* speedMap unit: KM/H */

  uint8_t latitude_degree; 			/* latitude value [0-89] degree */
  uint8_t latitude_minute; 			/* latitude value [0-59] minute */
  uint32_t latitude_second; 		/* latitude value [0-59] second*/

  uint8_t longitude_degree; 		/* longitude value [0-179] degree */
  uint8_t longitude_minute;	 		/* longitude value [0-59] minute */
  uint32_t longitude_second; 		/* longitude value [0-59] second */

  uint8_t direction_latitude; 		/* lattitude direction 0: North latitude 1: South latitude */
  uint8_t direction_longitude;		/* longitude direction 0: East Longitude 1: West Longitude */
  uint16_t gps_angle; 				/* GPS angle unit */
  int16_t altitude;			/* altitude, height above sea level */

  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;

  int32_t reserved;
}GpsStatus;


/*
** Vehcile temperature message
*/
typedef struct struct_TemperatureState{
  float cabin_centigrade;
  float oil_centigrade;
  uint32_t reserved;
}TemperatureState;


/*
** Vehcile Speed message
*/
typedef struct struct_SpeedState{
  uint32_t sampling_time; /* in milisecond */
  uint32_t speed_kmh; /* km/h */
  uint32_t pulse_num; /* pulse num per second for pulse encoder */
  uint32_t reserved;
}SpeedState;

/*
**
** Vehicle key state message
**
*/
typedef struct struct_VehicleIOState{
  uint8_t car_key; /* Car key update 0:close 1:open*/
  uint32_t general_io_state;          /* I/O */
  uint8_t  general_io_type[32];       /* I/O type */
  uint32_t reserved;
}IOState;

/**
* Vehicle audio operation message
*/
typedef struct struct_AudioPlayTask
{
  uint32_t     audio_type;          /*wav file or TTS text*/
  uint8_t      audio_volume;
  uint8_t      speaker_type;
  uint8_t      handle_type;
  uint8_t      execute_type;
  uint16_t     text_length; 		/*length of text type*/
  char         text[0];             /*TTS TEXT, or wav file path */
}AudioPlayTask;

/**
 * Vehicle dialog display operation message
 */
typedef struct struct_DialogDisplayTask
{
  uint8_t task_type;
  uint8_t delay_time;
  uint16_t text_length; 	/* length of text*/
  uint32_t reserved;
  char text[0];             /* content of text*/
}DialogDisplayTask;

/**
 * Vehicle JT808 protocoloperation message
 */
typedef struct struct_JTProtocolExtend
{
  uint8_t  peripheral_id; 	/* Id for Extend Message for */
  uint16_t text_length; 	/* length of message */
  uint8_t  priority;		/*priority 0:normal  1:urgent*/
  char text[0];			    /* content of message，and its length is equalt to text_length*/
}JTProtocolExtend;

/**
 * Vehicle AI event operation message
 */
typedef struct struct_AIEvent
{
  char     event_name[ITEM_NAME_MAX_LENGTH]; /*AI event name which was identified by IPC */
  uint8_t  sensitivity;                      /*sensitivity of AI event */
}AIEvent;

/**
 * Vehicle  parameter setting message
 */
typedef struct struct_SettingParam
{
  char    param_name[ITEM_NAME_MAX_LENGTH]; /* parameter name for setting  */
  uint8_t param_type; // 0:u32; 1:int32; 2:float
  union {
    uint32_t u32t;
    int32_t  i32t;
    float    f64t;
  } param_value;
}SettingParam;

#endif //TBOX_DRIVER__VEHICLE_PROPERTY_ACQ_H_
