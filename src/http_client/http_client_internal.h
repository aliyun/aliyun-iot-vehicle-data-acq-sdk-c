//
// Created by root on 2021/4/2.
//

#ifndef TBOX_DRIVER__HTTP_CLIENT_UTIL_H_
#define TBOX_DRIVER__HTTP_CLIENT_UTIL_H_

//#include "curl/curl.h"
#include "curl/curl.h"

typedef struct  struct_ResponseMemory{
  char *response;
  size_t size;
}ResponseMemory;


/**
 *
 * @param url
 * @param content
 * @param size
 * @return
 */
CURLcode curl_post_req(char *url, char *content, size_t size);


#endif //TBOX_DRIVER__HTTP_CLIENT_UTIL_H_


//int send_curl_req(char *url,
//                  struct CBC *cbc,
//                  const char *cipher_suite,
//                  int proto_version);


//
//struct CBC {
//  char *buf;
//  size_t pos;
//  size_t size;
//};
//
//
//struct WriteThis {
//  const char *readptr;
//  size_t sizeleft;
//};
