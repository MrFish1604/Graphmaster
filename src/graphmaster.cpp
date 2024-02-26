#include "graphmaster.h"
#include <fstream>
#include <ctime>
#include <stdexcept>

Graphmaster::Graphmaster(): _last_answer(nullptr), _nbr_nodes(1) {}

std::string Graphmaster::ask(const std::string& path)
{
    AnswerNode& an = get_answer(path);
    return an.answer();
}

std::string Graphmaster::ask(const std::string& path, int& score)
{
    AnswerNode& an = get_answer(path);
    score = an.score();
    return an.answer();
}

AnswerNode& Graphmaster::get_answer(const std::string& path)
{
    RootNode& root = _last_answer!=nullptr && _last_answer->_root!=nullptr && std::time(nullptr)-_last_answer_epoch<_last_answer->time_limit() ? *(_last_answer->_root) : _root;
    std::stringstream ss(path);
    AnswerNode* rtn = _ask(ss, root, 0, Dict<std::string>());
    _last_answer = rtn;
    if(rtn == nullptr)
        throw std::runtime_error("No answer found for the path: \"" + path + "\"\nConsider adding a default answer to avoid this error.");
    _last_answer_epoch = std::time(nullptr);
    return *rtn;
}

AnswerNode* Graphmaster::_ask(std::stringstream& ss, ANode& node, unsigned int score, Dict<std::string> dict)
{
    std::string word;
    int pos = ss.tellg();
    if(!(ss >> word))
    {
        ss.clear();
        for(size_t i=0; i<node.nbr_children(); i++)
        {
            if(node[i].is_answer())
            {
                AnswerNode* answer = static_cast<AnswerNode*>(node+i);
                answer->score() = score;
                answer->collected() = dict;
                return answer;
            }
        }
        return nullptr;
    }
    AnswerNode* best = nullptr;
    for(size_t i=0; i<node.nbr_children(); i++)
    {
        if(node[i].match(word))
        {
            AnswerNode* r = _ask(ss, node[i], score, dict);
            if(r!=nullptr && (best==nullptr || *r < *best))
            {
                best = r;
            }
        }
        if(node[i].label() == "*")
        {
            dict.add("*", word);
            AnswerNode* r = _ask(ss, node[i], score+3, dict);
            dict.rmlast();
            if(r!=nullptr && (best==nullptr || *r < *best))
            {
                best = r;
            }
        }
        if(node[i].label() == "**")
        {
            dict.add("**", word);
            AnswerNode* r = _ask(ss, node[i], score+2, dict);
            dict.rmlast();
            if(r!=nullptr && (best==nullptr || *r < *best))
            {
                best = r;
            }
        }
        if(node[i].label()[0]=='$')
        {
            dict.add(node[i].label().substr(1), word);
            AnswerNode* r = _ask(ss, node[i], score+1, dict);
            dict.rmlast();
            if(r!=nullptr && (best==nullptr || *r < *best))
            {
                best = r;
            }
        }
    }
    if(node.label() == "*" && best==nullptr)
    {
        dict.last_star() += " " + word;
        best = _ask(ss, node, score+3, dict);
        dict.rmlast();
    }
    ss.seekg(pos);
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
    learn(path, answer, _root);
}

void Graphmaster::learn(const std::string& path, const std::string& answer, RootNode& root)
{
    std::stringstream ss(" "+path);
    ANode& node = _walk_to(ss, root);
    _nbr_nodes++;
    _expend(ss, node).append_child(new AnswerNode(answer));
}

AbstractNode& Graphmaster::_expend(std::stringstream& ss, ANode& from)
{
    std::string word;
    ss >> word;
    if(word!="")
    {
        _nbr_nodes++;
        if(contains(word, '|'))
            return _expend(ss, from.append_child(new OrNode(word)));
        return _expend(ss, from.append_child(new Node(word)));
    }
    return from;
}

bool contains(std::string& str, char c)
{
    if(str[0]==c)
        return true;
    for(size_t i=1; i<str.size(); i++)
        if(str[i-1]!='\\' && str[i]==c)
            return true;
    return false;
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

int Graphmaster::shrink()
{
    return _shrink(_root);
}

int Graphmaster::_shrink(AbstractNode& node)
{
    int sum = 0;
    for(size_t i=0; i<node.nbr_children(); i++)
        sum += _shrink(node[i]);
    return sum + node.shrink();
}

#define KW(x) (c==x && (buff=="" || buff[buff.size()-1]!='\\'))
int Graphmaster::parse(const std::string& filename)
{
    std::ifstream file(filename);
    if(!file)
        return -1;
    
    size_t rtn = _nbr_nodes;
    char c;
    std::string buff = "";
    std::string prompt;
    std::string answer;
    int w = 0;
    int line = 0;
    while(file.get(c))
    {
        if(c=='\n')
        {
            line++;
            continue;
        }
        if(c=='\t' || (buff=="" && c==' '))
            continue;
        if(c=='#')
        {
            while(file.get(c) && c!='\n');
            line++;
            continue;
        }
        switch(w)
        {
            case 0:
                if(KW('{'))
                {
                    prompt = buff;
                    w = 1;
                    buff = "";
                    continue;
                }
                break;
            case 1:
                if(KW('('))
                {
                    if(buff=="reply")
                        w = 2;
                    else
                        return -line;
                    buff = "";
                    continue;
                }
                if(KW('}'))
                {
                    if(prompt=="" || answer=="")
                        return -line;
                    learn(prompt, answer);
                    buff = "";
                    prompt = "";
                    answer = "";
                    w = 0;
                    continue;
                }
                break;
            case 2:
                if(KW(')'))
                {
                    answer = buff;
                    buff = "";
                    w = 1;
                    continue;
                }
                break;

        }
        buff += LOWER(c);
    }
    return _nbr_nodes - rtn;
}

std::string trim(std::string& str)
{
    size_t i=0;
    while(str[i]==' ' || str[i]=='\t' || str[i]=='\n')
        i++;
    size_t j=str.size()-1;
    while(str[j]==' ' || str[j]=='\t' || str[j]=='\n')
        j--;
    return str.substr(i, j-i+1);
}