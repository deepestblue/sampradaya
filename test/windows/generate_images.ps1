Param(
    [Parameter(Mandatory=$true)][string]$outputRoot
)

$ErrorActionPreference="Stop"

function MkDirIfNotExists() {
    Param([Parameter(Mandatory = $True)] [String] $DirectoryToCreate)
    if (Test-Path -LiteralPath $DirectoryToCreate) {
        return
    }
    New-Item -Path $DirectoryToCreate -ItemType Directory -ErrorAction Stop | Out-Null
}

MkDirIfNotExists $outputRoot

Get-ChildItem "$PSScriptRoot/../cases" -Name *txt | ForEach-Object {
    $dirName = $_ -replace '.txt', ''
    $outputDir = "$outputRoot/$dirName"
    MkDirIfNotExists $outputDir
    & "$PSScriptRoot/rasterise_text" "$PSScriptRoot/../cases/$_" $outputDir
}
