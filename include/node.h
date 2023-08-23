#ifndef NODE_H
#define NODE_H

#include <string>
#include "array.h"

class Array;

class AbstractNode{
public:    
    AbstractNode();
    virtual std::string label();
    virtual AbstractNode* add_child();
    virtual ~AbstractNode();
private:
    AbstractNode* _parent;
    Array* _children;
};

class Node: public AbstractNode{
public:
    Node(const std::string& label);
    std::string label() override;
    ~Node() override;
private:
    std::string _label;
};

#endif
