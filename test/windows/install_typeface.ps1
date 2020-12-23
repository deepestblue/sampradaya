$ErrorActionPreference="Stop"

$fontsFolder = (New-Object -ComObject Shell.Application).Namespace(0x14)
if ($fontsFolder.ParseName('Sampradaya Regular')) {
    # Install anyway
    # return
}

$sampradayaPath = (Get-Item '../../src/Sampradaya.ttf').fullname
$fontsFolder.CopyHere($sampradayaPath, 4 + 16)
