---
trigger: glob
glob:
  - "**/CMakeLists.txt"
---

# CLion / CMake 构建约定（编辑 CMakeLists.txt 时生效）

本项目 CLion 通过根目录 `CMakeLists.txt` 驱动构建，改配置或向学生解释运行方式时遵守：

## 现有配置事实

- `CMAKE_C_STANDARD 17` + `CMAKE_C_EXTENSIONS ON` = 对应 `gcc -std=gnu17`（教学口径 C99，C17 兼容）。编译选项 `-Wall -g`（开警告 + 调试信息）。
- 教材例题用 `file(GLOB_RECURSE ... CONFIGURE_DEPENDS "0_textbook_code/*.c")` **自动为每个 `.c` 生成独立可执行目标**，目标名把文件名里的 `.` 换成 `_`（`li2.1.c` → `li2_1`）。`CONFIGURE_DEPENDS` 保证增删 `.c` 后 CLion 自动重载 CMake，**无需手动加运行配置**——在右上角运行配置下拉框直接选目标即可。
- 根目录示例（`main`、`hello_world`）与 `1_demo/` 的 demo 用显式 `add_executable` 单独定义。

## 为什么 `.puml` / `.svg` / `.md` 不影响构建

GLOB 只匹配 `*.c`，所以旁挂的 `liX.Y.puml`、课件 `slides/*.svg`、讲稿 `*.md` **不产生构建目标、不干扰编译**——这正是"图旁挂、源文件内只放指路注释"设计能成立的前提（见 `textbook-c-code.md`）。

## 新增文件怎么跑

- **新增教材例题 `.c`**（放进 `0_textbook_code/`）：什么都不用改，CLion 重新加载后自动出现新目标。
- **新增单文件练习**：`add_executable(目标名 你的文件.c)`。
- **新增多文件工程**：`add_executable(目标名 文件1.c 文件2.c ...)`（多 `.c` 一起列，漏列会 `undefined reference`）。或直接用 `make`（见 `1_demo/multifile_demo/`）。

## 常见坑

- 改完 `CMakeLists.txt` 若新目标没出现：先让 CMake 重新加载（File → Reload CMake Project，或改一下文件触发 `CONFIGURE_DEPENDS` 重扫）。
- 在 `1_demo/multifile_demo/` 里按 F5/▷ 只编译单文件会链接失败——多文件应用 `make` 或在 CMake 里把多个 `.c` 列进同一 `add_executable`。
- 目标名不能含 `.`（故 `li2.1` → `li2_1`）。
