#include <iostream>//ǰ׺���ʽ��prefixExpression
#include <stack>//���Խ���С�����㣬�Ӽ��˳����˷�
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
        //����ֱ�Ӽ����׺
        char c = expression[i];
        if (isalnum(c) || c == '.') {//isalnum:�������ĸ�����֣������档
            postfix += c;//�������ַ������׺
            if (!isalnum(expression[i + 1]) && expression[i + 1] != '.') {
                postfix += ',';
            }
        }
        //������ֱ����ջ
        else if (c == '(') {
            operators.push(c);//��������ѹ��ջ
        }
        //�����ţ���ջ�������׺��ֱ������������
        else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                postfix += operators.top();
                operators.pop();//�������ջ
            }
            operators.pop();//�����ų�ջ
        }
        //�������ջ������������Ӹߵ�������
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

// ִ��������������һ�������֮��Ĳ���
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
            cout << "0������������" << endl;
            return -1;
        }
        return operand1 / operand2;
    case '^':
        if (operand1 == 0 && operand2 == 0) {
            cout << "0��0�η������壡" << endl;
            return -1;
        }
        return pow(operand1, operand2);
    default:
        return 0;
    }
}
//
//// �����׺���ʽ��ֵ
//double evaluatePostfix(string postfix) {
//    stack<double> operands; // ���ڴ洢��������ջ
//    int len = postfix.length(); // ��ȡ��׺���ʽ�ĳ���
//    double operand = 0; // ���ڴ洢��ǰ�Ĳ�����
//
//    for (int i = 0; i < len; i++) {
//        char c = postfix[i];
//        if (isspace(c)) { // ����ַ��ǿո���������
//            continue;
//        }
//        if (isdigit(c)) { // ����ַ�������
//            operand = 0; // ���ò���������
//
//            // ����λ��������ȡ����
//            while (i < len && isdigit(postfix[i])) {
//                operand = operand * 10 + (postfix[i] - '0');
//                i++;
//            }
//            //i--; // ����һ��λ�ã���Ϊ for ѭ����������һ��
//            operands.push(operand); // ��������λ����������ջ
//        }
//        else { // ����ַ��������
//            // ����ջ����������������ִ�в�����Ȼ�󽫽��ѹ��ջ��
//            double operand2 = operands.top();
//            operands.pop();
//            double operand1 = operands.top();
//            operands.pop();
//
//            operands.push(performOperation(operand1, operand2, c));
//        }
//    }
//
//    return operands.top(); // �������ս��
//}

//...
// �����׺���ʽ��ֵ
double evaluatePostfix(string postfix) {
    stack<double> operands; // ���ڴ洢��������ջ
    int len = postfix.length(); // ��ȡ��׺���ʽ�ĳ���
    double operand = 0; // ���ڴ洢��ǰ�Ĳ�����
    bool isDecimal = false; // ����Ƿ�����С����

    for (int i = 0; i < len; i++) {
        char c = postfix[i];
        if (isspace(c) || c == ',') { // ����ַ��ǿո���������
            continue;
        }
        if (isdigit(c) || c == '.') { // ����ַ������ֻ�С����
            if (c == '.') {
                isDecimal = true; // ����С����
                continue;
            }
            operand = 0; // ���ò���������

            // ����λ����С��������ȡ����
            double factor = 1; // ���ڴ���С��λ��
            while (i < len && (isdigit(postfix[i]) || postfix[i] == '.')) {
                if (postfix[i] == '.') {
                    isDecimal = true; // ����С����
                    i++;
                    continue;
                }
                if (!isDecimal) {
                    operand = operand * 10 + (postfix[i] - '0');
                }
                else {
                    factor *= 0.1; // С�����ֵ�Ȩ��
                    operand += (postfix[i] - '0') * factor;
                }
                i++;
            }
            isDecimal = false;
            i--; // ����һ��λ�ã���Ϊ for ѭ����������һ��
            operands.push(operand); // ��������λ����С����������ջ
        }
        else { // ����ַ��������
            // ����ջ����������������ִ�в�����Ȼ�󽫽��ѹ��ջ��
            double operand2 = operands.top();
            operands.pop();
            double operand1 = operands.top();
            operands.pop();

            operands.push(performOperation(operand1, operand2, c));
        }
    }

    return operands.top(); // �������ս��
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
        if (expression[i] == '=') {//�����еȺ�
            //�����Ⱥż������Ĳ���
            break;
        }
        //������
    }
    return expression.substr(0,i);
}
//�����ű�����Ϊ�Ǽ��ŵ�������ڸ���ǰ��0
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
        cout << "������ʽ: ";
        getline(cin, infixExpression);
        int precision;
        cout << "С���������λ���֣�:";
        cin >> precision;
        infixExpression = handleEndEqualSign(infixExpression);

        eraseBlank(infixExpression);

        insertZero(infixExpression);
        string postfixExpression = infixToPostfix(infixExpression);
        //cout << "Postfix expression: " << postfixExpression << endl;
        cout << "�������ǣ�" << scientific << setprecision(precision) << evaluatePostfix(postfixExpression) << endl
            << "���û�б������������ȷ��!" << endl;
        cout << "-------------------------------------------�ָ���-----------------------------------------------" << endl;

    }
}



int main() {//��Ҫ����Ӣ���ַ����������ţ���Ȼ�����
    string infixExpression;


    calculate(infixExpression);















    //if (checkMathFault(infixExpression)) {
    //    string postfixExpression = infixToPostfix(infixExpression);
    //    cout << "Postfix expression: " << postfixExpression << endl;
    //    cout << evaluatePostfix(postfixExpression) << endl;
    //}
    //else {
    //    cout << "������ı��ʽ����ѧ�ϲ����������������룡" << endl;
    //}
    //
    return 0;
}



//����׳ˣ�
//������������֣�
//�Ƚ��׳����������ԭ����ɾ��������������
//���ǰ���ǣ���ô�죿
//1���ݹ�����ǰ������ݣ��������յ�ֵ�ٽ�������

//�������Ǻ���
//�������������sin,cos,tan,arcsin,arccos,arctan,
//�Ƚ����Ǽ���������ٸ���ԭ����









