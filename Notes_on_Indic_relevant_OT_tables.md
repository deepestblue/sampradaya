# Indic‐relevant OpenType tables

## Tables

| Table | Type | Stage | Purpose | Example |
| :---: | :--: | :---: | :-------| :-----: |
| akhn  | S4   | 1     | Akhaṇḍa consonant+virama+consonant preferential ligatures. | 𑌕𑍍𑌷 |
| rphf  | S4   | 2     | Replaces repha+virāma with post form. | 𑌰𑍍𑌣 |
| rkrf  | S4   | 2     | Replaces virāma+rēpha with a new glyph. | 𑌕𑍍𑌰 |
| pref  | S4   | 2     | Contextually changes a virāma+consonant into a pre‐base form. Not relevant to Grantha. | ശ്രീ |
| blwf  | S4   | 2     | Contextually changes a virāma+consonant with a below‐base form. | 𑌶𑍍𑌲 |
| abvf  | S1   | 2     | Contextually changes the above‐base part of a vowel marker. Not relevant to Grantha. | កើ |
| half  | S4   | 2     | Half forms of consonants. Not relevant to Grantha. | प्प |
| pstf  | S4   | 2     | Contextually changes a virāma+consonant into a post‐base form. | 𑌖𑍍𑌯 |
| vatu  | S4   | 2     | Replaces consonant+virāma+rēpha with a new glyph. | 𑌤𑍍𑌰 |
| cjct  | S4   | 2     | Conjunct consonant+virama+consonant ligatures, later than Akhaṇḍa ligatures. Not used here. | द्ग |
| pres  | S4,5 | 3     | Ligates a base glyph and a pre‐base glyph. Not relevant to Grantha. | ક્ક |
| abvs  | S4   | 3     | Ligates a base glyph and an above‐base glyph. Not used here. | டி |
| blws  | S4   | 3     | Ligates a base glyph and a below‐base glyph. Not used here. | ക്ല |
| psts  | S4   | 3     | Ligates a base glyph and a post‐base glyph. Used here to switch around the post‐rēpha and i/ī‐markers. | 𑌕𑍍𑌰𑌿 |
| haln  | S4   | 3     | Replaces consonant+virāma with a terminal form. | 𑌮𑍍 |
| abvm  | P4,5 | 4     | Positions anusvāras etc. above the base. Used for kerning ī‐markers. | 𑌟𑍀 |
| blwm  | P4,5 | 4     | Positions u‐markers etc. above the base. Not used here. | कु |

## Type descriptions

| Type  | Description |
| :---: | :---------- |
| GSUB1 | Simple substitution |
| GSUB4 | Ligatures |
| GSUB5 | Contextual substitution |
| GPOS4 | Positioning of mark glyphs relative to base |
| GPOS5 | Positioning of mark glyphs relative to ligature |

## Stages

| Stage | Description |
| :---: | :---------- |
| 1     | Default glyph pre‐processing |
| 2     | Orthographic unit shaping |
| 3     | Standard typographic presentation |
| 4     | Positioning |
