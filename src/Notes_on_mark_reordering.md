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
* IV: i or ī vowel marker
* OV: Other vowel marker, incl. virāma
* Vi: Virāma marker
* An: Anunāsika
* Ay: Ayōgavāha
* UA: Underlay accent marker
* OA: Overlay accent marker
* Rp: Pre rēpha
* Rk: Post rēpha
* Y: Post yakāra
* P: Pluta marker

Order:

* Anunāsika before all vowel markers and virāma
* Anunāsika before pre rēpha
* Anunāsika before post yakāra
* Anunāsika before post rēpha
* Accent markers before ayōgavāhas
* Accent markers before all vowel markers and virāma, except for the haln cases
* Accent markers before pre rēpha
* Accent markers before post yakāra
* Accent markers before post rēpha
* Anudātta before anunāsika
* i or ī vowel markers and virāma marker before pre rēpha
* i or ī vowel markers before post yakāra
* i or ī vowel markers before post rēpha
