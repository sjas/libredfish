//----------------------------------------------------------------------------
// Copyright Notice:
// Copyright 2018 DMTF. All rights reserved.
// License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libredfish/blob/master/LICENSE.md
//----------------------------------------------------------------------------

/**
 * @file internal_service.h
 * @author Patrick Boyd
 * @brief File containing the interface for the internal redfish service representation.
 *
 * This file explains the interface for the internal redfish service representation.
 */
#ifndef _INT_SERVICE_H_
#define _INT_SERVICE_H_

#include <stdbool.h>

#include <jansson.h>
#include <curl/curl.h>
#include "queue.h"

/**
 * @brief A redfish service.
 *
 * A strcuture defining redfish service data.
 */
typedef struct _redfishService {
    /** The host, including protocol schema **/
    char* host;
    /** The queue of asynchronous HTTP(s) requests **/
    queue* queue;
    /** The thread running asynchronous HTTP(s) requests **/
    thread asyncThread;
    /** The non-async CURL implementation **/
    CURL* curl;
    /** A json object containing all Redfish versions supported by this service **/
    json_t* versions;
    /** 
     * Flags about this service 
     *
     * @see REDFISH_FLAG_SERVICE_NO_VERSION_DOC
     **/
    unsigned int flags;
    /** 
     * A redfish session token. If set this will be used for authentication 
     *
     * @see bearerToken
     * @see otherAuth
     */
    char* sessionToken;
    /** 
     * A bearer token. If set and sessionToken is not this will be used
     * for authentication.
     *
     * @see sessionToken
     * @see otherAuth
     */
    char* bearerToken;
    /** 
     * Raw authorization field (usually basic auth). This is the last resort.
     *
     * @see sessionToken
     * @see bearerToken
     */
    char* otherAuth;
    /** A lock to regulate access to this struct **/
#ifdef _MSC_VER
    HANDLE mutex;
#else
    pthread_mutex_t mutex;
#endif
    /** A reference count for this structure. Once this reaches 0 it will be freed **/
    size_t refCount;
    /** An indicator to the async thread to terminate itself **/
    bool selfTerm;
} redfishService;


#endif
