package com.example.native_view_example

import android.content.Context
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.view.View
import io.flutter.plugin.platform.PlatformView

internal class NativeView(context:Context,id:Int,creationParams:Map<String?,Any?>?):PlatformView,SurfaceHolder.Callback {
    private val surfaceView:SurfaceView
    private val surfaceHolder:SurfaceHolder

    override fun getView(): View {
        return surfaceView
    }

    override fun dispose() { }

    init {
        surfaceView = SurfaceView(context)
        surfaceHolder = surfaceView.holder
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
//        surfaceHolder.surface
        TODO("Not yet implemented")
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        TODO("Not yet implemented")
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        TODO("Not yet implemented")
    }

    companion object{
        init {
//            System.mapLibraryName()
//            System.loadLibrary("native_view")
            System.loadLibrary("native_binding")
        }
    }

}