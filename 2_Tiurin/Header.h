/**
 * @mainpage Программа для раскрытия скобок в логическом выражении по законам де Моргана и удаления двойных отрицаний
 *
 * @section purpose Назначение
 * Программа преобразует логическое выражение, заданное в постфиксной форме (токены разделены пробелами),
 * в дерево разбора, применяет законы де Моргана и удаляет двойные отрицания, после чего выводит
 * полученное дерево в формате DOT в указанный файл.
 *
 * @section input Входные данные
 * - Входной текстовый файл, содержащий одну строку с выражением в постфиксной записи.
 *   Допустимые символы: заглавные латинские буквы (переменные), операторы `!`, `*`, `+`.
 * - Максимальная длина строки: 1000 символов.
 *
 * @section output Выходные данные
 * - Текстовый файл с описанием дерева в формате DOT.
 * - В случае ошибки – соответствующее сообщение в консоль, выходной файл не создаётся.
 *
 * @section errors Обработка ошибок
 * Список ошибок и сообщений приведён во внешней спецификации.
 *
 * @section author Автор
 * Студент группы ПрИн-266, Тюрин С.С.
 *
 * @section date Дата
 * 2026 г.
 */



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
    FILE_NOT_OPENING,          ///< Ошибка открытия файла (входной/выходной)
    MORE_THAN_ONE_STRING,      ///< Во входном файле больше одной строки
    STRING_TOO_LONG,           ///< Длина строки превышает 1000 символов
    WRONG_SYMBOL,              ///< Обнаружен недопустимый символ
    NOT_ENOUGH_OPERANDS,       ///< Недостаточно операндов для операции
    NOT_ENOUGH_OPERATORS,      ///< В стеке остались лишние операнды (не хватает операторов)
    EMPTY_TREE                 ///< Пустое дерево (нет ни одного токена)
};

/**
 * @struct ErrorInfo
 * @brief Информация об ошибке
 */
struct ErrorInfo
{
    ErrorCode type;        ///< Код ошибки (тип ошибки)
    int position;          ///< Позиция в строке (1‑based) или -1, если не применимо
    char badChar;          ///< Недопустимый символ (для WRONG_SYMBOL)
    string filePath;       ///< Путь к файлу (для ошибок ввода/вывода)

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
    string value;   ///< Текст лексемы (например, "A", "!", "*", "+")
    int position;   ///< Позиция первого символа лексемы в исходной строке
};


/**
 * @defgroup Functions Функции программы
 * @brief Набор функций для разбора, преобразования и вывода логического дерева
 */



/**
 * @struct ExprNode
 * @brief Узел дерева логического выражения
 *
 * Поля публичные (как в спецификации), но добавлены конструкторы,
 * деструктор и оператор присваивания для удобного управления памятью.
 */
struct ExprNode
{
    NodeType type;       ///< Тип узла (VAR, NOT, AND, OR)
    char name;           ///< Имя переменной (только для VAR)
    int ID;              ///< Уникальный идентификатор узла (для вывода в DOT)
    ExprNode* left;      ///< Указатель на левого потомка
    ExprNode* right;     ///< Указатель на правого потомка

    static int nextID;   ///< статический счётчик

    // Конструкторы

    /**
     * @brief Конструктор для узла-переменной
     * @param[in] t Тип узла (должен быть VAR)
     * @param[in] n Имя переменной (заглавная латинская буква)
     */
    ExprNode(NodeType t, char n);

    /**
     * @brief Конструктор для узла-операции
     * @param[in] t Тип узла (NOT, AND, OR)
     * @param[in] l Указатель на левого потомка
     * @param[in] r Указатель на правого потомка (для NOT – nullptr)
     */
    ExprNode(NodeType t, ExprNode* l, ExprNode* r);

    /**
     * @brief Деструктор (рекурсивно удаляет потомков)
     */
    ~ExprNode();

    /**
     * @brief Оператор присваивания (копирует type, name, left, right, но не ID)
     * @param[in] other Другой узел, из которого копируются поля
     * @return Ссылка на текущий узел
     */
    ExprNode& operator=(const ExprNode& other);
};

/**
 * @brief Разбивает строку на токены с сохранением позиций
 * @ingroup Functions
 * @param[in] expr Входная строка в постфиксной форме
 * @return vector<Token> Список токенов (value, position)
 */
vector<Token> tokenize(const string& expr);

/**
 * @brief Строит дерево логического выражения из строки в постфиксной записи
 * @ingroup Functions
 * @param[in] expr Входная строка
 * @param[out] errors Вектор для сбора ошибок
 * @return ExprNode* Указатель на корень дерева или nullptr при ошибке
 */
ExprNode* createLogicalTree(const string& expr, vector<ErrorInfo>& errors);

/**
 * @brief Упрощает дерево: применяет закон де Моргана и удаляет двойные отрицания
 * @ingroup Functions
 * @param[in,out] node Корень дерева (изменяется на месте)
 */
void simplifyTree(ExprNode* node);

/**
 * @brief Применяет закон де Моргана к узлу NOT, если его операнд — AND или OR
 * @ingroup Functions
 * @param[in,out] node Узел (изменяется на месте)
 */
void deMorganTransform(ExprNode* node);

/**
 * @brief Удаляет двойное отрицание: !!X -> X
 * @ingroup Functions
 * @param[in,out] node Узел (изменяется на месте)
 */
void removeDoubleNegation(ExprNode* node);

/**
 * @brief Сохраняет дерево в файл в формате DOT
 * @ingroup Functions
 * @param[in] root Корень дерева
 * @param[in] path Путь к выходному файлу
 * @param[out] errors Вектор для сбора ошибок
 */
void saveDotToFile(ExprNode* root, const string& path, vector<ErrorInfo>& errors);

/**
 * @brief Вспомогательная рекурсивная функция обхода и вывода узлов в DOT-формате
 * @ingroup Functions
 * @param[in] node Текущий узел
 * @param[out] out Выходной поток (файл)
 */
void saveExprNodeToDot(ExprNode* node, ostream& out);