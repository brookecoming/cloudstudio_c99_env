# -*- coding: utf-8 -*-
"""节首「动笔划课本」页 SVG 生成器（通用版，摘自 _gen_concepts0X.py）。

用法（两种，任选其一）：
  A. 就地改：把本文件复制到本章 slides/ 下，直接编辑下方 SECTIONS，然后  python gen_concepts.py
  B. 外部数据：python gen_concepts.py --data 第4章_sections.py [--out 输出目录]
     数据文件里须定义一个 SECTIONS 列表（结构见下）。

SECTIONS 结构（每章唯一要改的东西）：
  [ (节号, 节标题, [(扫描件页码, 课本原句), ...], (重点, 难点, 易错易混)), ... ]

版式（勿轻改，改动前先投屏核对可读）：1280×960 4:3；标题条 #2B6CB0；
主体深蓝 #1A365D；页码高亮 #FBD38D；易错带 #FEB2B2；原句 21 / 行高 30 / 句距 15。
铁律见同目录 SKILL.md：原句须课本一字可定位、每句带核对过的 P 页码、全页用「划」不用「圈」、
每页 ≤6~7 条超了拆 _2/_3、页脚三合一（重点/难点/易错易混）、无计时口令。
"""
import argparse
import html
import importlib.util
import os

# ---- 就地改的默认数据；用 --data 时被外部 SECTIONS 覆盖 ----
# 每章开始时把这里替换成本章各小节（或写到外部 --data 文件）。
SECTIONS = []

# ---- 版式常量 ----
W, H = 1280, 960
FS = 21            # 原句字号
LH = 30            # 行高
GAP = 15           # 句间距
X0 = 70
XMAX = 1210
PAGE_COLOR = "#FBD38D"


def cw(ch, fs):
    """单字符近似像素宽：中文按全宽、西文按半宽。"""
    return fs if ord(ch) > 0x2E80 else fs * 0.55


def px(s, fs):
    return sum(cw(c, fs) for c in s)


def wrap(text, fs, budget):
    """按像素预算把 text 切成多行（尽量在中文/标点自然断开）。"""
    lines, cur, curw = [], "", 0.0
    for ch in text:
        w = cw(ch, fs)
        if curw + w > budget and cur:
            # 避免行首是收尾标点
            if ch in "，。；：、）)”％%":
                cur += ch
                curw += w
                continue
            lines.append(cur)
            cur, curw = ch, w
        else:
            cur += ch
            curw += w
    if cur:
        lines.append(cur)
    return lines


def esc(s):
    return html.escape(s, quote=False)


def build(sec):
    num, title, items, (zh, nd, yc) = sec
    parts = []
    parts.append(f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}">')
    parts.append(f'<rect width="{W}" height="{H}" fill="#F7FAFC"/>')
    # 顶部标题条
    parts.append('<rect x="40" y="28" width="1200" height="92" rx="14" fill="#2B6CB0"/>')
    parts.append(f'<text x="{X0}" y="86" font-family="Microsoft YaHei" font-size="30" font-weight="bold" '
                 f'fill="#FFFFFF">【动笔划课本】　§{num}　{esc(title)}</text>')
    # 主体深色框
    parts.append('<rect x="40" y="138" width="1200" height="678" rx="14" fill="#1A365D"/>')
    parts.append(f'<text x="{X0}" y="186" font-family="Microsoft YaHei" font-size="24" font-weight="bold" '
                 f'fill="#FBD38D">翻到课本 §{num}，把下面这些【原句】用笔划出来（P 为课本页码）：</text>')
    y = 232
    for page, text in items:
        prefix = f"P{page}　"
        pfx_w = px(prefix, FS)
        # 统一用去掉前缀后的预算，避免首行切分怪异
        lines = wrap(text, FS, (XMAX - X0) - pfx_w)
        for i, ln in enumerate(lines):
            if i == 0:
                parts.append(
                    f'<text x="{X0}" y="{y}" font-family="Microsoft YaHei" font-size="{FS}" font-weight="bold" '
                    f'fill="#FFFFFF"><tspan fill="{PAGE_COLOR}">P{page}</tspan>　{esc(ln)}</text>')
            else:
                parts.append(
                    f'<text x="{X0 + pfx_w:.0f}" y="{y}" font-family="Microsoft YaHei" font-size="{FS}" '
                    f'font-weight="bold" fill="#E2E8F0">{esc(ln)}</text>')
            y += LH
        y += GAP
    # 底部 重点/难点/易错易混 带
    parts.append('<rect x="40" y="832" width="1200" height="100" rx="14" fill="#4A5568"/>')
    parts.append(f'<text x="{X0}" y="870" font-family="Microsoft YaHei" font-size="18" font-weight="bold" '
                 f'fill="#FFFFFF">【重点】{esc(zh)}　　【难点】{esc(nd)}</text>')
    parts.append(f'<text x="{X0}" y="906" font-family="Microsoft YaHei" font-size="18" font-weight="bold" '
                 f'fill="#FEB2B2">【易错易混】{esc(yc)}</text>')
    parts.append('</svg>')
    return "\n".join(parts)


def load_external(path):
    spec = importlib.util.spec_from_file_location("chapter_data", path)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    if not hasattr(mod, "SECTIONS"):
        raise SystemExit(f"数据文件 {path} 里没有定义 SECTIONS")
    return mod.SECTIONS


def main():
    ap = argparse.ArgumentParser(description="节首动笔划课本页 SVG 生成器")
    ap.add_argument("--data", help="外部数据 .py（须定义 SECTIONS）；不给则用本文件内置 SECTIONS")
    ap.add_argument("--out", help="输出目录；默认脚本所在目录")
    args = ap.parse_args()

    sections = load_external(args.data) if args.data else SECTIONS
    if not sections:
        raise SystemExit("SECTIONS 为空：请编辑本文件顶部，或用 --data 指定本章数据文件")

    out = args.out or (os.path.dirname(os.path.abspath(__file__)))
    os.makedirs(out, exist_ok=True)
    for sec in sections:
        svg = build(sec)
        path = os.path.join(out, f"{sec[0]}concepts.svg")
        with open(path, "w", encoding="utf-8") as f:
            f.write(svg)
        print("wrote", path)
    print("DONE", len(sections))


if __name__ == "__main__":
    main()
