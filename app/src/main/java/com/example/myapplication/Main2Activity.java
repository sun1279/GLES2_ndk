package com.example.myapplication;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class Main2Activity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main2);

        Button bt1 = findViewById(R.id.button2);
        bt1.setOnClickListener(new onClickListener1());
    }

    public class onClickListener1 implements View.OnClickListener {
        public void onClick(View v) {
            // Toast.makeText(MainActivity.this, "LLLLLLLL", Toast.LENGTH_SHORT).show();
            // finish();
            TextView tv = findViewById(R.id.t2);
            tv.setText("AAAAAAAAAAAAAAA\n");
           // Intent i = new Intent("android.intent.action.DIAL");
           //  i.addCategory("com.example.activitytest.MY_CATEGORY");
           // startActivity(i);
        }


    }
}