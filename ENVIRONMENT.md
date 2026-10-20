# 开发环境选择指南

本项目支持**三种开发环境**，选一种你喜欢的即可，也可以混着用：

---

## 📊 三环境对照表

| 环境 | 适合人群 | 优势 | 配置文件 | 快速开始 |
|------|---------|------|---------|---------|
| **🌐 Cloud Studio<br>（腾讯云在线 IDE）** | 不想装软件、多设备切换 | 浏览器打开就能用，0 安装 | `.vscode/` | 主 README |
| **💻 本地 VSCode** | 习惯本地开发、网络不稳 | 离线可用，插件丰富 | `.vscode/` | 主 README |
| **🔧 本地 CLion** | JetBrains 全家桶用户 | 强大智能提示、重构工具 | `CMakeLists.txt` | [CLion_SETUP.md](CLion_SETUP.md) |

---

## 🎯 快速决策

- **第一次学编程？** → 推荐 **Cloud Studio**（无需配环境，专注学代码）
- **已经装了 VSCode？** → 直接用**本地 VSCode**（导入插件即可）
- **熟悉 JetBrains IDE？** → 用 **CLion**（体验更专业）

---

## 🔄 环境间切换

**好消息**：三种环境操作**基本一样**，学一种就会用另外两种：

| 操作 | Cloud Studio / VSCode | CLion |
|------|----------------------|-------|
| 运行代码 | 右上角 **▷** 或 **F5** | 右上角 **▶ Run** |
| 断点调试 | **F5** 进入调试模式 | 🐞 **Debug** |
| 多文件编译 | 终端敲 `make` | 🔨 **Build** 或终端 `make` |

---

## 📚 详细文档

- **VSCode / Cloud Studio 用户** → 看主 [README.md](README.md)
- **CLion 用户** → 看 [CLion_SETUP.md](CLion_SETUP.md)
- **AI 助教使用** → 看 [AGENTS.md](AGENTS.md)

---

**一句话**：无论用哪个环境，代码都一样，编译标准都一样（`-std=gnu17`），写一次到处跑。🚀
