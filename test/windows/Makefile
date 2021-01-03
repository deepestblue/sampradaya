PS = powershell.exe -NoLogo -NoProfile -NonInteractive -ExecutionPolicy Bypass -Command

install:
	$(PS) ./install_typeface.ps1

CPPFLAGS = /utf-8 /EHsc /std:c++latest /nologo /experimental:external /external:anglebrackets /external:W0 /Wall
LINKFLAGS = /nologo d2d1.lib dwrite.lib
OBJECTS	= rasterise_text.obj rasterise_text_impl.obj

rasterise_text.obj: ../rasterise_text.cpp
	$(CPP) $(CPPFLAGS) /c ../rasterise_text.cpp

rasterise_text.exe: $(OBJECTS)
	$(CPP) $(LINKFLAGS) $(OBJECTS)

regenerate_masters: rasterise_text.exe
	$(PS) ./generate_images.ps1 master_images

test: rasterise_text.exe
	$(PS) ./run_tests.ps1 master_images

clean:
	$(PS) (Remove-Item -Force rasterise_text.exe, *.obj -ErrorAction Ignore)

.PHONY: install regenerate_masters test clean
