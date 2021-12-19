# Reordering

Unfortunately, USE implementations across Windows Uniscribe, Core Text and harfbuzz do not reorder Grantha glyphs correctly. This means we need to implement this reordering in the GSUB tables ourselves.

## Possible base+mark combinations

In datastream:
Rp?B(S<sub>2</sub>S<sub>3</sub>?)?Rk?Y?(IV|OV)?An?Ay?(UAcc|OAcc)?

In rendering:
B(S<sub>2</sub>S<sub>3</sub>?)?UAcc?An?OAcc?IV?Rk?Y?Rp?OV?Ay?

Note: Pl(UAccOAcc)? can optionally come at the end of the stream but does not require any rearrangement

Legend:

* ?: Regex semantics
* B: Base Glyph
* S<sub>2</sub>: 2<sup>nd</sup> level stacked conjunct consonant
* S<sub>3</sub>: 3<sup>rd</sup> level stacked conjunct consonant
* IV: i or I vowel marker
* OV: Other vowel marker, incl. virama
* An: Anunasika
* Ay: Ayogavaha
* UAcc: Underlay accent marker
* OAcc: Overlay accent marker
* Rp: Pre repha
* Rk: Post repha
* Y: Post yakara
* P: Pluta marker

Raw:

* anunasika moves before all vowel markers and virama: 56-66 & 24-26
* anunasika moves before e200, i.e. prerepha: 67, 27-29
* anunasika moves before postrepha and yakara: 68-69, 30-32
* anudatta/svarita/dirghasvarita before anusvara, visarga and ardhavisarga
* anudatta/svarita/dirghasvarita before all vowel markers and virama, except for the haln cases
* anudatta/svarita/dirghasvarita before e200, i.e. prerepha
* anudatta/svarita/dirghasvarita before postrepha and yakara
