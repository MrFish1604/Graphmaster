#ifndef GRAPHMASTER_H
#define GRAPHMASTER_H

#include "node.h"
#include <sstream>
#include "dict.h"
#include <ctime>

#define LOWER(c) (c>='A' && c<='Z' ? c+32 : c)

class Graphmaster{
public:
    Graphmaster();
    AnswerNode& get_answer(const std::string& path);
    std::string ask(const std::string& path);
    std::string ask(const std::string& path, int& score);
    void learn(const std::string& path, const std::string& answer);
    void learn(const std::string& path, const std::string& answer, RootNode& root);
    size_t nbr_nodes() const;
    std::string str();
    int parse(const std::string& filename);
    int shrink();
    RootNode& root();
private:
    int _shrink(AbstractNode& node);
    AnswerNode* _ask(std::stringstream& ss, ANode& node, unsigned int score, Dict<std::string> dict);
    AbstractNode& _walk_to(std::stringstream& ss, ANode& node);
    AbstractNode& _expend(std::stringstream& ss, ANode& from);
    std::string _str(ANode& node, const size_t n=0);
    RootNode _root;
    AnswerNode* _last_answer;
    std::time_t _last_answer_epoch;
    size_t _nbr_nodes;
};

std::string trim(std::string& str);
bool contains(std::string& str, char c);

#endif
