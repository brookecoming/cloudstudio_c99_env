/*
 * 例 4.7：用 switch 语句处理菜单命令。在许多应用程序中，用菜单对流程进行控制，例如从键盘输入一个 'A' 或 'a' 字符，就会执行 A 操作，输入一个 'B' 或 'b' 字符，就会执行 B 操作。
 * 【本例课本直接给出了 C 源码（main + action1 + action2），本文件忠实照搬，只补充便于讲解的中文注释】
 *
 * 题目背景（switch 的"菜单分发"用法 + 多个 case 共用一组语句）：
 * 这是一个非常简单的示意程序：假如有一个菜单，对两个整数进行运算，输入 a 或 A 调用 action1 做相加，输入 b 或 B 调用 action2 做相乘。
 * 课本用本例展示"多个 case 标号共用一组执行语句"（case 'a': 和 case 'A': 叠在一起，中间不写语句），大小写都能命中同一操作。
 *
 * 解题思路（对照课本）：
 * ① 在 main 里先声明两个函数 action1、action2（函数声明写在 main 内部）。
 * ② 定义 ch 存命令字符，a=15、b=23 是两个待运算的整数。
 * ③ getchar() 读入一个命令字符 ch。
 * ④ switch(ch)：case 'a' 与 case 'A' 共用 action1(a,b)；case 'b' 与 case 'B' 共用 action2(a,b)；其余走 default 用 putchar('\a') 发出警告声。
 * ⑤ action1 打印 x+y，action2 打印 x*y。
 *
 * 关键知识点（对应课本"程序分析/注意"）：
 * - 多个 case 标号可以共用一组执行语句：case 'a': 后面不写语句、直接跟 case 'A':，两者命中后都执行同一行 action1(a,b);break;。
 * - 执行完一个 case 标号后面的语句后，就从此标号开始执行下去、不再进行判断，所以共用 case 正是利用了这个"穿透"特性。
 * - 最后一个 case 子句（今为 default 子句）中可不必加 break，因为流程已到了 switch 结构的结束处。
 * - 函数声明可以写在 main 函数内部（void action1(int,int),action2(int,int);），表示这两个函数在本函数里被调用。
 *
 * 难点：
 * - 难点是理解"case 'a': 后面是空的"不是漏写，而是故意让它穿透到下一行 case 'A': 共用的语句上。
 *
 * 易错易混点：
 * - 共用 case 时第一个 case 后不要写 break，否则穿透链被截断、'a' 就什么都不做了。
 * - getchar() 读一个字符，不需要 &，返回值直接赋给 char。
 * - default 里 putchar('\a') 是响铃转义字符，屏幕上看不到字，只会"嘀"一声（或终端闪一下），别误以为没输出就是错。
 *
 * 内存与编译器原理：
 * - a、b 是 main 里的两个 int 格子（15、23），通过实参传给 action1/action2 的形参 x、y（值传递，函数内改 x、y 不影响 a、b）。
 * - switch 编译后按 ch 的值跳到对应 case 标号；空 case 就是"两个入口标号指向同一段代码"。
 * - 函数调用有压栈/返回开销，但本例只调一次，可忽略；重点是体会"菜单分发"的结构。
 */

#include <stdio.h>            /* 引入标准输入输出库，提供 printf、scanf、getchar、putchar */

int main()                    /* 主函数 */
{
    void action1(int, int), action2(int, int);   /* 函数声明：本函数里要调用这两个函数 */
    char ch;                  /* ch 存放读入的命令字符 */
    int a = 15, b = 23;       /* a、b 是两个待运算的整数 */

    ch = getchar();           /* 读入一个命令字符 */

    switch (ch)               /* 按命令字符分发到不同操作 */
    {
        case 'a':             /* 小写 a 与下面大写 A 共用同一组语句（故意空着以穿透） */
        case 'A': action1(a, b); break;   /* 调用 action1，执行 A 操作（相加） */
        case 'b':             /* 小写 b 与下面大写 B 共用同一组语句 */
        case 'B': action2(a, b); break;   /* 调用 action2，执行 B 操作（相乘） */
        default: putchar('\a');           /* 输入其他字符：发出警告声（课本用省略号表示还可加 C、D 操作） */
    }

    return 0;                 /* 程序正常结束 */
}

void action1(int x, int y)    /* 执行加法的函数 */
{
    printf("x+y=%d\n", x + y);   /* 打印两数之和 */
}

void action2(int x, int y)    /* 执行乘法的函数 */
{
    printf("x*y=%d\n", x * y);   /* 打印两数之积 */
}

/*
 * 运行结果（本机 CLion / MinGW-GCC 实测）：
 * 输入小写 a，回车后屏幕输出：
 * x+y=38
 * 因为 'a' 穿透到 case 'A' 共用的语句，调用 action1(15,23) 打印 15+23=38。
 * 输入 b 或 B 则调用 action2，输出 x*y=345；输入其他字符走 default，只响铃不打印可见字符。
 * 与课本示意一致。
 *
 * 【运行与演示】点 ▶ 运行后分别输入 a、A、b、B、z 各跑一次，体会大小写共用 case 与 default 兜底。
 */

/*
 * 【讲解过程稿】
 *
 * 开场：同学们，上例 switch 一个 case 干一件事。今天这题是"菜单"：输入 a 或 A 都算 A 操作、输入 b 或 B 都算 B 操作——大小写共用一条分支怎么写？【互动停顿：停 2~3 秒，问"case 后面能不能不写语句？"让学生猜，再继续】
 * 这题在干什么：读一个命令字符，switch 把它分发到 action1（相加）或 action2（相乘），都不认识就响铃警告。
 * 大思路：把 switch 想成"前台分诊"：ch 是挂号单，case 是各科室门牌，action1/action2 是科室里的医生。门牌 'a' 和 'A' 挂在同一间科室门口，所以两行 case 叠在一起。
 * 逐块讲：
 *   - 第一块 main 开头：先声明 action1、action2 两个函数，再定义 ch、a=15、b=23。
 *   - 第二块 getchar 读命令字符。
 *   - 第三块 switch：case 'a': 空着、case 'A': 调 action1 后 break；case 'b': 空着、case 'B': 调 action2 后 break；default 响铃。【此处投屏 li4.7.puml 盒图】
 *   - 第四块是 main 外面两个函数体：action1 打 x+y，action2 打 x*y。
 * 关键转折：全程序最妙的是"空 case"。case 'a': 后面故意不写语句，让流程穿透到下一行 case 'A': 的语句上，于是小写大写走同一条路。这正是上例说的"穿透"特性，这里把坑变成了用法。
 * 收尾：一句话总结——多个 case 共用一组语句，靠的就是不写 break 的穿透。留个问题：如果在 case 'a': 后面手滑加了 break，输入 a 会输出什么？【互动停顿：留 3~5 秒，揭晓"什么都不输出，直接跳出 switch"】
 */

/*
 * 【配图·选择结构盒图】见同目录 li4.7.puml（用 plantuml4idea 插件打开即渲染，讲"菜单分发 + 空 case 穿透共用"时投屏）。
 * 图只在 li4.7.puml 存一份，本文件不内嵌图，避免两处维护。
 */
