@echo off
chcp 1251 > nul
set RESULTS=results.txt

echo. > %RESULTS%
echo === 01 Simple variable === >> %RESULTS%
2_Tiurin test01.txt out01.txt >> %RESULTS% 2>&1
type out01.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 02 Unary NOT === >> %RESULTS%
2_Tiurin test02.txt out02.txt >> %RESULTS% 2>&1
type out02.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 03 Binary AND === >> %RESULTS%
2_Tiurin test03.txt out03.txt >> %RESULTS% 2>&1
type out03.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 04 Binary OR === >> %RESULTS%
2_Tiurin test04.txt out04.txt >> %RESULTS% 2>&1
type out04.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 05 De Morgan === >> %RESULTS%
2_Tiurin test05.txt out05.txt >> %RESULTS% 2>&1
type out05.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 06 De Morgan === >> %RESULTS%
2_Tiurin test06.txt out06.txt >> %RESULTS% 2>&1
type out06.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 07 Double negation !!A === >> %RESULTS%
2_Tiurin test07.txt out07.txt >> %RESULTS% 2>&1
type out07.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 08 Nested De Morgan === >> %RESULTS%
2_Tiurin test08.txt out08.txt >> %RESULTS% 2>&1
type out08.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 09 Complex === >> %RESULTS%
2_Tiurin test09.txt out09.txt >> %RESULTS% 2>&1
type out09.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 10 Triple negation + double on A+B and E === >> %RESULTS%
2_Tiurin test10.txt out10.txt >> %RESULTS% 2>&1
type out10.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 11 Chain of AND (5 variables) === >> %RESULTS%
2_Tiurin test11.txt out11.txt >> %RESULTS% 2>&1
type out11.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 12 Large De Morgan !((A+B)*(C+D)*(E+F)) === >> %RESULTS%
2_Tiurin test12.txt out12.txt >> %RESULTS% 2>&1
type out12.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 13 Double negation in subtree (A !!B) -> A B === >> %RESULTS%
2_Tiurin test13.txt out13.txt >> %RESULTS% 2>&1
type out13.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 14 Leading spaces === >> %RESULTS%
2_Tiurin test14.txt out14.txt >> %RESULTS% 2>&1
type out14.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 15 Tabs === >> %RESULTS%
2_Tiurin test15.txt out15.txt >> %RESULTS% 2>&1
type out15.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 16 Newlines === >> %RESULTS%
2_Tiurin test16.txt out16.txt >> %RESULTS% 2>&1
type out16.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 17 Invalid symbol '@' === >> %RESULTS%
2_Tiurin test17.txt out17.txt >> %RESULTS% 2>&1
type out17.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 18 Missing operand after '+' === >> %RESULTS%
2_Tiurin test18.txt out18.txt >> %RESULTS% 2>&1
type out18.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 19 Missing operator (only operands) === >> %RESULTS%
2_Tiurin test19.txt out19.txt >> %RESULTS% 2>&1
type out19.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 20 Unary without operand '!' === >> %RESULTS%
2_Tiurin test20.txt out20.txt >> %RESULTS% 2>&1
type out20.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 21 Empty file === >> %RESULTS%
2_Tiurin test21.txt out21.txt >> %RESULTS% 2>&1
type out21.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 22 Two lines in file === >> %RESULTS%
2_Tiurin test22.txt out22.txt >> %RESULTS% 2>&1
type out22.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 23 Too long line (1001 chars) === >> %RESULTS%
2_Tiurin test23.txt out23.txt >> %RESULTS% 2>&1
type out23.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 24 Multiple errors in expression === >> %RESULTS%
2_Tiurin test24.txt out24.txt >> %RESULTS% 2>&1
type out24.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 25 Only operators === >> %RESULTS%
2_Tiurin test25.txt out25.txt >> %RESULTS% 2>&1
type out25.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 26 Chain of 10 ANDs === >> %RESULTS%
2_Tiurin test26.txt out26.txt >> %RESULTS% 2>&1
type out26.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 27 Mixed double negations === >> %RESULTS%
2_Tiurin test27.txt out27.txt >> %RESULTS% 2>&1
type out27.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 28 Nested NOT with AND === >> %RESULTS%
2_Tiurin test28.txt out28.txt >> %RESULTS% 2>&1
type out28.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 29 Already simplified === >> %RESULTS%
2_Tiurin test29.txt out29.txt >> %RESULTS% 2>&1
type out29.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 30 Complex test 1 (large nested) === >> %RESULTS%
2_Tiurin test30.txt out30.txt >> %RESULTS% 2>&1
type out30.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 31 Complex test 2 (OR of ANDs) === >> %RESULTS%
2_Tiurin test31.txt out31.txt >> %RESULTS% 2>&1
type out31.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 32 Complex test 3 (massive simplification) === >> %RESULTS%
2_Tiurin test32.txt out32.txt >> %RESULTS% 2>&1
type out32.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 33 Missing input file === >> %RESULTS%
2_Tiurin test33.txt out33.txt >> %RESULTS% 2>&1
type out33.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo === 34 No write permission === >> %RESULTS%
2_Tiurin test01.txt Z:\readonly\out34.txt >> %RESULTS% 2>&1
type Z:\readonly\out34.txt >> %RESULTS% 2>&1

echo. >> %RESULTS%
echo All tests completed. Check %RESULTS%
pause

