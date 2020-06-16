using namespace System.Drawing.Imaging
using namespace System.IO
using namespace Microsoft.Test.VisualVerification

Param(
    [Parameter(Mandatory=$true)][string]$master_images
)

$ErrorActionPreference="Stop"

Add-Type -Assembly System.Drawing
[Reflection.Assembly]::LoadFile("$PSScriptRoot/TestApiCore.dll") | Out-Null
$snapshotVerifier = New-Object -TypeName SnapshotColorVerifier

function MkDirIfNotExists() {
    Param([Parameter(Mandatory = $True)] [String] $DirectoryToCreate)
    if (Test-Path -LiteralPath $DirectoryToCreate) {
        return
    }
    New-Item -Path $DirectoryToCreate -ItemType Directory -ErrorAction Stop | Out-Null
}

function New-TemporaryDirectory {
    $parent = [Path]::GetTempPath()
    $name = [Path]::GetRandomFileName()
    New-Item -ItemType Directory -Path (Join-Path $parent $name)
}

function Compare-Images {
Param(
    [Parameter(Mandatory=$true)][string]$expectedPath,
    [Parameter(Mandatory=$true)][string]$actualPath,
    [Parameter(Mandatory=$true)][string]$diffPath
)
    $expected = [Snapshot]::FromFile($expectedPath);
    $actual = [Snapshot]::FromFile($actualPath);

    $diff = $actual.CompareTo($expected);

    if ($snapshotVerifier.Verify($diff) -eq $([VerificationResult]::Pass)) {
        return
    }

    MkDirIfNotExists (Split-Path -Path $diffPath)
    $diff.ToFile($diffPath, $([ImageFormat]::Bmp));
}

$tmpDir = New-TemporaryDirectory

robocopy $master_images "$tmpDir/expected" /MIR /Z /UNICODE /NFL /NDL /NP /NJH /NJS /NS /NC

MkDirIfNotExists "$tmpDir/actual"
./generate_images.ps1 "$tmpDir/actual"

MkDirIfNotExists "$tmpDir/diff"

Get-ChildItem -Path "$tmpDir/actual" -Recurse -File | ForEach-Object {
    $actual = $_.FullName
    $tmpPath = $tmpDir.FullName
    $expected = $actual.replace($tmpPath + '\actual\', $tmpPath + '\expected\')
    $diff = $actual.replace($tmpPath + '\actual\', $tmpPath + '\diff\')

    if (! (Compare-Object $(Get-Content $expected) $(Get-Content $actual))) {
        # Equal
        return
    }

    echo "Actual: ", $actual
    echo "Expected: ", $expected
    echo "Diff: ", $diff

    Compare-Images $expected $actual $diff
}

Get-ChildItem "$tmpDir/diff" -Recurse | ForEach-Object {
    Write-Output $_.FullName
}
