#include "pch.h"
#include "CppUnitTest.h"

#include "../2_Tiurin/Header.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
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
}
