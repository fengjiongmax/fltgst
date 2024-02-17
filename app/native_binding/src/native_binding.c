#include "native_binding.h"
#include <gst/gst.h>
#include <gst/video/video.h>

#if ANDROID
#include <jni.h>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#endif

#ifdef IOS
#include "gst_ios_init.h"
#endif

// for video  gst-launch-1.0 -v videotestsrc pattern=snow ! video/x-raw,width=1280,height=720 ! autovideosink
typedef struct _FltGstData
{
    GstElement *pipeline;
    GstElement *audiotestsrc;
    GstElement *audioconvert;
    GstElement *autoaudiosink;

    GstElement *videotestsrc;
    GstElement *autovideosink;
    GMainLoop *mainloop;

    GstElement *overlay;

    GMainContext *context;

#if ANDROID
    ANativeWindow *native_window;
#endif
} FltGstData;

FltGstData *data;

FFI_PLUGIN_EXPORT void init(void)
{
#ifdef IOS
    gst_ios_init();
#endif
    // init
    gst_init(NULL, NULL);
    data = g_new0(FltGstData,1);
//    data = (FltGstData *)malloc(sizeof(FltGstData));
//#ifdef ANDROID
//    data->native_window = NULL;
//#endif
//    data->pipeline = NULL;
//    data->mainloop = NULL;
}

gpointer run_main_loop_thread(gpointer user_data)
{
    g_main_loop_run(data->mainloop);
    return NULL;
}

FFI_PLUGIN_EXPORT void run_mainloop(void)
{
    // run main loop
    if (data->mainloop == NULL || !g_main_loop_is_running(data->mainloop))
    {
        if (data->mainloop)
        {
            g_main_loop_unref(data->mainloop);
        }
        g_thread_new("main_loop", run_main_loop_thread, NULL);
    }
}

static void
error_cb (GstBus * bus, GstMessage * msg, FltGstData * data)
{
    GError *err;
    gchar *debug_info;
    gchar *message_string;

    gst_message_parse_error (msg, &err, &debug_info);
    message_string =
            g_strdup_printf ("Error received from element %s: %s",
                             GST_OBJECT_NAME (msg->src), err->message);
    g_clear_error (&err);
    g_free (debug_info);
//    set_ui_message (message_string, data);
    g_free (message_string);
//    data->target_state = GST_STATE_NULL;
    gst_element_set_state (data->pipeline, GST_STATE_NULL);
}

FFI_PLUGIN_EXPORT void setup_pipeline(void)
{
    // setup pipeline
    data->audiotestsrc = gst_element_factory_make("audiotestsrc", NULL);
    data->audioconvert = gst_element_factory_make("audioconvert", NULL);
    data->autoaudiosink = gst_element_factory_make("autoaudiosink", NULL);
    data->videotestsrc = gst_element_factory_make("videotestsrc", NULL);
    data->autovideosink = gst_element_factory_make("autovideosink", NULL);
    data->pipeline = gst_pipeline_new(NULL);
//    data->mainloop = g_main_loop_new(NULL, FALSE);

    if (!data->audiotestsrc || !data->audioconvert || !data->autoaudiosink || !data->pipeline || !data->videotestsrc || !data->autovideosink)
    {
        g_printerr("Elements could not be created.\n");
        gst_object_unref(data->pipeline);
        return;
    }

    gst_bin_add_many(GST_BIN(data->pipeline), data->videotestsrc, data->autovideosink, NULL);
    if (!gst_element_link_many(data->videotestsrc, data->autovideosink, NULL))
    {
        g_printerr("Elements could not be created.\n");
        gst_object_unref(data->pipeline);
        return;
    }
    gst_element_set_state (data->pipeline, GST_STATE_READY);

    data->overlay = gst_bin_get_by_interface(GST_BIN(data->pipeline),GST_TYPE_VIDEO_OVERLAY);

//    data->context = g_main_context_new ();
//    g_main_context_push_thread_default (data->context);

    // setup bus
    GstBus *bus = gst_element_get_bus(data->pipeline);
    g_signal_connect (G_OBJECT (bus), "message::error", (GCallback) error_cb,
                      data);
    g_object_unref(bus);
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
//    run_mainloop();
}

#if ANDROID

JNIEXPORT void JNICALL
Java_com_example_native_1view_1example_NativeView_nativeSurfaceFianlize(JNIEnv *env, jobject thiz)
{
    // TODO: implement nativeSurfaceFianlize()
    GST_DEBUG("Releasing Native Window %p", data->native_window);
    if (data->pipeline)
    {
        gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(data->autovideosink),
                                            (guintptr)NULL);
        gst_element_set_state(data->pipeline, GST_STATE_READY);
    }

    ANativeWindow_release(data->native_window);
    data->native_window = NULL;
}

JNIEXPORT void JNICALL
Java_com_example_native_1view_1example_NativeView_nativeSurfaceInit(JNIEnv *env, jobject thiz,
                                                                    jobject surface)
{
    // TODO: implement nativeSurfaceInit()
    ANativeWindow *new_native_window = ANativeWindow_fromSurface(env, surface);
    GST_DEBUG("Received surface %p (native window %p)", surface,
              new_native_window);

    if (data->native_window)
    {
        ANativeWindow_release(data->native_window);
        if (data->native_window == new_native_window)
        {
            GST_DEBUG("New native window is the same as the previous one %p",
                      data->native_window);
            if (data->pipeline)
            {
                gst_video_overlay_expose(GST_VIDEO_OVERLAY(data->pipeline));
                gst_video_overlay_expose(GST_VIDEO_OVERLAY(data->pipeline));
            }
            return;
        }
        else
        {
            GST_DEBUG("Released previous native window %p", data->native_window);
            //            data->initialized = FALSE;
        }
    }
    data->native_window = new_native_window;

    GstVideoOverlay  *overlay = GST_VIDEO_OVERLAY(data->overlay);
    if (data->native_window && data->pipeline)
    {
        gst_video_overlay_set_window_handle(overlay,(guintptr)data->native_window);
    }

    GstState cur_state;
    gst_element_get_state(GST_ELEMENT(data->pipeline),&cur_state,NULL,0);

}

JNIEXPORT void JNICALL
Java_com_example_native_1view_1example_NativeView_startPipeline(JNIEnv *env, jobject thiz) {
    // TODO: implement startPipeline()
    start_pipeline();
}

JNIEXPORT void JNICALL
Java_com_example_native_1view_1example_NativeView_nativeTest(JNIEnv *env, jobject thiz)
{
    // TODO: implement nativeTest()
    GST_DEBUG("Called from jni");
}

// static void
// gst_native_surface_init (JNIEnv * env, jobject thiz, jobject surface)
//{
////    CustomData *data = GET_CUSTOM_DATA (env, thiz, custom_data_field_id);
////    if (!data)
////        return;
//    ANativeWindow *new_native_window = ANativeWindow_fromSurface (env, surface);
//    GST_DEBUG ("Received surface %p (native window %p)", surface,
//               new_native_window);
//
//    if (data->native_window) {
//        ANativeWindow_release (data->native_window);
//        if (data->native_window == new_native_window) {
//            GST_DEBUG ("New native window is the same as the previous one %p",
//                       data->native_window);
//            if (data->pipeline) {
//                gst_video_overlay_expose (GST_VIDEO_OVERLAY (data->pipeline));
//                gst_video_overlay_expose (GST_VIDEO_OVERLAY (data->pipeline));
//            }
//            return;
//        } else {
//            GST_DEBUG ("Released previous native window %p", data->native_window);
////            data->initialized = FALSE;
//        }
//    }
//    data->native_window = new_native_window;
//
////    check_initialization_complete (data);
//}

///* Library initializer */
// jint JNI_OnLoad(JavaVM *vm, void *reserved) {
//     JNIEnv *env = NULL;
//
//     if ((*vm)->GetEnv (vm, (void **) &env, JNI_VERSION_1_4) != JNI_OK) {
//         __android_log_print (ANDROID_LOG_ERROR, "native_view",
//                              "Could not retrieve JNIEnv");
//         return 0;
//     }
//
//     jclass klass = (*env)->FindClass (env,
//                                       "com/example/native_view_example/NativeView");
//
////    (*env)->RegisterNatives (env, klass, native_methods,
////                             G_N_ELEMENTS (native_methods));
//
//    return JNI_VERSION_1_4;
//}
#endif

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
