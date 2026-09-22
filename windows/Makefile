PS = powershell.exe -NoLogo -NoProfile -NonInteractive -ExecutionPolicy Bypass -Command
TYPEFACEPATH = '../src/Sampradaya.ttf'
BINDIR = ../vendor/alekhana

install:
	$(PS) ./install_typeface.ps1 -Path $(TYPEFACEPATH) -Name 'Sampradaya Regular'

fetch-alekhana:
	$(PS) ./fetch_alekhana.ps1

regenerate_masters: fetch-alekhana
	$(PS) $(BINDIR)/generate_images.ps1 -Rasteriser $(BINDIR)/rasterise_text_windows.exe -OutputRoot ../masters/windows -TypeFace $(TYPEFACEPATH) -TestCases ../cases

test: fetch-alekhana
	$(PS) $(BINDIR)/run_tests.ps1 -TypeFace $(TYPEFACEPATH) -Rasteriser $(BINDIR)/rasterise_text_windows.exe -MasterImages ../masters/windows -TestCases ../cases

.PHONY: install fetch-alekhana regenerate_masters test
