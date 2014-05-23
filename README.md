This is a project to create a Unicode-compliant Grantha font based on the proposed [Unicode 7.0](http://www.unicode.org/versions/beta-7.0.0.html) [encoding](http://std.dkuug.dk/JTC1/SC2/WG2/docs/n4135.pdf) in the U+11300 SMP block.

### How to Use

Simply download the font file and drop it into /Library/Fonts/ (to install system-wide) or ~/Library/Fonts/ (to install at user-scope). You may need to restart your application (or logout/login) for the font to be usable. If you're looking to input Grantha text, I highly recommend [Lipika](https://github.com/ratreya/Lipika_IME) with the [Ksharanam key-map for Grantha](http://code.ambari.sh/keymap/src).

### Implementation

#### Glyphs

The font supports glyphs for all the characters in the basic character set, as well as glyphs for below-base forms of consonants. The font also supports initial and terminal half-repha forms, the terminal half-ya form, and most pre-formed conjunct consonants commonly used.

#### AAT tables
The typeface has the following ligature look-up sub-tables:

0. 'rlig'
 * Akhanda forms
 * Post-glyph half-repha form
 * Pre-composed stacked forms
 * Infra-baseline half-form stacks of akhanda forms
 * Infra-baseline half-form stacks of other consonants
 * Post-forms of conjunct-ya and conjunct-repha

0. Some Single substitution look-up tables, redundant but inserted by FontForge.

0. Indic state machine to move Post-glyph half-pre-repha to after a full consonant.
   The goal is to shift the half-pre-repha rightwards, past the next “consonant cluster”.
   But what is a consonant-cluster? Let Class `A` comprise full consonants, stacked conjuncts and the viramacihna. Likewise, Class `B` is pre/post repha/ya, below-base half-consonants. A consonant cluster starts with a class `A` glyph and has any number of Class `B` glyphs. In other words, `AB*`. We achieve this using 2 state machines. First, we move the repha-glyph past an A, using a simple machine. And …

0. Indic state machine to move Post-glyph half-pre-repha to after post-forms.
   … we then move the half-pre-repha past any sequence of Class `B` glyphs, using a simple machine.

0. Insertion state machine to insert ē marker before ō and au markers.
   This is a simple machine that transitions to a new state when a consonant is encountered. In that state, were it encounter an ō or an au marker, it simply inserts an ē marker before.

0. Contextual state machine for substituting ō and au markers with their post-consonant halves.
   This is also a simple machine that matches ō and au markers that come after a consonant and an ē marker. Such an ō or an au marker would be replaced. The machine has a transition to state 1 on seeing a consonant, a state 1 → state 2 transition on seeing an ē marker, and a state 2 substitution for the ō and au markers.

0. Single substitution look-up table for the previous state machine.

0. Indic state machine to move ē and ai vowel markers to before the glyph.
   The goal is to shift the ē and ai markers leftwards, before the previous consonant cluster. As above, a consonant cluster is `AB*`, with the above definitions of Classes `A` and `B`.
   To achieve this, we keep marking every class `A` glyph as the first, in case it turns out be succeeded by a `B*` and then an ē or an ai vowel marker. If, indeed that happens, we simply replace `xD` with `Dx`.

### TODO:
* Add terminal forms of ta, na and ma
* Fix advanced_test.txt and other test-cases

### Acknowledgements:
* Prof. R. Kalyana Krishnan (Retd.), IIT-Madras, for his IITMGrantha font, whose glyphs are reused here with permission
* Vinodh Rajan, for his [wonderful site on Grantha](http://www.virtualvinodh.com/grantha-lipitva)
* Anupama Krishnan, for her work on Unicodifying the IITMGrantha font, and for design help
* Ranganath Atreya, for [LipikaIME](https://github.com/ratreya/Lipika_IME)
* The developers of [FontForge](http://fontforge.github.io/)
* The [Unicode consortium](http://unicode.org)