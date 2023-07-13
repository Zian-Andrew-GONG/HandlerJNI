package com.example.handlerjni;

public class RunnableWrapper implements Runnable {
    // lambda address
    private long nativePtr;

    public RunnableWrapper(long nativePtr) {
        this.nativePtr = nativePtr;
    }

    private native void nativeRun();

    private native void nativeDestroy();

    @Override
    public void run() {
        nativeRun();
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        nativeDestroy();
    }
}
