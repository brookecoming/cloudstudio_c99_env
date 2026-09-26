# 3_slides/ — 幻灯片(PPT)与课件

本目录存放**幻灯片 / 课件**。两种用法:

## 路线 A:用 Marp 把幻灯片写成 Markdown(推荐,能进 Git)

1. 装推荐扩展 **Marp for VS Code**(`marp-team.marp-vscode`);
2. 新建 `.md`,文件头加:
   ```markdown
   ---
   marp: true
   theme: default
   paginate: true
   ---
   ```
3. 页与页之间用一行 `---` 分隔;点右上角 **Marp 预览**图标即分页预览;
4. 预览面板可**导出 PDF / PPTX / HTML**。

## 路线 B:已有现成 `.pptx` 课件

网页 IDE 渲染 `.pptx` 支持差,不建议直接丢进 IDE。稳妥做法:

1. 用 Office/WPS 把 `.pptx` **另存为 PDF**;
2. 把 PDF 放进本目录;
3. 装推荐扩展 **PDF**(`tomoki1207.pdf`),在 IDE 内**直接打开在线看**(可缩放/翻页)。

> Markdown 渲染本身无需插件:打开 `.md` 按 `Ctrl+Shift+V` 即内置预览;
> 增强(公式/导出)可装 `Markdown Preview Enhanced`。
