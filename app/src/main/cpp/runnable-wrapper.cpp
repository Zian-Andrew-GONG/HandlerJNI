#include <jni.h>
#include <functional>
#include "runnable-wrapper.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_example_handlerjni_RunnableWrapper_nativeRun(JNIEnv *env, jobject thiz) {
    auto runnableClz = env->FindClass("com/example/handlerjni/RunnableWrapper");
    auto nativePtrId = env->GetFieldID(runnableClz, "nativePtr", "J");
    auto nativePtr = env->GetLongField(thiz, nativePtrId);
    auto cbPtr = reinterpret_cast<std::shared_ptr<RunnableFunction> *>(nativePtr);
    auto cb = **cbPtr;
    cb();
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_handlerjni_RunnableWrapper_nativeDestroy(JNIEnv *env, jobject thiz) {
    auto clazz = env->GetObjectClass(thiz);
    auto field = env->GetFieldID(clazz, "nativePtr", "J");
    auto ptr = env->GetLongField(thiz, field);
    if (ptr != 0) {
        auto raw_ptr = reinterpret_cast<std::shared_ptr<RunnableFunction> *>(ptr);
        delete raw_ptr;
        env->SetLongField(thiz, field, (jlong) 0);
    }
}