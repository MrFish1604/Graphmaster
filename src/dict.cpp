#include "dict.h"
#include <sstream>

template<typename T>
Dict<T>::Dict(): _data(nullptr), _last(nullptr) {}

template<typename T>
T& Dict<T>::add(const std::string label, T data)
{
    if(_data == nullptr)
    {
        _data = new DictWrapper<T>;
        _data->data = data;
        _data->label = label;
        _last = _data;
        return _last->data;
    }
    _last->next = new DictWrapper<T>;
    _last = _last->next;
    _last->data = data;
    _last->label = label;
    return _last->data;
}

template<typename T>
Dict<T>::~Dict()
{
    DictWrapper<T>* tmp;
    while(_data != nullptr)
    {
        tmp = _data;
        _data = _data->next;
        delete tmp;
    }
}

template<typename T>
T& Dict<T>::operator[](const std::string& label)
{
    DictWrapper<T>* tmp = _data;
    while(tmp != nullptr)
    {
        if(tmp->label == label)
            return tmp->data;
        tmp = tmp->next;
    }
    return add(label, T());
}

template<typename T>
bool Dict<T>::exists(const std::string& label)
{
    DictWrapper<T>* tmp = _data;
    while(tmp != nullptr)
    {
        if(tmp->label == label)
            return true;
        tmp = tmp->next;
    }
    return false;
}

template<typename T>
std::string Dict<T>::str() const
{
    std::stringstream ss;
    DictWrapper<T>* tmp = _data;
    while(tmp != nullptr)
    {
        ss << tmp->label << ": " << tmp->data << "\n";
        tmp = tmp->next;
    }
    return ss.str();
}