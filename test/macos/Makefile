TYPEFACENAME := ../../src/Sampradaya

%.ttf: %.sfd
	../../src/sfd2ttf.pe ${TYPEFACENAME}

build: ${TYPEFACENAME}.ttf

install: build
	install ${TYPEFACENAME}.ttf ~/Library/Fonts/

CXXFLAGS := -O3 -std=c++2a -Weverything -ferror-limit=1 -ftemplate-backtrace-limit=1 -Wno-missing-prototypes -Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-disabled-macro-expansion -Wno-global-constructors -Wno-exit-time-destructors -Wno-float-equal -Wno-poison-system-directories
LDFLAGS := -ferror-limit=1
OBJECTS	:= rasterise_text.o qt/rasterise_text_impl.o
BINS := qt/rasterise_text

rasterise_text.o: ../rasterise_text.cpp ../rasterise_text.hpp
	$(CXX) $(CXXFLAGS) -c ../rasterise_text.cpp

qt/rasterise_text_impl.o: qt/rasterise_text_impl.cpp ../rasterise_text.hpp
	$(CXX) $(CXXFLAGS) --system-header-prefix=QtGui/ --system-header-prefix=QtWidgets/ -F/usr/local/opt/qt/Frameworks -c qt/rasterise_text_impl.cpp -o $@
qt/rasterise_text: ${OBJECTS}
	${CXX} ${LDFLAGS} ${OBJECTS} -F/usr/local/opt/qt/Frameworks -framework QtGui -framework QtCore -framework QtWidgets -o $@

regenerate_qt_masters: build qt/rasterise_text
	./generate_images qt/master_images qt/rasterise_text

test: build ${BINS}
	./run_tests qt

clean:
	${RM} ${OBJECTS} ${BINS}
	${RM} ${TYPEFACENAME}.ttf
	-find ${TMPDIR} -iname 'run_tests*' -type d 2>/dev/null | xargs ${RM} -r

.PHONY: build install regenerate_qt_masters test clean
