//
//  AsyncLog.c
//  Ctest
//
//  Created by Yanjiu Huang on 3/24/14.
//  Copyright (c) 2014 Yanjiu Huang. All rights reserved.
//

#include <stdio.h>
#include <stdarg.h>

#include "AsyncLog.h"

void async_log_error(const char* format, ...){
    va_list arg;
    
    va_start(arg, format);
    vfprintf(stderr, format, arg);
    va_end(arg);
}

void async_log_debug(const char* format, ...){
    va_list arg;
    
    va_start(arg, format);
    vfprintf(stdout, format, arg);
    va_end(arg);
}

void async_log_info(const char* format, ...){
    va_list arg;
    
    va_start(arg, format);
    vfprintf(stdout, format, arg);
    va_end(arg);
}