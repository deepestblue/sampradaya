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

Single substitution look-up table (redundant).

Indic state machine to move Post-glyph half-pre-repha to after 
the glyph.

Insertion state machine to insert ē-marker before ō- and au- markers.

Contextual state machine for substituting ō- and au- markers with their post-consonant halves.

Single substitution look-up table for the previous state machine.

Indic state machine to move ē and ai to before the glyph.

##### TODO:
* Add terminal forms of ta, na and ma
* Fix advanced_test.txt and other test-cases
