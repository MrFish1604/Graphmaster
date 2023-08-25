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
    Dict(const Dict<T>& dict);
    void operator=(const Dict<T>& dict);
    void operator-=(const Dict<T>& dict);
    void clear();
    void rm(std::string label);
    void rmlast();
    T& add(std::string label, T data);
    std::string str() const;
    T& operator[](const std::string& label);
    bool exists(const std::string& label);
    T& last_star();
    ~Dict();
private:
    int _next;
    struct DictWrapper<T>* _data;
    struct DictWrapper<T>* _last;
};

template class Dict<std::string>;
template class Dict<int>;

#endif
