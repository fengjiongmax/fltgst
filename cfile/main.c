#include <gst/gst.h>

// gst-launch-1.0 audiotestsrc ! audioconvert ! autoaudiosink

typedef struct _FltGstData
{
    GstElement *pipeline;
    GstElement *audiotestsrc;
    GstElement *audioconvert;
    GstElement *autoaudiosink;

    GMainLoop *mainloop;
} FltGstData;

FltGstData *data;

void init(void)
{
    // init
    gst_init(NULL, NULL);
    data = (FltGstData *)malloc(sizeof(FltGstData));
}

void setup_pipeline(void)
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

void start_pipeline(void)
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

void run_mainloop(void)
{
    // run main loop
    g_main_loop_run(data->mainloop);
}

void free_resource(void)
{
    // free resources
    gst_element_set_state(data->pipeline, GST_STATE_NULL);
    gst_object_unref(data->pipeline);
}

int main(int argc, char const *argv[])
{
    init();
    setup_pipeline();
    start_pipeline();
    run_mainloop();
    free_resource();
    return 0;
}
