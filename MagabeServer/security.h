/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef SECURITY_H
#define SECURITY_H
#include "Helpers/simplecrypt.h"
#include <assert.h>

class Security
{
public:
  Security(const Security& rhs) = delete;
  Security& operator =(const Security& rhs) = delete;
  static SimpleCrypt *instance(){
    // no need to delete it, it will be deleted by Os, when app exit,coz it is static
    static SimpleCrypt *crypto;
    if(!crypto){
        crypto = new SimpleCrypt(Q_UINT64_C(0xbd4f9fefd46f3a27));
      }
    return crypto;
  }
protected:
  Security(){} //only can be instantiated inside class or inside chird class
  ~Security(){}
};

#endif // SECURITY_H
