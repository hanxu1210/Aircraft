#include "PluginUtilsEx.h"

namespace cocos2d { namespace plugin {
    std::map<std::string, std::string> PluginUtilsEx::getMapFromHashMap(JNIEnv* env, jobject hashMap)
    {
        std::map<std::string, std::string> valueMap;

        if(hashMap)
        {
            jclass jclass_of_hashmap = (env)->GetObjectClass(hashMap);
            //得到jni entryset方法
            jmethodID entrySetMethod = (env)->GetMethodID(jclass_of_hashmap, "entrySet", "()Ljava/util/Set;");
            //交用hashMap对象的entrySet方法，返回jni set对象
            jobject jobject_of_entryset = env->CallObjectMethod(hashMap, entrySetMethod);

            //得到jni Set类型
            jclass jclass_of_set = (env)->FindClass("java/util/Set");

            if (jclass_of_set == 0) {
                //得到set类型失败
                outputLog("PluginUtilsEx", "java/util/Set lookup failed\n");
                return valueMap;
            }
            //得到对象jclass_of_set 的 iterator方法
            jmethodID iteratorMethod = env->GetMethodID(jclass_of_set, "iterator", "()Ljava/util/Iterator;");

            //通过对象jobject_of_entryset 对象调用iterator 方法，得到jni itreator对象
            jobject jobject_of_iterator = env->CallObjectMethod(jobject_of_entryset, iteratorMethod);

            //得到Iterator类
            jclass jclass_of_iterator = (env)->FindClass("java/util/Iterator");

            //得到iterator hasNext方法
            jmethodID hasNextMethod = env->GetMethodID(jclass_of_iterator, "hasNext", "()Z");

            //调用hasNext方法
            jboolean bHasNext = env->CallBooleanMethod(jobject_of_iterator, hasNextMethod);

            jmethodID nextMethod = env->GetMethodID(jclass_of_iterator, "next", "()Ljava/lang/Object;");

            while (bHasNext) {
                //得到实例类型
                jclass jclass_of_mapentry = (env)->FindClass("java/util/Map$Entry");
                jobject jobject_of_Entry=env->CallObjectMethod(jobject_of_iterator, nextMethod);
                //得到getKey方法
                jmethodID getKeyMethod = env->GetMethodID(jclass_of_mapentry, "getKey", "()Ljava/lang/Object;");
                //得到getValue方法
                jmethodID getValueMethod = env->GetMethodID(jclass_of_mapentry, "getValue", "()Ljava/lang/Object;");

                jstring js_keyStr = (jstring)env->CallObjectMethod(jobject_of_Entry, getKeyMethod);
                jstring js_value = (jstring)env->CallObjectMethod(jobject_of_Entry, getValueMethod);

                std::string c_key = PluginJniHelper::jstring2string(js_keyStr);
                std::string cc_Value = PluginJniHelper::jstring2string(js_value);
                valueMap[c_key] = cc_Value;


                bHasNext=env->CallBooleanMethod(jobject_of_iterator, hasNextMethod);

                //删除引用
                env->DeleteLocalRef(js_value);
                env->DeleteLocalRef(js_keyStr);
                env->DeleteLocalRef(jobject_of_Entry);
                env->DeleteLocalRef(jclass_of_mapentry);

            }
            //删除引用
            env->DeleteLocalRef(jclass_of_iterator);
            env->DeleteLocalRef(jobject_of_iterator);
            env->DeleteLocalRef(jclass_of_set);
            env->DeleteLocalRef(jobject_of_entryset);
            env->DeleteLocalRef(jclass_of_hashmap);

        }

        return valueMap;
    }

}}