# Indic‐relevant TT tables

## Tables

| Table | Type | Purpose | Example |
| :---: | :--: | :-------| :-----: |
| akhn  | S4   | Akhaṇḍa consonant+virama+consonant preferential ligatures. | 𑌕𑍍𑌷 |
| rlig  | S3   | Not specific to Indic; required ligatures for display in cursive. Used here for stacked ligatures. | 𑌸𑍍𑌤 |
| rphf  | S4   | Replaces repha+virāma with post form. | 𑌰𑍍𑌣 |
| rkrf  | S4   | Replaces virāma+rēpha with a new glyph. | 𑌕𑍍𑌰 |
| half  | S4   | Half forms of consonants. Not relevant to Grantha. | प्प |
| haln  | S4   | Replaces consonant+virāma with a terminal form. | 𑌮𑍍 |
| vatu  | S4   | Replaces consonant+virāma+rēpha with a new glyph. | न्र |
| cjct  | S4   | Conjunct consonant+virama+consonant ligatures, later than Akhaṇḍa ligatures. Not used here. | द्ग |
| abvf  | S1   | Contextually changes the above‐base part of a vowel marker. Not relevant to Grantha. | កើ |
| blwf  | S4   | Contextually changes a virāma+consonant with a below‐base form. | 𑌶𑍍𑌲 |
| pref  | S4   | Contextually changes a virāma+consonant into a pre‐base form. | ശ്രീ |
| pstf  | S4   | Contextually changes a virāma+consonant into a post‐base form. | 𑌖𑍍𑌯|
| abvs  | S4   | Ligates a base glyph and an above‐base glyph. | 𑌟𑍀 |
| blws  | S4   | Ligates a base glyph and a below‐base glyph. | ക്ല |
| pres  | S4,5 | Ligates a base glyph and a pre‐base glyph. | ક્ક |
| psts  | S4   | Ligates a base glyph and a post‐base glyph. | വ്വ |
| abvm  | P4,5 | Positions anusvāras etc. above the base. Not used here. | उं |
| blwm  | P4,5 | Positions u‐markers etc. above the base. Not used here. | कु |

## Type descriptions

| Type  | Description |
| :---: | :---------- |
| GSUB1 | Simple substitution |
| GSUB3 | Variant selection |
| GSUB4 | Ligatures |
| GSUB5 | Contextual substitution |
| GPOS4 | Positioning of mark glyphs relative to base |
| GPOS5 | Positioning of mark glyphs relative to ligature |

## Order of processing

'akhn', 'rphf', 'rkrf', 'pref', 'blwf', 'half', 'pstf', 'cjct'.

## References

* [Enabling Typography: towards a general model of OpenType Layout](http://tiro.com/John/Enabling_Typography_(OTL).pdf)
* The English Wikipedia's list of typographic features](https://en.wikipedia.org/wiki/List_of_typographic_features)
