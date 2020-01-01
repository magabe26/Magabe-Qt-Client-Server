/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */

#ifndef MCPPCALLBACK_H
#define MCPPCALLBACK_H
#include <QString>
#include "mopaque.h"

typedef void * AbstractCallback;

template <typename CallbackType = AbstractCallback>
struct AbstractCallbackWrapper{
    QString tag;
    CallbackType cb = 0;
    void *opaque = (void*)0;
    bool operator ==(const AbstractCallbackWrapper& other){
        return (this->tag == other.tag);
    }
};

template<typename... T>
class CallbackParamValues{
public:
    typedef std::tuple<typename std::__decay_and_strip<T>::__type...> TupleType;
    template<typename... T_>
    static  CallbackParamValues<T_...> create(T_... args){
        CallbackParamValues<T_...> cbParamValues;
        cbParamValues.sharedMOpaque_ =  MOpaque<>::createShared(args...);
        return cbParamValues;
    }
    TupleType tuple(){
        return sharedMOpaque_->tuple();
    }
private:
    friend class CallbackParamValues<>;
    std::shared_ptr<MOpaque<T...>>  sharedMOpaque_;
};


template<typename CallbackWrapper,typename... CallbackParamValueTypes>
struct TwoParameterCallbacks{

    static CallbackParamValues<CallbackParamValueTypes...> generateCallbackParamValues(CallbackParamValueTypes... callbackParamValueTypes){
        return  CallbackParamValues<CallbackParamValueTypes...>::create(callbackParamValueTypes...);
    }

    void setDefaultCallbackParamValues(const CallbackParamValues<CallbackParamValueTypes...> &defaultCallbackParamValues)
    {
        defaultCallbackParamValues_ = defaultCallbackParamValues;
    }

    void invokeCallback(const QString &tag, CallbackParamValues<CallbackParamValueTypes ...> values){
        CallbackWrapper signatureCbWrapper;
        signatureCbWrapper.tag = tag;
        for(int index = callbacks_.indexOf(signatureCbWrapper);
            index != -1 ;
            index = callbacks_.indexOf(signatureCbWrapper)){
            auto cbWrapper = callbacks_.takeAt(index);
            if(cbWrapper.cb){
                auto tuple  = values.tuple();
                cbWrapper.cb(MOpaque<>::createShared(std::get<0>(tuple))
                             ,cbWrapper.opaque);
            }
        }
    }

    void registerCallback(const CallbackWrapper& callbackWrapper){
        if((!callbackWrapper.tag.isEmpty()) && (callbackWrapper.cb)){
            callbacks_.append((CallbackWrapper)callbackWrapper);
        }
    }

    void removeAllCallbacks(){
        foreach (auto cbWrapper, callbacks_) {
            invokeCallback(cbWrapper.tag,defaultCallbackParamValues_);
        }
        callbacks_.clear();
    }

    bool containsTag(const QString &tag){
        CallbackWrapper signatureCbWrapper;
        signatureCbWrapper.tag = tag;
        return callbacks_.contains(signatureCbWrapper);
    }

    int count(){
        return callbacks_.count();
    }

protected:
    CallbackWrapper getCallbackWrapper(const QString& tag){
        CallbackWrapper signatureCbWrapper;
        signatureCbWrapper.tag = tag;
        for(int index = callbacks_.indexOf(signatureCbWrapper);
            index != -1 ;
            index = callbacks_.indexOf(signatureCbWrapper)){
            return callbacks_.at(index);
        }
        return signatureCbWrapper;
    }

    CallbackWrapper removeAndReturnCallbackWrapper(const QString& tag){
        CallbackWrapper signatureCbWrapper;
        signatureCbWrapper.tag = tag;
        for(int index = callbacks_.indexOf(signatureCbWrapper);
            index != -1 ;
            index = callbacks_.indexOf(signatureCbWrapper)){
            return callbacks_.takeAt(index);
        }
        return signatureCbWrapper;
    }

    QList<CallbackWrapper>& callbacks(){
        return callbacks_;
    }

private:
    QList<CallbackWrapper> callbacks_;
    CallbackParamValues<CallbackParamValueTypes...> defaultCallbackParamValues_;
};


/**
 * ----------------------ThreeParameterCallbacks------------------------------
 */
template<typename CallbackWrapper,typename... CallbackParamValueTypes>
struct ThreeParameterCallbacks{

    static CallbackParamValues<CallbackParamValueTypes...> generateCallbackParamValues(CallbackParamValueTypes... callbackParamValueTypes){
        return  CallbackParamValues<CallbackParamValueTypes...>::create(callbackParamValueTypes...);
    }

    void setDefaultCallbackParamValues(const CallbackParamValues<CallbackParamValueTypes...> &defaultCallbackParamValues)
    {
        defaultCallbackParamValues_ = defaultCallbackParamValues;
    }

    void invokeCallback(const QString &tag, CallbackParamValues<CallbackParamValueTypes ...> values){
        CallbackWrapper signatureCbWrapper;
        signatureCbWrapper.tag = tag;
        for(int index = callbacks_.indexOf(signatureCbWrapper);
            index != -1 ;
            index = callbacks_.indexOf(signatureCbWrapper)){
            auto cbWrapper = callbacks_.takeAt(index);
            if(cbWrapper.cb){
                auto tuple  = values.tuple();
                //const int tupleSize = std::tuple_size<decltype(tuple)>();
                cbWrapper.cb(MOpaque<>::createShared(std::get<0>(tuple)
                             ,std::get<1>(tuple))
                             ,cbWrapper.opaque);
            }
        }
    }

    void registerCallback(const CallbackWrapper& callbackWrapper){
        if((!callbackWrapper.tag.isEmpty()) && (callbackWrapper.cb)){
            callbacks_.append((CallbackWrapper)callbackWrapper);
        }
    }

    void removeAllCallbacks(){
        foreach (auto cbWrapper, callbacks_) {
            invokeCallback(cbWrapper.tag,defaultCallbackParamValues_);
        }
        callbacks_.clear();
    }

    bool containsTag(const QString &tag){
        CallbackWrapper signatureCbWrapper;
        signatureCbWrapper.tag = tag;
        return callbacks_.contains(signatureCbWrapper);
    }

    int count(){
        return callbacks_.count();
    }


protected:
    CallbackWrapper getCallbackWrapper(const QString& tag){
        CallbackWrapper signatureCbWrapper;
        signatureCbWrapper.tag = tag;
        for(int index = callbacks_.indexOf(signatureCbWrapper);
            index != -1 ;
            index = callbacks_.indexOf(signatureCbWrapper)){
            return callbacks_.at(index);
        }
        return signatureCbWrapper;
    }

    CallbackWrapper removeAndReturnCallbackWrapper(const QString& tag){
        CallbackWrapper signatureCbWrapper;
        signatureCbWrapper.tag = tag;
        for(int index = callbacks_.indexOf(signatureCbWrapper);
            index != -1 ;
            index = callbacks_.indexOf(signatureCbWrapper)){
            return callbacks_.takeAt(index);
        }
        return signatureCbWrapper;
    }

    QList<CallbackWrapper>& callbacks(){
        return callbacks_;
    }

private:
    QList<CallbackWrapper> callbacks_;
    CallbackParamValues<CallbackParamValueTypes...> defaultCallbackParamValues_;
};

/**
 * ----------------------FourParameterCallbacks---------------------------------
 */
template<typename CallbackWrapper,typename... CallbackParamValueTypes>
struct FourParameterCallbacks{

    static CallbackParamValues<CallbackParamValueTypes...> generateCallbackParamValues(CallbackParamValueTypes... callbackParamValueTypes){
        return  CallbackParamValues<CallbackParamValueTypes...>::create(callbackParamValueTypes...);
    }

    void setDefaultCallbackParamValues(const CallbackParamValues<CallbackParamValueTypes...> &defaultCallbackParamValues)
    {
        defaultCallbackParamValues_ = defaultCallbackParamValues;
    }

    void invokeCallback(const QString &tag, CallbackParamValues<CallbackParamValueTypes ...> values){
        CallbackWrapper signatureCbWrapper;
        signatureCbWrapper.tag = tag;
        for(int index = callbacks_.indexOf(signatureCbWrapper);
            index != -1 ;
            index = callbacks_.indexOf(signatureCbWrapper)){
            auto cbWrapper = callbacks_.takeAt(index);
            if(cbWrapper.cb){
                auto tuple  = values.tuple();

                cbWrapper.cb(MOpaque<>::createShared(
                              std::get<0>(tuple)
                             ,std::get<1>(tuple)
                             ,std::get<2>(tuple))
                             ,cbWrapper.opaque);
            }
        }
    }

    void registerCallback(const CallbackWrapper& callbackWrapper){
        if((!callbackWrapper.tag.isEmpty()) && (callbackWrapper.cb)){
            callbacks_.append(callbackWrapper);
        }
    }

    void removeAllCallbacks(){
        foreach (auto cbWrapper, callbacks_) {
            invokeCallback(cbWrapper.tag,defaultCallbackParamValues_);
        }
        callbacks_.clear();
    }

    bool containsTag(const QString &tag){
        CallbackWrapper signatureCbWrapper;
        signatureCbWrapper.tag = tag;
        return callbacks_.contains(signatureCbWrapper);
    }

    int count(){
        return callbacks_.count();
    }

protected:
    CallbackWrapper getCallbackWrapper(const QString& tag){
        CallbackWrapper signatureCbWrapper;
        signatureCbWrapper.tag = tag;
        for(int index = callbacks_.indexOf(signatureCbWrapper);
            index != -1 ;
            index = callbacks_.indexOf(signatureCbWrapper)){
            return callbacks_.at(index);
        }
        return signatureCbWrapper;
    }

    CallbackWrapper removeAndReturnCallbackWrapper(const QString& tag){
        CallbackWrapper signatureCbWrapper;
        signatureCbWrapper.tag = tag;
        for(int index = callbacks_.indexOf(signatureCbWrapper);
            index != -1 ;
            index = callbacks_.indexOf(signatureCbWrapper)){
            return callbacks_.takeAt(index);
        }
        return signatureCbWrapper;
    }

    QList<CallbackWrapper>& callbacks(){
        return callbacks_;
    }


private:
    QList<CallbackWrapper> callbacks_;
    CallbackParamValues<CallbackParamValueTypes...> defaultCallbackParamValues_;
};


/**
 * ----------------------------FiveParameterCallbacks------------------------------------
 */
template<typename CallbackWrapper,typename... CallbackParamValueTypes>
struct FiveParameterCallbacks{

    static CallbackParamValues<CallbackParamValueTypes...> generateCallbackParamValues(CallbackParamValueTypes... callbackParamValueTypes){
        return  CallbackParamValues<CallbackParamValueTypes...>::create(callbackParamValueTypes...);
    }

    void setDefaultCallbackParamValues(const CallbackParamValues<CallbackParamValueTypes...> &defaultCallbackParamValues)
    {
        defaultCallbackParamValues_ = defaultCallbackParamValues;
    }

    void invokeCallback(const QString &tag, CallbackParamValues<CallbackParamValueTypes ...> values){
        CallbackWrapper signatureCbWrapper;
        signatureCbWrapper.tag = tag;
        for(int index = callbacks_.indexOf(signatureCbWrapper);
            index != -1 ;
            index = callbacks_.indexOf(signatureCbWrapper)){
            auto cbWrapper = callbacks_.takeAt(index);
            if(cbWrapper.cb){
                auto tuple  = values.tuple();

                cbWrapper.cb(MOpaque<>::createShared(std::get<0>(tuple)
                             ,std::get<1>(tuple)
                             ,std::get<2>(tuple)
                             ,std::get<3>(tuple))
                             ,cbWrapper.opaque);
            }
        }
    }

    void registerCallback(const CallbackWrapper& callbackWrapper){
        if((!callbackWrapper.tag.isEmpty()) && (callbackWrapper.cb)){
            callbacks_.append((CallbackWrapper)callbackWrapper);
        }
    }

    void removeAllCallbacks(){
        foreach (auto cbWrapper, callbacks_) {
            invokeCallback(cbWrapper.tag,defaultCallbackParamValues_);
        }
        callbacks_.clear();
    }

    bool containsTag(const QString &tag){
        CallbackWrapper signatureCbWrapper;
        signatureCbWrapper.tag = tag;
        return callbacks_.contains(signatureCbWrapper);
    }

    int count(){
        return callbacks_.count();
    }

protected:
    CallbackWrapper getCallbackWrapper(const QString& tag){
        CallbackWrapper signatureCbWrapper;
        signatureCbWrapper.tag = tag;
        for(int index = callbacks_.indexOf(signatureCbWrapper);
            index != -1 ;
            index = callbacks_.indexOf(signatureCbWrapper)){
            return callbacks_.at(index);
        }
        return signatureCbWrapper;
    }

    CallbackWrapper removeAndReturnCallbackWrapper(const QString& tag){
        CallbackWrapper signatureCbWrapper;
        signatureCbWrapper.tag = tag;
        for(int index = callbacks_.indexOf(signatureCbWrapper);
            index != -1 ;
            index = callbacks_.indexOf(signatureCbWrapper)){
            return callbacks_.takeAt(index);
        }
        return signatureCbWrapper;
    }

    QList<CallbackWrapper>& callbacks(){
        return callbacks_;
    }

private:
    QList<CallbackWrapper> callbacks_;
    CallbackParamValues<CallbackParamValueTypes...> defaultCallbackParamValues_;
};


/**
 * ----------------------SixParameterCallbacks--------------------------
 */
template<typename CallbackWrapper,typename... CallbackParamValueTypes>
struct SixParameterCallbacks{

    static CallbackParamValues<CallbackParamValueTypes...> generateCallbackParamValues(CallbackParamValueTypes... callbackParamValueTypes){
        return  CallbackParamValues<CallbackParamValueTypes...>::create(callbackParamValueTypes...);
    }

    void setDefaultCallbackParamValues(const CallbackParamValues<CallbackParamValueTypes...> &defaultCallbackParamValues)
    {
        defaultCallbackParamValues_ = defaultCallbackParamValues;
    }

    void invokeCallback(const QString &tag, CallbackParamValues<CallbackParamValueTypes ...> values){
        CallbackWrapper signatureCbWrapper;
        signatureCbWrapper.tag = tag;
        for(int index = callbacks_.indexOf(signatureCbWrapper);
            index != -1 ;
            index = callbacks_.indexOf(signatureCbWrapper)){
            auto cbWrapper = callbacks_.takeAt(index);
            if(cbWrapper.cb){
                auto tuple  = values.tuple();

                cbWrapper.cb(MOpaque<>::createShared(std::get<0>(tuple)
                             ,std::get<1>(tuple)
                             ,std::get<2>(tuple)
                             ,std::get<3>(tuple)
                             ,std::get<4>(tuple))
                             ,cbWrapper.opaque);
            }
        }
    }

    void registerCallback(const CallbackWrapper& callbackWrapper){
        if((!callbackWrapper.tag.isEmpty()) && (callbackWrapper.cb)){
            callbacks_.append((CallbackWrapper)callbackWrapper);
        }
    }

    void removeAllCallbacks(){
        foreach (auto cbWrapper, callbacks_) {
            invokeCallback(cbWrapper.tag,defaultCallbackParamValues_);
        }
        callbacks_.clear();
    }

    bool containsTag(const QString &tag){
        CallbackWrapper signatureCbWrapper;
        signatureCbWrapper.tag = tag;
        return callbacks_.contains(signatureCbWrapper);
    }

    int count(){
        return callbacks_.count();
    }

protected:
    CallbackWrapper getCallbackWrapper(const QString& tag){
        CallbackWrapper signatureCbWrapper;
        signatureCbWrapper.tag = tag;
        for(int index = callbacks_.indexOf(signatureCbWrapper);
            index != -1 ;
            index = callbacks_.indexOf(signatureCbWrapper)){
            return callbacks_.at(index);
        }
        return signatureCbWrapper;
    }

    CallbackWrapper removeAndReturnCallbackWrapper(const QString& tag){
        CallbackWrapper signatureCbWrapper;
        signatureCbWrapper.tag = tag;
        for(int index = callbacks_.indexOf(signatureCbWrapper);
            index != -1 ;
            index = callbacks_.indexOf(signatureCbWrapper)){
            return callbacks_.takeAt(index);
        }
        return signatureCbWrapper;
    }

    QList<CallbackWrapper>& callbacks(){
        return callbacks_;
    }


private:
    QList<CallbackWrapper> callbacks_;
    CallbackParamValues<CallbackParamValueTypes...> defaultCallbackParamValues_;
};

#endif // MCPPCALLBACK_H
