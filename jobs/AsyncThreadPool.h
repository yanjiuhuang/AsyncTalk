//
//  AsyncThreadPool.h
//  Ctest
//
//  Created by Yanjiu Huang on 3/24/14.
//  Copyright (c) 2014 Yanjiu Huang. All rights reserved.
//

#ifndef Ctest_AsyncThreadPool_h
#define Ctest_AsyncThreadPool_h

#include <pthread.h>

#ifdef __cplusplus
extern "C"{
#endif
    
    #define ASYNC_THREAD_INIT_STATE     0
    #define ASYNC_THREAD_READY_STATE    1
    #define ASYNC_THREAD_RUNNING_STATE  2
    #define ASYNC_THREAD_DONE_STATE     3
    #define ASYNC_THREAD_ERROR_STATE    4
    #define ASYNC_THREAD_SUSPEND_STATE  5

    typedef void* (*async_thread_handle_t)(void*);
    
    
    typedef struct async_job{
        async_thread_handle_t   handle;
        void*                   data;
        void*                   result;
        int                     state;
    }async_job_t;
    
    typedef void (*async_thread_callback_t)(async_job_t* job);
    
    typedef struct async_thread{
        int                         async_id;
        pthread_t*                  pthread;
        int                         state;
        
        async_job_t*                job;
        async_thread_callback_t     callback;
        
    }async_thread_t;
    
    

    void init_async_thread_pool(int initSize);
    async_thread_t* create_new_async_thread(async_job_t* job, async_thread_callback_t callback);
    int add_async_thread(async_thread_t* thread);
    int extend_async_thread_pool();
    int shrink_async_thread_pool();
    
    int schedule_async_task(async_job_t* job, async_thread_callback_t callback);
    
    int close_async_thread_pool();
    
    
    
#ifdef __cplusplus
}
#endif

#endif
