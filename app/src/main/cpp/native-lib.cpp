#include <jni.h>
#include <string>
#include <exception>
#include "runnable-wrapper.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_handlerjni_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

static JavaVM* jvm = nullptr;

JavaVM* GetJvm() {
    if (jvm == nullptr) {
        throw std::runtime_error("JavaVM must be initialed");
    }
    return jvm;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    jvm = vm;

    JNIEnv* env = NULL;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        throw std::runtime_error("GetEnv Failed");
    }

    return JNI_VERSION_1_6;
}

JNIEnv *getEnv(JavaVM *jvm) {
    JNIEnv *currentEnv = nullptr;
    auto ret = jvm->GetEnv((void **) &currentEnv, JNI_VERSION_1_6);
    if (ret != JNI_OK) {
        throw std::runtime_error("GetEnv Failed");
    }
    return currentEnv;
}

static int counter = 0;

extern "C"
JNIEXPORT void JNICALL
Java_com_example_handlerjni_Caller_call(JNIEnv *env, jclass clazz) {
    // get mHandler
    auto handlerId = env->GetStaticFieldID(clazz, "mHandler", "Landroid/os/Handler;");
    auto handlerObj = env->GetStaticObjectField(clazz, handlerId);
    // get mListener
    auto listenerId = env->GetStaticFieldID(clazz, "mListener",
                                            "Lcom/example/handlerjni/Listener;");
    auto listenerObj = env->GetStaticObjectField(clazz, listenerId);
    auto g_listener = env->NewGlobalRef(listenerObj);
    // callback
    auto callback = [g_listener]() {
        JNIEnv *local_env = getEnv(GetJvm());
        auto methodId = local_env->GetMethodID(local_env->GetObjectClass(g_listener), "onCalled",
                                               "(I)V");
        local_env->CallVoidMethod(g_listener, methodId, counter++);
        if (counter > 5000) counter = 0;  // reset counter
        local_env->DeleteGlobalRef(g_listener);
    };
    // convert callback lambda to RunnableWrapper obj
    auto runnable = runnableConverter(env, callback);
    // call "post" method
    auto postId = env->GetMethodID(env->GetObjectClass(handlerObj), "post",
                                   "(Ljava/lang/Runnable;)Z");
    env->CallBooleanMethod(handlerObj, postId, runnable);
}