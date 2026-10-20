#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
跨页泄漏扫描：检查口播稿里有没有把"下一页才该讲的结论词"提前说出来（一页只讲一页）。

规则出处：lecture-script.md ④b（衔接句硬约束）——
    判据：往前抢答案＝违规，往前留问题＝过渡，往后引旧知识＝验证。
    自检：拿下一页标题里的关键词回搜本页，非本页标题自带的就删。

用法：
    python check_no_leak.py [第N章_口播稿.md] [--depth N] [--allow 词,词] [--json]
    不带路径时自动在当前目录找 *口播稿*.md。
    --depth 2 同时检查“下两页”（默认 1，只看紧挨着的下一页）。
    稿首“> **主线**：…”与“> **泄漏白名单**：…”两行里的词自动列入允许名单。

分级输出（对应人设的三分法）：
    🔴 陈述句里说出下页结论词 = 抢答案（违规，需删或改成不含结论词的问句）
    🟡 问句里提到下页结论词   = 留问不留答的过渡（通常可保留，供人一眼确认）
    往后引旧知识（回指已讲页）不列入扫描范围（只看向前几页的标题）。

页头格式（与 sync_notes.py 一致）：
    ### 🗂 第 N 张 · <标题>【约 X 分钟】

判定为"总览页"的（本页天然要提前点名后续章节/站点）跳过检查：
    标题含 划概念页 / 地图 / 主线 / 升级链 / 总结 / 收束 / 参考 / 目录

退出码：0 = 无🔴抢答案（可能还有🟡留问提示）；1 = 有🔴命中（只供人判，不自动改稿）。
"""
import argparse
import glob
import os
import re
import sys

# 通用词：出现在任何页标题里都不算"抢答案"
STOP = {
    "约", "分钟", "第", "张", "站", "页", "上", "下", "一", "二", "三", "四", "五",
    "结构", "全章", "本章", "本节", "收", "收官", "压轴", "带读", "重做", "热身",
    "必考", "核心", "最硬", "最值钱", "动笔", "圈课本", "课本", "课件", "补充",
}
# 本页标题自带的词永远允许
OVERVIEW_HINT = ("划概念页", "地图", "主线", "升级链", "总结", "收束", "参考", "目录")

SPLIT_RE = re.compile(r"[·：:、，,。/→\-—「」『』“”\"'（）()\[\]\s之的]+")
HEAD_RE = re.compile(r"^#+\s*.*?第\s*(\d+)\s*张\s*·\s*(.+?)(?:【[^】]*】)?\s*$")


def title_tokens(title: str):
    """标题 -> 关键词集合（长度>=2，去通用词）"""
    out = []
    for tok in SPLIT_RE.split(title):
        tok = tok.strip()
        if len(tok) < 2 or tok in STOP:
            continue
        if re.fullmatch(r"[\d\s.]+", tok):
            continue
        out.append(tok)
    return out


def main_line_allowlist(text: str):
    """稿首引言区（第一个页头之前）里的允许词：
    ① “> **主线**：…” 行（贯穿全章的题目如“求 5!”不算泄密）；
    ② “> **泄漏白名单**：词、词” 行（本章定名、板书总结词等合法命中）。
    注：白名单写在稿首，不在任何一页正文里，因此不会被 sync_notes.py 写进演讲者备注。"""
    allow = set()
    for line in text.splitlines():
        if line.startswith("### "):
            break
        s = line.strip()
        if not s.startswith(">"):
            continue
        if "主线" in s:
            for tok in re.findall(r"[0-9A-Za-z\u4e00-\u9fff]*![0-9A-Za-z]*", s):
                if len(tok) >= 2:
                    allow.add(tok)
            for tok in re.findall(r"[“\"']([^”\"']{2,12})[”\"']", s):
                allow.add(tok)
        if "白名单" in s:
            seg = s.split("：", 1)[-1]
            for junk in ("——", "—", "～", "~"):
                seg = seg.split(junk)[0]      # 只取注释句之前的词表
            for tok in re.split(r"[、，,;；\s]+", seg):
                tok = tok.strip("* >“”\"")
                # 只收干净词（中文/字母/数字/!），不把解说句当允许词
                if len(tok) >= 2 and re.fullmatch(r"[0-9A-Za-z\u4e00-\u9fff!]+", tok):
                    allow.add(tok)
    return allow


def sentence_of(body: str, tok: str):
    """返回包含 tok 的那一句（含结尾标点）"""
    flat = body.replace("\n", " ")
    for part in re.split(r"(?<=[。？?！!；;])", flat):
        if tok in part:
            return part.strip()
    return flat.strip()


def parse_pages(text: str):
    """返回 [(page_no, title, body_lines)]，按稿中顺序"""
    pages = []
    cur = None
    for line in text.splitlines():
        m = HEAD_RE.match(line.strip())
        if m:
            if cur:
                pages.append(cur)
            cur = [int(m.group(1)), m.group(2).strip(), []]
            continue
        if cur is not None:
            if line.strip().startswith("## ") or line.strip() == "---":
                pages.append(cur)
                cur = None
                continue
            cur[2].append(line)
    if cur:
        pages.append(cur)
    return [(n, t, "\n".join(b)) for n, t, b in pages]


def main():
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(encoding="utf-8")

    ap = argparse.ArgumentParser()
    ap.add_argument("script", nargs="?")
    ap.add_argument("--depth", type=int, default=1, help="往后检查几页的标题（默认 1）")
    ap.add_argument("--json", action="store_true")
    ap.add_argument("--allow", default="", help="额外允许词，逗号分隔（本页本来就该讲的术语）")
    args = ap.parse_args()

    path = args.script
    if not path:
        cands = sorted(glob.glob("*口播稿*.md"))
        if not cands:
            print("[错误] 当前目录没找到 *口播稿*.md")
            return 2
        path = cands[0]
    if not os.path.isfile(path):
        print(f"[错误] 文件不存在：{path}")
        return 2

    raw = open(path, encoding="utf-8").read()
    pages = parse_pages(raw)
    allow = main_line_allowlist(raw) | {w.strip() for w in args.allow.split(",") if w.strip()}
    if not pages:
        print(f"[错误] {path} 里没解析到「### 🗂 第 N 张 · 标题」页头")
        return 2

    findings = []
    for idx, (no, title, body) in enumerate(pages):
        if any(h in title for h in OVERVIEW_HINT):
            continue
        own = set(title_tokens(title))
        for ahead in range(1, args.depth + 1):
            if idx + ahead >= len(pages):
                break
            nxt_no, nxt_title, _ = pages[idx + ahead]
            for tok in title_tokens(nxt_title):
                if tok in own or tok in allow:
                    continue
                if tok in body:
                    sent = sentence_of(body, tok)
                    findings.append({
                        "page": no, "page_title": title,
                        "leak": tok, "from_next": nxt_no,
                        "next_title": nxt_title,
                        "sentence": sent,
                        "kind": "ask" if sent.rstrip().endswith(("？", "?")) else "tell",
                    })

    if args.json:
        import json
        print(json.dumps(findings, ensure_ascii=False, indent=2))
        return 1 if findings else 0

    print(f"[扫描] {path}　共 {len(pages)} 页　向后检查 {args.depth} 页标题"
          + (f"　允许词：{'、'.join(sorted(allow))}" if allow else ""))
    tells = [f for f in findings if f["kind"] == "tell"]
    asks = [f for f in findings if f["kind"] == "ask"]
    
    if tells:
        print(f"\n  🔴 抢答案（陈述句里说出下页结论词）{len(tells)} 处 —— 必须删或改成不含结论词的问句：")
        for f in tells:
            print(f"\n  第 {f['page']} 张 · {f['page_title']}")
            print(f"    「{f['leak']}」本该到第 {f['from_next']} 张（{f['next_title']}）才讲")
            print(f"    本页却写：{f['sentence']}")
    if asks:
        print(f"\n  🟡 留问过渡（只在下页结论词上打了问号）{len(asks)} 处 —— 通常合规，确认没顺手把答案说出来就行：")
        for f in asks:
            print(f"    第 {f['page']} 张：「{f['leak']}」→ {f['sentence'][:70]}")
    if not findings:
        print("  ✅ 0 命中：没有任何一页提前说出下一页标题里的结论词。")
        return 0
    print("\n  判定口径（lecture-script.md ④b）：往前抢答案＝违规；往前留问题＝过渡；往后引旧知识＝验证。")
    print("  注：转折页/板书记录页（如“立规矩”页本身就要点名三种结构）属合法，可用 --allow 加入允许词。")
    return 1 if tells else 0


if __name__ == "__main__":
    sys.exit(main())
