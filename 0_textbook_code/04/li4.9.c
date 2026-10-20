/*
 * 例 4.9：求 ax^2+bx+c=0 方程的解。
 * 【本例课本直接给出了 C 源码，本文件忠实照搬，只补充便于讲解的中文注释】
 *
 * 题目背景（例 4.1 的完整版：把"漏掉的情况"全补上）：
 * 在例 4.1 中曾编写过程序，但实际上应该有以下几种可能：
 * ① a=0，不是二次方程。
 * ② b^2-4ac=0，有两个相等实根。
 * ③ b^2-4ac>0，有两个不等实根。
 * ④ b^2-4ac<0，有两个共轭复根，应当以 p+qi 和 p-qi 的形式输出复根，其中 p=-b/2a，q=(sqrt(b^2-4ac))/2a。
 * 本例用多层 if—else 把这四种情况一次处理完，是选择结构的综合大例。
 *
 * 解题思路（对照课本 N-S 图 4.15）：
 * ① 读入 a、b、c。先打前缀 "The equation "。
 * ② 若 fabs(a)<=1e-6（即 a 视为 0），打印 "is not a quadratic"，结束。
 * ③ 否则算 disc=b*b-4*a*c；若 fabs(disc)<=1e-6（即 disc 视为 0），打印两个相等实根 -b/(2a)。
 * ④ 否则若 disc>1e-6，用求根公式算 x1、x2 打印两个不等实根。
 * ⑤ 否则（disc<0）算实部 realpart=-b/(2a)、虚部 imagpart=sqrt(-disc)/(2a)，打印一对共轭复根。
 *
 * 关键知识点（对应课本"程序分析"）：
 * - 程序中用 disc 代表 b^2-4ac，先计算 disc 的值，以减少以后的重复计算。
 * - 判断 b^2-4ac 是否等于 0 时不能直接写 if(disc==0)：实数在计算和存储时有微小误差，本来为零的量可能被判为不等于零而导致结果错误；办法是判别 fabs(disc) 是否小于一个很小的数（如 1e-6），小于就认为 disc 等于 0。
 * - 同理 a 是否为 0 也用 fabs(a)<=1e-6 判断，而不是 a==0。
 * - 程序中以 realpart 代表实部 p、以 imagpart 代表虚部 q，以增加可读性。
 * - 输出复根时，在虚部的格式声明（%8.4f）后面人为地加一个普通字符 "i"，就能输出 "p+qi" 这样的复数形式。
 *
 * 难点：
 * - 难点是"浮点判零要用 fabs 加阈值"这一工程习惯：== 对 double 不可靠，这是全章最容易忽略又最重要的一个点。
 *
 * 易错易混点：
 * - 判 a 或 disc 为零用 fabs(x)<=1e-6，别写 x==0。
 * - 复根虚部是 sqrt(-disc)（disc 为负，取相反数再开方），别写成 sqrt(disc) 那样对负数开方。
 * - scanf 读 double 用 %lf，且课本格式是 "%lf,%lf,%lf"，输入用逗号分隔。
 * - 需要 #include <math.h> 才能用 fabs 和 sqrt。
 *
 * 内存与编译器原理：
 * - a,b,c,disc,x1,x2,realpart,imagpart 全是 double（8 字节双精度），因为开方和除法会产生小数。
 * - double 二进制存储无法精确表示多数十进制小数，比较相等会有误差，所以用 fabs 阈值判零。
 * - sqrt 对负数在实数域无定义，程序靠先判 disc 符号保证 sqrt 的实参非负（复根分支用 -disc）。
 */

#include <stdio.h>            /* 引入标准输入输出库，提供 printf 和 scanf */
#include <math.h>             /* 引入数学库，提供 fabs 和 sqrt */

int main()                    /* 主函数 */
{
    double a, b, c, disc, x1, x2, realpart, imagpart;   /* 全部用 double */

    scanf("%lf,%lf,%lf", &a, &b, &c);   /* 读入三个系数，逗号分隔，double 用 %lf */
    printf("The equation ");            /* 先打前缀，后面各分支接不同结论 */

    if (fabs(a) <= 1e-6)                /* a 视为 0：不是二次方程 */
        printf("is not a quadratic\n");
    else                                /* a 非 0，是二次方程，继续判别 */
    {
        disc = b * b - 4 * a * c;       /* 先算判别式，后面复用 */
        if (fabs(disc) <= 1e-6)         /* disc 视为 0：两个相等实根 */
            printf("has two equal roots:%8.4f\n", -b / (2 * a));
        else                            /* disc 不为 0，再分正负 */
            if (disc > 1e-6)            /* disc>0：两个不等实根 */
            {
                x1 = (-b + sqrt(disc)) / (2 * a);
                x2 = (-b - sqrt(disc)) / (2 * a);
                printf("has distinct real roots:%8.4f and %8.4f\n", x1, x2);
            }
            else                        /* disc<0：两个共轭复根 */
            {
                realpart = -b / (2 * a);          /* 复根实部 p */
                imagpart = sqrt(-disc) / (2 * a); /* 复根虚部 q（disc 为负，取 -disc 开方） */
                printf(" has complex roots:\n");
                printf("%8.4f+%8.4fi\n", realpart, imagpart);   /* 输出 p+qi */
                printf("%8.4f-%8.4fi\n", realpart, imagpart);   /* 输出 p-qi */
            }
    }

    return 0;                 /* 程序正常结束 */
}

/*
 * 运行结果（本机 CLion / MinGW-GCC 实测，运行 3 次）：
 * (1) 输入 1,2,1，得到两个相等的实根：
 * The equation has two equal roots: -1.0000
 * (2) 输入 1,2,2，得到两个共轭的复根：
 * The equation  has complex roots:
 *  -1.0000+  1.0000i
 *  -1.0000-  1.0000i
 * （虚部用 %8.4f 占 8 列，故 + 号后有两个空格；课本扫描件排版把空格压窄了，数值完全一致。）
 * (3) 输入 2,6,1，得到两个不等的实根：
 * The equation has distinct real roots: -0.1771 and  -2.8229
 * 三次结果与课本运行结果一致。
 *
 * 【运行与演示】点 ▶ 运行后分别输入 "1,2,1"、"1,2,2"、"2,6,1" 各跑一次，覆盖相等实根/共轭复根/不等实根三种情况。
 * 再输入 "0,2,1" 可看到 a=0 走 "is not a quadratic" 分支。
 */

/*
 * 【讲解过程稿】
 *
 * 开场：同学们，例 4.1 我们只判了"判别式负不负"，其实漏了好几种情况。今天把账一次算清。【互动停顿：停 2~3 秒，问"ax^2+bx+c=0 一共有几种可能？"让学生数，再揭晓"四种：a=0、disc=0、disc>0、disc<0"】
 * 这题在干什么：读入 a、b、c，先排除 a=0，再按判别式 disc 的零/正/负分三路，分别输出相等实根、不等实根、共轭复根。
 * 大思路：把它想成"先验身份、再分三路"。第一道门看 a 是不是 0（不是二次方程就出门）；进门后看 disc 这盏灯：灭（=0）走相等实根，亮正走不等实根，亮负走复根。
 * 逐块讲：
 *   - 第一块读入加前缀：三个 double，scanf 用 %lf 逗号分隔；先打 The equation 前缀。
 *   - 第二块 if(fabs(a)<=1e-6)：a 当 0 处理，打 is not a quadratic。
 *   - 第三块算 disc 后 if(fabs(disc)<=1e-6)：相等实根，直接打 -b/(2a)。
 *   - 第四块 else 里再分：disc>1e-6 用求根公式打两个不等实根；否则算 realpart、imagpart 打一对共轭复根。【此处投屏 li4.9.puml 盒图】
 * 关键转折：全程序最要紧的是"判零不用 == 而用 fabs 加 1e-6"。double 存小数有微小误差，本来该是 0 的 disc 可能差一点点，用 == 就漏判；fabs(disc)<=1e-6 才是工程上可靠的"视为零"。这一点比求根公式本身更值钱。
 * 收尾：一句话总结——多路选择靠层层 if—else，浮点判零靠 fabs 阈值。留个问题：复根分支里为什么开方用 sqrt(-disc) 而不是 sqrt(disc)？【互动停顿：留 3~5 秒，揭晓"此分支 disc 为负，取相反数变正才能开方"】
 */

/*
 * 【配图·选择结构盒图】见同目录 li4.9.puml（用 plantuml4idea 插件打开即渲染，讲"先验 a、再按 disc 分三路"时投屏）。
 * 图只在 li4.9.puml 存一份，本文件不内嵌图，避免两处维护。
 */
