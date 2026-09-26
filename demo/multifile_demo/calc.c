#include "calc.h"   // 包含自己的头文件用双引号 "";系统头文件才用尖括号 <>

// ===== 实现文件:写函数的具体实现(函数体)=====

int add(int a, int b) {
    return a + b;
}

int max(int a, int b) {
    // 三目运算符:如果 a > b 成立就取 a,否则取 b
    return (a > b) ? a : b;
}
