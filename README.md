# Overview
This SDK was used for the eco-system partners which provided the video surveillance terminal of commercial vehicle. It is not suitable for the general-purpose of vehicle data acquisition    
1. Provided a standard and extensive definition for data structure of data acquisition, especially for commercial vehicle, such as GPS, Speed, Voltage and so forth;
1. Designed a well-packaged communication framework between the hardware drivers and user application in this SDK. Several callback interface was pre-defined,  the developers of hardware drivers only need to customerize these callback functions according to different hardware functions. All data-exchange and validation are packaged efficiently and securely;
1. Plain C language programming friendly and compatible for all Linux environment and various CPU archs. following google c/c++ style




---

# Architecture
Entities: Rule Engine App, Vehicle Property Acq App, Vehicle Proerpty Acq SDK
Callback Interface(needed to be implement by User): 

- vehicle_prop_get_cb;
- vehicle_prop_set_cb;
- vehicle_prop_release_memory;





![image.png](https://intranetproxy.alipay.com/skylark/lark/0/2021/png/88890/1619576148412-3d42ece3-8fb7-4fc1-bf56-2a05bfe5d58e.png#align=left&display=inline&height=560&id=QoWwZ&margin=%5Bobject%20Object%5D&name=image.png&originHeight=1119&originWidth=1575&size=137854&status=done&style=none&width=787.5)

---

# Data Structure
## Vehicle Property Definition
```c
/**
 * Encapsulates the property name and the associated value. It
 * is used across various API calls to set values, get values or
 * to notify values changed.
 */
typedef struct vehicle_property {
  /** Property identifier */
  int prop_id;

  /** Time is elapsed nanoseconds since boot */
  long timestamp;

  /** The length of the property body */
  size_t length;

  /** The property body */
  char body[0];
} vehicle_property_t;
```


## Data Mapping Relation 
![image.png](https://intranetproxy.alipay.com/skylark/lark/0/2021/png/88890/1619578937991-bdf82e58-8954-40bf-b50c-256dcbf982a7.png#align=left&display=inline&height=365&id=Vn4YL&margin=%5Bobject%20Object%5D&name=image.png&originHeight=634&originWidth=1373&size=64225&status=done&style=none&width=790)


## 

---

# Inteface 
## Callback Definition
```c
typedef int (*vehicle_property_set_callback)(const vehicle_property_t *data);
typedef int (*vehicle_property_get_callback)(vehicle_property_t *data);
typedef void (*vehicle_release_memory_from_get_callback)(vehicle_property_t *data);

typedef struct vehicle_callbacks {
  /** set to sizeof(vehicle_callbacks_t) */
  size_t size;
  vehicle_property_get_callback vehicle_get_cb;
  vehicle_property_set_callback vehicle_set_cb;
  vehicle_release_memory_from_get_callback vehicle_release_memory_from_get_cb;
} vehicle_callbacks_t;
```


```c
/**
 * Init the vehicle client.
 */
int vehicle_init(vehicle_callbacks_t callbacks);

/**
 * Releases resources that created in vehicle_init().
 */
int vehicle_release();

/**
 * Suggests that an vehicle property has changed.
 *
 * data is OWNED by the caller, who is responsible for releasing
 * the memory after the method returns.
 */
int vehicle_prop_changed(int property_id, const void *property_data_struct, size_t property_data_struct_length);


```

---

# File Struct
Totally, there are ten files, detailed descriptions was shown as below.

| File Name | Description |
| --- | --- |
| htt_client_internal.c | a internal http client implement. |
| htt_client_internal.h |  |
| http_server_internal.c | a internal http server implement. |
| http_server_internal.h |  |
| msgpack_util.c | msg pack utility |
| msgpack_util.h |  |
| vehicle_property_acq.c | inteface definition and export |
| vehicle_proerpty_acq.h |  |
| driver_app_demo.c |  example |
| safety_engine_demo.c | example |


---

# Compiling Procedure
## Compiling Tool
| cmake | 3.5.1+ |
| --- | --- |
| gcc | 5.4.0+ |

## Dependency Library
| libcurl | 7.77.0 |
| --- | --- |
| libmicrohttpd | 0.9.72 |
| msgpack | 3.3.0 |

## Compiling Procedure
```bash
#compile the dependency library by Makefile in third-party
make -C third-party -f Makefile prepare

#compile project and example of CMakeList.txt
cmake . && make

```
the operation of cross-platfrom compiling could be executed by ./build-cross-compile.sh


---

# Licensing
MIT 










