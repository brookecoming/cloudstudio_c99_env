cmake_minimum_required(VERSION 3.10)
project(c99_env C)

# 设置 C 标准为 C17（对应 gcc -std=gnu17）
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
# 使用 GNU 扩展（对应 -std=gnu17）
set(CMAKE_C_EXTENSIONS ON)

# 编译选项：开启所有警告 + 生成调试信息
add_compile_options(-Wall -g)

# ===== 可执行程序定义 =====

# 1. 根目录的入门示例（单文件）
add_executable(main main.c)

# 1.1 Hello World 入门示例
add_executable(hello_world hello_world.c)

# 2. 自定义头文件 demo（单编译单元，头文件被包含）
add_executable(demo_custom_header
    1_demo/自定义头文件/main.c
)

# 3. 多文件工程 demo（多编译单元 + 链接）
add_executable(demo_multifile
    1_demo/multifile_demo/main.c
    1_demo/multifile_demo/calc.c
)

# 4. 教材例题：自动为 0_textbook_code 下每个 .c 文件生成一个可执行目标
#    例：0_textbook_code/02/li2.1.c  ->  目标名 li2_1（点号换成下划线）
#    CONFIGURE_DEPENDS 保证新增/删除文件后 CLion 会自动重新加载 CMake
file(GLOB_RECURSE TEXTBOOK_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/0_textbook_code/*.c"
)
foreach(src ${TEXTBOOK_SOURCES})
    get_filename_component(fname ${src} NAME)              # 取文件名（含扩展名）li1.1.c
    string(REGEX REPLACE "\\.c$" "" stem ${fname})         # 只去掉末尾的 .c -> li1.1
    string(REPLACE "." "_" target_name ${stem})            # li1.1 -> li1_1（目标名不能含点）
    add_executable(${target_name} ${src})
endforeach()

# ===== 说明 =====
# CLion 用户：
# 1. 打开本项目后，CLion 会自动加载此 CMakeLists.txt
# 2. 点击右上角 🔨 Build 编译所有目标
# 3. 点击 ▶ Run 选择要运行的目标（main / demo_custom_header / demo_multifile）
# 4. 断点调试：点行号左侧设断点，点 🐞 Debug 即可

# 若要添加新的练习文件：
# - 单文件程序：add_executable(你的目标名 你的文件.c)
# - 多文件程序：add_executable(你的目标名 文件1.c 文件2.c ...)
