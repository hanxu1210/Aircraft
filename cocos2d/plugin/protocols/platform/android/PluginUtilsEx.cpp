#include "PluginUtilsEx.h"

namespace cocos2d { namespace plugin {
    std::map<std::string, std::string> PluginUtilsEx::getMapFromHashMap(JNIEnv* env, jobject hashMap)
    {
        std::map<std::string, std::string> valueMap;

        if(hashMap)
        {
            jclass jclass_of_hashmap = (env)->GetObjectClass(hashMap);
            //�õ�jni entryset����
            jmethodID entrySetMethod = (env)->GetMethodID(jclass_of_hashmap, "entrySet", "()Ljava/util/Set;");
            //����hashMap�����entrySet����������jni set����
            jobject jobject_of_entryset = env->CallObjectMethod(hashMap, entrySetMethod);

            //�õ�jni Set����
            jclass jclass_of_set = (env)->FindClass("java/util/Set");

            if (jclass_of_set == 0) {
                //�õ�set����ʧ��
                outputLog("PluginUtilsEx", "java/util/Set lookup failed\n");
                return valueMap;
            }
            //�õ�����jclass_of_set �� iterator����
            jmethodID iteratorMethod = env->GetMethodID(jclass_of_set, "iterator", "()Ljava/util/Iterator;");

            //ͨ������jobject_of_entryset �������iterator �������õ�jni itreator����
            jobject jobject_of_iterator = env->CallObjectMethod(jobject_of_entryset, iteratorMethod);

            //�õ�Iterator��
            jclass jclass_of_iterator = (env)->FindClass("java/util/Iterator");

            //�õ�iterator hasNext����
            jmethodID hasNextMethod = env->GetMethodID(jclass_of_iterator, "hasNext", "()Z");

            //����hasNext����
            jboolean bHasNext = env->CallBooleanMethod(jobject_of_iterator, hasNextMethod);

            jmethodID nextMethod = env->GetMethodID(jclass_of_iterator, "next", "()Ljava/lang/Object;");

            while (bHasNext) {
                //�õ�ʵ������
                jclass jclass_of_mapentry = (env)->FindClass("java/util/Map$Entry");
                jobject jobject_of_Entry=env->CallObjectMethod(jobject_of_iterator, nextMethod);
                //�õ�getKey����
                jmethodID getKeyMethod = env->GetMethodID(jclass_of_mapentry, "getKey", "()Ljava/lang/Object;");
                //�õ�getValue����
                jmethodID getValueMethod = env->GetMethodID(jclass_of_mapentry, "getValue", "()Ljava/lang/Object;");

                jstring js_keyStr = (jstring)env->CallObjectMethod(jobject_of_Entry, getKeyMethod);
                jstring js_value = (jstring)env->CallObjectMethod(jobject_of_Entry, getValueMethod);

                std::string c_key = PluginJniHelper::jstring2string(js_keyStr);
                std::string cc_Value = PluginJniHelper::jstring2string(js_value);
                valueMap[c_key] = cc_Value;


                bHasNext=env->CallBooleanMethod(jobject_of_iterator, hasNextMethod);

                //ɾ������
                env->DeleteLocalRef(js_value);
                env->DeleteLocalRef(js_keyStr);
                env->DeleteLocalRef(jobject_of_Entry);
                env->DeleteLocalRef(jclass_of_mapentry);

            }
            //ɾ������
            env->DeleteLocalRef(jclass_of_iterator);
            env->DeleteLocalRef(jobject_of_iterator);
            env->DeleteLocalRef(jclass_of_set);
            env->DeleteLocalRef(jobject_of_entryset);
            env->DeleteLocalRef(jclass_of_hashmap);

        }

        return valueMap;
    }

}}