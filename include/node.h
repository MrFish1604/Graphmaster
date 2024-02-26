#ifndef NODE_H
#define NODE_H

#include <string>
#include <iterator>
#include "array.h"
#include <ostream>
#include "dict.h"

class Array;
class Graphmaster;

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

class RootNode: public AbstractNode{
    std::string label() override;
    std::string str() override;
};

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

class OrNode: public AbstractNode{
public:
    OrNode(const std::string& label);
    std::string label() override;
    std::string str() override;
    bool match(std::string& word) override;
    ~OrNode();
private:
    char** _labels;
    int _nbr_labels;

};

class AnswerNode: public AbstractNode{
public:
    AnswerNode(const std::string& answer);
    AnswerNode(const std::string& answer, const int tl);
    std::string label() override;
    std::string str() override;
    bool is_answer() override;
    Dict<std::string>& collected();
    unsigned int& score();
    std::string answer();
    int time_limit();
    ~AnswerNode();
#ifndef TEST
private:
#endif
    std::string _answer;
    unsigned int _score;
    Dict<std::string> _collected;
    RootNode* _root;
    int _time_limit;
friend bool operator>(const AnswerNode& node1, const AnswerNode& node2);
friend bool operator<(const AnswerNode& node1, const AnswerNode& node2);
friend class Graphmaster;
friend AnswerNode* parse_answer(Graphmaster& gm, std::stringstream& ss);
};

std::ostream& operator<<(std::ostream& stream, AbstractNode& node);

#endif
