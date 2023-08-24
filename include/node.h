#ifndef NODE_H
#define NODE_H

#include <string>
#include <iterator>
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
    virtual AbstractNode& append_child(AbstractNode* child);
    virtual bool match(std::string& word);
    virtual bool is_answer();
    virtual std::string str();
    virtual ~AbstractNode();
protected:
    AbstractNode* _parent;
    Array* _children;

friend AbstractNode* operator+(const AbstractNode& node, const size_t index);
};
typedef AbstractNode ANode;

class Node: public AbstractNode{
public:
    Node(const std::string& label);
    std::string label() override;
    std::string str() override;
    bool match(std::string& word) override;
    Node* add_child(const std::string& label);
private:
    std::string _label;
};

class AnswerNode: public AbstractNode{
public:
    AnswerNode(const std::string& answer);
    std::string label() override;
    std::string str() override;
    bool is_answer() override;
    std::string answer();
private:
    std::string _answer;
};

class RootNode: public AbstractNode{
    std::string label() override;
    std::string str() override;
};

#endif
