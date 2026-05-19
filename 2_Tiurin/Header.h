#pragma once


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <cctype>
#include <windows.h>

using namespace std;

/**
 * @enum NodeType
 * @brief Типы узлов дерева логического выражения
 */
enum NodeType
{
    VAR,     ///< Переменная
    NOT,     ///< Отрицание "!"
    AND,     ///< Конъюнкция "*"
    OR       ///< Дизъюнкция "+"
};

/**
 * @enum ErrorCode
 * @brief Коды ошибок согласно внешней спецификации
 */
enum ErrorCode
{
    FILE_NOT_OPENING,
    MORE_THAN_ONE_STRING,
    STRING_TOO_LONG,
    WRONG_SYMBOL,
    NOT_ENOUGH_OPERANDS,
    NOT_ENOUGH_OPERATORS,
    EMPTY_TREE
};

/**
 * @struct ErrorInfo
 * @brief Информация об ошибке
 */
struct ErrorInfo
{
    ErrorCode type;
    int position;        
    char badChar;
    string filePath;

    // Преобразует ошибку в строку согласно внешней спецификации
    string toString() const
    {
        switch (type)
        {
        case FILE_NOT_OPENING:
            return "Ошибка: неверно указан файл с входными данными. Возможно, файл не существует";
        case MORE_THAN_ONE_STRING:
            return "Ошибка: программа принимает на вход файлы из одной строки. Разместите каждую строчку в отдельном файле.";
        case STRING_TOO_LONG:
            return "Ошибка: превышено количество символов в строке";
        case WRONG_SYMBOL:
            return "Ошибка: обнаружен недопустимый символ на " + to_string(position) + " позиции";
        case NOT_ENOUGH_OPERANDS:
            return "Ошибка: отсутствует операнд логического выражения";
        case NOT_ENOUGH_OPERATORS:
            return "Ошибка: отсутствует символ логической операции";
        case EMPTY_TREE:
            return "Ошибка: пустое дерево";
        default:
            return "Неизвестная ошибка";
        }
    }
};

/**
 * @struct Token
 * @brief Лексема с позицией
 */
struct Token
{
    string value;
    int position;  
};

/**
 * @struct ExprNode
 * @brief Узел дерева логического выражения
 *
 * Поля публичные (как в спецификации), но добавлены конструкторы,
 * деструктор и оператор присваивания для удобного управления памятью.
 */
struct ExprNode
{
    NodeType type;
    char name;           // для VAR
    int ID;
    ExprNode* left;
    ExprNode* right;

    static int nextID;   // статический счётчик

    // Конструкторы
    ExprNode(NodeType t, char n);
    ExprNode(NodeType t, ExprNode* l, ExprNode* r);

    // Деструктор (рекурсивно удаляет потомков)
    ~ExprNode();

    // Оператор присваивания (копирует type, name, left, right, но не ID)
    ExprNode& operator=(const ExprNode& other);
};

/**
 * @brief Разбивает строку на токены с сохранением позиций
 * @param[in] expr Входная строка в постфиксной форме
 * @return vector<Token> Список токенов (value, position)
 */
vector<Token> tokenize(const string& expr);

/**
 * @brief Строит дерево логического выражения из строки в постфиксной записи
 * @param[in] expr Входная строка
 * @param[out] errors Вектор для сбора ошибок
 * @return ExprNode* Указатель на корень дерева или nullptr при ошибке
 */
ExprNode* createLogicalTree(const string& expr, vector<ErrorInfo>& errors);

/**
 * @brief Упрощает дерево: применяет закон де Моргана и удаляет двойные отрицания
 * @param[in,out] node Корень дерева (изменяется на месте)
 */
void simplifyTree(ExprNode* node);

/**
 * @brief Применяет закон де Моргана к узлу NOT, если его операнд — AND или OR
 * @param[in,out] node Узел (изменяется на месте)
 */
void deMorganTransform(ExprNode* node);

/**
 * @brief Удаляет двойное отрицание: !!X -> X
 * @param[in,out] node Узел (изменяется на месте)
 */
void removeDoubleNegation(ExprNode* node);

/**
 * @brief Сохраняет дерево в файл в формате DOT
 * @param[in] root Корень дерева
 * @param[in] path Путь к выходному файлу
 * @param[out] errors Вектор для сбора ошибок
 */
void saveDotToFile(ExprNode* root, const string& path, vector<ErrorInfo>& errors);

/**
 * @brief Вспомогательная рекурсивная функция обхода и вывода узлов в DOT-формате
 * @param[in] node Текущий узел
 * @param[out] out Выходной поток (файл)
 */
void saveExprNodeToDot(ExprNode* node, ostream& out);