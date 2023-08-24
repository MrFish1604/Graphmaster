#include "graphmaster.h"
#include <iostream>

Graphmaster::Graphmaster() {}

AbstractNode* Graphmaster::walk_to(const std::string& path)
{
    std::stringstream ss(path);
    std::cout << path << std::endl;
    return _walk_to(ss, _root);
}

AbstractNode* Graphmaster::_walk_to(std::stringstream& ss, ANode& node)
{
    std::cout << node.label() << std::endl;
    if(node.is_answer())
        return &node;
    std::string word;
    ss >> word;
    for(size_t i=0; i<node.nbr_children(); i++)
    {
        if(node[i].is_answer())
            return node + i;
        if(node[i].match(word))
        {
            ANode* answer = _walk_to(ss, node[i]);
            if(answer != nullptr)
                return answer;
        }
    }
    return nullptr;
}

RootNode& Graphmaster::root()
{
    return _root;
}

AbstractNode* operator+(const AbstractNode& node, const size_t index)
{
    return *node._children + index;
}