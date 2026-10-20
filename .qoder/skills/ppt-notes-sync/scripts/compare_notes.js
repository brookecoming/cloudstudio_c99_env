// 通用「PPT 演讲者备注 vs 讲稿 md」比对器
// 用法: node compare_notes.js <dump.txt> <稿.md> <out.txt> [oral|prompter]
//   oral     : 口播稿格式（每张 = "### 🗂 第 N 张 · 标题" + 其下整段正文）
//   prompter : 提词本格式（每张只取 **【口播】** 引号块）
// 说明: 归一化后(去 **加粗/`代码`/空白/```围栏)按公共前缀-公共后缀剥离，
//       打印每张的差异中段；PPT 无备注的尾页会标 "only in PPT"。
const fs = require('fs');
const [, , dumpPath, mdPath, outPath, mode = 'oral'] = process.argv;
if (!dumpPath || !mdPath || !outPath) {
  console.error('用法: node compare_notes.js <dump.txt> <稿.md> <out.txt> [oral|prompter]');
  process.exit(2);
}

function norm(s) {
  if (s == null) return '';
  return s.replace(/```[a-zA-Z]*\s*/g, '')
    .replace(/`/g, '').replace(/\*\*/g, '')
    // 折叠 PowerPoint 自动更正产生的弯引号→直引号（纯字形噪声，非内容差异）
    .replace(/[\u2018\u2019]/g, "'").replace(/[\u201c\u201d]/g, '"')
    .replace(/\s+/g, '').trim();
}
const stripTitle = t => (t || '').replace(/^▶?\s*第\s*\d+\s*张\s*[·・]\s*/, '').trim();

// ---- 解析 dump（按放映顺序编号）----
const dumpRaw = fs.readFileSync(dumpPath, 'utf8');
const parts = dumpRaw.split(/^========== SLIDE (\d+) ==========$/m);
const dump = {};
for (let i = 1; i < parts.length; i += 2) {
  const num = parseInt(parts[i]);
  const lines = parts[i + 1].split(/\r?\n/).map(l => l.trim())
    .filter(l => l && l !== '1.7.2013' && l !== '\u2039#\u203a' && l !== '(无备注)' && l !== '(备注页缺失)');
  let title = '', content = [];
  for (const l of lines) {
    if (!title && /^\u25b6\s*第\s*\d+\s*张/.test(l)) { title = l.replace(/^\u25b6\s*/, ''); continue; }
    if (title) content.push(l);
  }
  dump[num] = { title, content: content.join('\n'), hasNote: lines.length > 0 };
}

// ---- 解析讲稿 md ----
const mdLines = fs.readFileSync(mdPath, 'utf8').split(/\r?\n/);
const md = {};
let cur = null, buf = [], capturing = (mode !== 'prompter');
const flush = () => { if (cur !== null && buf.length) { md[cur] = md[cur] || {}; md[cur].content = buf.join('\n').trim(); } buf = []; };

for (const line of mdLines) {
  const h = line.match(/^###.*?第\s*(\d+)\s*张\s*[·・]?\s*(.*)$/);
  if (h) {
    flush();
    cur = parseInt(h[1]);
    md[cur] = { title: (h[2] || '').trim(), content: '' };
    capturing = (mode !== 'prompter');
    continue;
  }
  if (cur === null) continue;
  // oral: 段落级终止符（一级/二级标题、分隔线、引用块）
  if (mode === 'oral' && (/^---\s*$/.test(line) || /^##[^#]/.test(line) || /^#\s/.test(line) || /^>\s/.test(line))) {
    flush(); cur = null; continue;
  }
  if (mode === 'prompter') {
    if (/【口播】/.test(line)) { capturing = true; continue; }
    if (/^---\s*$/.test(line)) { flush(); capturing = false; continue; }
    if (!capturing) continue;
  }
  buf.push(line);
}
flush();

// ---- 逐张比对 ----
const out = [];
const log = s => out.push(s);
const nums = Array.from(new Set([...Object.keys(dump).map(Number), ...Object.keys(md).map(Number)])).sort((a, b) => a - b);
let diffs = 0;

function region(a, b) {
  let i = 0; while (i < a.length && i < b.length && a[i] === b[i]) i++;
  let j = 0; while (j < a.length - i && j < b.length - i && a[a.length - 1 - j] === b[b.length - 1 - j]) j++;
  return { pre: a.slice(Math.max(0, i - 20), i), aMid: a.slice(i, a.length - j), bMid: b.slice(i, b.length - j), post: a.slice(a.length - j, a.length - j + 20) };
}

for (const n of nums) {
  const d = dump[n], m = md[n];
  if (!d) { log(`\n[第${n}张] 仅讲稿有（PPT 无此编号）`); diffs++; continue; }
  if (!d.hasNote) continue;                       // PPT 此页无备注，跳过（非差异）
  if (!m) { log(`\n[第${n}张] 仅 PPT 有备注，讲稿缺此张: "${stripTitle(d.title)}"`); diffs++; continue; }
  const dt = norm(stripTitle(d.title)), mt = norm(stripTitle(m.title));
  if (dt !== mt) { log(`\n[第${n}张] 标题差异:\n  PPT: ${stripTitle(d.title)}\n  讲稿: ${stripTitle(m.title)}`); diffs++; }
  const dc = norm(d.content), mc = norm(m.content);
  if (dc !== mc) {
    const r = region(dc, mc);
    log(`\n[第${n}张] 正文差异:`);
    log(`  …${r.pre}⟬PPT: ${r.aMid}⟭⟬讲稿: ${r.bMid}⟭${r.post}…`);
    diffs++;
  }
}
log(`\n==== 共比对 ${nums.length} 张，差异 ${diffs} 处（不含 PPT 无备注页）====`);
fs.writeFileSync(outPath, out.join('\n'), 'utf8');
console.log(`done -> ${outPath}`);
