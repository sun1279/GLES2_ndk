package com.example.myapplication;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


import android.opengl.GLSurfaceView.Renderer;
import android.widget.TextView;




public class RendererWrapper implements Renderer {
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        GLESWrapper.on_surface_created();

        //System.out.println("===>start!");
        //GLES20.glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        System.out.println("===>start!111");
        //GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT); //this is a must


    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
      //  GLESWrapper.on_surface_changed(width, height);
        System.out.println("===onSurfaceChanged?/**/");
        // No-op
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        GLESWrapper.on_draw_frame();
        System.out.println("===This?/**/");
       // GLES20.glClear(GL_COLOR_BUFFER_BIT);
       // GLES20.glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        //GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT); //this is a must
    }
}
