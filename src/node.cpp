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

void AbstractNode::append_child(AbstractNode* child)
{
    child->_parent = this;
    _children->append(child);
}

size_t AbstractNode::nbr_children()
{
    return _children->size();
}

AbstractNode& AbstractNode::operator[](const size_t index)
{
    return _children->operator[](index);
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

AnswerNode::AnswerNode(const std::string& answer): _answer(answer) {}

std::string AnswerNode::label()
{
    return _answer;
}

std::string AnswerNode::answer()
{
    return _answer;
}