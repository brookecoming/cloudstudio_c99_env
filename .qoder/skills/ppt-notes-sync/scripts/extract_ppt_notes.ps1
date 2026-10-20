# 提取 PPTX 每页演讲者备注，按【放映顺序】导出为纯文本
# 用法:
#   powershell -ExecutionPolicy Bypass -File extract_ppt_notes.ps1 `
#     -PptxPath "C:\path\to\第N章 xxx.pptx" -OutPath "C:\path\to\_ppt_notes_dump.txt"
# 说明:
#   - 通过 presentation.xml 的 sldIdLst + 各 slide 的 rels 精确定位备注页, 保证编号=放映顺序
#   - 过滤页码字段(a:fld)与日期占位噪声(如 1.7.2013), 只保留真实备注正文

param(
    [Parameter(Mandatory = $true)][string]$PptxPath,
    [Parameter(Mandatory = $true)][string]$OutPath
)

Add-Type -AssemblyName System.IO.Compression.FileSystem

if (-not (Test-Path -LiteralPath $PptxPath)) {
    Write-Error "找不到 PPTX: $PptxPath"
    exit 1
}

$zip = [System.IO.Compression.ZipFile]::OpenRead($PptxPath)

function Get-EntryText($zip, $name) {
    $entry = $zip.Entries | Where-Object { $_.FullName -eq $name }
    if (-not $entry) { return $null }
    $reader = New-Object System.IO.StreamReader($entry.Open(), [System.Text.Encoding]::UTF8)
    $txt = $reader.ReadToEnd()
    $reader.Close()
    return $txt
}

# ---- 1. 从 presentation.xml 读取放映顺序(rId 序列) ----
$presXml = Get-EntryText $zip "ppt/presentation.xml"
$presRels = Get-EntryText $zip "ppt/_rels/presentation.xml.rels"
if (-not $presXml -or -not $presRels) {
    $zip.Dispose()
    Write-Error "缺少 presentation.xml 或其 rels, 无法确定放映顺序"
    exit 1
}

# rId -> slideN.xml 映射
$ridToSlide = @{}
foreach ($m in [regex]::Matches($presRels, '<Relationship[^>]*Id="([^"]+)"[^>]*Target="([^"]*slides/slide\d+\.xml)"[^>]*/>')) {
    $ridToSlide[$m.Groups[1].Value] = ("ppt/" + ($m.Groups[2].Value -replace '^/?', '' -replace '^/ppt/', ''))
}

# sldIdLst 中 r:id 的出现顺序 = 放映顺序
[xml]$pdoc = $presXml
$orderedSlidePaths = @()
$ids = $pdoc.GetElementsByTagName("p:sldId")
foreach ($id in $ids) {
    $rid = $id.GetAttribute("r:id", "http://schemas.openxmlformats.org/officeDocument/2006/relationships")
    if ($rid -and $ridToSlide.ContainsKey($rid)) { $orderedSlidePaths += $ridToSlide[$rid] }
}
# 兜底: 若上面为空, 用文件名数字排序
if ($orderedSlidePaths.Count -eq 0) {
    $orderedSlidePaths = $zip.Entries |
        Where-Object { $_.FullName -match '^ppt/slides/slide\d+\.xml$' } |
        Sort-Object { [int]([regex]::Match($_.FullName, 'slide(\d+)\.xml').Groups[1].Value) } |
        ForEach-Object { $_.FullName }
}

$sb = New-Object System.Text.StringBuilder

for ($i = 0; $i -lt $orderedSlidePaths.Count; $i++) {
    $slidePath = $orderedSlidePaths[$i]
    $dispNum = $i + 1
    [void]$sb.AppendLine("========== SLIDE $dispNum ==========")

    # slide 的 rels -> notesSlide
    $slideBase = [System.IO.Path]::GetFileName($slidePath)
    $relsPath = "ppt/slides/_rels/$slideBase.rels"
    $rels = Get-EntryText $zip $relsPath
    $notesFile = $null
    if ($rels) {
        $m = [regex]::Match($rels, 'Target="[^"]*?(notesSlides/notesSlide\d+\.xml)"')
        if ($m.Success) { $notesFile = "ppt/" + $m.Groups[1].Value }
    }

    if (-not $notesFile) { [void]$sb.AppendLine("(无备注)"); [void]$sb.AppendLine(""); continue }

    $notesXml = Get-EntryText $zip $notesFile
    if (-not $notesXml) { [void]$sb.AppendLine("(备注页缺失)"); [void]$sb.AppendLine(""); continue }

    [xml]$doc = $notesXml
    $ns = New-Object System.Xml.XmlNamespaceManager($doc.NameTable)
    $ns.AddNamespace("a", "http://schemas.openxmlformats.org/drawingml/2006/main") | Out-Null

    foreach ($p in $doc.SelectNodes("//a:p", $ns)) {
        $texts = $p.SelectNodes(".//a:t[not(ancestor::a:fld)]", $ns)
        $line = ($texts | ForEach-Object { $_.InnerText }) -join ""
        if ($line.Trim() -eq "1.7.2013") { continue }   # 日期占位噪声
        [void]$sb.AppendLine($line)
    }
    [void]$sb.AppendLine("")
}

$zip.Dispose()

[System.IO.File]::WriteAllText($OutPath, $sb.ToString(), (New-Object System.Text.UTF8Encoding($false)))
Write-Host "OK: 共 $($orderedSlidePaths.Count) 张 -> $OutPath"
