TYPEFACENAME := ../../../src/Sampradaya

%.ttf: %.sfd
	../../../src/sfd2ttf.pe ${TYPEFACENAME}

build: ${TYPEFACENAME}.ttf

install: build
	install ${TYPEFACENAME}.ttf ~/Library/Fonts/

CXXFLAGS := -std=c++2a -Weverything -ferror-limit=1 -ftemplate-backtrace-limit=1 -Wno-missing-prototypes -Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-disabled-macro-expansion -Wno-global-constructors -Wno-exit-time-destructors -Wno-float-equal -Wno-poison-system-directories
LDFLAGS := -ferror-limit=1
LDLIBS := -lstdc++
OBJECTS	:= ../../rasterise_text.o rasterise_text_impl.o

rasterise_text: ${OBJECTS}
	${CC} ${LDFLAGS} ${OBJECTS} ${LDLIBS} -o $@

regenerate_masters: build rasterise_text
	../generate_images master_images ./rasterise_text

test: build rasterise_text
	../run_tests master_images ./rasterise_text

clean:
	${RM} rasterise_text ${OBJECTS}
	${RM} ${TYPEFACENAME}.ttf
	-find ${TMPDIR} -iname 'run_tests*' -type d 2>/dev/null | xargs ${RM} -r

.PHONY: build install regenerate_masters test clean
