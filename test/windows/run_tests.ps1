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

function New-TemporaryDirectory {
    $parent = [System.IO.Path]::GetTempPath()
    $name = [System.IO.Path]::GetRandomFileName()
    New-Item -ItemType Directory -Path (Join-Path $parent $name)
}

$tmpDir = New-TemporaryDirectory

robocopy $($args[0]) "$tmpDir/expected" /MIR /Z /UNICODE /NFL /NDL /NP /NJH /NJS /NS /NC

MkDirIfNotExists "$tmpDir/actual"
./generate_images.ps1 "$tmpDir/actual"

MkDirIfNotExists "$tmpDir/diff"

Get-ChildItem -Path "$tmpDir/actual" -Recurse -File | ForEach-Object {
    $actual = $_.FullName

}

#Get-ChildItem "$tmpDir/diff"

echo $tmpDir
