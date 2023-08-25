#include "dict.h"
#include <sstream>

template<typename T>
Dict<T>::Dict(): _next(1), _data(nullptr), _last(nullptr) {}

template<typename T>
Dict<T>::Dict(const Dict<T>& dict): _next(dict._next)
{
    if(dict._data==nullptr)
    {
        _data = nullptr;
        _last = nullptr;
        return;
    }
    _data = new DictWrapper<T>;
    _data->data = dict._data->data;
    _data->label = dict._data->label;
    _data->next = nullptr;
    _last = _data;
    DictWrapper<T>* tmp = dict._data->next;
    while(tmp!=nullptr)
    {
        _last->next = new DictWrapper<T>;
        _last = _last->next;
        _last->data = tmp->data;
        _last->label = tmp->label;
        tmp = tmp->next;
    }
    _last->next = nullptr;
}

template<typename T>
void Dict<T>::clear()
{
    DictWrapper<T>* tmp;
    while(_data != nullptr)
    {
        tmp = _data;
        _data = _data->next;
        delete tmp;
    }
    _last = nullptr;
    _next = 1;
}

template<typename T>
void Dict<T>::operator=(const Dict<T>& dict)
{
    clear();
    DictWrapper<T>* tmp = dict._data;
    while(tmp!=nullptr)
    {
        operator[](tmp->label) = tmp->data;
        tmp = tmp->next;
    }

}

template<typename T>
void Dict<T>::operator-=(const Dict<T>& dict)
{
    DictWrapper<T>* tmp = dict._data;
    while(tmp != nullptr)
    {
        rm(tmp->label);
        tmp = tmp->next;
    }
}

template<typename T>
void Dict<T>::rm(std::string label)
{   
    if(_data->label == label)
    {
        if(_data->label=="*" || _data->label=="**")
            _next--;
        if(_data==_last)
        {
            _last = nullptr;
            return;
        }
        DictWrapper<T>* tmp = _data->next;
        delete _data;
        _data = tmp;
        return;
    }
    DictWrapper<T>* tmp0 = _data;
    DictWrapper<T>* tmp = _data->next;
    while(tmp != nullptr)
    {
        if(tmp->label == label)
        {
            if(tmp->label=="*" || tmp->label=="**")
                _next--;
            tmp0->next = tmp->next;
            delete tmp;
            return;
        }
        tmp0 = tmp;
        tmp = tmp->next;
    }
}

template<typename T>
void Dict<T>::rmlast()
{
    if(_data==_last)
    {
        if(_next!=1)
            _next = 1;
        delete _data;
        _data = nullptr;
        _last = nullptr;
        return;
    }
    DictWrapper<T>* tmp = _data;
    while(tmp->next != _last)
        tmp = tmp->next;
    if(_last->label=="*" || _last->label=="**")
        _next--;
    delete _last;
    _last = tmp;
    _last->next = nullptr;
}

template<typename T>
T& Dict<T>::add(std::string label, T data)
{
    if(label == "*" || label == "**")
        label = std::to_string(_next++);
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
    std::stringstream ss("DICT:\n");
    DictWrapper<T>* tmp = _data;
    while(tmp != nullptr)
    {
        ss << tmp->label << ": " << tmp->data << "\n";
        tmp = tmp->next;
    }
    return ss.str();
}

template<typename T>
T& Dict<T>::last_star()
{
    return operator[](std::to_string(_next-1));
}