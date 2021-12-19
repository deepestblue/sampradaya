# Reordering

Unfortunately, USE implementations across Windows Uniscribe, Core Text and harfbuzz do not reorder Grantha glyphs correctly. This means we need to implement this reordering in the GSUB tables ourselves.

## Possible base+mark combinations

In datastream:
Rp?B(S<sub>2</sub>S<sub>3</sub>?)?Rk?Y?(IV|OV|Vi)?An?Ay?(UA|OA)?

Automatically rearranged by renderers:
B(S<sub>2</sub>S<sub>3</sub>?)?Rk?Y?Rp?(IV|OV|Vi)?An?Ay?(UA|OA)?

Expected rendering:
B(S<sub>2</sub>S<sub>3</sub>?)?UA?An?OA?IV?Rk?Y?Vi?Rp?OV?Ay?

Note: Pl(UAccOAcc)? can optionally come at the end of the stream but does not require any rearrangement

Legend:

* ?: Regex semantics
* (): Regex semantics
* B: Base Glyph
* S<sub>2</sub>: 2<sup>nd</sup> level stacked conjunct consonant
* S<sub>3</sub>: 3<sup>rd</sup> level stacked conjunct consonant
* IV: i or I vowel marker
* OV: Other vowel marker, incl. virama
* Vi: Virama marker
* An: Anunasika
* Ay: Ayogavaha
* UA: Underlay accent marker
* OA: Overlay accent marker
* Rp: Pre repha
* Rk: Post repha
* Y: Post yakara
* P: Pluta marker

Raw:

* anunasika moves before all vowel markers and virama: 56-66, 24-26
* anunasika moves before e200, i.e. pre repha: 67, 27-29
* anunasika moves before post yakara: 68, 30-32
* anunasika moves before post repha: 69, 33-35
* anudatta/svarita/dirghasvarita before anusvara, visarga and ardhavisarga: 34-42, 56-58
* anudatta/svarita/dirghasvarita before all vowel markers and virama, except for the haln cases: 1-33, 62-64
* anudatta/svarita/dirghasvarita before e200, i.e. pre repha: 46-48, 89-91
* anudatta/svarita/dirghasvarita before post yakara: 49-51, 92-94
* anudatta/svarita/dirghasvarita before post repha: 52-54, 95-97
* anudatta before anunasika: 55, 98-100
* i or I vowel markers move before pre repha, post repha and post yakara
* Virama markers moves before pre repha
