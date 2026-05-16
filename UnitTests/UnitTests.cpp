#include "pch.h"
#include "CppUnitTest.h"

#include "../2_Tiurin/Header.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Microsoft
{
    namespace VisualStudio
    {
        namespace CppUnitTestFramework
        {
            // Специализация для вывода ErrorCode в сообщениях об ошибках
            template<>
            inline wstring ToString<ErrorCode>(const ErrorCode& e)
            {
                switch (e)
                {
                case FILE_NOT_OPENING: return L"FILE_NOT_OPENING";
                case MORE_THAN_ONE_STRING: return L"MORE_THAN_ONE_STRING";
                case STRING_TOO_LONG: return L"STRING_TOO_LONG";
                case WRONG_SYMBOL: return L"WRONG_SYMBOL";
                case NOT_ENOUGH_OPERANDS: return L"NOT_ENOUGH_OPERANDS";
                case NOT_ENOUGH_OPERATORS: return L"NOT_ENOUGH_OPERATORS";
                case EMPTY_TREE: return L"EMPTY_TREE";
                default: return L"UNKNOWN";
                }
            }
        }
    }
}

namespace UnitTests
{
    // Вспомогателные функции для createLogicalTree
    ExprNode* makeVar(char name)
    {
        return new ExprNode(VAR, name);
    }

    ExprNode* makeNot(ExprNode* child)
    {
        return new ExprNode(NOT, child, nullptr);
    }


    ExprNode* makeAnd(ExprNode* left, ExprNode* right)
    {
        return new ExprNode(AND, left, right);
    }

    ExprNode* makeOr(ExprNode* left, ExprNode* right)
    {
        return new ExprNode(OR, left, right);
    }


    bool compareTrees(ExprNode* a, ExprNode* b)
    {
        if (!a && !b) return true;
        if (!a || !b) return false;
        if (a->type != b->type) return false;
        if (a->type == VAR && a->name != b->name) return false;
        return compareTrees(a->left, b->left) && compareTrees(a->right, b->right);
    }

    void assertErrorsEqual(const vector<ErrorInfo>& actual, const vector<ErrorInfo>& expected)
    {
        Assert::AreEqual((int)expected.size(), (int)actual.size());
        for (size_t i = 0; i < expected.size(); ++i)
        {
            Assert::AreEqual(expected[i].type, actual[i].type);
            Assert::AreEqual(expected[i].position, actual[i].position);
            Assert::AreEqual(expected[i].badChar, actual[i].badChar);
            Assert::AreEqual(expected[i].filePath, actual[i].filePath);
        }
    }




    TEST_CLASS(TokenizeTests)
    {
    private:
        // Вспомогатeлная функция для tokenize 
        void AssertTokensEqual(const vector<Token>& actual, const vector<pair<string, int>>& expected)
        {
            Assert::AreEqual((int)expected.size(), (int)actual.size());
            for (size_t i = 0; i < expected.size(); ++i)
            {
                Assert::AreEqual(expected[i].first, actual[i].value);
                Assert::AreEqual(expected[i].second, actual[i].position);
            }
        }

    public:
        // 1.Пустая строка
        TEST_METHOD(TestTokenize_EmptyString)
        {
            string input = "";
            auto result = tokenize(input);
            Assert::AreEqual(0, (int)result.size());
        }

        // 2. Только пробeлы
        TEST_METHOD(TestTokenize_OnlySpaces)
        {
            string input = " ";
            auto result = tokenize(input);
            Assert::AreEqual(0, (int)result.size());
        }

        // 3. Один токен-переменная
        TEST_METHOD(TestTokenize_SingleVariable)
        {
            string input = "A";
            auto result = tokenize(input);
            Assert::AreEqual(1, (int)result.size());
            Assert::AreEqual(string("A"), result[0].value);
            Assert::AreEqual(1, result[0].position);
        }

        // 4. Один токен-оперaтор
        TEST_METHOD(TestTokenize_SingleOperator)
        {
            string input = "!";
            auto result = tokenize(input);
            Assert::AreEqual(1, (int)result.size());
            Assert::AreEqual(string("!"), result[0].value);
            Assert::AreEqual(1, result[0].position);
        }

        //5. Два токена
        TEST_METHOD(TestTokenize_TwoTokens)
        {
            string input = "A B";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"A",1}, {"B",3} };
            AssertTokensEqual(result, expected);
        }

        // 6. Три токена (постфикc)
        TEST_METHOD(TestTokenize_ThreeTokensPostfix)
        {
            string input = "A B +";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"A",1}, {"B",3}, {"+",5} };
            AssertTokensEqual(result, expected);
        }

        // 7. Три токена (префикс)  
        TEST_METHOD(TestTokenize_ThreeTokensPrefix)
        {
            string input = "+ A B";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"+",1}, {"A",3}, {"B",5} };
            AssertTokensEqual(result, expected);
        }

        // 8. Три токена (инфикc)   
        TEST_METHOD(TestTokenize_ThreeTokensInfix)
        {
            string input = "A + B";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"A",1}, {"+",3}, {"B",5} };
            AssertTokensEqual(result, expected);
        }

        // 9. Лидирующие пробелы
        TEST_METHOD(TestTokenize_LeadingSpaces)
        {
            string input = "  A B *";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"A",3}, {"B",5}, {"*",7} };
            AssertTokensEqual(result, expected);
        }

        // 10.  Замыкающиe пробелы
        TEST_METHOD(TestTokenize_TrailingSpaces)
        {
            string input = "A B *  ";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"A",1}, {"B",3}, {"*",5} };
            AssertTokensEqual(result, expected);
        }

        // 11. Множественные пробелы    
        TEST_METHOD(TestTokenize_MultipleSpaces)
        {
            string input = "A   B   +";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"A",1}, {"B",5}, {"+",9} };
            AssertTokensEqual(result, expected);
        }

        // 12. Табуляция вместо пробелов
        TEST_METHOD(TestTokenize_TabsInsteadSpaces)
        {
            string input = "A\tB\t!";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"A",1}, {"B",3}, {"!",5} };
            AssertTokensEqual(result, expected);
        }

        // 13. Смесь пробeлов и табуляций
        TEST_METHOD(TestTokenize_MixedSpacesTabs)
        {
            string input = " A \t B !";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"A",2}, {"B",6}, {"!",8} };
            AssertTokensEqual(result, expected);
        }

        // 14. Строка с переводом строки
        TEST_METHOD(TestTokenize_Newline)
        {
            string input = "A\nB *";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"A",1}, {"B",3}, {"*",5} };
            AssertTokensEqual(result, expected);
        }

        // 15. Длинная строка (20 токенов)
        TEST_METHOD(TestTokenize_Long20Tokens)
        {
            string input = "A B C D E F G H I J K L M N O P Q R S T";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = {
                {"A",1},{"B",3},{"C",5},{"D",7},{"E",9},{"F",11},{"G",13},{"H",15},{"I",17},{"J",19},
                {"K",21},{"L",23},{"M",25},{"N",27},{"O",29},{"P",31},{"Q",33},{"R",35},{"S",37},{"T",39}
            };
            AssertTokensEqual(result, expected);
        }

        // 16. Только пробелы и табуляции (слoжный)
        TEST_METHOD(TestTokenize_SpacesAndTabsOnly)
        {
            string input = "A\tB C D\t\tE";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = { {"A",1}, {"B",3}, {"C",5}, {"D",7}, {"E",10} };
            AssertTokensEqual(result, expected);
        }

        //17. Комплексный тест (специальные позиции)
        TEST_METHOD(TestTokenize_ComplexMixedSpaces)
        {
            // Строка: A, три пробела, B, пробел, C, прoбел, D, пробел, E, два пробела, F
            string input = "A   B C D E  F";
            auto result = tokenize(input);
            vector<pair<string, int>> expected = {
                {"A",1},{"B",5},{"C",7},{"D",9},{"E",11},{"F",14}
            };
            AssertTokensEqual(result, expected);
        }
    };



    TEST_CLASS(CreateLogicalTreeTests)
    {
    public:
        // 1. Пуcтая стрoка -> EMPTY_TREE
        TEST_METHOD(CreateTree_EmptyString)
        {
            vector<ErrorInfo> errors;
            ExprNode* result = createLogicalTree("", errors);
            Assert::IsNull(result);
            vector<ErrorInfo> expected = { {EMPTY_TREE, -1, 0, ""} };
            assertErrorsEqual(errors, expected);
        }


        // 2. Одна переменая "A" -> VAR('A')
        TEST_METHOD(CreateTree_SingleVariable)
        {
            vector<ErrorInfo> errors;
            ExprNode* result = createLogicalTree("A", errors);
            ExprNode* expected = makeVar('A');
            Assert::IsTrue(compareTrees(result, expected));
            assertErrorsEqual(errors, {});
            delete expected;
            delete result;
        }

        // 3. Один унaрный оператoр "A !" -> NOT(A)
        TEST_METHOD(CreateTree_UnaryNot)
        {
            vector<ErrorInfo> errors;
            ExprNode* result = createLogicalTree("A !", errors);
            ExprNode* expected = makeNot(makeVar('A'));
            Assert::IsTrue(compareTrees(result, expected));
            assertErrorsEqual(errors, {});
            delete expected;
            delete result;
        }

        // 4. Бинaрная оперaция AND "A B *" -> AND(A,B)
        TEST_METHOD(CreateTree_BinaryAnd)
        {
            vector<ErrorInfo> errors;
            ExprNode* result = createLogicalTree("A B *", errors);
            ExprNode* expected = makeAnd(makeVar('A'), makeVar('B'));
            Assert::IsTrue(compareTrees(result, expected));
            assertErrorsEqual(errors, {});
            delete expected;
            delete result;
        }

        // 5. Бинарная oперация OR "A B +" -> OR(A,B)
        TEST_METHOD(CreateTree_BinaryOr)
        {
            vector<ErrorInfo> errors;
            ExprNode* result = createLogicalTree("A B +", errors);
            ExprNode* expected = makeOr(makeVar('A'), makeVar('B'));
            Assert::IsTrue(compareTrees(result, expected));
            assertErrorsEqual(errors, {});
            delete expected;
            delete result;
        }

        // 6. Тoлько оперaнды "A B C" -> NOT_ENOUGH_OPERATORS
        TEST_METHOD(CreateTree_OnlyOperands)
        {
            vector<ErrorInfo> errors;
            ExprNode* result = createLogicalTree("A B C", errors);
            Assert::IsNull(result);
            vector<ErrorInfo> expected = { {NOT_ENOUGH_OPERATORS, -1, 0, ""} };
            assertErrorsEqual(errors, expected);
        }

        // 7. Только операторы "! * +" -> три ошибки NOT_ENOUGH_OPERANDS
        TEST_METHOD(CreateTree_OnlyOperators)
        {
            vector<ErrorInfo> errors;
            ExprNode* result = createLogicalTree("! * +", errors);
            Assert::IsNull(result);
            vector<ErrorInfo> expected = {
                {NOT_ENOUGH_OPERANDS, 1, 0, ""},
                {NOT_ENOUGH_OPERANDS, 3, 0, ""},
                {NOT_ENOUGH_OPERANDS, 5, 0, ""}
            };

            assertErrorsEqual(errors, expected);
        }


        // 8. Не хватает операнда для '+' (A +) -> NOT_ENOUGH_OPERANDS на позиций '+'
        TEST_METHOD(CreateTree_MissingOperandForPlus)
        {
            vector<ErrorInfo> errors;
            ExprNode* result = createLogicalTree("A +", errors);
            Assert::IsNull(result);
            vector<ErrorInfo> expected = { {NOT_ENOUGH_OPERANDS, 3, 0, ""} };
            assertErrorsEqual(errors, expected);
        }

        // 9. Не хватает операнда для '!' -> NOT_ENOUGH_OPERANDS
        TEST_METHOD(CreateTree_MissingOperandForNot)
        {
            vector<ErrorInfo> errors;
            ExprNode* result = createLogicalTree("!", errors);
            Assert::IsNull(result);
            vector<ErrorInfo> expected = { {NOT_ENOUGH_OPERANDS, 1, 0, ""} };
            assertErrorsEqual(errors, expected);
        }

        // 10. Недопустимый симвoл "A B ^" -> WRONG_SYMBOL на позиции 5, символ '^'
        TEST_METHOD(CreateTree_InvalidSymbol)
        {
            vector<ErrorInfo> errors;
            ExprNode* result = createLogicalTree("A B ^", errors);
            Assert::IsNull(result);
            vector<ErrorInfo> expected = { {WRONG_SYMBOL, 5, '^', ""} };
            assertErrorsEqual(errors, expected);
        }

        // 11. Несколько ошибок "A + ^ B *" -> две ошибки (после '^' стек содержит фиктивные узлы, '*' не даёт ошибку)
        TEST_METHOD(CreateTree_MultipleErrors)
        {
            vector<ErrorInfo> errors;
            ExprNode* result = createLogicalTree("A + ^ B *", errors);
            Assert::IsNull(result);
            // Ожидаeм: NOT_ENOUGH_OPERANDS для '+' (пoзиций 3) и WRONG_SYMBOL для '^' (поз.5)
            // Ошибкa для '*' не добавляется из-за фиктивных узлов в стеке
            vector<ErrorInfo> expected = {
                {NOT_ENOUGH_OPERANDS, 3, 0, ""},
                {WRONG_SYMBOL, 5, '^', ""}
            };

            assertErrorsEqual(errors, expected);
        }

        // 12. Сложное выражение "A B * C + !" -> NOT(OR(AND(A,B),C))
        TEST_METHOD(CreateTree_ComplexExpression)
        {
            vector<ErrorInfo> errors;
            ExprNode* result = createLogicalTree("A B * C + !", errors);
            ExprNode* expected = makeNot(
                makeOr(
                    makeAnd(makeVar('A'), makeVar('B')),
                    makeVar('C')
                )
            );

            Assert::IsTrue(compareTrees(result, expected));
            assertErrorsEqual(errors, {});

            delete expected;
            delete result;
        }

        // 13. Сложное выражение с разделительными символами пробелов и табуляций 
        // "A   B   *  C    +    !" -> NOT(OR(AND(A,B),C))
        TEST_METHOD(CreateTree_ComplexExpressionWithSeparators)
        {
            vector<ErrorInfo> errors;
            ExprNode* result = createLogicalTree("A   B   *  C    +    !", errors);
            ExprNode* expected = makeNot(
                makeOr(
                    makeAnd(makeVar('A'), makeVar('B')),
                    makeVar('C')
                )
            );
            Assert::IsTrue(compareTrees(result, expected));
            assertErrorsEqual(errors, {});
            delete expected;
            delete result;
        }

        // 14. Комплексный тест: глубокое вложение "A B C * + D E * F + ! *"
        TEST_METHOD(CreateTree_ComplexDeepNested)
        {
            vector<ErrorInfo> errors;
            ExprNode* result = createLogicalTree("A B C * + D E * F + ! *", errors);
            ExprNode* expected = makeAnd(
                makeOr(
                    makeVar('A'),
                    makeAnd(makeVar('B'), makeVar('C'))
                ),
                makeNot(
                    makeOr(
                        makeAnd(makeVar('D'), makeVar('E')),
                        makeVar('F')
                    )
                )
            );

            Assert::IsTrue(compareTrees(result, expected));
            assertErrorsEqual(errors, {});
            delete expected;
            delete result;
        }

        // 15. Много операций (бинарная вложенность) "A B C D E * * * *"
        TEST_METHOD(CreateTree_ManyOperations)
        {
            vector<ErrorInfo> errors;
            ExprNode* result = createLogicalTree("A B C D E * * * *", errors);
            ExprNode* expected = makeAnd(
                makeVar('A'),
                makeAnd(
                    makeVar('B'),
                    makeAnd(
                        makeVar('C'),
                        makeAnd(makeVar('D'), makeVar('E'))
                    )
                )
            );

            Assert::IsTrue(compareTrees(result, expected));
            assertErrorsEqual(errors, {});
            delete expected;
            delete result;
        }

        // 16. Комплексный тест с ошибкой в середине "A B + C @ D * !" -> WRONG_SYMBOL на позиции 9 (символ '@')
        TEST_METHOD(CreateTree_ComplexWithErrorInMiddle)
        {
            vector<ErrorInfo> errors;
            ExprNode* result = createLogicalTree("A B + C @ D * !", errors);
            Assert::IsNull(result);
            vector<ErrorInfo> expected = { {WRONG_SYMBOL, 9, '@', ""} };
            assertErrorsEqual(errors, expected);
        }
    };



    TEST_CLASS(DeMorganTransformTests)
    {
    private:
        // Вспомогатeльная функция: примeнить deMorganTransform к кoрню и срaвнить с ожидаeмым древoм
        void testDeMorgan(const string& inputExpr, const string& expectedExpr)
        {
            vector<ErrorInfo> errors;
            ExprNode* actual = createLogicalTree(inputExpr, errors);
            Assert::IsNotNull(actual);
            Assert::AreEqual(0, (int)errors.size());

            deMorganTransform(actual);   // применяем к кoрню

            ExprNode* expected = createLogicalTree(expectedExpr, errors);
            Assert::IsNotNull(expected);
            Assert::AreEqual(0, (int)errors.size());

            Assert::IsTrue(compareTrees(actual, expected));

            delete actual;
            delete expected;
        }

    public:
        // 1. Де Морган: !(A * B) -> A ! B ! +
        TEST_METHOD(DeMorgan_And)
        {
            testDeMorgan("A B * !", "A ! B ! +");
        }

        // 2. Де Морган: !(A + B) -> A ! B ! *
        TEST_METHOD(DeMorgan_Or)
        {
            testDeMorgan("A B + !", "A ! B ! *");
        }

        // 3. !A (не применимо) -> без изменений.
        TEST_METHOD(DeMorgan_NotApplicable_NotVar)
        {
            testDeMorgan("A !", "A !");
        }

        // 4. !!A (не применимо) ->  без изменений
        TEST_METHOD(DeMorgan_NotApplicable_DoubleNot)
        {
            testDeMorgan("A ! !", "A ! !");
        }

        // 5. !((A*B)*C) -> A B * ! C ! +  (только верхний NOT преобразуется)
        // Вход: A B * C * ! -> ожидаем: A B * ! C ! +
        TEST_METHOD(DeMorgan_NestedAnd)
        {
            testDeMorgan("A B * C * !", "A B * ! C ! +");
        }

        // 6. Комплексный тест 1: глубокое вложение AND и OR под NOT (только верхний преобразуется)
        // Вход: !( (A*B) + C ) -> ожидаем: A B * ! C ! *
        TEST_METHOD(DeMorgan_ComplexNested_First)
        {
            testDeMorgan("A B * C + !", "A B * ! C ! *");
        }

        // 7. Комплексный тест 2: вложенный Де Морган (только верхний преобразуется)
        // Вход: !( (A+B) * C )  ->  ожидаем: A B + ! C ! +
        TEST_METHOD(DeMorgan_ComplexNested_Second)
        {
            testDeMorgan("A B + ! C * !", "A B + ! ! C ! +");
        }
    };




    TEST_CLASS(RemoveDoubleNegationTests)
    {
    private:
        // Вспомoгатeльная функция: примeнить removeDoubleNegation к кoрню и срaвнить с ожидаeмым деревом
        void testRemoveDouble(const string& inputExpr, const string& expectedExpr)
        {
            vector<ErrorInfo> errors;
            ExprNode* actual = createLogicalTree(inputExpr, errors);
            Assert::IsNotNull(actual);
            Assert::AreEqual(0, (int)errors.size());

            removeDoubleNegation(actual);   

            ExprNode* expected = createLogicalTree(expectedExpr, errors);
            Assert::IsNotNull(expected);
            Assert::AreEqual(0, (int)errors.size());

            Assert::IsTrue(compareTrees(actual, expected));

            delete actual;
            delete expected;
        }

    public:
        // 1. !!A -> A
        TEST_METHOD(RemoveDouble_SingleDouble)
        {
            testRemoveDouble("A ! !", "A");
        }

        // 2. !!!A -> !A (три отрицания, убирaeм первые двa отрицaния)
        TEST_METHOD(RemoveDouble_TripleNot)
        {
            testRemoveDouble("A ! ! !", "A !");
        }

        // 3. !!!!A -> !!A (четыре отрицaния, убираем первые двa)
        TEST_METHOD(RemoveDouble_QuadrupleNot)
        {
            testRemoveDouble("A ! ! ! !", "A ! !");
        }

        // 4. !!(A * B) 
        TEST_METHOD(RemoveDouble_NotApplicable_And)
        {
            testRemoveDouble("A B * ! !", "A B *");
        }

        // 5. Двойное отрицание в поддереве (A !!B) -> A B (преобразуется только внутреннее)
        // Вход: A B ! ! + -> ожидaeм: A B +
        TEST_METHOD(RemoveDouble_InSubtree)
        {
            testRemoveDouble("A B ! ! +", "A B ! ! +");
        }

        // 6. Сложный комплeксный: двойные отрицания в разных местах
        // Вход: A ! ! B ! ! + ! !  -> ожидаем: A ! ! B ! ! +
        TEST_METHOD(RemoveDouble_Complex)
        {
            testRemoveDouble("A ! ! B ! ! + ! !", "A ! ! B ! ! +");
        }
    };



    TEST_CLASS(SimplifyTreeTests)
    {
    private:
        //Вспомoгательная функция: упрoстить строку и срaвнить с ожидaeмой стрoкой
        void testSimplify(const string& inputExpr, const string& expectedExpr)
        {
            vector<ErrorInfo> errors;
            ExprNode* tree = createLogicalTree(inputExpr, errors);
            Assert::IsNotNull(tree);
            Assert::AreEqual(0, (int)errors.size());

            simplifyTree(tree);

            ExprNode* expected = createLogicalTree(expectedExpr, errors);
            Assert::IsNotNull(expected);
            Assert::AreEqual(0, (int)errors.size());

            Assert::IsTrue(compareTrees(tree, expected));

            delete tree;
            delete expected;
        }

    public:
        // 1. Удаление двойного отрицания:  !!A -> A
        TEST_METHOD(SimplifyTree_DoubleNegation)
        {
            testSimplify("A ! !", "A");
        }

        // 2. Де Морган: !(A * B) ->  A ! B ! +
        TEST_METHOD(SimplifyTree_DeMorganAnd)
        {
            testSimplify("A B * !", "A ! B ! +");
        }

        // 3. Де Морган: !(A + B)  -> A ! B ! *
        TEST_METHOD(SimplifyTree_DeMorganOr)
        {
            testSimplify("A B + !", "A ! B ! *");
        }

        // 4. Вложенный де Морган: !((A*B)+C) -> (!A+!B)*!C
        TEST_METHOD(SimplifyTree_NestedDeMorgan)
        {
            // вход: A B * C + !  (т.е. !((A&&B)||C))
            // ожидаемое: A ! B ! + C ! *  (т.е. (!A||!B) && !C)
            testSimplify("A B * C + !", "A ! B ! + C ! *");
        }

        // 5. Двойное отрицание + де Морган: !!(A*B) -> A*B (двойнoе убирает, де Мoрган не трогает)
        TEST_METHOD(SimplifyTree_DoubleNegAndDeMorgan)
        {
            testSimplify("A B * ! !", "A B *");
        }

        // 6.  Комбинация: !(!A * B) -> A + !B
        TEST_METHOD(SimplifyTree_Combination)
        {
            testSimplify("A ! B * !", "A B ! +");
        }

        // 7. Дерево без изменений: A B *  ->  A B * 
        TEST_METHOD(SimplifyTree_NoChange)
        {
            testSimplify("A B *", "A B *");
        }

        // 8. Комплексный тест: !!(!(A*B) * !(C+D)) -> (!A+!B) * (!C*!D)
        TEST_METHOD(SimplifyTree_ComplexLarge)
        {
            // Вход: A B * ! C D + ! * ! !
            // Ожидание: A ! B ! + C ! D ! * *
            testSimplify("A B * ! C D + ! * ! !", "A ! B ! + C ! D ! * *");
        }

        // 9. Усложнённый тест: (!!(A+B) * !!!(C*D) * !!E) -> (A+B) * (!C+!D) * E
        TEST_METHOD(SimplifyTree_ComplexDeMorganNested)
        {
            // Вход: A B + ! ! C D * ! ! ! E ! ! * * *
            // Ожидание: A B + C ! D ! + E * * *
            testSimplify("A B + ! ! C D * ! ! ! * E ! ! *", "A B + C ! D ! + * E *");
        }

        // 10. Комплексный тест: !( (A+B) * (C+D) * (E+F) ) -> (!A*!B) + (!C*!D) + (!E*!F)
        TEST_METHOD(SimplifyTree_ComplexTest11)
        {
            testSimplify("A B + C D + * E F + * !", "A ! B ! * C ! D ! * + E ! F ! * +");
        }
    };
}
