#include "graphmaster.h"
#include <iostream>

Graphmaster::Graphmaster() {}

AbstractNode* Graphmaster::ask(const std::string& path)
{
    std::stringstream ss(path);
    std::cout << path << std::endl;
    return _ask(ss, _root);
}

AbstractNode* Graphmaster::_ask(std::stringstream& ss, ANode& node)
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
            ANode* answer = _ask(ss, node[i]);
            if(answer != nullptr)
                return answer;
        }
    }
    return nullptr;
}

AbstractNode& Graphmaster::_walk_to(std::stringstream& ss, ANode& node)
{
    std::string word;
    long pos = ss.tellg();
    ss >> word;
    if(word == "")
        return node;
    for(size_t i=0; i<node.nbr_children(); i++)
    {
        if(node[i].match(word))
            return _walk_to(ss, node[i]);
    }
    ss.seekp(pos+1);
    ss << word;
    ss.seekg(pos);
    return node;
}

RootNode& Graphmaster::root()
{
    return _root;
}

AbstractNode* operator+(const AbstractNode& node, const size_t index)
{
    return *node._children + index;
}