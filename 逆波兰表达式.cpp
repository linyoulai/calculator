#include <iostream>//前缀表达式：prefixExpression
#include <stack>//可以进行小数运算，加减乘除，乘方
#include <string>
#include <iomanip>
#include <string>
#include <cmath>
using namespace std;

int precedence(char op) {
    if (op == '+' || op == '-') {
        return 1;
    }
    else if (op == '*' || op == '/') {
        return 2;
    }
    else if (op == '^') {
        return 3;
    }
    return 0;
}

string infixToPostfix(string expression) {
    stack<char> operators;
    string postfix;
    int len = expression.length();
    for (int i = 0; i < len; i++) {
        //数字直接加入后缀
        char c = expression[i];
        if (isalnum(c) || c == '.') {//isalnum:如果是字母或数字，返回真。
            postfix += c;//将数字字符加入后缀
            if (!isalnum(expression[i + 1]) && expression[i + 1] != '.') {
                postfix += ',';
            }
        }
        //左括号直接入栈
        else if (c == '(') {
            operators.push(c);//将左括号压入栈
        }
        //右括号，出栈并加入后缀，直到遇到左括号
        else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                postfix += operators.top();
                operators.pop();//运算符出栈
            }
            operators.pop();//左括号出栈
        }
        //运算符入栈：保持运算符从高到低排列
        else {
            while (!operators.empty() && precedence(c) <= precedence(operators.top())) {
                postfix += operators.top();
                operators.pop();
            }
            operators.push(c);
        }
    }

    while (!operators.empty()) {
        postfix += operators.top();
        operators.pop();
    }

    return postfix;
}

// 执行两个操作数和一个运算符之间的操作
double performOperation(double operand1, double operand2, char op) {
    switch (op) {
    case '+':
        return operand1 + operand2;
    case '-':
        return operand1 - operand2;
    case '*':
        return operand1 * operand2;
    case '/':
        if (operand2 == 0) {
            cout << "0不能作除数！" << endl;
            return -1;
        }
        return operand1 / operand2;
    case '^':
        if (operand1 == 0 && operand2 == 0) {
            cout << "0的0次方无意义！" << endl;
            return -1;
        }
        return pow(operand1, operand2);
    default:
        return 0;
    }
}
//
//// 计算后缀表达式的值
//double evaluatePostfix(string postfix) {
//    stack<double> operands; // 用于存储操作数的栈
//    int len = postfix.length(); // 获取后缀表达式的长度
//    double operand = 0; // 用于存储当前的操作数
//
//    for (int i = 0; i < len; i++) {
//        char c = postfix[i];
//        if (isspace(c)) { // 如果字符是空格，跳过处理
//            continue;
//        }
//        if (isdigit(c)) { // 如果字符是数字
//            operand = 0; // 重置操作数变量
//
//            // 将多位数完整提取出来
//            while (i < len && isdigit(postfix[i])) {
//                operand = operand * 10 + (postfix[i] - '0');
//                i++;
//            }
//            //i--; // 回退一个位置，因为 for 循环会再自增一次
//            operands.push(operand); // 将整个多位数操作数入栈
//        }
//        else { // 如果字符是运算符
//            // 弹出栈顶的两个操作数，执行操作，然后将结果压入栈中
//            double operand2 = operands.top();
//            operands.pop();
//            double operand1 = operands.top();
//            operands.pop();
//
//            operands.push(performOperation(operand1, operand2, c));
//        }
//    }
//
//    return operands.top(); // 返回最终结果
//}

//...
// 计算后缀表达式的值
double evaluatePostfix(string postfix) {
    stack<double> operands; // 用于存储操作数的栈
    int len = postfix.length(); // 获取后缀表达式的长度
    double operand = 0; // 用于存储当前的操作数
    bool isDecimal = false; // 标记是否遇到小数点

    for (int i = 0; i < len; i++) {
        char c = postfix[i];
        if (isspace(c) || c == ',') { // 如果字符是空格，跳过处理
            continue;
        }
        if (isdigit(c) || c == '.') { // 如果字符是数字或小数点
            if (c == '.') {
                isDecimal = true; // 遇到小数点
                continue;
            }
            operand = 0; // 重置操作数变量

            // 将多位数或小数完整提取出来
            double factor = 1; // 用于处理小数位数
            while (i < len && (isdigit(postfix[i]) || postfix[i] == '.')) {
                if (postfix[i] == '.') {
                    isDecimal = true; // 遇到小数点
                    i++;
                    continue;
                }
                if (!isDecimal) {
                    operand = operand * 10 + (postfix[i] - '0');
                }
                else {
                    factor *= 0.1; // 小数部分的权重
                    operand += (postfix[i] - '0') * factor;
                }
                i++;
            }
            isDecimal = false;
            i--; // 回退一个位置，因为 for 循环会再自增一次
            operands.push(operand); // 将整个多位数或小数操作数入栈
        }
        else { // 如果字符是运算符
            // 弹出栈顶的两个操作数，执行操作，然后将结果压入栈中
            double operand2 = operands.top();
            operands.pop();
            double operand1 = operands.top();
            operands.pop();

            operands.push(performOperation(operand1, operand2, c));
        }
    }

    return operands.top(); // 返回最终结果
}
//...

bool checkMathFault(string infixExpression) {
    int len = infixExpression.size();
    for (int i = 0; i < len; i++) {
        if (infixExpression[i] == '/' && infixExpression[i + 1] == '0'){
            return false;
        }
    }
    return true;
}

string handleEndEqualSign(string expression) {
    int len = expression.size();
    int i;
    for (i = 0; i < len; i++) {
        if (expression[i] == '=') {//后面有等号
            //舍弃等号及其后面的部分
            break;
        }
        //检查错误
    }
    return expression.substr(0,i);
}
//处理负号被误认为是减号的情况：在负号前加0
void insertZero(string& expression) {
    int len = expression.size();
    int i = 0;
    if (expression[i] == '-') {
        expression.insert(i, 1, '0');
    }
    for (i = 1; i < len; i++) {
        if (expression[i] == '-' && (expression[i - 1] < '0' || expression[i - 1] > '9')) {
            expression.insert(i, 1, '0');
            len++;
            i++;
        }
    }
}

void eraseBlank(string& expression) {
    for (int i = 0; i < expression.size();) {
        if (expression[i] == ' ') {
            expression.erase(i, 1);
        }
        else {
            i++;
        }
    }
}

void calculate(string infixExpression) {
    {
        cout << "输入算式: ";
        getline(cin, infixExpression);
        int precision;
        cout << "小数点后保留几位数字？:";
        cin >> precision;
        infixExpression = handleEndEqualSign(infixExpression);

        eraseBlank(infixExpression);

        insertZero(infixExpression);
        string postfixExpression = infixToPostfix(infixExpression);
        //cout << "Postfix expression: " << postfixExpression << endl;
        cout << "计算结果是：" << scientific << setprecision(precision) << evaluatePostfix(postfixExpression) << endl
            << "如果没有报错，结果就是正确的!" << endl;
        cout << "-------------------------------------------分割线-----------------------------------------------" << endl;

    }
}



int main() {//需要输入英文字符，包括括号，不然会出错
    string infixExpression;


    calculate(infixExpression);















    //if (checkMathFault(infixExpression)) {
    //    string postfixExpression = infixToPostfix(infixExpression);
    //    cout << "Postfix expression: " << postfixExpression << endl;
    //    cout << evaluatePostfix(postfixExpression) << endl;
    //}
    //else {
    //    cout << "您输入的表达式在数学上不成立，请重新输入！" << endl;
    //}
    //
    return 0;
}



//处理阶乘：
//遍历，如果发现！
//先将阶乘算出来，将原数据删除，插入新数据
//如果前面是）怎么办？
//1、递归运算前面的内容，返回最终的值再进行运算

//处理三角函数
//遍历，如果发现sin,cos,tan,arcsin,arccos,arctan,
//先将他们计算出来，再覆盖原数据









