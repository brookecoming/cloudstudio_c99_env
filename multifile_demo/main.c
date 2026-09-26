#include <stdio.h>   // 系统头文件:尖括号 <>
#include "calc.h"    // 自己写的头文件:双引号 ""

// ===== 主程序:只负责"用"函数,不关心它们怎么实现的 =====

int main() {
    int x = 12, y = 5;

    printf("x = %d, y = %d\n", x, y);
    printf("x + y       = %d\n", add(x, y));   // 调用 calc.c 里的 add
    printf("max(x, y)   = %d\n", max(x, y));   // 调用 calc.c 里的 max

    return 0;
}
