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

#if ANDROID

JNIEXPORT void JNICALL
Java_com_example_native_1view_1example_NativeView_nativeSurfaceFianlize(JNIEnv *env, jobject thiz)
{
    // TODO: implement nativeSurfaceFianlize()
    GST_DEBUG("Releasing Native Window %p", data->native_window);
    if (data->pipeline)
    {
        gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(data->overlay),
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
