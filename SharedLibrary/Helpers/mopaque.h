/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef MOPAQUE_H
#define MOPAQUE_H
#include <tuple>
#include <iostream>
#include <memory>
#include "debug.h"

/** ---------------- MOpaque-------------
 * To get value from MOpaque use  std::get eg.  std::get<0>(tuple);
 */
template<typename... T>
class MOpaque
{

public:
    typedef std::tuple<typename std::__decay_and_strip<T>::__type...> TupleType;
    TupleType tuple(){
        return tuple_;
    }

    ~MOpaque(){
#if PRINT_MOpaque_Class_DEBUG_MESSAGES
        if(isShared_){
            std::cout << "Shared MOpaque (objectName -> " << objectName_ << ")destroyed" <<std::endl;
        }else{
            std::cout << "MOpaque (objectName -> " << objectName_ << ") destroyed" <<std::endl;
        }
#endif
    }

    /*Used to create Unique MOpaque, that can be passed to call backs that accept void* */
    template<typename... T_>
    static  MOpaque<T_...>* createUnique(T_... args){
        auto* opaque  = new MOpaque<T_...>();
        opaque->isShared_ = false;
        opaque->tuple_ = std::tuple<typename std::__decay_and_strip<T_>::__type...>(std::forward<T_>(args)...);
        return opaque;
    }

    void* toVoidPtr(){
        return (void*)this;
    }

    /*Used to create Shared MOpaque, that can be used to pass values to function that accept
      Shared MOpaque as a parameter , But not void*
     WARNING -> Do not pass Shared MOpaque as  void *
    */
    template<typename... T_>
    static  std::shared_ptr<MOpaque<T_...>> createShared(T_... args){
        typedef std::shared_ptr<MOpaque<T_...>> SharedMOpaque;
        SharedMOpaque sharedOpaque  = SharedMOpaque(new MOpaque<T_...>());//std::make_shared<MOpaque<T_...>>();
        sharedOpaque->isShared_ = true;
        sharedOpaque->tuple_ = std::tuple<typename std::__decay_and_strip<T_>::__type...>(std::forward<T_>(args)...);
        return sharedOpaque;
    }

    /*Dont for get to destory a created non shared MOpaque by calling this*/
    template<typename... T_>
    static  void destroy(MOpaque<T_...>* opaque,std::string errorMessage = std::string()){
        if(opaque){
            if(opaque->isShared_){
                if(errorMessage.empty()){
                    std::cerr << "Can not use destroy on Unique MOpaque (objectName -> " <<opaque->objectName_ << ")"
                              <<std::endl;
                }else{
                    std::cerr << errorMessage <<std::endl;
                }
                return;
            }
            delete opaque;
            opaque = nullptr;
        }
    }

    template<typename... T_>
    static  void setObjectName(MOpaque<T_...>* opaque,std::string objectName){
        if(opaque){
            opaque->objectName_ = objectName;
        }
    }

    template<typename... T_>
    static  void setObjectName(std::shared_ptr<MOpaque<T_...>> sharedOpaque,std::string objectName){
        if(sharedOpaque.get()){
            sharedOpaque->objectName_ = objectName;
        }
    }

    bool isShared(){
        return isShared_;
    }

    bool isUnique(){
        return (!isShared_);
    }

private:
    friend class MOpaque<>;//so that we can use MOpaque<> to call create And destroy
    MOpaque():isShared_(false){}
    TupleType tuple_;
    bool isShared_;
    std::string objectName_;
};


#endif // MOPAQUE_H


