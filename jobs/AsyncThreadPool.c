//
//  AsyncThreadPool.c
//  Ctest
//
//  Created by Yanjiu Huang on 3/24/14.
//  Copyright (c) 2014 Yanjiu Huang. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "AsyncThreadPool.h"
#include "AsyncLog.h"

#define MAX_ASYNC_THREAD_NUM 256

async_thread_t* tasks[MAX_ASYNC_THREAD_NUM];
static int async_thread_base_id = 0;
static pthread_mutex_t tasks_lock = PTHREAD_MUTEX_INITIALIZER;

static void* __run_async_job(void* data){
    
    async_thread_t* thread = (async_thread_t*)data;
    thread->state = ASYNC_THREAD_RUNNING_STATE;
    
    if(thread->job && thread->job->handle){
        
        thread->job->handle(thread->job->data);
        
        if(thread->callback){
            thread->state = ASYNC_THREAD_DONE_STATE;
            thread->callback(thread->job);
        }
        
    }
    
    return NULL;
}

void init_async_thread_pool(int initSize){
    
    memset(tasks, 0, sizeof(tasks));
}

async_thread_t* create_new_async_thread(async_job_t* job, async_thread_callback_t callback){
    
    async_thread_t* thread = (async_thread_t*)malloc(sizeof(async_thread_t));
    
    if(!thread){
        return NULL;
    }
    
    // need to create the new instance of ptread_t
    thread->job = job;
    thread->callback = callback;
    thread->state = ASYNC_THREAD_INIT_STATE;
    
    return thread;
}

int add_async_thread(async_thread_t* thread){
    
    int c;
    
    if(thread == NULL){
        async_log_error("The async thread to be added is null\n");
        return -1;
    }
    
    
    
    if(async_thread_base_id >= MAX_ASYNC_THREAD_NUM){
        async_log_error("Exceed the max async task number: %d\n", MAX_ASYNC_THREAD_NUM);
        return -1;
    }
    
    thread->async_id = async_thread_base_id++;
    
    pthread_mutex_lock(&tasks_lock);
    tasks[thread->async_id] = thread;
    pthread_mutex_unlock(&tasks_lock);
    
    // initialize the pthread for this job
    thread->pthread = (pthread_t*)malloc(sizeof(pthread_t));
    if(thread->pthread == NULL){
        async_log_error("Can't create the internal thread for our job: %d", thread->async_id);
        return -1;
    }
    
    // currently we just setup the pthread attribute to NULL
    c = pthread_create(thread->pthread, NULL, __run_async_job, thread);
    if(c < 0){
        async_log_error("Failed to create the pthread_t\n");
        return -1;
    }
    
    return 0;
}

int extend_async_thread_pool(){
    return 0;
}

int shrink_async_thread_pool(){
    return 0;
}

int schedule_async_task(async_job_t* job, async_thread_callback_t callback){
    
    async_thread_t* t = create_new_async_thread(job, callback);
    if(add_async_thread(t)){
        async_log_error("Can't schedule the async task\n");
        return -1;
    }
    
    return 0;
}

int close_async_thread_pool(){
    return 0;
}

