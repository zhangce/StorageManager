//
//  ObjectStore_PG.h
//  SMan
//
//  Created by Ce Zhang on 9/10/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef SMan_ObjectStore_PG_h
#define SMan_ObjectStore_PG_h

#include <jni.h>

namespace hazy{
  namespace sman{
    
    template<class VALUE, PropertyType PROPERTY>
    class ObjStore<VALUE, STORAGE_JHASH, PROPERTY>{
      
    private:
      
      JavaVM *jvm;
      JNIEnv *env;
      JavaVMInitArgs vm_args;
      JavaVMOption* options;
      jclass cls;
      jclass globid;
      
      jmethodID id_get;
      jmethodID id_set;
      jmethodID id_load;
      jmethodID id_init;
      
    public:
      
      ~ObjStore(){
        
      }
            
      ObjStore(){
        
        if(PROPERTY == JAVAHASH_MM){
          std::cout << "INFO: " << "Use specification STORAGE_JHASH @ HashMapConnector." << std::endl;
        }else if(PROPERTY == JAVAHASH_ACCU){
          std::cout << "INFO: " << "Use specification STORAGE_JHASH @ AccumuloConnector." << std::endl;
        }
        
        options = new JavaVMOption[1];
        
        options[0].optionString =
        "-Djava.class.path=/Users/czhang/Desktop/Codes/AccumuloConnector/bin:/Users/czhang/Desktop/Codes/AccumuloConnector/lib/postgresql-8.4-701.jdbc4.jar";
        vm_args.version = JNI_VERSION_1_6;
        vm_args.nOptions = 1;
        vm_args.options = options;
        vm_args.ignoreUnrecognized = false;
        JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
        delete options;
        
        if(PROPERTY == JAVAHASH_MM){
          cls = env->FindClass("HashMapConnector");
        }else if(PROPERTY == JAVAHASH_ACCU){
          cls = env->FindClass("AccumuloConnector");
        }
       
        id_init = env->GetStaticMethodID(cls, "init", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
        id_get = env->GetStaticMethodID(cls, "get", "(I)Ljava/lang/Object;");
        id_set = env->GetStaticMethodID(cls, "set", "(I[B)V");
        id_load = env->GetStaticMethodID(cls, "load", "(I[B)V");
        
        jobjectArray ret;
        ret= (jobjectArray)env->NewObjectArray(4,
                                               env->FindClass("java/lang/String"),
                                               env->NewStringUTF(""));

        env->CallStaticVoidMethod(cls, id_init, env->NewStringUTF("jdbc:postgresql://localhost:5432/postgres"),
                                                env->NewStringUTF("postgres"),
                                                env->NewStringUTF("bB19871121"),
                                                env->NewStringUTF("tmp_kv_2"));
      }
      
      StatusType get(int64_t key, VALUE & value){
        
        JNIEnv *env2;
        jvm->AttachCurrentThread((void**)&env2, NULL);
        
        jbyteArray barr = (jbyteArray) env2->CallObjectMethod(cls, id_get, key);
        
        VALUE tmp;
        
        env2->GetByteArrayRegion(barr, 0, sizeof(VALUE), (jbyte*) &tmp);

        value = tmp;
        
        env2->DeleteLocalRef(barr);
        
        return STATUS_SUCCESS;
      }
      
      StatusType set(int64_t key, const VALUE & value){
        
        JNIEnv *env2;
        jvm->AttachCurrentThread((void**)&env2, NULL);
        
        jbyteArray serialized = env2->NewByteArray(sizeof(VALUE));
        
        env2->SetByteArrayRegion(serialized, 0, sizeof(VALUE), (jbyte*) &value);
        
        env2->CallStaticVoidMethod(cls, id_set, key, serialized);
        
        env2->DeleteLocalRef(serialized);
        

        return STATUS_SUCCESS;
      }
      
      StatusType load(int64_t key, const VALUE & value){
        
        JNIEnv *env2; 
        jvm->AttachCurrentThread((void**)&env2, NULL);
              
        jbyteArray serialized = env2->NewByteArray(sizeof(VALUE));
        
        env2->SetByteArrayRegion(serialized, 0, sizeof(VALUE), (jbyte*) &value);
        
        //jsize alen = env->GetArrayLength(serialized);
        
        env2->CallStaticVoidMethod(cls, id_load, key, serialized);
        
        env2->DeleteLocalRef(serialized);
        
        return STATUS_SUCCESS;
      }
      
      StatusType apply(int64_t key, void (*func)(VALUE &)){
        
        
        VALUE v;
        
        this->get(key, v);

        func(v);
        
        this->set(key, v);
        
        return STATUS_SUCCESS;
      }
      
    };
    
    
  }
}


#endif
