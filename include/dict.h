#ifndef DICT_H
#define DICT_H

#include <string>

template<typename T>
struct DictWrapper{
    T data;
    std::string label;
    DictWrapper<T>* next=nullptr;
};

template<typename T>
class Dict{ // Should be about 10 entries in it. -> Linear search is good enough.
public:
    Dict();
    T& add(const std::string label, T data);
    std::string str() const;
    T& operator[](const std::string& label);
    bool exists(const std::string& label);
    ~Dict();
private:
    struct DictWrapper<T>* _data;
    struct DictWrapper<T>* _last;
};

template class Dict<std::string>;
template class Dict<int>;

#endif
