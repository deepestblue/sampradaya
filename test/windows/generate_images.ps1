cl "$PSScriptRoot/rasterise_text.cc" /EHsc /std:c++latest /nologo /W4 d2d1.lib dwrite.lib /Fo: "$PSScriptRoot/" /Fe: "$PSScriptRoot/" | Out-Null

function MkDirIfNotExists() {
    Param([Parameter(Mandatory = $True)] [String] $DirectoryToCreate)
    if (Test-Path -LiteralPath $DirectoryToCreate) {
        return
    }
    New-Item -Path $DirectoryToCreate -ItemType Directory -ErrorAction Stop | Out-Null
}

if ($args.Count -lt 1) {
    throw "Required argument missing"
}

$outputRoot = "$((Get-Location).Path)/$($args[0])"
MkDirIfNotExists $outputRoot

Get-ChildItem "$PSScriptRoot/../cases" -Name *txt | ForEach-Object {
    $dirName = $_ -replace '.txt', ''
    $outputDir = "$outputRoot/$dirName"
    MkDirIfNotExists $outputDir
    & "$PSScriptRoot/rasterise_text" "$PSScriptRoot/../cases/$_" $outputDir
}
