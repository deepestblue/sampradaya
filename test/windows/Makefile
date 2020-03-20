PS = powershell.exe -NoLogo -NoProfile -NonInteractive -ExecutionPolicy Bypass -Command

regenerate_masters:
	$(PS) ./generate_images.ps1 master_images

build:

clean:
	$(PS) Remove-Item -Force rasterise_text.exe, rasterise_text.obj

install: build
	$(PS) ./install_typeface.ps1

test: rasterise_text.exe
#	$(PS) ./run_tests.ps1

#.cc.obj:
#  $(cc) $(cdebug) $(cflags) $(cvars) $*.c

rasterise_text.exe: rasterise_text.cc
#	$testBuildOutput = cl "$PSScriptRoot/rasterise_text.cpp" /utf-8 /EHsc /std:c++latest /nologo /experimental:external /external:anglebrackets /external:W0 /Wall d2d1.lib dwrite.lib /Fo: "$PSScriptRoot/" /Fe: "$PSScriptRoot/" | Out-String
#	if (! $?) {
#		Write-Output $testBuildOutput
#		throw "Test build exited with: $lastexitcode."
#	}

.PHONY: build install test regenerate_masters clean
