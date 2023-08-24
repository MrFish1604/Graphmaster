#include "node.h"
#include <iostream>

AbstractNode::AbstractNode(): _parent(nullptr), _children(new Array(0)) {}

std::string AbstractNode::label()
{
    return "";
}

AbstractNode* AbstractNode::add_child()
{
    AbstractNode* child = new AbstractNode();
    child->_parent = this;
    _children->append(child);
    return child;
}

AbstractNode& AbstractNode::append_child(AbstractNode* child)
{
    child->_parent = this;
    _children->append(child);
    return _children->top();
}

size_t AbstractNode::nbr_children()
{
    return _children->size();
}

AbstractNode& AbstractNode::operator[](const size_t index)
{
    return _children->operator[](index);
}

bool AbstractNode::match(std::string& word)
{
    return false && word!="";
}

bool AbstractNode::is_answer()
{
    return false;
}

size_t AbstractNode::shrink()
{
    return _children->shrink();
}

AbstractNode::~AbstractNode()
{
    for(size_t i=0; i<_children->size(); i++)
        delete (*_children+i);
    delete _children;
}

Node::Node(const std::string& label): _label(label) {}

std::string Node::label()
{
    return _label;
}

Node* Node::add_child(const std::string& label)
{
    Node* child = new Node(label);
    child->_parent = this;
    _children->append(child);
    return child;
}

bool Node::match(std::string& word)
{
    return word == _label;
}

AnswerNode::AnswerNode(const std::string& answer): _answer(answer) {}

std::string AnswerNode::label()
{
    return _answer;
}

std::string AnswerNode::answer()
{
    return _answer;
}

bool AnswerNode::is_answer()
{
    return true;
}

std::string RootNode::label()
{
    return "ROOT";
}

std::string AbstractNode::str()
{
    return "<AbstractNode>";
}

std::string Node::str()
{
    return "<" + _label + ">";
}

std::string AnswerNode::str()
{
    return "{" + _answer + "}";
}

std::string RootNode::str()
{
    return "[ROOT]";
}