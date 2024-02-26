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
    learn(path, new AnswerNode(answer), root);
}

void Graphmaster::learn(const std::string& path, AnswerNode* answer_node, RootNode& root)
{
    std::stringstream ss(" "+path);
    ANode& node = _walk_to(ss, root);
    _nbr_nodes++;
    _expend(ss, node).append_child(answer_node);
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
        if(node[i].is_answer()){
            AnswerNode& an = static_cast<AnswerNode&>(node[i]);
            if(an._root!=nullptr)
                rtn += _str(*an._root, n+2);
        }
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

std::stringstream lexer(const std::string& filename)
{
    std::stringstream ss;
    std::ifstream file(filename);
    if(!file)
    {
        ss << "Error: could not open file \"" << filename << "\"\n";
        return ss;
    }

    char c;
    std::string buff = "";
    while(file.get(c))
    {
        switch(c){
            case ' ':
            case '\n':
            case '\t':
                if(buff!=""){
                    ss << buff << ' ';
                    buff = "";
                }
                break;
            case '{':
            case '}':
            case '(':
            case ')':
            case '|':
                if(buff!=""){
                    ss << buff << ' ';
                    buff = "";
                }
                ss << c << ' ';
                break;
            case '\\':
                if(file.get(c))
                    buff += c;
                break;
            case '#':
                while(file.get(c) && c!='\n');
                break;
            default:
                buff += c;
        }
    }
    file.close();
    return ss;
}

std::string peek_token(std::stringstream& ss)
{
    if(ss.eof())
        throw std::runtime_error("Parser error: unexpected end of file. (peek_token)");
    std::string token;
    long pos = ss.tellg();
    ss >> token;
    ss.seekg(pos);
    return token;
}

void parser(Graphmaster& gm, const std::string& filename)
{
    std::stringstream ss = lexer(filename);
    while(ss.good() && !peek_token(ss).empty())
    {
        parse_rule(gm, ss, gm._root);
    }
}

void parse_rule(Graphmaster& gm, std::stringstream& ss, RootNode& root)
{
    std::string token;
    token = consume(ss);
    std::string prompt = "";
    while(token != "{")
    {
        prompt += token + " ";
        token = consume(ss);
        if(token == "}")
            throw std::runtime_error("Parser error: Got '}' before '{'.");
    }
    if(prompt == "")
        throw std::runtime_error("Parser error: expected a prompt before '{', got nothing.");
    AnswerNode* an = parse_answer(gm, ss);
    gm.learn(prompt, an, root);
}


AnswerNode* parse_answer(Graphmaster& gm, std::stringstream& ss)
{
    std::string token;
    AnswerNode* an = new AnswerNode("");
    while((token = peek_token(ss)) != "}")
    {
        if(token == REPLY)
        {
            consume(ss);
            token = consume(ss);
            if(token!="(")
                throw std::runtime_error("Parser error: expected '(' after 'reply', got '"+ token + "'.");
            token = consume(ss);
            an->_answer += token;
            while(peek_token(ss) != ")")
            {
                token = consume(ss);
                an->_answer += " " + token;
            }
            token = consume(ss);
            continue;
        }
        if(token == TTL)
        {
            consume(ss);
            token = consume(ss);
            if(token!="(")
                throw std::runtime_error("Parser error: expected '(' after 'ttl', got '"+ token + "'.");
            token = consume(ss);
            try{
                an->_time_limit = std::stoi(token);
                token = consume(ss);
                if(token!=")")
                    throw std::runtime_error("Parser error: expected ')' after '"+ std::to_string(an->_time_limit) + "', got '"+ token + "'.");
            }
            catch(std::invalid_argument& e)
            {
                throw std::runtime_error("Parser error: expected an integer after 'ttl', got '"+ token + "'.");
            }
            continue;
        }
        an->_root = new RootNode();
        parse_rule(gm, ss, *(an->_root));
    }
    consume(ss);
    return an;
}

int ij = 0;
std::string consume(std::stringstream& ss)
{
    if(ss.eof())
        throw std::runtime_error("Parser error: unexpected end of file. (consume)");
    std::string token;
    ss >> token;
    if(token.empty())
        throw std::runtime_error("Parser error: unexpected end of file. (consume)");
    return token;
}
