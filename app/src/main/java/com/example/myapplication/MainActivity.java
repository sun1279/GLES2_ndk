package com.example.myapplication;

import android.content.Intent;
import android.opengl.GLSurfaceView;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
   // static {
   //     System.loadLibrary("ssss");
   // }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
  //  public native String get();

    @Override
    protected void onCreate(Bundle savedInstanceState) {

   /*     super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText("ADFSDFRE\n");

        Button bt1 = findViewById(R.id.button);
        bt1.setOnClickListener(new onClickListener());
    {
            @Override
            public void onClick(View v)
            {
                Toast.makeText(MainActivity.this, "LLLLLLLL", Toast.LENGTH_SHORT).show();
            }

        });*/

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //TextView tv = findViewById(R.id.sample_text);
        //tv.setText(get());
        //tv.setText("ADFSDFRE\n");
        //GLSurfaceView Myglview = (GLSurfaceView) findViewById(R.id.glSurfaceView);
        GLSurfaceView Myglview = new GLSurfaceView(this);

       // if (isProbablyEmulator()) {
            // Avoids crashes on startup with some emulator images.
            Myglview.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        //}

        Myglview.setEGLContextClientVersion(2);
        Myglview.setRenderer(new RendererWrapper());
        setContentView(Myglview);


        }



    public class onClickListener implements View.OnClickListener {
        public void onClick(View v)
        {
            // Toast.makeText(MainActivity.this, "LLLLLLLL", Toast.LENGTH_SHORT).show();
            // finish();
            //TextView tv = findViewById(R.id.sample_text);
            // tv.setText("AAAAAAAAAAAAAAA\n");
            Intent i = new Intent("com.example.activitytest.ACTION_START");
            // i.addCategory("com.example.activitytest.MY_CATEGORY");
            startActivity(i);
        }
    }
}
