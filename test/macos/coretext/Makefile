CXXFLAGS := -std=c++2a -Weverything -fdiagnostics-show-note-include-stack -fmacro-backtrace-limit=1 -ferror-limit=1 -ftemplate-backtrace-limit=1 -Wno-missing-prototypes -Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-disabled-macro-expansion -Wno-global-constructors -Wno-exit-time-destructors -Wno-float-equal -Wno-poison-system-directories
LDFLAGS := -ferror-limit=1
LDLIBS := -lstdc++ -framework ImageIO -framework CoreGraphics -framework CoreText -framework CoreServices -framework CoreFoundation
OBJECTS	:= ../../rasterise_text.o rasterise_text_impl.o

rasterise_text: ${OBJECTS}
	${CC} ${LDFLAGS} ${OBJECTS} ${LDLIBS} -o $@

clean:
	${RM} rasterise_text ${OBJECTS}
