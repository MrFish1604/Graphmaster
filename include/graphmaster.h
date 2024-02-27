#ifndef GRAPHMASTER_H
#define GRAPHMASTER_H

#include "node.h"
#include <sstream>
#include "dict.h"
#include <ctime>

#define REPLY "reply"
#define EXEC "exec"
#define TTL "ttl"

#define CMD_SIZE 32
#define PIPE_BUFFER_SIZE 256

#define LOWER(c) (c>='A' && c<='Z' ? c+32 : c)

class Graphmaster{
public:
    Graphmaster();
    AnswerNode& get_answer(const std::string& path);
    std::string ask(const std::string& path);
    std::string ask(const std::string& path, int& score);
    void learn(const std::string& path, const std::string& answer);
    void learn(const std::string& path, const std::string& answer, RootNode& root);
    void learn(const std::string& path, AnswerNode* answer_node, RootNode& root);
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
friend void parser(Graphmaster& gm, const std::string& filename);
};

std::stringstream lexer(const std::string& filename);
void parse_rule(Graphmaster& gm, std::stringstream& ss, RootNode& root);
AnswerNode* parse_answer(Graphmaster& gm, std::stringstream& ss);

std::string consume(std::stringstream& ss);

std::string peek_token(std::stringstream& ss);

std::string my_exec(const std::string& cmd, int& err);

std::string trim(std::string& str);
bool contains(std::string& str, char c);

#endif
