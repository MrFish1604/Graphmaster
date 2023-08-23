#include "node.h"
#include <iostream>

AbstractNode::AbstractNode(): _parent(nullptr), _children(new Array(0))
{

}

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

AbstractNode::~AbstractNode()
{
    delete _children;
}

Node::Node(const std::string& label): _label(label)
{

}

std::string Node::label()
{
    return _label;
}

Node::~Node()
{
    
}