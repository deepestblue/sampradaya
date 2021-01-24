TYPEFACENAME := ../../src/Sampradaya

%.ttf: %.sfd
	../../src/sfd2ttf.pe ${TYPEFACENAME}

build: ${TYPEFACENAME}.ttf

install: build
	install -C -m 644 ${TYPEFACENAME}.ttf ~/Library/Fonts/

CXXFLAGS := -O3 -std=c++2a -Weverything -ferror-limit=1 -ftemplate-backtrace-limit=1 -Wno-missing-prototypes -Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-disabled-macro-expansion -Wno-global-constructors -Wno-exit-time-destructors -Wno-float-equal -Wno-poison-system-directories --system-header-prefix=boost/
LDFLAGS := -ferror-limit=1
QTOBJECTS := rasterise_text.o qt/rasterise_text_impl.o
CORETEXTOBJECTS := rasterise_text.o coretext/rasterise_text_impl.o
BINS := qt/rasterise_text coretext/rasterise_text

rasterise_text.o: ../rasterise_text.cpp ../rasterise_text.hpp
	$(CXX) $(CXXFLAGS) -c ../rasterise_text.cpp

qt/rasterise_text_impl.o: qt/rasterise_text_impl.cpp ../rasterise_text.hpp
	$(CXX) $(CXXFLAGS) --system-header-prefix=QtGui/ --system-header-prefix=QtWidgets/ -F/usr/local/opt/qt/Frameworks -c qt/rasterise_text_impl.cpp -o $@
qt/rasterise_text: ${QTOBJECTS}
	${CXX} ${LDFLAGS} ${QTOBJECTS} -F/usr/local/opt/qt/Frameworks -framework QtGui -framework QtCore -framework QtWidgets -o $@

coretext/rasterise_text: ${CORETEXTOBJECTS}
	${CXX} ${LDFLAGS} ${CORETEXTOBJECTS} -framework ImageIO -framework CoreGraphics -framework CoreText -framework CoreServices -framework CoreFoundation -o $@

regenerate_qt_masters: build qt/rasterise_text
	./generate_images qt/master_images qt/rasterise_text

regenerate_coretext_masters: build coretext/rasterise_text
	./generate_images coretext/master_images coretext/rasterise_text

regenerate_masters: regenerate_qt_masters regenerate_coretext_masters

test_build: ${BINS}

test: build test_build
	./run_tests qt
	./run_tests coretext

clean:
	${RM} ${QTOBJECTS} ${CORETEXTOBJECTS} ${BINS}
	${RM} ${TYPEFACENAME}.ttf
	-find ${TMPDIR} -iname 'run_tests*' -type d 2>/dev/null | xargs ${RM} -r

.PHONY: build install regenerate_qt_masters test_build test clean
