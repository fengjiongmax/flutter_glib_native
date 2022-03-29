#include <android/log.h>

#include "native_register.h"

#include "dart_api/dart_api.h"
#include "dart_api/dart_api_dl.h"
#include "dart_api/dart_native_api.h"

Dart_Port dart_port;

GMutex mutex;
GCond cond;

GThread *main_thread;

native_run_func main_thread_run_func;

void register_log_func();

void write_log(gchar *msg, GLogLevelFlags log_level)
{
    g_log_structured("register", log_level, "MESSAGE", msg);
}

intptr_t native_initialize_dart_api(gpointer data)
{
    return Dart_InitializeApiDL(data);
}

void native_register_dart_port(Dart_Port value)
{
    register_log_func();

    main_thread = g_thread_self();

    write_log("Registering dart port", G_LOG_LEVEL_INFO);
    dart_port = value;
    g_mutex_init(&mutex);
    g_cond_init(&cond);
}

void native_register_main_thread_callback(native_run_func func)
{
    main_thread_run_func = func;
}

void run_func_threaded(NativeFuncParam *params)
{
    Dart_CObject dart_param;

    dart_param.type = Dart_CObject_kInt64;
    dart_param.value.as_int64 = (intptr_t)params;

    write_log("Posting to dart...", G_LOG_LEVEL_INFO);

    const gboolean result = Dart_PostCObject_DL(dart_port, &dart_param);

    if (!result)
    {
        write_log("Error posting message from C to dart", G_LOG_LEVEL_ERROR);
    }

    g_mutex_lock(&mutex);
    while (!params->finished)
    {
        g_cond_wait(&cond, &mutex);
    }
    g_mutex_unlock(&mutex);
}

void run_func_main_thread(NativeFuncParam *params)
{
    main_thread_run_func(params);
}

void run_func(NativeFuncParam *params)
{
    if (main_thread == g_thread_self())
    {
        run_func_main_thread(params);
    }
    else
    {
        run_func_threaded(params);
    }
}

gpointer work_threaded_func(gpointer data)
{
    write_log("Work in another thread", G_LOG_LEVEL_INFO);
    NativeFuncParam *params = malloc(sizeof(NativeFuncParam));
    params->finished = FALSE;
    params->func_type = HELLO;

    run_func(params);

    free(params);
    return NULL;
}

gpointer work_param_func(gpointer user_data)
{
    write_log("Work with parameter", G_LOG_LEVEL_INFO);

    NativeFuncParam *params = malloc(sizeof(NativeFuncParam));
    params->finished = FALSE;
    params->func_type = SAY;

    params->params[0] = user_data;

    run_func(params);

    free(params);
    return NULL;
}

void native_start_work()
{
    write_log("Registering work", G_LOG_LEVEL_INFO);
    g_thread_new("Thread 1", work_threaded_func, NULL);
    g_thread_new("Thread 2", work_param_func, "Threaded call!!!");
    work_param_func("Calling from main thread");

    write_log("Starting g main loop", G_LOG_LEVEL_INFO);
    g_main_loop_new(NULL, TRUE);
    write_log("Work finished", G_LOG_LEVEL_INFO);
}

void native_work_finished(NativeFuncParam *param)
{
    g_mutex_lock(&mutex);
    param->finished = TRUE;
    g_cond_signal(&cond);
    g_mutex_unlock(&mutex);
}


GLogWriterOutput
log_writer(
    GLogLevelFlags log_level,
    const GLogField *fields,
    gsize n_fields,
    gpointer user_data)
{
    gchar *message = NULL;

    for (int i = 0; i < n_fields; i++)
    {
        if (strcmp(fields[i].key, "MESSAGE") == 0)
        {
            message = strdup(fields[i].value);
        }
    }

    __android_log_write(ANDROID_LOG_INFO, "native", message);
}

void register_log_func()
{
    g_log_set_writer_func(log_writer, NULL, NULL);
}