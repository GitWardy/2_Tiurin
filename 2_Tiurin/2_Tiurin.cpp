#include "Header.h"

// Инициализация статического счётчика
int ExprNode::nextID = 0;


/**
 * Конструктор для узла-переменной
 */
ExprNode::ExprNode(NodeType t, char n)
    : type(t), name(n), left(nullptr), right(nullptr)
{
    ID = nextID++;   // присвоить уникальный ID
}

/**
 * Конструктор для узла-операции
 */
ExprNode::ExprNode(NodeType t, ExprNode* l, ExprNode* r)
    : type(t), name(0), left(l), right(r)
{
    ID = nextID++;   // присвоить уникальный ID
}


/**
 * Деструктор: рекурсивно удаляет левoгo и правoгo потомков
 */
ExprNode::~ExprNode()
{
    if (left) delete left;
    if (right) delete right;                                                  
}

/**
 * Оператор присваивания: кoпирует type, name, left, right.
 * Поле ID не кoпируется (остаётся уникальным).
 */
ExprNode& ExprNode::operator=(const ExprNode& other)
{
    if (this != &other)
    {
        type = other.type;
        name = other.name;
        left = other.left;
        right = other.right;
    }

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