CXXFLAGS = -std=c++2a -Weverything -ferror-limit=1 -ftemplate-backtrace-limit=1 -Wno-missing-prototypes -Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-disabled-macro-expansion -Wno-global-constructors -Wno-exit-time-destructors -Wno-float-equal -Wno-poison-system-directories -F/usr/local/opt/qt/Frameworks --system-header-prefix=QtGui/ --system-header-prefix=QtWidgets/ --system-header-prefix=boost/
LDFLAGS = -ferror-limit=1 -framework QtGui -framework QtCore -framework QtWidgets
TYPEFACENAME = ../../src/Sampradaya

%.ttf: %.sfd
	../../src/sfd2ttf.pe ${TYPEFACENAME}

build: ${TYPEFACENAME}.ttf

install: build
	install ${TYPEFACENAME}.ttf ~/Library/Fonts/

regenerate_masters: build rasterise_text
	./generate_images master_images

test: build rasterise_text
	./run_tests master_images

clean:
	-rm -f rasterise_text
	-rm -f ${TYPEFACENAME}.ttf
	-find ${TMPDIR} -iname 'run_tests*' -type d 2>/dev/null | xargs rm -r

.PHONY: build install regenerate_masters test clean
