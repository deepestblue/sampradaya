@ECHO OFF

cl rasterise_text.cxx /EHsc /std:c++latest /nologo /W4 d2d1.lib dwrite.lib

IF NOT EXIST master_images MD master_images
IF NOT EXIST master_images\character_set MD master_images\character_set
rasterise_text ..\cases\character_set.txt master_images\character_set
IF NOT EXIST master_images\e_ai_rearrangement MD master_images\e_ai_rearrangement
rasterise_text ..\cases\e_ai_rearrangement.txt master_images\e_ai_rearrangement
IF NOT EXIST master_images\misc_conjuncts MD master_images\misc_conjuncts
rasterise_text ..\cases\misc_conjuncts.txt master_images\misc_conjuncts
IF NOT EXIST master_images\o_au_rearrangement MD master_images\o_au_rearrangement
rasterise_text ..\cases\o_au_rearrangement.txt master_images\o_au_rearrangement
IF NOT EXIST master_images\post_ya_post_repha MD master_images\post_ya_post_repha
rasterise_text ..\cases\post_ya_post_repha.txt master_images\post_ya_post_repha
IF NOT EXIST master_images\pre_repha_with_vowel_matras MD master_images\pre_repha_with_vowel_matras
rasterise_text ..\cases\pre_repha_with_vowel_matras.txt master_images\pre_repha_with_vowel_matras
IF NOT EXIST master_images\pre_repha_complex_cases MD master_images\pre_repha_complex_cases
rasterise_text ..\cases\pre_repha_complex_cases.txt master_images\pre_repha_complex_cases
IF NOT EXIST master_images\consonant_vowel_ligatures MD master_images\consonant_vowel_ligatures
rasterise_text ..\cases\consonant_vowel_ligatures.txt master_images\consonant_vowel_ligatures
