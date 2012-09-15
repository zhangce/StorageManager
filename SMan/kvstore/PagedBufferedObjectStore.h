//
//  PagedBufferedObjectStore.h
//  SMan
//
//  Created by Ce Zhang on 9/14/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef SMan_PagedBufferedObjectStore_h
#define SMan_PagedBufferedObjectStore_h


#include "../common/Common.h"

#include "../objstore/BufferedObjStore.h"

namespace hazy{
  namespace sman{
    
    template<class VALUE, int ncontent>
    class page{
    public:
      VALUE contents[ncontent];
    };
    
    template<class VALUE, StorageType STORAGE, PropertyType PROPERTY>
    class PagedBufferedObjStore{
    public:
            
      const int NVALUE_PER_PAGE;
      
      BufferedObjStore< page<VALUE, 49600/sizeof(VALUE)>, STORAGE, PROPERTY > pagestore;
      
      PagedBufferedObjStore(int nbuffer) : NVALUE_PER_PAGE(49600/sizeof(VALUE)),
        pagestore(BufferedObjStore< page<VALUE, 49600/sizeof(VALUE)>, STORAGE, PROPERTY >(nbuffer))
      {
        
        std::cout << "Use PagedBufferedObjStore..." << std::endl;
      }
      
      ~PagedBufferedObjStore(){
        
      }
      
      StatusType apply(int64_t key, void (*func)(VALUE &)){
        
        VALUE v;
        
        this->get(key, v);
        
        func(v);
        
        this->set(key, v);
        
        return STATUS_SUCCESS;
      }
      
      StatusType get(int64_t key, VALUE & value){
        
        int64_t pid = key / NVALUE_PER_PAGE;
        int64_t poffset = key % NVALUE_PER_PAGE;
        
        pagestore.template get_meta<VALUE>(pid, poffset, value);
        
        return STATUS_SUCCESS;
      }
      
      StatusType set(int64_t key, const VALUE & value){
        
        int64_t pid = key / NVALUE_PER_PAGE;
        int64_t poffset = key % NVALUE_PER_PAGE;
        
        pagestore.template set_meta<VALUE>(pid, poffset, value);
                
        return STATUS_SUCCESS;
      }
      
      StatusType load(int64_t key, const VALUE & value){
        
        int64_t pid = key / NVALUE_PER_PAGE;
        int64_t poffset = key % NVALUE_PER_PAGE;
        
        pagestore.load(pid, page<VALUE, 49600/sizeof(VALUE)>());
        this->set(key, value);
        
        return STATUS_SUCCESS;
      }
      
    };
    
  }
}



#endif
