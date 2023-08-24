#include "graphmaster.h"
#include <iostream>

Graphmaster::Graphmaster(): _nbr_nodes(1)
{
    learn("*", "Answer not found");
}

AnswerNode& Graphmaster::ask(const std::string& path)
{
    std::stringstream ss(path);
    std::cout << path << std::endl;
    AnswerNode* rtn = _ask(ss, _root, 0);
    return *rtn;
}

AnswerNode* Graphmaster::_ask(std::stringstream& ss, ANode& node, unsigned int score)
{
    std::string word;
    ss >> word;
    AnswerNode* best = nullptr;
    for(size_t i=0; i<node.nbr_children(); i++)
    {
        if(word == "")
        {
            AnswerNode* answer = nullptr;
            if(node[i].is_answer())
            {
                answer = static_cast<AnswerNode*>(node+i);
                answer->score() = score;
            }
            return answer;
        }
        if(node[i].match(word))
        {
            AnswerNode* r = _ask(ss, node[i], score);
            if(r!=nullptr && (best==nullptr || *r < *best))
            {
                best = r;
            }
        }
    }
    return best;
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

void Graphmaster::learn(const std::string& path, const std::string& answer)
{
    std::stringstream ss(" "+path);
    ANode& node = _walk_to(ss, _root);
    _nbr_nodes++;
    _expend(ss, node).append_child(new AnswerNode(answer));
}

AbstractNode& Graphmaster::_expend(std::stringstream& ss, ANode& from)
{
    std::string word;
    ss >> word;
    std::cout << word << std::endl;
    if(word!="")
    {
        _nbr_nodes++;
        return _expend(ss, from.append_child(new Node(word)));
    }
    return from;
}

size_t Graphmaster::nbr_nodes() const
{
    return _nbr_nodes;
}

RootNode& Graphmaster::root()
{
    return _root;
}

std::string Graphmaster::str()
{
    return _str(_root);
}

std::string Graphmaster::_str(ANode& node, const size_t n)
{
    std::string rtn = "";
    rtn.insert(0, n, '\t');
    rtn +=  node.str() + "\n";
    for(size_t i=0; i<node.nbr_children(); i++)
    {
        rtn += _str(node[i], n+1);
    }
    return rtn;
}

AbstractNode* operator+(const AbstractNode& node, const size_t index)
{
    return *node._children + index;
}