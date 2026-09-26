#ifndef CALC_H
#define CALC_H

// ===== 头文件:只放"函数声明",告诉编译器这些函数在别处实现 =====
// 上面三行 #ifndef/#define/#endif 叫"头文件卫士",防止同一个头文件被重复包含

int add(int a, int b);   // 求两数之和
int max(int a, int b);   // 求两数中的较大值

#endif
