# mermaid/ — 课本章节流程图 / 原理图

本目录按**章节**存放《C 程序设计》的流程图、原理图,统一用 **Mermaid** 纯文本描述。
好处:图是文本,能进 Git、能 diff、能改;渲染/缩放/导出交给插件或在线工具。

## 约定

- 每章一个 `.mmd` 文件,命名如:`第1章_编译链接运行流程.mmd`、`第3章_选择结构流程图.mmd`;
- 文件内容就是 Mermaid 源码(`flowchart` / `sequenceDiagram` / `classDiagram` 等均可);
- 新增章节图时,照抄现有文件改内容即可。

## 怎么在线渲染 / 缩放 / 导出

**方式 1:Mermaid Editor 插件(推荐,已加入工作区推荐)**

1. 导入仓库后按提示安装 `Mermaid Editor`(`tomoyukim.vscode-mermaid-editor`);
2. 打开任意 `.mmd`,右侧/下方即出现**实时预览**;
3. **放大缩小**:用 VS Code 窗口缩放 `Ctrl +` / `Ctrl -`(或预览内滚轮);
4. **下载到本地**:点预览面板的**导出按钮**,可选 **PNG / SVG**。
   - 想要"无限放大不失真"就导 **SVG**;要贴文档/微信就导 **PNG**。

**方式 2:嵌在 Markdown 里**

若把 mermaid 代码块写进 `.md`(三个反引号 + `mermaid`),装了 `bierner.markdown-mermaid` 后,
Markdown 预览(`Ctrl+Shift+V`)即可直接渲染该图。

**方式 3:零安装在线(保底)**

把 `.mmd` 内容复制粘贴到 <https://mermaid.live>,在线渲染、缩放、导出 PNG/SVG 全都有,
不依赖任何插件,学生在家没装环境也能看。

## 现有图清单

| 文件 | 内容 |
|------|------|
| `第1章_编译链接运行流程.mmd` | 编辑 → 编译 → 链接 → 运行 的完整流程(含报错回退) |
| `第3章_选择结构流程图.mmd` | if-else 选择结构求 max 的流程图 |
