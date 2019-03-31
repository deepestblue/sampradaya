@ECHO OFF

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
IF NOT EXIST master_images\pre_repha_half_form MD master_images\pre_repha_half_form
rasterise_text ..\cases\pre_repha_half_form.txt master_images\pre_repha_half_form
