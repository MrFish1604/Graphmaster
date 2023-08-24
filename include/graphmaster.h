#ifndef GRAPHMASTER_H
#define GRAPHMASTER_H

#include "node.h"
#include <sstream>

class Graphmaster{
public:
    Graphmaster();
    AbstractNode* ask(const std::string& path);
    AbstractNode& _walk_to(std::stringstream& ss, ANode& node);
    RootNode& root();
private:
    AbstractNode* _ask(std::stringstream& ss, ANode& node);
    RootNode _root;

};

#endif
