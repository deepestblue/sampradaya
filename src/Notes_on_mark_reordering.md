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

Order:

* anunasika moves before all vowel markers and virama
* anunasika moves before pre repha
* anunasika moves before post yakara
* anunasika moves before post repha
* anudatta/svarita/dirghasvarita before anusvara, visarga and ardhavisarga
* anudatta/svarita/dirghasvarita before all vowel markers and virama, except for the haln cases
* anudatta/svarita/dirghasvarita before pre repha
* anudatta/svarita/dirghasvarita before post yakara
* anudatta/svarita/dirghasvarita before post repha
* anudatta before anunasika
* i or I vowel markers and virama marker move before pre repha
* i or I vowel markers move before post yakara
* i or I vowel markers move before post repha
