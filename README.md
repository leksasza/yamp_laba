# Грамматика обрабатываемый программой
**Граматика представлена в нормальной форме Хомского**
<br>
<br>
Function —> Begin Descriptions Operators End<br>
Begin —> Type FunctionName() {<br>
End —> return Id ; }<br>
FunctionName —> Id<br>
Descriptions —> Descr | Descr Descriptions <br>
Operators —> Op | Op Operators<br>
Descr —> Type VarList ;<br>
VarList —> Id | Id , VarList<br>
Type —> int | float<br>
Op —> Id = Expr ; <br>
SimpleExpr —> Id | Const | ( Expr )<br>
Expr —> Term | Term + Expr | Term - Expr<br>
Term —> SimpleExpr | SimpleExpr * Term | SimpleExpr / Term |SimpleExpr % Term<br>
Id —> Letter | Letter LetNum<br>
LetNum —> Letter | Digit | Letter LetNum | Digit LetNum<br>
Const —> IntNumber | DoubleNumber<br>
IntNumber —> Digit IntNumber<br>
DoubleNumber —> IntNumber.IntNumber<br>
Digit —> 0 | 1 | ... | 9<br>
Letter —> a | b | ... | z | A | B | ... | Z<br>
# Классы программы
## dfa и State
Класс DFA, реализует детерминированный конечный автомат для обработки текста программы. ДКА представлен в файле **lexical_dfa.txt**. <br>
Класс State реализует состояние ДКА.

## hashtable и hashitem
Реализация хэш-таблицы для хранения лексем.
## lexicalanalysis
Класс реализует лексический анализ заданной программы. Он обрабатывает лексемы и ищет ошибки с помощью ДКА и сохраняет результат в виде хэш-таблицы лексем в файл **hashtable.txt**.
## parser
Класс реализует синтаксический анализ заданной программы. Он разбивает программу в дерево по терминалам и нетерминалам грамматики и сохраняет результат в файл **tree.txt**.  
## tree
Реализация дерева для сохранения в нем синтаксического анализаци текста.
## Semantic
Класс реализует семантический разбор заданной программы. Он обрабатывает дерево и хэш-таблицу, ищет ошибки и выводит результат в обратной польской записи в файл **reversenotation.txt**
