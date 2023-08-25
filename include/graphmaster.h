#ifndef GRAPHMASTER_H
#define GRAPHMASTER_H

#include "node.h"
#include <sstream>

#define LOWER(c) (c>='A' && c<='Z' ? c+32 : c)

class Graphmaster{
public:
    Graphmaster();
    AnswerNode& ask(const std::string& path);
    void learn(const std::string& path, const std::string& answer);
    size_t nbr_nodes() const;
    std::string str();
    int parse(const std::string& filename);
    int shrink();
    RootNode& root();
private:
    int _shrink(AbstractNode& node);
    AnswerNode* _ask(std::stringstream& ss, ANode& node, unsigned int score);
    AbstractNode& _walk_to(std::stringstream& ss, ANode& node);
    AbstractNode& _expend(std::stringstream& ss, ANode& from);
    std::string _str(ANode& node, const size_t n=0);
    RootNode _root;
    size_t _nbr_nodes;
};

std::string trim(std::string& str);

#endif
