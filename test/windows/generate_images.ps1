$ErrorActionPreference="Stop"

if ($args.Count -lt 1) {
    throw "Required argument missing."
}

function MkDirIfNotExists() {
    Param([Parameter(Mandatory = $True)] [String] $DirectoryToCreate)
    if (Test-Path -LiteralPath $DirectoryToCreate) {
        return
    }
    New-Item -Path $DirectoryToCreate -ItemType Directory -ErrorAction Stop | Out-Null
}

$outputRoot = "$((Get-Location).Path)/$($args[0])"
MkDirIfNotExists $outputRoot

Get-ChildItem "$PSScriptRoot/../cases" -Name *txt | ForEach-Object {
    $dirName = $_ -replace '.txt', ''
    $outputDir = "$outputRoot/$dirName"
    MkDirIfNotExists $outputDir
    & "$PSScriptRoot/rasterise_text" "$PSScriptRoot/../cases/$_" $outputDir
}
