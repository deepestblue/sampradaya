[Reflection.Assembly]::LoadFile("Z:\projects\Sampradaya\test\windows\TestApiCore.dll")

$master = [Microsoft.Test.VisualVerification.Snapshot]::FromFile("Z:/projects/Sampradaya/test/windows/master_images/character_set/1.bmp");

