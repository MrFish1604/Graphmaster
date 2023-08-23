#include "array.h"
#include <cstdlib>

Array::Array(const size_t capa): _size(0), _capacity(capa)
{
    if(_capacity == 0)
        _data = nullptr;
    else
        _data = (AbstractNode**)calloc(_capacity, sizeof(AbstractNode*));
}

void Array::append(AbstractNode* node)
{
    if(_size == _capacity)
        grow();
    _data[_size++] = node;
}

AbstractNode& Array::operator[](const size_t index)
{
    return *_data[index];
}

AbstractNode* operator+(const Array& array, const size_t index)
{
    return array._data[index];
}

AbstractNode& Array::top()
{
    return *_data[_size-1];
}

size_t Array::shrink()
{
    _data = (AbstractNode**)reallocarray(_data, _size, sizeof(AbstractNode*));
    const size_t r = _capacity - _size;
    _capacity = _size;
    return r;
}

size_t Array::size() const
{
    return _size;
}

void Array::grow()
{
    if(_capacity == 0)
    {
        _capacity = 8;
        _data = (AbstractNode**)reallocarray(_data, _capacity, sizeof(AbstractNode*));
        return;    
    }
    _capacity = _capacity << 1;
    _data = (AbstractNode**)reallocarray(_data, _capacity, sizeof(AbstractNode*));
}

Array::~Array()
{
    free(_data);
}

