#include "native_binding.h"
#include <gst/gst.h>

#ifdef IOS
    #include "gst_ios_init.h"
#endif

typedef struct _FltGstData
{
    GstElement *pipeline;
    GstElement *audiotestsrc;
    GstElement *audioconvert;
    GstElement *autoaudiosink;

    GMainLoop *mainloop;
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

    data->mainloop = g_main_loop_new(NULL, FALSE);

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


FFI_PLUGIN_EXPORT void free_resource(void)
{
    // free resources
    g_main_loop_unref(data->mainloop);
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
