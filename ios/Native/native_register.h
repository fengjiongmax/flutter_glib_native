#ifndef NATIVE_REGISTER_H
#define NATIVE_REGISTER_H

#include <glib.h>

#include "dart_api/dart_native_api.h"

typedef enum
{
    HELLO,
    SAY
} NativeFuncTypes;

typedef struct
{
    NativeFuncTypes func_type;
    gpointer params[10];
    gboolean finished;
} NativeFuncParam;

typedef void (*native_run_func)(NativeFuncParam *);

DART_EXPORT intptr_t native_initialize_dart_api(gpointer data);
void native_register_dart_port(Dart_Port value);
void native_work_finished(NativeFuncParam *param);
void native_start_work();
void native_register_main_thread_callback(native_run_func func);

#endif // NATIVE_REGISTER_H