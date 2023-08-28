#include "node.h"
#include <cstdlib>
#include <sstream>

AbstractNode::AbstractNode(): _parent(nullptr), _children(new Array(0)) {}

std::string AbstractNode::label()
{
    return "";
}

AbstractNode* AbstractNode::add_child()
{
    AbstractNode* child = new AbstractNode();
    child->_parent = this;
    _children->append(child);
    return child;
}

AbstractNode& AbstractNode::append_child(AbstractNode* child)
{
    child->_parent = this;
    _children->append(child);
    return _children->top();
}

size_t AbstractNode::nbr_children()
{
    return _children->size();
}

AbstractNode& AbstractNode::operator[](const size_t index)
{
    return _children->operator[](index);
}

bool AbstractNode::match(std::string& word)
{
    return false && word!="";
}

bool AbstractNode::is_answer()
{
    return false;
}

size_t AbstractNode::shrink()
{
    return _children->shrink();
}

AbstractNode::~AbstractNode()
{
    for(size_t i=0; i<_children->size(); i++)
        delete (*_children+i);
    delete _children;
}

Node::Node(const std::string& label): _label(label) {}

std::string Node::label()
{
    return _label;
}

Node* Node::add_child(const std::string& label)
{
    Node* child = new Node(label);
    child->_parent = this;
    _children->append(child);
    return child;
}

bool Node::match(std::string& word)
{
    return word == _label;
}

AnswerNode::AnswerNode(const std::string& answer): _answer(answer) {}

std::string AnswerNode::label()
{
    return _answer;
}

std::string AnswerNode::answer()
{
    std::stringstream label;
    label << _answer << ' ';
    std::stringstream answer;
    std::string word;
    label >> word;
    while(label.good())
    {
        if(word[0] == '*')
            answer << _collected.get_star();
        else if(word[0] == '$')
            answer << _collected[word.substr(1, word.length()-1)];
        else
            answer << word;
        answer << ' ';
        label >> word;
    }
    return answer.str();
}

bool AnswerNode::is_answer()
{
    return true;
}

unsigned int& AnswerNode::score()
{
    return _score;
}

std::string RootNode::label()
{
    return "ROOT";
}

std::string AbstractNode::str()
{
    return "<AbstractNode>";
}

std::string Node::str()
{
    return "<" + _label + ">";
}

std::string AnswerNode::str()
{
    return "{" + _answer + "}";
}

std::string RootNode::str()
{
    return "[ROOT]";
}

bool operator>(const AnswerNode& node1, const AnswerNode& node2)
{
    return node1._score > node2._score;
}

bool operator<(const AnswerNode& node1, const AnswerNode& node2)
{
    return node1._score < node2._score;
}

std::ostream& operator<<(std::ostream& stream, AbstractNode& node)
{
    stream << node.str();
    return stream;
}

AbstractNode* operator+(const AbstractNode& node, const size_t index)
{
    return *node._children + index;
}

char* copystr(std::string str)
{
    char* cstr = (char*) calloc(str.length()+1, sizeof(char));
    size_t i;
    for(i=0; i<str.length(); i++)
        cstr[i] = str[i];
    cstr[i] = '\0';
    return cstr;
}

OrNode::OrNode(const std::string& label)
{
    int s = 8;
    _labels = (char**) calloc(s, sizeof(char*));
    int i = 0;
    int k = 0;
    size_t l;
    for(l=0; l<label.length(); l++)
    {
        if(label[l]=='|')
        {
            if(i==s)
                _labels = (char**) reallocarray(_labels, s*=2, sizeof(char*));
            _labels[i++] = copystr(label.substr(k, l-k));
            k = l;
        }
    }
    if(i==s)
        _labels = (char**) reallocarray(_labels, s++, sizeof(char*));
    _labels[i++] = copystr(label.substr(k, l-k+1));
    _nbr_labels = i;
    if(i!=s)
        _labels = (char**) reallocarray(_labels, i, sizeof(char*));
}

std::string OrNode::label()
{
    std::string l = std::string(_labels[0]);
    for(int i=1; i<_nbr_labels; i++)
    {
        l += "|" + std::string(_labels[i]);
    }
    return l;
}

std::string OrNode::str()
{
    return "||" + label() + "||";
}

bool OrNode::match(std::string& word)
{
    for(int i=0; i<_nbr_labels; i++)
    {
        if(word == _labels[i])
            return true;
    }
    return false;
}

OrNode::~OrNode()
{
    for(int i=0; i<_nbr_labels; i++)
        free(_labels[i]);
    free(_labels);
}

Dict<std::string>& AnswerNode::collected()
{
    return _collected;
}