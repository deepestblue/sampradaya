# Implementation

## Glyphs

The typeface supports glyphs for all the characters in the basic character set, as well as glyphs for below-base forms of consonants. The typeface also supports pre-consonantal and post-consonantal half-repha forms, the post-consonantal half-ya form, and most combined conjunct consonants commonly used.

## AAT tables

The typeface has the following ligature look-up sub-tables:

* 'rlig'
    1. Combined (akhanḍa) forms
    1. Composite stacked forms
    1. Ligation of i/ī with some consonants
    1. Pre-consonantal half-repha form
    1. Post-consonantal forms of ya and repha
    1. Infra-baseline half-form stacks of akhanḍa forms
    1. Infra-baseline half-form stacks of other consonants
    1. Terminal forms of select consonants

    The order of the ligature sub-tables is crucial. Specifically, here are the various dependencies between them:

    Precedence          | Example
    --------------------|--------
    4 precedes 6 & 7    | arkaḥ
    4 precedes 5        | aryamā
    1 precedes 7        | āntyam, mantraḥ
    2 precedes 7        | astram
    1 & 2 precede 6 & 7 | lakṣmīḥ/payastvam
    6 & 7 precede 8     | atra

    In effect, {1,2}→{6,7}; 4→5→{6,7}→8. Thus the order we use: 1, 2, 3, 4, 5, 6, 7, 8.

* Some Single substitution look-up tables, redundant but inserted by FontForge.
* Indic state machine to move pre-consonantal repha to after a full consonant.
    The goal is to shift the repha rightwards, past the next “consonant cluster”.
    But what is a consonant-cluster? Let Class `A` comprise terminal half-forms, Class `B` comprise full consonants, combined/composite stacked conjuncts and the virāmacihna. Likewise, Class `C` is pre/post repha/ya and infra-base half-consonants. A consonant cluster starts with an optional class `A` glyph, a single Class `B` glyph, and then any number of Class `C` glyphs. In other words, `A?BC*`. We achieve this using 3 state machines. First, we move the repha past a Class `A` glyph, using a simple machine. And …
* Indic state machine to move pre-consonantal repha to after a full consonant.
    … we then move the repha past a Class `B` glyphs, using a simple machine. And finally, …
* Indic state machine to move pre-consonantal repha to after post-forms.
    … we move the repha past any sequence of Class `C` glyphs, using a simple machine. We also use this opportunistically to move the repha past i/ī vowel markers and the virāmacihna.
* Indic state machine to move post-consonantal repha/ya forms to after i/ī vowel markers.
    Simple machine to move an i/ī marker before any combination of post-consonantal repha/ya.
* Insertion state machine to insert ē marker before ō and au markers.
    This is a simple machine that transitions to a new state when a consonant is encountered. In that state, were it encounter an ō or an au marker, it simply inserts an ē marker before.
* Contextual state machine for substituting ō and au markers with their post-glyph halves.
    This is also a simple machine that matches ō and au markers that come after a consonant and an ē marker. Such an ō or an au marker would be replaced. The machine has a transition to state 1 on seeing a consonant, a state 1 → state 2 transition on seeing an ē marker, and a state 2 substitution for the ō and au markers.
* Single substitution look-up table for the previous state machine.
* Indic state machine to move ē and ai vowel markers to before the glyph.
    The goal is to shift the ē and ai markers leftwards, before the previous consonant cluster. As above, a consonant cluster is `A?BC*`, with the above definitions of Classes `A`, `B`, and `C`.
    To achieve this, we keep marking every Class `A` glyph as the first, in case it turns out be so. Then, whether or not we have already seen a Class `A` glyph, we eat up any consecutive Class `B` sequence, marking the first such glyph as the first, unless preceded by a Class `A` glyph. Lastly, we move past successive Class `C` glyphs and then look for an ē or an ai vowel marker. If that happens, we simply replace `xD` with `Dx`.