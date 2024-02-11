#include "native_binding.h"
#include <gst/gst.h>
#include <jni.h>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

#ifdef IOS
#include "gst_ios_init.h"
#endif

static JavaVM *java_vm;

typedef struct _FltGstData
{
    GstElement *pipeline;
    GstElement *audiotestsrc;
    GstElement *audioconvert;
    GstElement *autoaudiosink;

    ANativeWindow *native_window;

} FltGstData;

FltGstData *data;

FFI_PLUGIN_EXPORT void init(void)
{
#ifdef IOS
    gst_ios_init();
#endif
    // init
    gst_init(NULL, NULL);
    data = (FltGstData *)malloc(sizeof(FltGstData));
}

FFI_PLUGIN_EXPORT void setup_pipeline(void)
{
    // setup pipeline
    data->audiotestsrc = gst_element_factory_make("audiotestsrc", NULL);
    data->audioconvert = gst_element_factory_make("audioconvert", NULL);
    data->autoaudiosink = gst_element_factory_make("autoaudiosink", NULL);
    data->pipeline = gst_pipeline_new(NULL);

    if (!data->audiotestsrc || !data->audioconvert || !data->autoaudiosink || !data->pipeline)
    {
        g_printerr("Elements could not be created.\n");
        gst_object_unref(data->pipeline);
        return;
    }

    gst_bin_add_many(GST_BIN(data->pipeline), data->audiotestsrc, data->audioconvert, data->autoaudiosink, NULL);
    if (!gst_element_link_many(data->audiotestsrc, data->audioconvert, data->autoaudiosink, NULL))
    {
        g_printerr("Elements could not be created.\n");
        gst_object_unref(data->pipeline);
        return;
    }
}

FFI_PLUGIN_EXPORT void set_video_surface(gpointer surface)
{
    // ANativeWindow *new_native_window = ANativeWindow_fromSurface()
}

FFI_PLUGIN_EXPORT void start_pipeline(void)
{
    // start pipeline
    GstStateChangeReturn ret;
    ret = gst_element_set_state(data->pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr("Elements could not be created.\n");
        gst_object_unref(data->pipeline);
        return;
    }
}
static JNINativeMethod native_methods[] = {
};
/* Library initializer */
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    java_vm = vm;

    if ((*vm)->GetEnv (vm, (void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        __android_log_print (ANDROID_LOG_ERROR, "native_view",
                             "Could not retrieve JNIEnv");
        return 0;
    }

//    jclass klass = (*env)->FindClass (env,
//                                      "com/example/native_binding/NativeView");

    jclass  klass = (*env)->FindClass(env,"com/example/native_view_example/NativeView");
    (*env)->RegisterNatives (env, klass, native_methods,
                             G_N_ELEMENTS (native_methods));

//    (*env)->RegisterNatives (env, klass, native_methods,
//                             G_N_ELEMENTS (native_methods));

//    com.example.native_view_example
    return JNI_VERSION_1_4;
}


FFI_PLUGIN_EXPORT void free_resource(void)
{
    // free resources
    gst_element_set_state(data->pipeline, GST_STATE_NULL);
    gst_object_unref(data->pipeline);
}

// A very short-lived native function.
//
// For very short-lived functions, it is fine to call them on the main isolate.
// They will block the Dart execution while running the native function, so
// only do this for native functions which are guaranteed to be short-lived.
FFI_PLUGIN_EXPORT intptr_t sum(intptr_t a, intptr_t b) { return a + b; }

// A longer-lived native function, which occupies the thread calling it.
//
// Do not call these kind of native functions in the main isolate. They will
// block Dart execution. This will cause dropped frames in Flutter applications.
// Instead, call these native functions on a separate isolate.
FFI_PLUGIN_EXPORT intptr_t sum_long_running(intptr_t a, intptr_t b)
{
    // Simulate work.
#if _WIN32
    Sleep(5000);
#else
    usleep(5000 * 1000);
#endif
    return a + b;
}
