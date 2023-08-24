#ifndef GRAPHMASTER_H
#define GRAPHMASTER_H

#include "node.h"
#include <sstream>

class Graphmaster{
public:
    Graphmaster();
    AbstractNode* walk_to(const std::string& path);
    RootNode& root();
private:
    AbstractNode* _walk_to(std::stringstream& ss, ANode& node);
    RootNode _root;

};

#endif
