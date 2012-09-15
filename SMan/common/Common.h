//
//  Common.h
//  SMan
//
//  Created by Ce Zhang on 9/7/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef SMan_Common_h
#define SMan_Common_h


#define OBJECTSTORE_INIT_CONTAINER_SIZE 100



#include <iostream>
#include <inttypes.h>
#include <assert.h>
#include <string>

#include <fcntl.h>
#include <vector>
#include <list>

#include <semaphore.h>

#include <string.h>
#include <stdio.h>

#include "Timer.h"

namespace hazy {
  namespace sman{
    
    enum PropertyType {
      PROPERTY_NIL = 0x00000000,
      JAVAHASH_MM  = 0x00000001,
      JAVAHASH_ACCU= 0x00000010,
    };
    
    enum StorageType {
      STORAGE_MM    = 0x00000001,
      STORAGE_MMAP  = 0x00000010,
      STORAGE_FILE  = 0x00000100,
      STORAGE_JHASH = 0x00010000,
      STORAGE_HBASE = 0x00100000
    };

    enum BufferType {
      BUFFER_NIL   = 0x00000001,
      BUFFER_LRU   = 0x00000010,
      BUFFER_RANDOM= 0x00000100
    };
    
    enum StatusType {
      STATUS_SUCCESS         = 0x00000001,
      STATUS_GET_KEYNOTFOUND = 0x00000010,
      STATUS_SET_INVALIDKEY  = 0x00000100
    };
    

    int ntmp = 0;
    std::string getNextTmpFileName(){
      char tmp[1000];
      sprintf(tmp, "/tmp/%d", (++ntmp));
      return std::string(tmp);
    }
    
    
  }
}

#endif
