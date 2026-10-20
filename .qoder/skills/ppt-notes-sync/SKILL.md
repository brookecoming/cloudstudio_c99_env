---
name: ppt-notes-sync
description: Sync PPTX speaker notes (演讲者备注) back into the chapter 口播稿.md and 提词本.md（反向回填：PPT 备注 → 讲稿）, keyed by slide 放映编号 (one-to-one with 台词). Use when the user asks to 同步PPT备注 / 把PPT的备注同步到口播稿 / 从pptx回填讲稿 / 检查PPT备注与口播稿/提词本差异 / 对账 speaker notes vs 讲稿. Complements the forward flow (口播稿 → PPT via ppt-course-builder/scripts/sync_notes.py); 默认方向是「口播稿为唯一事实源」，仅当用户明确说"以 PPT 为准 / 把 PPT 备注进口播稿"时才用本技能反向回填. Triggers on .pptx + 口播稿/提词本 + 同步/备注/差异/比对. NOT for 生成/装配整章课件（那是 ppt-course-builder）、画课件配图（ppt-diagram-maker）、节首「动笔划课本」concepts 页（ppt-concepts-pagegen）、从截图提取 .c 代码（textbook-code-extractor）.
---

# ppt-notes-sync

把某章 PPT 的**演讲者备注**回填/对账到同章的 `第N章_口播稿.md`（逐张全文）与 `第N章_提词本.md`（浓缩 cue 版），**按放映编号一一对应**。

## 关键约定（先读）

- **编号 = 放映顺序**，不是 pptx 编辑视图左下角含隐藏页的页码，也不是 `slideN.xml` 的文件名编号。提取脚本已用 `presentation.xml` 的 `sldIdLst` 精确定序，务必信任它的输出编号。
- **本项目默认方向是反的**：常规流程是「口播稿 = 唯一事实源 → 跑 `sync_notes.py` 生成 PPT 备注」。本 skill 处理的是**反向回填**（PPT 备注 → 讲稿），仅当用户明确说"以 PPT 为准 / 把 PPT 备注同步进口播稿"时使用。
- **口播稿 vs 提词本，处理方式不同**：
  - `口播稿.md` 与 PPT 备注是**逐字对应**关系 → 可比对、可逐处同步。
  - `提词本.md` 是**浓缩改写版**（其头部自己写明"措辞不一致以口播稿为准"），【口播】本就与 PPT 备注文字不同 → **不做逐字同步**。只把 PPT 里**新增的实质信息/知识点指针**（而非同义改写）人工并入提词本对应张。
- **不要盲目覆盖加粗**：PPT 备注是纯文本，`**` 加粗是 md 侧的强调标记。同步只改文字内容，保留 md 原有格式标记。

## 工作流

复制此清单跟踪进度：

```
- [ ] 1. 提取 PPT 备注 → _ppt_notes_dump.txt（按放映顺序）
- [ ] 2. 比对 dump vs 口播稿.md → 列出真实差异
- [ ] 3. 逐处同步差异到 口播稿.md（PPT 为准）
- [ ] 4. 仅把实质新增并入 提词本.md（人工判断，非逐字）
- [ ] 5. 重跑比对验证：口播稿应为 0 差异
```

### 步骤 1：提取备注

```
powershell -ExecutionPolicy Bypass -File scripts/extract_ppt_notes.ps1 `
  -PptxPath "<绝对路径>\第N章 xxx.pptx" `
  -OutPath  "<同目录>\_ppt_notes_dump.txt"
```

输出以 `========== SLIDE N ==========` 分隔，每张首行 `▶第N张·标题` 为标题、其余为备注正文；无备注页标 `(无备注)`。

> ⚠️ Windows/PowerShell 环境注意：路径含空格（如 `第2章 算法--程序的灵魂1003.pptx`）时，若命令行引号被吞导致
> "positional parameter" 报错，改用**一个 runner `.ps1` 内部写死带引号的调用**，再用 `powershell -File runner.ps1` 执行。
> shell 是 powershell，多命令用 `;` 连接，**不要用 `&&`**。

### 步骤 2：比对

```
node scripts/compare_notes.js <_ppt_notes_dump.txt> <第N章_口播稿.md> <_diff.txt> oral
```

`compare_notes.js` 归一化（去 `**`/反引号/``` 围栏/空白）后按公共前后缀剥离，只打印每张**差异中段**，末尾给「共比对 X 张，差异 Y 处」。

- `oral` 模式：解析 `### … 第 N 张 · 标题` + 其下整段正文。用于**口播稿**。
- `prompter` 模式：每张只取 `**【口播】**` 到下一 `---` 之间的引号块。用于**提词本**。

> `prompter` 模式对提词本会报大量差异——这是**浓缩改写导致的预期噪声**，不代表要同步。把它当"逐张并排对照表"用，人工挑出 PPT 侧的**实质新增**。

### 步骤 3-4：同步

- 对 `口播稿.md`：用 SearchReplace 逐处把差异段改为 PPT 备注文字，保留 md 格式。
- 对 `提词本.md`：只并入 PPT 新增的实质内容（如某个"可指给学生看的课本图"指针），同义改写不动。

### 步骤 5：验证

重跑步骤 2 的 `oral` 比对，应显示「差异 0 处」。

## 脚本

- `scripts/extract_ppt_notes.ps1`：参数化提取器（`-PptxPath` / `-OutPath`），按放映顺序导出备注；过滤页码字段(`a:fld`)与日期占位噪声(如 `1.7.2013`)。可复用于任意章节 pptx。
- `scripts/compare_notes.js`：通用比对器（`oral` / `prompter` 双模式），`node compare_notes.js <dump> <md> <out> [mode]`。

## 已知坑

- 文件名编号 ≠ 放映顺序：必须走 `sldIdLst`（脚本已处理）。
- 提词本【口播】的 `flush()` 曾在 `---` 与下一张 header 处被调用两次、第二次用空 buf 覆盖内容 → 现 `flush()` 仅在 `buf.length>0` 时写入。
- 代码块 ` ```c ` 残留的 `c`、`**` 加粗、变体选择符 emoji 都可能造成假差异 → `norm()` 已统一剥离 ``` 围栏/反引号/`**`/空白；header 正则不强依赖 emoji。
