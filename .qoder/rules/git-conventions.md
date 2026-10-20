---
trigger: model_decision
description: 涉及 git 操作时使用：提交(commit)、查看状态、决定哪些文件该入库、处理 .bak 备份文件、写提交信息、或学生问 git 相关命令时。
---

# git 提交与备份规范

## 这个仓库的入库边界（据 .gitignore 实际内容）

**已忽略、绝不提交**：
- `AGENTS.md`（AI 助教人设卡，本地/个人专用）。
- `*.pdf` 与 `0_textbook_code/**/textbook/`（教材扫描件/页面图，版权 + 体积，仅本地核对用）。
- 编译产物：`main`、`app`、`*.o`、`*.out`、`*.exe`、`*.dSYM/`。
- IDE 本地配置：`.idea/`、`cmake-build-*/`、`*.iml`。
- 编辑器/系统临时：`*.swp`、`.DS_Store`、`Thumbs.db`。
- 逐章一次性改稿/生成脚本：`0_textbook_code/**/_*.py`（如 `_repl03.py`、`_fix02_tk.py`，跑完即废；可复用引擎已收进 `.qoder/skills/*/scripts/`，不入库）。

**应正常入库（团队共享）**：
- `.qoder/rules/**`、`.qoder/skills/**`（规则与技能是项目资产，随仓库走）。
- `0_textbook_code/{章}/*.c`、`*.puml`、`slides/*.svg`、各章 `_口播稿.md`/`_提词本.md`/`备课导航.md`（注：`slides/` 与讲稿是老师原创产物正常入库；但章目录下以 `_` 开头的一次性 `.py` 转换脚本不入库，见上）。
- `README.md`、`ENVIRONMENT.md`、`CLion_SETUP.md`、`CMakeLists.txt`、`投影建议.md` 等。

## 备份文件 .bak 处理

脚本改 PPT/讲稿时会生成 `*.bak-*` 备份（如 `...pptx.bak-preNotes03`、`...md.bak-preFix02`）。它们是**一次性安全副本，不入库**——`.gitignore` 已含 `*.bak-*`。学生若把备份误加进来，用 `git rm --cached <文件>` 取消跟踪（不删本地）。

## 提交纪律（对零基础学生也照此讲）

- **绝不在用户没明确要求时擅自 `git commit`**；只改工作区，提交由用户自己核对后进行。
- 提交前先看 `git status` + `git diff`，确认没有把扫描件/`.bak`/编译产物误提交。
- 提交信息用**祈使句、说清"做了什么"**，中文即可，如：`为第4章例4.3添加讲解过程稿与puml`；一次提交只做一件逻辑事。
- 远程是 GitHub（`origin/main`）：**不 force push、不改 git config、不加 --no-verify**，除非用户明确要求。
