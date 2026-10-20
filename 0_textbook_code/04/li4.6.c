/*
 * 例 4.6：要求学生考试成绩按等级制给出：A 等 85 分以上，B 等 70~84 分，C 等 60~69 分，D 等 60 分以下。现给定一个等级，要求输出相应的分数段。
 * 【本例课本直接给出了 C 源码，本文件忠实照搬，只补充便于讲解的中文注释】
 *
 * 题目背景（switch 语句的第一个实战）：
 * 前面用 if 处理"区间/大小"判断，这一例是"等值多分支"：给定一个字符等级，从四条固定分支里挑一条。
 * 课本用 switch—case 展示这种"按值对号入座"的写法，比一串 else if 更清晰。
 *
 * 解题思路（对照课本）：
 * ① 读入一个字符 grade（等级）。
 * ② 用 switch(grade) 拿 grade 的值去逐个 case 比对：'A'、'B'、'C'、'D' 各对应一段分数。
 * ③ 命中哪个 case 就执行哪条 printf，并用 break 跳出 switch，防止"穿透"到下一条。
 * ④ 都不命中就走 default，打印输入错误提示。
 *
 * 关键知识点（对应课本"程序分析"）：
 * - switch 后面圆括号里的表达式值，与各个 case 后面的常量值相等时就执行该 case 后的语句。
 * - 每个 case 末尾的 break 用来跳出 switch；漏写 break 会"穿透"继续执行下一个 case 的语句（case 穿透是 switch 最常考的坑）。
 * - default 是"都不匹配时的兜底"，可以放在最后，也可以省略（省略后不匹配就什么都不做）。
 * - case 后面必须是常量（字符常量/整型常量），不能是变量或表达式。
 *
 * 难点：
 * - 难点是理解"switch 是入口跳转、不是逐条判断"：一旦命中就从那个 case 一直往下执行，直到遇到 break 或 switch 结束。
 *
 * 易错易混点：
 * - 每个 case 后别漏 break，否则输出会串到下一段（如输入 A 却连打 85~100、70~84、60~69、<60）。
 * - case 后是字符常量要加单引号 'A'，写成 A（变量）或 "A"（字符串）都错。
 * - switch(grade) 的 grade 是 char，scanf 用 %c 读一个字符。
 *
 * 内存与编译器原理：
 * - grade 是一个 char 格子；switch 编译后通常是"跳转表/比较链"，命中即跳到对应 case 标号处执行。
 * - break 编译成一条无条件跳转，直接跳到 switch 花括号之后，这就是"不穿透"的底层原因。
 * - 输出全是英文/ASCII 字符与数字，符合屏幕输出规范。
 */

#include <stdio.h>            /* 引入标准输入输出库，提供 printf 和 scanf */

int main()                    /* 主函数 */
{
    char grade;               /* grade 存放读入的等级字符 */

    scanf("%c", &grade);      /* 读入一个字符等级，变量前要加 & */

    printf("Your score:");    /* 先打提示前缀，不换行，等 switch 里接分数段 */

    switch (grade)            /* 拿 grade 的值去逐个 case 比对 */
    {
        case 'A': printf("85~100\n"); break;   /* 等级 A 对应 85~100，break 跳出 */
        case 'B': printf("70~84\n");  break;   /* 等级 B 对应 70~84 */
        case 'C': printf("60~69\n");  break;   /* 等级 C 对应 60~69 */
        case 'D': printf("<60\n");    break;   /* 等级 D 对应 60 以下 */
        default:  printf("enter data error!\n");   /* 都不匹配：输入错误兜底 */
    }

    return 0;                 /* 程序正常结束 */
}

/*
 * 运行结果（本机 CLion / MinGW-GCC 实测）：
 * 输入大写字母 A，回车后屏幕输出：
 * Your score:85~100
 * 因为 grade='A' 命中 case 'A'，打印 85~100 后 break 跳出。
 * 若输入 E 等不在 A~D 的字符，走 default，输出 Your score:enter data error!。
 * 与课本运行结果一致。
 *
 * 【运行与演示】点 ▶ 运行后分别输入 A、B、C、D、E 各跑一次，观察命中不同 case 与 default 兜底。
 * 还可故意把某个 break 注释掉再跑，直观看到"case 穿透"连打多段，是讲 break 的最好演示。
 */

/*
 * 【讲解过程稿】
 *
 * 开场：同学们，前面 if 管"比大小"。今天换一种选择：给一个等级字符，从四条固定分支里挑一条——这种"对号入座"用 switch 最清爽。【互动停顿：停 2~3 秒，问"如果 case 后面不写 break 会怎样？"让学生猜，再继续】
 * 这题在干什么：读一个等级字符 grade，switch 拿它去比 'A'/'B'/'C'/'D'，命中哪条就打哪段分数，都不中就走 default 报输入错误。
 * 大思路：把 switch 想成"电梯按楼层"：grade 是你要去的楼层，case 是各层按钮，break 是"到站开门出去"。不按 break，电梯就继续往下层跑（穿透）。
 * 逐块讲：
 *   - 第一块定义加读入：char grade，scanf 用 %c。
 *   - 第二块 printf 打前缀 Your score: 不换行，留给 switch 里接分数段。
 *   - 第三块 switch(grade) 四个 case 加一个 default：每条 case 打对应分数段后 break。【此处投屏 li4.6.puml 盒图】
 * 关键转折：全程序最要紧的是 break。switch 命中后是"从那个 case 一直往下执行"的，break 才把它截断成"只执行一条"。漏一个 break，输出立刻串味。
 * 收尾：一句话总结——switch 管等值多分支，break 管不穿透。留个问题：输入小写 a 会命中 case 'A' 吗？为什么？【互动停顿：留 3~5 秒，揭晓"不会，字符大小写 ASCII 码不同，会走 default"】
 */

/*
 * 【配图·选择结构盒图】见同目录 li4.6.puml（用 plantuml4idea 插件打开即渲染，讲"switch 按值跳转、break 截断"时投屏）。
 * 图只在 li4.6.puml 存一份，本文件不内嵌图，避免两处维护。
 */
