#ifndef ARRAY_H
#define ARRAY_H

#include <cstddef>
#include "node.h"

class AbstractNode;

class Array{
public:
    Array(const size_t capa);
    void append(AbstractNode* node);
    AbstractNode* operator[](const size_t index);
    size_t shrink();
    ~Array();
private:
    size_t _size;
    size_t _capacity;
    AbstractNode** _data;
    void grow();
};

#endif
