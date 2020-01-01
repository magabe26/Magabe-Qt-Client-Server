/**
 * Copyright 2018-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef MAGABESINGLETON_H
#define MAGABESINGLETON_H
#include <iostream>
#include <QDebug>

template<typename T>
class MagabeSingleton{
public:
  MagabeSingleton(const MagabeSingleton& rhs) = delete;
  MagabeSingleton& operator =(const MagabeSingleton& rhs) = delete;
  static T *instance(){
    // no need to delete it, it will be deleted by Os, when app exit,coz it is static
    static T *obj;
    if(!obj){
        try{
          obj = new T();
        }catch(std::bad_alloc e){
          qDebug() << "MagabeSingleton:: " << e.what();
          return instance();
        }catch(...){
          qDebug() << "MagabeSingleton:: memory allocation error";
          return instance();
        }
      }
    return obj;
  }
protected:
  MagabeSingleton(){} //only can be instantiated inside class or inside chird class
  ~MagabeSingleton(){}
};
#endif // MAGABESINGLETON_H
