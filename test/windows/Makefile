PS = powershell.exe -NoLogo -NoProfile -NonInteractive -ExecutionPolicy Bypass -Command
CPPFLAGS = /utf-8 /EHsc /std:c++latest /nologo /experimental:external /external:anglebrackets /external:W0 /Wall d2d1.lib dwrite.lib

install:
	$(PS) ./install_typeface.ps1

regenerate_masters: rasterise_text.exe
	$(PS) ./generate_images.ps1 master_images

test: rasterise_text.exe
	$(PS) ./run_tests.ps1 master_images

clean:
	$(PS) (Remove-Item -Force rasterise_text.exe, rasterise_text.obj, rasterise_text.pdb, ../../src/Sampradaya.ttf -ErrorAction Ignore)

.PHONY: install regenerate_masters test clean
