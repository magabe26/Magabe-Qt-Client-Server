#ifndef POINTERUTIL_H
#define POINTERUTIL_H
#include <QObject>

struct PointerUtil {

    static jlong toJlongPointer(QObject *obj){
        if(obj){
            return jlong(obj);
        }else{
            return jlong(0);
        }
    }

    template <typename T>
    static T* toObjectPointer(const jlong ptr){
        if(ptr == 0){
            return nullptr;
        }else{
            return reinterpret_cast<T*>(ptr);
        }
    }
};

#endif // POINTERUTIL_H
