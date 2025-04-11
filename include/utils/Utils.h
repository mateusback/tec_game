#ifndef UTILS_H
#define UTILS_H

#include <my-lib/any.h>

namespace Utils {

    //Criado por ehmcruz
    struct Descriptor {
        uint64_t id;
        Mylib::Any<sizeof(void*), sizeof(void*)> data;
    };

    
}

#endif