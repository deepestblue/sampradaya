build:

install: build
	install ../../src/osx/Sampradaya.ttf ~/Library/Fonts/

test: rasterise_text
	./run_tests

LDFLAGS = -ferror-limit=1 -F/usr/local/opt/qt/lib -framework QtGui -framework QtCore -framework QtWidgets
CXXFLAGS = -std=c++1y -Weverything -ferror-limit=1 -ftemplate-backtrace-limit=1 -Wno-missing-prototypes -Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-disabled-macro-expansion -Wno-global-constructors -Wno-exit-time-destructors -Wno-float-equal -I/usr/local/opt/qt/include --system-header-prefix=QtGui/ --system-header-prefix=QtWidgets/ --system-header-prefix=boost/

.PHONY: build install test regenerate_masters clean

regenerate_masters:
	./generate_images master_images

clean:
	-rm -f rasterise_text
	-find ${TMPDIR} -iname 'run_tests*' -type d 2>/dev/null | xargs rm -r
