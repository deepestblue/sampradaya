Param(
    [Parameter(Mandatory=$true)][string]$master_images
)

$ErrorActionPreference="Stop"

Add-Type -Assembly System.Drawing
[Reflection.Assembly]::LoadFile("Z:\projects\Sampradaya\test\windows\TestApiCore.dll")
$snapshotVerifier = New-Object -TypeName Microsoft.Test.VisualVerification.SnapshotColorVerifier

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

function Compare-Images {
Param(
    [Parameter(Mandatory=$true)][string]$expectedPath,
    [Parameter(Mandatory=$true)][string]$actualPath,
    [Parameter(Mandatory=$true)][string]$diffPath
)
    $expected = [Microsoft.Test.VisualVerification.Snapshot]::FromFile($expectedPath);
    $actual = [Microsoft.Test.VisualVerification.Snapshot]::FromFile($actualPath);

    $diff = $actual.CompareTo($expected);

    if ($snapshotVerifier.Verify($diff) -eq $([Microsoft.Test.VisualVerification.VerificationResult]::Success)) {
        return
    }

    MkDirIfNotExists (Split-Path -Path $diffPath)
    $diff.ToFile($diffPath, $([System.Drawing.Imaging.ImageFormat]::Bmp));
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
#        Compare-Images $expected $actual $diff
        return
    }

    Compare-Images $expected $actual $diff
}

Get-ChildItem "$tmpDir/diff" -Recurse | ForEach-Object {
    Write-Output $_.FullName
}
