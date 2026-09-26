#include <stdio.h>
#include "max.h"   // 包含自定义头文件:把 max 的"定义"拿进来用

// ===== 自定义头文件 demo:main.c 只负责调用,函数体在 max.h 里 =====

int main() {
    int x = 8, y = 13;

    printf("x = %d, y = %d\n", x, y);
    printf("max(x, y) = %d\n", max(x, y));   // 调用 max.h 里定义的 max

    return 0;
}
