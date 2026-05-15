#include "Header.h"

// Инициализация статического счётчика
int ExprNode::nextID = 0;

// Конструкторы и деструктор (простые заглушки)
ExprNode::ExprNode(NodeType t, char n)
    : type(t), name(n), left(nullptr), right(nullptr)
{
    ID = nextID++;
}

ExprNode::ExprNode(NodeType t, ExprNode* l, ExprNode* r)
    : type(t), name(0), left(l), right(r)
{
    ID = nextID++;
}

ExprNode::~ExprNode()
{
    // заглушка
}

ExprNode& ExprNode::operator=(const ExprNode& other)
{
    // заглушка
    return *this;
}

vector<Token> tokenize(const string& expr)
{
    vector<Token> result;   // заглушка: возвращаем пустой вектор
    return result;
}

ExprNode* createLogicalTree(const string& expr, vector<ErrorInfo>& errors)
{
    return nullptr;          // заглушка
}

void deMorganTransform(ExprNode* node)
{
    // заглушка
}

void removeDoubleNegation(ExprNode* node)
{
    // заглушка
}

void simplifyTree(ExprNode* node)
{
    // заглушка
}

void saveExprNodeToDot(ExprNode* node, ostream& out)
{
    // заглушка
}

void saveDotToFile(ExprNode* root, const string& path, vector<ErrorInfo>& errors)
{
    // заглушка
}

int main(int argc, char* argv[])
{
    // заглушка – просто возвращаем 0
    return 0;
}