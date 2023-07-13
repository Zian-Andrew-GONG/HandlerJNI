package com.example.handlerjni;

import android.os.Handler;

import androidx.annotation.NonNull;

public class Caller {
    static private Handler mHandler;
    static private Listener mListener;

    static native void call();

    static void addListener(@NonNull Handler handler, @NonNull Listener listener) {
        mListener = listener;
        mHandler = handler;
    }
}
