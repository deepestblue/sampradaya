This is a project to create a Unicode-compliant Grantha font based on the proposed [Unicode 7.0](http://www.unicode.org/versions/beta-7.0.0.html) 
[encoding](http://std.dkuug.dk/JTC1/SC2/WG2/docs/n4135.pdf) in the U+11300 SMP block.

The typeface has the following ligature look-up sub-tables:

'rlig'
* Akhanda forms
* Post-glyph half-repha form
* Pre-composed stacked forms
* Infra-baseline half-form stacks of akhanda forms
* Infra-baseline half-form stacks of other consonants
* Post-forms of conjunct-ya and conjunct-repha

Indic rearrangement state machine to move Post-glyph half-repha to after 
the glyph.

Indic rearrangement state machine to move E and ai to before the glyph.

##### TODO:
* Add terminal forms of ta, na and ma
* Add combining markers for O and au
* Fix advanced_test.txt
