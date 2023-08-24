#ifndef NODE_H
#define NODE_H

#include <string>
#include "array.h"

class Array;

class AbstractNode{
public:    
    AbstractNode();
    AbstractNode& operator[](const size_t index);
    size_t shrink();
    size_t nbr_children();
    virtual std::string label();
    virtual AbstractNode* add_child();
    virtual void append_child(AbstractNode* child);
    virtual ~AbstractNode();
protected:
    AbstractNode* _parent;
    Array* _children;
};

class Node: public AbstractNode{
public:
    Node(const std::string& label);
    std::string label() override;
    Node* add_child(const std::string& label);
private:
    std::string _label;
};

class AnswerNode: public AbstractNode{
public:
    AnswerNode(const std::string& answer);
    virtual std::string label() override;
    std::string answer();
private:
    std::string _answer;
};

#endif
