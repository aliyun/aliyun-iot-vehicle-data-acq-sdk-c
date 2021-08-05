//
// Created by root on 2021/4/1.
//
#include <memory.h>
#include <log.h>
#include "http_client_internal.h"
#include "stdlib.h"

ResponseMemory chunk;

static int is_merge_data = 0;
static size_t cb(void *data, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  ResponseMemory *mem = (ResponseMemory *)userp;

  if(is_merge_data) {
    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if (ptr == NULL)
      return 0;  /* out of memory! */

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;
    log_i("\nreceived: %s\n", mem->response);

  }else{
    if(mem->response != NULL){
      free(mem->response);
    }

    mem->response = malloc(realsize);
    mem->size = realsize;
    memcpy(mem->response, data, realsize);

    log_i("\nreceived: %s\n", mem->response);
  }
  return realsize;
}


/**
 *
 */
CURLcode curl_post_req(char *url, char *content, size_t size)
{
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content);

    /* if we don't provide POSTFIELDSIZE, libcurl will strlen() by
       itself */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, size);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);


    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }
    /* always cleanup */
    curl_easy_cleanup(curl);
    return res;
  }
  return CURLE_FAILED_INIT;
}
