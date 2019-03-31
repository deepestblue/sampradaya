# Unicode Grantha typeface

This is a project to create a Unicode-compliant Grantha typeface based on the proposed [Unicode 7.0](http://www.unicode.org/versions/beta-7.0.0.html) [encoding](http://std.dkuug.dk/JTC1/SC2/WG2/docs/n4135.pdf) in the U+11300 SMP block. For the moment, it only works on OS X. Windows support depends on Microsoft supporting the script in Uniscribe, the Windows rendering engine. It's understood that Windows 10's Uniscribe supports Grantha, but this typeface uses AAT and so will need to be enhanced to support Windows.

## How to Use

Simply download the font file and drop it into /Library/Fonts/ (to install system-wide) or ~/Library/Fonts/ (to install at user-scope). You may need to restart your application (or logout/login) for the font to be usable. If you're looking to input Grantha text, I highly recommend [Lipika](https://github.com/ratreya/Lipika_IME) with the [Ksharanam key-map for Grantha](https://bitbucket.org/OorNaattaan/keymap/src).

## Acknowledgements

* Prof. R. Kalyana Krishnan (Retd.), IIT-Madras, for his IITMGrantha typeface, whose glyphs are reused here with permission.
* Vinodh Rajan, for his [wonderful site on Grantha](http://www.virtualvinodh.com/grantha-lipitva).
* Anupama Krishnan, for her work on Unicodifying the IITMGrantha typeface, and for design help.
* Ranganath Atreya, for [LipikaIME](https://github.com/ratreya/Lipika_IME), invaluable in testing.
* The developers of [FontForge](http://fontforge.github.io/).
* The [Unicode consortium](http://unicode.org).
