#ifndef GRAPHMASTER_H
#define GRAPHMASTER_H

#include "node.h"
#include <sstream>

class Graphmaster{
public:
    Graphmaster();
    AbstractNode* ask(const std::string& path);
    void learn(const std::string& path, const std::string& answer);
    size_t nbr_nodes() const;
    std::string str();
    RootNode& root();
private:
    AbstractNode* _ask(std::stringstream& ss, ANode& node);
    AbstractNode& _walk_to(std::stringstream& ss, ANode& node);
    AbstractNode& _expend(std::stringstream& ss, ANode& from);
    std::string _str(ANode& node, const size_t n=0);
    RootNode _root;
    size_t _nbr_nodes;

};

#endif
