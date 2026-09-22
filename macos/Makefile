TYPEFACENAME := ../src/Sampradaya.ttf
BINDIR := vendor/alekhana

NAME_TA := சம்பிரதாயம்
NAME_SA := 𑌸𑌮𑍍𑌪𑍍𑌰𑌦𑌾𑌯𑌃

%.ttf: %.sfd
	../src/build.sh -f $@ -t ${NAME_TA} -s ${NAME_SA}

build: ${TYPEFACENAME}

install: build
	install -C -m 644 ${TYPEFACENAME} ~/Library/Fonts/

fetch-alekhana:
	./fetch_alekhana.sh

regenerate_qt_masters: build fetch-alekhana
	$(BINDIR)/generate_images -d ../masters/qt -r $(BINDIR)/rasterise_text_qt -t ${TYPEFACENAME} -c ../cases

regenerate_coretext_masters: build fetch-alekhana
	$(BINDIR)/generate_images -d ../masters/coretext -r $(BINDIR)/rasterise_text_coretext -t ${TYPEFACENAME} -c ../cases

regenerate_masters: regenerate_qt_masters regenerate_coretext_masters

test: build fetch-alekhana
	$(BINDIR)/run_tests -e ../masters/coretext -r $(BINDIR)/rasterise_text_coretext -t ${TYPEFACENAME} -c ../cases
	$(BINDIR)/run_tests -e ../masters/qt -r $(BINDIR)/rasterise_text_qt -t ${TYPEFACENAME} -c ../cases

fontbakery: build
	fontbakery check-universal --loglevel WARN ${TYPEFACENAME} --exclude-checkid nested_components --exclude-checkid unreachable-glyphs --exclude-checkid mark-chars

clean:
	${RM} ${TYPEFACENAME}
	-find ${TMPDIR} -iname 'run_tests*' -type d 2>/dev/null | xargs ${RM} -r

.PHONY: build install fetch-alekhana regenerate_masters regenerate_qt_masters regenerate_coretext_masters test fontbakery clean
