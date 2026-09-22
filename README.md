# Unicode Grantha typeface

Sampradaya is a Unicode Grantha typeface with the best support (as of 2022) for complex Grantha features like triply stacked conjunct consonants and Samavedic svara marks. It is tested on OSX 10.10+ and Windows 10+.

## How to Use

### On MacOS

Download Sampradaya.ttf from [releases](https://github.com/deepestblue/sampradaya/releases/) and drop it into /Library/Fonts/ (to install system-wide) or ~/Library/Fonts/ (to install at user-scope). You may need to restart your application (or logout/login) for the typeface to be usable.

### On Windows

Download Sampradaya.ttf from [releases](https://github.com/deepestblue/sampradaya/releases/), click the secondary mouse button on the downloaded file, and choose `Install` or `Install for all users` as appropriate.

## Building from source

The typeface is built from `src/Sampradaya.sfd` using FontForge. Building only works on macOS, since it relies on FontForge's command-line tooling; on Windows you can only test a `Sampradaya.ttf` that was built elsewhere (e.g. on macOS, or downloaded from a CI run).

### Prerequisites (macOS)

* [FontForge](https://fontforge.org/)
* [fonttools](https://github.com/fonttools/fonttools) (`brew install fonttools`)
* [gftools](https://github.com/googlefonts/gftools) (`pipx install gftools`)

### Build

```bash
cd macos
make build
```

This produces `src/Sampradaya.ttf`. `make install` additionally copies it into `~/Library/Fonts/`.

### Testing

Tests render the cases in `cases/` and compare them against the reference images in `masters/`, using [Alekhana](https://github.com/deepestblue/alekhana):

```sh
cd macos
make test
```

On Windows, place a built `Sampradaya.ttf` at `src/Sampradaya.ttf` first, then run:

```cmd.exe
cd windows
nmake test
```

Alekhana is downloaded automatically (per the version pinned in `versions.mk`) the first time you run `test` or `regenerate_masters`.

## References

* Vinodh Rajan's [Grantha primer](http://www.virtualvinodh.com/grantha-lipitva).
* Simon Cozens's book on [Fonts and Layout for Global Scripts](https://simoncozens.github.io/fonts-and-layout/).
* The SIL's book on [Font Development Best Practices](https://silnrsi.github.io/FDBP/en-US/OpenType.html).
* John Hudson's paper on [Enabling Typography: towards a general model of OpenType Layout](http://tiro.com/John/Enabling_Typography_(OTL).pdf).
* The English Wikipedia's [list of typographic features](https://en.wikipedia.org/wiki/List_of_typographic_features).
* Google Fonts [guidelines on vertical metrics](https://github.com/googlefonts/gf-docs/blob/master/VerticalMetrics/README.md).
* The [Fontforge tutorial's section on vertical metrics](https://fontforge.org/docs/tutorial/editexample5.html#setting-the-baseline-to-baseline-spacing-of-a-font).
* Microsoft Typography's [Indic script development specs](https://learn.microsoft.com/en-us/typography/script-development/tamil).
* [Design with Fontforge](http://designwithfontforge.com/en-US/Introduction.html#), part of free culture.
* [Glyphs' tutorial on vertical metrics](https://glyphsapp.com/learn/vertical-metrics).
* [Font Development Best Practice documentation's section on line metrics](https://silnrsi.github.io/FDBP/en-US/Line_Metrics.html).
* [Fontforge's SFD format documentation](https://fontforge.org/docs/techref/sfdformat.html).

## Acknowledgements

* Prof. R. Kalyana Krishnan (Retd.), IIT-Madras, for his IITMGrantha typeface, whose glyphs are reused here with permission.
* Anupama Krishnan, for her work on Unicodifying the IITMGrantha typeface, and for design help.
* Ranganath Atreya, for [LipikaIME](https://github.com/ratreya/Lipika_IME), invaluable in test case development.
* The developers of [FontForge](http://fontforge.github.io/).
* The [Unicode consortium](http://unicode.org).
* Corvel software's [Crowbar](https://www.corvelsoftware.co.uk/crowbar/), invaluable in testing.
