PS = powershell.exe -NoLogo -NoProfile -NonInteractive -ExecutionPolicy Bypass -Command

regenerate_masters:
	$(PS) ./generate_images.ps1 master_images

build:

install: build
	$(PS) '$$fonts = (New-Object -ComObject Shell.Application).Namespace(0x14); $$fonts.CopyHere("../../src/windows/Sampradaya.ttf")'

test: rasterise_text.exe
	$(PS) ./run_tests.ps1

.cc.obj:
  $(cc) $(cdebug) $(cflags) $(cvars) $*.c

rasterise_text.exe: rasterise_text.cc
	$testBuildOutput = cl "$PSScriptRoot/rasterise_text.cpp" /EHsc /std:c++latest /nologo /W4 d2d1.lib dwrite.lib /Fo: "$PSScriptRoot/" /Fe: "$PSScriptRoot/" | Out-String
	if (! $?) {
		Write-Output $testBuildOutput
		throw "Test build exited with: $lastexitcode."
	}

clean:
	Remove-Item -Force rasterise_text.exe rasterise_text.obj

.PHONY: build install test regenerate_masters clean
