TYPEFACENAME := ../../src/Sampradaya

%.ttf: %.sfd
	../../src/sfd2ttf.pe ${TYPEFACENAME}

build: ${TYPEFACENAME}.ttf

install: build
	install ${TYPEFACENAME}.ttf ~/Library/Fonts/

CXXFLAGS := -O3 -std=c++2a -Weverything -ferror-limit=1 -ftemplate-backtrace-limit=1 -Wno-missing-prototypes -Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-disabled-macro-expansion -Wno-global-constructors -Wno-exit-time-destructors -Wno-float-equal -Wno-poison-system-directories --system-header-prefix=QtGui/ --system-header-prefix=QtWidgets/ -F/usr/local/opt/qt/Frameworks
LDFLAGS := -ferror-limit=1
LDLIBS := -lstdc++ -F/usr/local/opt/qt/Frameworks -framework QtGui -framework QtCore -framework QtWidgets
OBJECTS	:= ./rasterise_text.o ./qt/rasterise_text_impl.o

rasterise_text.o: ../rasterise_text.cpp
	$(CXX) $(CXXFLAGS) -c ../rasterise_text.cpp

rasterise_text: ${OBJECTS}
	${CC} ${LDFLAGS} ${OBJECTS} ${LDLIBS} -o ./qt/$@

regenerate_masters: build ./qt/rasterise_text
	./generate_images ./qt/master_images ./qt/rasterise_text

test: build rasterise_text
	./run_tests ./qt/master_images ./qt/rasterise_text

clean:
	${RM} ./qt/rasterise_text ${OBJECTS}
	${RM} ${TYPEFACENAME}.ttf
	-find ${TMPDIR} -iname 'run_tests*' -type d 2>/dev/null | xargs ${RM} -r

.PHONY: build install regenerate_masters test clean
