# CLion 快速开始指南

> 本项目同时支持 **VSCode/Cloud Studio** 和 **CLion**。这份文档专门给 **CLion 用户**看。

---

## 一、为什么有这份文档？

本仓库最初为 **Cloud Studio / VSCode** 设计，配置文件在 `.vscode/` 目录。
但 CLion 是基于 **CMake** 构建系统的 IDE，不直接读取 `.vscode/` 的配置。

**好消息**：我们已经为 CLion 用户准备好了 `CMakeLists.txt`，你可以无缝使用本项目的所有示例和练习。

---

## 二、打开项目（首次使用）

1. **用 CLion 打开本仓库目录**：`File → Open → 选择 c99_env 文件夹`
2. CLion 会自动检测到 `CMakeLists.txt` 并加载项目
3. 等待 CMake 配置完成（右下角进度条）
4. 配置完成后，右上角会出现可运行的目标列表（如 `main`、`demo_multifile` 等）

---

## 三、编译与运行

### 方式 1：点击按钮（最简单）

1. 右上角下拉框选择要运行的目标（如 `main`）
2. 点击 **🔨 Build**（编译）或直接点 **▶ Run**（编译+运行）
3. 输出会显示在下方的 **Run** 面板

### 方式 2：右键菜单

在项目树中右键点击 `.c` 文件 → `Run 'xxx'` 或 `Debug 'xxx'`

### 方式 3：断点调试

1. 在代码行号左侧点击，设置断点（红点）
2. 点击右上角 **🐞 Debug**
3. 程序会在断点处暂停，左侧可查看变量值，工具栏可单步执行

---

## 四、当前可运行的目标

| 目标名称 | 对应文件 | 说明 |
|---------|---------|------|
| `main` | `main.c` | 入门示例（C99 特性测试） |
| `demo_custom_header` | `1_demo/自定义头文件/main.c` | 自定义头文件用法 |
| `demo_multifile` | `1_demo/multifile_demo/main.c + calc.c` | 多文件工程示例 |

---

## 五、添加自己的练习文件

### 单文件程序

1. 在项目中创建你的 `.c` 文件（如 `0_assignment/第3章/作业1.c`）
2. 打开 `CMakeLists.txt`，在末尾添加：
   ```cmake
   add_executable(作业1 0_assignment/第3章/作业1.c)
   ```
3. CLion 会自动重新加载配置
4. 右上角目标列表会出现 `作业1`，点击运行即可

### 多文件程序

```cmake
add_executable(我的项目
    你的main.c
    你的模块1.c
    你的模块2.c
)
```

---

## 六、编译标准说明

本项目使用 **C17 标准 + GNU 扩展**（等同于 `gcc -std=gnu17`）：
- 向下兼容 C99（教材主要使用的标准）
- 支持 C11/C17 的所有特性
- 保留 GNU 常用扩展，避免严格 ISO 模式的限制

CLion 的 `CMakeLists.txt` 已配置好这些选项，无需手动调整。

---

## 七、常见问题

**Q1：为什么 CLion 报 "Cannot resolve symbol" 或 "无法找到 CMakeLists.txt"？**
- CMake 配置可能未完成，点击 `Tools → CMake → Reload CMake Project`
- 或试试 `Tools → CMake → Reset Cache and Reload`

**Q2：能用 Makefile 吗？**
- 可以。`1_demo/multifile_demo/` 里的 `Makefile` 在 CLion 终端（`Alt+F12`）里直接敲 `make` 即可

**Q3：`.vscode/` 配置和我有关吗？**
- 无关。那是给 VSCode/Cloud Studio 用的，CLion 只看 `CMakeLists.txt`

**Q4：输入中文后乱码？**
- 确保源文件编码为 UTF-8：`File → File Encoding → UTF-8`
- CLion 终端：`Settings → Tools → Terminal → Shell integration` 确认终端编码为 UTF-8

**Q5：Build 输出里没有 `-std=gnu17` 怎么办？**
- `Tools → CMake → Reset Cache and Reload` 清理缓存
- 或删除项目根目录的 `cmake-build-*` 文件夹，重新打开项目

---

## 八、VSCode 用户 vs CLion 用户对照

| 操作 | VSCode/Cloud Studio | CLion |
|------|---------------------|-------|
| **运行单文件** | 右上角 ▷ 按钮 或 F5 | 右上角 ▶ Run |
| **断点调试** | F5（gdb） | 🐞 Debug（gdb）|
| **多文件编译** | 终端敲 `make` | 🔨 Build 或终端 `make` |
| **配置文件** | `.vscode/tasks.json` | `CMakeLists.txt` |
| **添加新目标** | 直接运行，或修改 tasks.json | 修改 `CMakeLists.txt` |

---

祝你用 CLion 学习愉快！有问题可以问 AI 助教（见主 `README.md`）。🎓
