package com.example.handlerjni;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.util.Log;
import android.widget.TextView;

import com.example.handlerjni.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'handlerjni' library on application startup.
    static {
        System.loadLibrary("handlerjni");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());

        HandlerThread handlerThread = new HandlerThread("handler-thread");
        handlerThread.start();
        Handler handlerA = new Handler(handlerThread.getLooper());
        Handler handlerB = new Handler(handlerThread.getLooper());
        Listener listener = count -> {
            Log.i("HandlerJNI", "callback(" + count + ") in thread: " + Thread.currentThread().getName());
        };
        Caller.addListener(handlerA, listener);
        new Thread(() -> {
            while (true) {
                Caller.call();
                try {
                    Thread.sleep(10);
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
            }
        }).start();

        handlerB.post(() -> {
            Log.i("HandlerJNI", "pure java");
        });
    }

    /**
     * A native method that is implemented by the 'handlerjni' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}