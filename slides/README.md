# slides/ — 幻灯片(PPT)与 Markdown 渲染说明

## 一、Markdown 在线渲染

- **不装插件也能看**:VSCode/Cloud Studio **内置 Markdown 预览**,打开 `.md` 按 `Ctrl+Shift+V` 即渲染。
- **增强(可选)**:装 `Markdown Preview Enhanced`(`shd101wyy.markdown-preview-enhanced`),
  支持数学公式、更多图表、**导出 PDF/HTML**。
- 嵌在 `.md` 里的 mermaid 图,由 `bierner.markdown-mermaid` 在预览中渲染。

## 二、PPT / 幻灯片:两种路线

### 路线 A:用 Marp 把幻灯片写成 Markdown(推荐,能进 Git)

1. 装推荐扩展 **Marp for VS Code**(`marp-team.marp-vscode`);
2. 打开本目录的 `.md`(文件头有 `marp: true`),右上角点 **Marp 预览**图标,即看到**幻灯片分页预览**;
3. 翻页:预览内方向键 / 滚轮;
4. **导出**:预览面板可导出 **PDF / PPTX / HTML** —— 想要传统 `.pptx` 就导 PPTX。
5. 新建一章幻灯片:复制 `第1章_示例幻灯片.md`,改内容;页与页之间用一行 `---` 分隔。

> 示例见 `第1章_示例幻灯片.md`。

### 路线 B:已有现成 `.pptx` 课件

VSCode/网页 IDE **对 `.pptx` 的渲染支持很差**,不建议直接丢进 IDE 看。稳妥做法:

1. 用 Office/WPS 把 `.pptx` **另存为 PDF**;
2. 把 PDF 放进仓库(或本目录);
3. 装推荐扩展 **PDF**(`tomoki1207.pdf`),在 IDE 内**直接打开 PDF 在线看**(可缩放/翻页)。

> 这样学生拉取仓库后,课件(PDF)与代码、示例、流程图在一起,全部在线可用。

## 现有文件

| 文件 | 说明 |
|------|------|
| `第1章_示例幻灯片.md` | Marp 幻灯片示例(4 页),可预览/导出 PPTX/PDF/HTML |
