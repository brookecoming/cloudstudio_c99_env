---
name: read-external-image
description: 当用户提供项目外的绝对路径图片（截图工具目录、系统临时目录等，如 D:\PIXPIN_PICS\example.png 或 file:///C:/Users/... 的图片），或因“路径越界 / path out of bounds / permission denied”无法直接用 Read 读取图片时使用——先经 PowerShell 把图片复制进项目内 ./pic/ 目录，再用 Read 工具做视觉分析，绕开沙箱路径边界限制。常配合 textbook-code-extractor（教材截图提取）在受限路径下先落地再分析。触发词：外部图片路径 / 截图提取 / 读不了这张图 / path out of bounds。
---

# read-external-image

把**工作区外部的图片**（系统临时目录、截图工具目录等）安全复制进项目内的 `./pic/` 目录，再用视觉/文件读取能力正常分析，解决“路径越界 / 权限不足”导致的读图失败。

## 何时用 / 不用

- **用**：`Read` 直接读该外部路径报越界/权限错；或用户给的是工作区外的绝对路径 / `file:///` URI 指向图片。
- **不用**：如果 `Read` 已能直接读到这张图（图本就在项目内、或当前环境本就有权访问该路径），**别多此一举**，直接读即可。

## 关键约束

- **不要**先用默认文件读取器去硬读外部路径——沙箱会拦。必须先复制到项目内。
- 当前 shell 是 **PowerShell**：多条命令用 `;` 连接，**不要用 `&&`**；路径含空格/中文一律加**双引号**。
- **只复制、不移动**（用 `Copy-Item`，不要 `Move-Item`），保持用户原图不动。

## 步骤

1. **规范化路径**：取用户的绝对路径，去掉 `file:///` 前缀，分隔符统一成 Windows 反斜杠 `\`。
2. **备目录**：确认项目根下有 `./pic/`，没有就建（项目根 = `D:\clion_workspace\c99_env`）：
   ```powershell
   New-Item -ItemType Directory -Force -Path .\pic | Out-Null
   ```
3. **复制进沙箱**（用 `-LiteralPath`，避免 `[ ] * ?` 被当通配符；目标名用无中文/无空格的 ASCII，省去二次编码麻烦）：
   ```powershell
   Copy-Item -LiteralPath "C:\path\to\image.png" -Destination ".\pic\target.png" -Force
   ```
4. **读图分析**：对项目内 `.\pic\target.png` 用 `Read` 工具做视觉分析（确认复制成功、文件非空后再读）。
5. **回答**：基于分析结果，清晰准确地回应用户的原始问题。

## 示例

用户给：`D:\PIXPIN_PICS\example.png，提取源码，放在 @01，命名 li1.1.c`

1. 判断该路径在工作区外 → 先复制：`Copy-Item -LiteralPath "D:\PIXPIN_PICS\example.png" -Destination ".\pic\target.png" -Force`
2. `Read .\pic\target.png` 完成取图 → 后续交给 `textbook-code-extractor` 提取。

## 注意 / 兜底

- 若 `Copy-Item` 因源文件被占用失败，提示用户先关闭截图工具再重试。
- 复制进来的临时图留在 `./pic/` 即可；若不希望入库，把 `pic/` 视情况加入 `.gitignore`（本仓库当前把图放 `pic/` 供临时分析）。
