#ifndef HANDLERJNI_RUNNABLE_WRAPPER_H
#define HANDLERJNI_RUNNABLE_WRAPPER_H

#include <jni.h>
typedef std::function<void()> RunnableFunction;

inline jobject runnableConverter(JNIEnv *env, const RunnableFunction callback) {
    std::shared_ptr<RunnableFunction> lambda = std::make_shared<RunnableFunction>(callback);
    auto ptr = new std::shared_ptr<RunnableFunction>(lambda);
    auto runnableClz = env->FindClass("com/example/handlerjni/RunnableWrapper");
    auto constructor = env->GetMethodID(runnableClz, "<init>", "(J)V");
    auto runnable = env->NewObject(runnableClz, constructor, (jlong) ptr);
    return runnable;
}

#endif //HANDLERJNI_RUNNABLE_WRAPPER_H
