project(buildcd)

#if (MSVC)
#	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
#endif()

# dependencies: bmfontgen.exe, eventeditor.exe, fontgen.exe, mksplash.exe, satcode-insert.exe, vcdbuild.exe, xdelta.exe

set( evn_SOURCES
 files/000_00_0.EVN files/000_00_1.EVN
	files/000_00_2.EVN files/000_00_3.EVN files/000_00_4.EVN
	files/000_00_7.EVN files/000_00_8.EVN files/000_00_9.EVN
	files/000_01_0.EVN files/000_01_1.EVN files/000_01_2.EVN
	files/000_01_3.EVN files/000_01_4.EVN files/000_01_5.EVN
	files/000_01_6.EVN files/000_01_7.EVN files/000_01_8.EVN
	files/000_01_9.EVN files/000_02_0.EVN files/000_02_1.EVN
	files/000_02_2.EVN files/000_02_3.EVN files/000_02_4.EVN
	files/000_02_5.EVN files/000_02_6.EVN files/000_02_7.EVN
	files/000_02_8.EVN files/000_02_9.EVN files/000_03_0.EVN
	files/000_03_1.EVN files/000_03_2.EVN files/000_03_3.EVN
	files/000_03_4.EVN files/000_03_5.EVN files/000_03_6.EVN
	files/000_03_7.EVN files/000_03_8.EVN files/000_03_9.EVN
	files/000_03_A.EVN files/000_04_0.EVN files/000_04_1.EVN
	files/000_04_2.EVN files/000_04_3.EVN files/000_04_4.EVN
	files/000_04_5.EVN files/000_04_6.EVN files/000_04_7.EVN
	files/000_04_8.EVN files/000_04_9.EVN files/000_06_0.EVN
	files/000_06_1.EVN files/000_06_4.EVN files/000_06_5.EVN
	files/000_06_6.EVN files/000_06_7.EVN files/000_06_9.EVN
	files/000_07_1.EVN files/000_07_2.EVN files/000_07_3.EVN
	files/000_07_4.EVN files/000_07_5.EVN files/000_07_6.EVN
	files/000_07_7.EVN files/000_07_8.EVN files/000_07_9.EVN
	files/000_07_A.EVN files/000_07_B.EVN files/000_07_C.EVN
	files/001_00_0.EVN files/001_00_1.EVN files/002_00_0.EVN
	files/002_00_1.EVN files/002_00_2.EVN files/002_00_3.EVN
	files/002_00_4.EVN files/002_01_0.EVN files/002_01_1.EVN
	files/002_01_2.EVN files/002_01_3.EVN files/002_01_4.EVN
	files/003_00_0.EVN files/004_00_0.EVN files/004_00_1.EVN
	files/004_00_2.EVN files/004_00_3.EVN files/005_00_0.EVN
	files/007_00_0.EVN files/007_00_1.EVN files/007_06_0.EVN
	files/007_09_0.EVN files/009_00_0.EVN files/011_00_0.EVN
	files/012_00_0.EVN files/012_00_1.EVN files/012_00_2.EVN
	files/013_00_0.EVN files/014_00_0.EVN files/015_00_0.EVN
	files/015_00_1.EVN files/015_00_2.EVN files/015_00_3.EVN
	files/015_00_4.EVN files/015_01_0.EVN files/015_01_2.EVN
	files/015_01_3.EVN files/015_01_4.EVN files/017_00_0.EVN
	files/019_08_0.EVN files/020_00_0.EVN files/020_00_1.EVN
	files/021_00_0.EVN files/022_00_0.EVN files/024_00_0.EVN
	files/024_07_0.EVN files/024_13_0.EVN files/025_00_0.EVN
	files/026_00_0.EVN files/026_00_1.EVN files/026_00_2.EVN
	files/026_00_3.EVN files/026_01_0.EVN files/026_01_1.EVN
	files/026_01_2.EVN files/026_01_3.EVN files/027_00_0.EVN
	files/029_05_0.EVN files/029_12_0.EVN files/029_24_0.EVN
	files/030_00_0.EVN files/031_00_0.EVN files/031_00_1.EVN
	files/031_00_2.EVN files/031_00_3.EVN files/031_01_0.EVN
	files/031_02_0.EVN files/031_02_1.EVN files/031_02_2.EVN
	files/031_02_3.EVN files/035_05_0.EVN files/035_08_0.EVN
	files/035_11_0.EVN files/036_00_0.EVN files/037_00_1.EVN
	files/038_00_0.EVN files/038_01_0.EVN files/038_17_0.EVN
	files/038_17_1.EVN files/038_18_0.EVN files/038_18_1.EVN
	files/038_20_0.EVN files/038_20_1.EVN files/039_00_0.EVN
	files/039_00_1.EVN files/039_00_2.EVN files/039_00_3.EVN
	files/039_00_4.EVN files/039_00_5.EVN files/039_00_6.EVN
	files/039_00_7.EVN files/039_00_8.EVN files/039_00_9.EVN
	files/039_00_A.EVN files/039_01_0.EVN files/039_01_1.EVN
	files/039_01_2.EVN files/039_01_3.EVN files/039_01_4.EVN
	files/039_01_5.EVN files/039_01_6.EVN files/039_01_7.EVN
	files/039_01_8.EVN files/039_01_9.EVN files/039_02_0.EVN
	files/039_02_1.EVN files/039_02_2.EVN files/039_02_3.EVN
	files/039_02_4.EVN files/039_02_5.EVN files/039_02_6.EVN
	files/039_02_7.EVN files/039_02_8.EVN files/039_02_9.EVN
	files/039_09_3.EVN files/040_00_0.EVN files/041_00_0.EVN
	files/041_00_1.EVN files/041_00_2.EVN files/041_00_3.EVN
	files/041_00_4.EVN files/041_01_0.EVN files/041_01_1.EVN
	files/041_01_2.EVN files/041_01_3.EVN files/041_01_4.EVN
	files/042_00_1.EVN files/042_00_2.EVN files/043_00_0.EVN
	files/043_00_1.EVN files/043_00_2.EVN files/043_00_3.EVN
	files/043_00_4.EVN files/043_01_0.EVN files/043_01_1.EVN
	files/043_01_2.EVN files/043_01_3.EVN files/043_01_4.EVN
	files/043_02_0.EVN files/043_02_1.EVN files/043_02_2.EVN
	files/043_02_3.EVN files/043_02_4.EVN files/044_00_1.EVN
	files/045_00_0.EVN files/045_00_1.EVN files/045_00_2.EVN
	files/045_00_3.EVN files/048_00_0.EVN files/048_01_0.EVN
	files/050_11_0.EVN files/052_00_0.EVN files/052_00_1.EVN
	files/055_00_0.EVN files/055_00_1.EVN files/055_00_2.EVN
	files/055_00_3.EVN files/055_00_4.EVN files/055_01_0.EVN
	files/055_01_1.EVN files/055_01_2.EVN files/055_01_3.EVN
	files/055_01_4.EVN files/055_02_0.EVN files/055_02_2.EVN
	files/055_02_3.EVN files/055_02_4.EVN files/057_09_0.EVN
	files/057_16_0.EVN files/057_28_0.EVN files/058_00_0.EVN
	files/061_00_0.EVN files/061_00_1.EVN files/061_00_2.EVN
	files/061_01_0.EVN files/061_01_1.EVN files/061_01_2.EVN
	files/061_03_1.EVN files/061_03_2.EVN files/061_08_1.EVN
	files/061_08_2.EVN files/062_00_0.EVN files/063_07_0.EVN
	files/063_07_1.EVN files/063_08_0.EVN files/063_08_1.EVN
	files/063_08_2.EVN files/067_00_0.EVN files/067_00_1.EVN
	files/068_01_0.EVN files/068_08_0.EVN files/068_09_0.EVN
	files/069_00_0.EVN files/069_00_1.EVN files/071_07_0.EVN
	files/071_07_1.EVN files/071_10_0.EVN files/071_10_1.EVN
	files/071_21_0.EVN files/071_21_1.EVN files/073_00_0.EVN
	files/073_18_0.EVN files/073_18_1.EVN files/073_22_0.EVN
	files/073_22_1.EVN files/073_24_0.EVN files/073_25_0.EVN
	files/073_25_1.EVN files/073_44_0.EVN files/073_44_1.EVN
	files/074_00_1.EVN files/074_00_2.EVN files/074_00_3.EVN
	files/074_00_4.EVN files/074_01_0.EVN files/074_01_1.EVN
	files/074_01_2.EVN files/074_01_3.EVN files/074_01_4.EVN
	files/074_02_1.EVN files/074_02_2.EVN files/074_02_3.EVN
	files/074_02_4.EVN files/076_03_0.EVN files/076_03_1.EVN
	files/076_12_0.EVN files/076_12_1.EVN files/076_13_0.EVN
	files/076_13_1.EVN files/078_00_0.EVN files/079_00_0.EVN
	files/079_00_1.EVN files/079_01_1.EVN files/079_02_1.EVN
	files/079_03_0.EVN files/079_03_1.EVN files/079_04_1.EVN
	files/081_00_0.EVN files/083_00_0.EVN files/083_01_0.EVN
	files/089_00_0.EVN files/089_05_0.EVN files/089_11_0.EVN
	files/090_00_0.EVN files/090_00_1.EVN files/091_00_0.EVN
	files/093_07_0.EVN files/093_15_0.EVN files/100_12_0.EVN
	files/101_14_0.EVN files/102_01_0.EVN files/102_15_0.EVN
	files/103_01_0.EVN files/103_15_0.EVN files/104_00_0.EVN
	files/105_00_0.EVN files/106_00_0.EVN files/107_00_0.EVN
	files/107_00_1.EVN files/107_01_0.EVN files/107_01_1.EVN
	files/107_02_0.EVN files/107_02_1.EVN files/107_03_0.EVN
	files/107_03_1.EVN files/107_04_0.EVN files/107_04_1.EVN
	files/109_00_0.EVN files/109_00_1.EVN files/109_01_0.EVN
	files/109_01_1.EVN files/110_00_0.EVN files/110_00_1.EVN
	files/110_00_2.EVN files/110_01_0.EVN files/110_01_2.EVN
	files/111_00_0.EVN files/112_00_0.EVN files/114_00_0.EVN
	files/114_00_1.EVN files/114_00_2.EVN files/116_05_0.EVN
	files/116_08_0.EVN files/117_00_0.EVN files/117_00_1.EVN
	files/117_00_2.EVN files/117_01_0.EVN files/117_01_1.EVN
	files/117_01_2.EVN files/119_00_0.EVN files/119_00_1.EVN
	files/119_00_2.EVN files/121_00_0.EVN files/121_00_1.EVN
	files/121_00_2.EVN files/123_00_0.EVN files/123_00_1.EVN
	files/124_00_0.EVN files/124_00_1.EVN files/124_00_2.EVN
	files/124_00_3.EVN files/124_01_0.EVN files/124_01_1.EVN
	files/124_01_2.EVN files/124_01_3.EVN files/125_00_0.EVN
	files/125_01_0.EVN files/127_07_0.EVN files/127_08_0.EVN
	files/129_00_0.EVN files/129_01_0.EVN files/129_02_0.EVN
	files/133_00_0.EVN files/135_07_0.EVN files/135_10_0.EVN
	files/135_21_0.EVN files/137_00_0.EVN files/137_18_0.EVN
	files/137_22_0.EVN files/137_25_0.EVN files/137_44_0.EVN
	files/138_00_0.EVN files/138_00_1.EVN files/138_01_0.EVN
	files/138_01_1.EVN files/138_02_0.EVN files/138_02_1.EVN
	files/140_00_0.EVN files/140_00_1.EVN files/140_01_0.EVN
	files/140_01_1.EVN files/142_00_0.EVN files/142_00_1.EVN
	files/144_00_0.EVN files/144_00_1.EVN files/146_00_0.EVN
	files/146_01_0.EVN files/148_07_0.EVN files/148_08_0.EVN
	files/149_09_0.EVN files/149_16_0.EVN files/149_28_0.EVN
	files/150_00_0.EVN files/150_00_1.EVN files/152_00_0.EVN
	files/152_00_1.EVN files/152_00_2.EVN files/153_00_0.EVN
	files/153_00_2.EVN files/153_01_1.EVN files/154_00_0.EVN
	files/155_00_0.EVN files/156_00_0.EVN files/156_00_1.EVN
	files/156_01_0.EVN files/156_01_1.EVN files/158_00_0.EVN
	files/160_00_0.EVN files/160_00_1.EVN files/162_00_0.EVN
	files/164_00_0.EVN files/166_00_0.EVN files/167_00_0.EVN
	files/167_01_0.EVN files/169_00_0.EVN files/171_00_0.EVN
	files/175_00_0.EVN files/176_00_0.EVN 
)

# Disc dependencies
set( buildcd_SOURCES
	IP.BIN
	files/0.BIN files/ADRI.CHR
	files/ADRI.PAK files/AERI.CHR files/AERI.PAK
	files/ANML.CHR files/ANML.PAK files/AREL.CHR
	files/AREL.PAK files/ASTL1C.CHB files/ASTL1C.MCB
	files/ASTL1E.CHB files/ASTL1E.MCB files/ASTL1K.CHB
	files/ASTL1K.MCB files/ASTL2C.CHB files/ASTL2C.MCB
	files/ASTL2E.CHB files/ASTL2E.MCB files/ASTL2K.CHB
	files/ASTL2K.MCB files/ASTL3C.CHB files/ASTL3C.MCB
	files/ASTL3K.CHB files/ASTL3K.MCB files/ASTP.CHR
	files/ASTP.PAK files/ASTP1C.CHB files/ASTP1C.MCB
	files/ASTP1K.CHB files/ASTP1K.MCB files/ASTP2C.CHB
	files/ASTP2C.MCB files/ASTP2E.CHB files/ASTP2E.MCB
	files/ASTP2K.CHB files/ASTP2K.MCB files/AT1C.CHR
	files/AT1C.PAK files/AT1T.CHR files/AT1T.PAK
	files/AT2C.CHR files/AT2C.PAK files/AT3C.CHR
	files/AT3C.PAK files/BABA.CHR files/BABA.PAK
	files/BARB.CHR files/BARB.PAK files/BARB.PRG
	files/BARBALI.SND files/BASIL.PRG files/BASIRISK.SND
	files/BASK.CHR files/BASK.PAK files/BA_A.CHR
	files/BA_A.PAK files/BIKG1C.CHB files/BIKG1C.MCB
	files/BIKG1E.CHB files/BIKG1E.MCB files/BIKG1K.CHB
	files/BIKG1K.MCB files/BK1C.CHR files/BK1C.PAK
	files/BLUD.PRG files/BLUD2.CHR files/BLUD2.PAK
	files/BLUD2.PRG files/BLUD2_4.CHR files/BLUD2_4.PAK
	files/BOOK1K.MPB files/BOYS.CHR files/BOYS.PAK
	files/BRED1E.CHB files/BRED1E.MCB files/CARD.CHR
	files/CARD.PAK files/CARD.PRG files/CARDKISI.SND
	files/CARD_DEM.SND files/CARD_GOL.SND files/CARD_GRI.SND
	files/CARD_RYO.SND files/CARD_TAR.SND files/CAVE.CHR
	files/CAVE.PAK files/CAVE1C.CHB files/CAVE1C.MCB
	files/CAVE1E.CHB files/CAVE1E.MCB files/CAVE1K.CHB
	files/CAVE1K.MCB files/CAVE2C.CHB files/CAVE2C.MCB
	files/CAVE2E.CHB files/CAVE2E.MCB files/CAVE2K.CHB
	files/CAVE2K.MCB files/CAVE3C.CHB files/CAVE3C.MCB
	files/CAVE3E.CHB files/CAVE3E.MCB files/CAVE3K.CHB
	files/CAVE3K.MCB files/CBUC.CHR files/CBUC.PAK
	files/CENT.PRG files/CENTOURS.SND files/CEYE.CHR
	files/CEYE.PAK files/CEYE.PRG files/CHAP.CHR
	files/CHAP.PAK files/COCKPIT.CHB files/COCKPIT.CLB
	files/COMM.CHR files/COMM.PAK files/CREW_C.CHR
	files/CREW_C.PAK files/CREW_N.CHR files/CREW_N.PAK
	files/DC1C.CHR files/DC1C.PAK files/DC1T.CHR
	files/DC1T.PAK files/DC2C.CHR files/DC2C.PAK
	files/DEMN.CHR files/DEMN.PAK files/DEMO.CHB
	files/DEMO.CHR files/DEMO.CLB files/DEMO.MPB
	files/DEMO.PAK files/DEMON.PRG files/DEM_GOL.SND
	files/DG1C.CHR files/DG1C.PAK files/DG1R.CHR
	files/DG1R.PAK files/DG2C.CHR files/DG2C.PAK
	files/DG3C.CHR files/DG3C.PAK files/DG3R.CHR
	files/DG3R.PAK files/DGRAD.PRG files/DM1C.CHR
	files/DM1C.PAK files/DNGN1C.CHB files/DNGN1C.MCB
	files/DNGN1E1.CHB files/DNGN1E1.MCB files/DNGN1E2.CHB
	files/DNGN1E2.MCB files/DNGN1E3.CHB files/DNGN1E3.MCB
	files/DNGN1K.CHB files/DNGN1K.MCB files/DNGN2C.CHB
	files/DNGN2C.MCB files/DNGN2E.CHB files/DNGN2E.MCB
	files/DNGN2K.CHB files/DNGN2K.MCB files/DNGN3C.CHB
	files/DNGN3C.MCB files/DNGN3E.CHB files/DNGN3E.MCB
	files/DNGN3K.CHB files/DNGN3K.MCB files/DNGN4C.CHB
	files/DNGN4C.MCB files/DNGN4K.CHB files/DNGN4K.MCB
	files/DNGN5E.CHB files/DNGN5E.MCB files/DNGN5K.CHB
	files/DNGN5K.MCB files/DNGN6C.CHB files/DNGN6C.MCB
	files/DNGN6K.CHB files/DNGN6K.MCB files/DNGN7C.CHB
	files/DNGN7C.MCB files/DNGN7K.CHB files/DNGN7K.MCB
	files/DOCA.CHR files/DOCA.PAK files/DOCA1C.CHB
	files/DOCA1C.MCB files/DOCA1K.CHB files/DOCA1K.MCB
	files/DOCA2C.CHB files/DOCA2C.MCB files/DOCA2E.CHB
	files/DOCA2E.MCB files/DOCA2K.CHB files/DOCA2K.MCB
	files/DODO.CHR files/DODO.PAK files/DODO.SND
	files/DOHDOH.PRG files/DOMI1C.CHB files/DOMI1C.MCB
	files/DOMI1K.CHB files/DOMI1K.MCB files/DOMI2C.CHB
	files/DOMI2C.MCB files/DOMI2E.CHB files/DOMI2E.MCB
	files/DOMI2K.CHB files/DOMI2K.MCB files/DOPELGEN.SND
	files/DR1T.CHR files/DR1T.PAK files/DRAG1E.CHB
	files/DRAG1E.MCB files/DRAGON.PRG files/DRAGON.SND
	files/DRA_NAKU.SND files/DREK.CHR files/DREK.PAK
	files/DRGN.CHR files/DRGN.PAK files/DRGR.CHR
	files/DRGR.PAK files/DRGR1C.CHB files/DRGR1C.MCB
	files/DRGR1K.CHB files/DRGR1K.MCB files/DRGR2C.CHB
	files/DRGR2C.MCB files/DRGR2K.CHB files/DRGR2K.MCB
	files/DWAF.CHR files/DWAF.PAK files/D_ICE.CHR
	files/D_ICE.PAK files/EDED.CHR files/EDED.PAK
	files/EDED1C.CHB files/EDED1C.MCB files/EDED1E.CHB
	files/EDED1E.MCB files/EDED1K.MCB files/EDED1KA.CHB
	files/EDIV.CHR files/EDIV.PAK files/EDIV_1.CHR
	files/EDIV_1.PAK files/EDIV_2.CHR files/EDIV_2.PAK
	files/EDIV_3.CHR files/EDIV_3.PAK files/EDOW.CHR
	files/EDOW.PAK files/EDO_DP_D.SND files/EDO_DP_G.SND
	files/EDPO1C.CHB files/EDPO1C.MCB files/EDPO1E.CHB
	files/EDPO1E.MCB files/EDPO1K.CHB files/EDPO1K.MCB
	files/EDW_VO.SND files/EE0C.CHR files/EE0C.PAK
	files/EE1C.CHR files/EE1C.PAK files/EE1R.CHR
	files/EE1R.PAK files/EE2C.CHR files/EE2C.PAK
	files/EERIEL.PRG files/EGNS.CHR files/EGNS.PAK
	files/EGRAD.PRG files/ENDING.SND files/EPRO.CHR
	files/EPRO.PAK files/EREL0C.CHB files/EREL0C.MCB
	files/EREL0E.CHB files/EREL0E.MCB files/EREL0K.CHB
	files/EREL0K.MCB files/EREL1C.CHB files/EREL1C.MCB
	files/EREL1EA.CHB files/EREL1EA.MCB files/EREL1EB.CHB
	files/EREL1EB.MCB files/EREL1K.CHB files/EREL1K.MCB
	files/EREL2C.CHB files/EREL2C.MCB files/EREL2E.CHB
	files/EREL2E.MCB files/EREL2K.CHB files/EREL2K.MCB
	files/ERIL.CHR files/ERIL.PAK files/EVAS3C.MCB
	files/EVAS3K.MCB files/EVBK1C.MCB files/EVBK1E.MCB
	files/EVBK1K.MCB files/EVCV.CHR files/EVCV.PAK
	files/EVDR2C.MCB files/EVDR2K.MCB files/EVEE.CHR
	files/EVEE.PAK files/EVEE0C.MCB files/EVEE0E.MCB
	files/EVEE0K.MCB files/EVEE2C.MCB files/EVEE2E.MCB
	files/EVEE2K.MCB files/EVGS1C.MCB files/EVGS1E.MCB
	files/EVGS1K.MCB files/EVHN2C.MCB files/EVHN2E.MCB
	files/EVHN2K.MCB files/EVJE.CHR files/EVJE.PAK
	files/EVKG2C.MCB files/EVKG2E.MCB files/EVKG2K.MCB
	files/EVKV1C.MCB files/EVKV1K.MCB files/EVMO1E.MCB
	files/EVMO2C.MCB files/EVMO2E.MCB files/EVMO2K.MCB
	files/EVMR1K1.MCB files/EVMR1K2.MCB files/EVMR2C.MCB
	files/EVMR2E.MCB files/EVMR2K.MCB files/EVMR3C.MCB
	files/EVMR3E.MCB files/EVMR3K.MCB files/EVNE.CHR
	files/EVNE.PAK files/EVNW1C.MCB files/EVNW1K.MCB
	files/EVPP2C.MCB files/EVPP2E.MCB files/EVPP2K.MCB
	files/EVPR1C.MCB files/EVPR1K.MCB files/EVPR3C.MCB
	files/EVPR3E.MCB files/EVPR3K.MCB files/EVRC1C.MCB
	files/EVRC1E.MCB files/EVRC1K.MCB files/EVRN2C.MCB
	files/EVRN2K.MCB files/EVSA1E.MCB files/EVSA1K.MCB
	files/EVSG1C.MCB files/EVSG1K.MCB files/EVSG2C.MCB
	files/EVSG2K.MCB files/EVSG3C.MCB files/EVSG3E.MCB
	files/EVSG3K.MCB files/EVSI1C.MCB files/EVSI1K.MCB
	files/EVSM2C.MCB files/EVSM4E.MCB files/EVSM5K.MCB
	files/EVSM5KA.CHB files/EVTN2C.MCB files/EVTN2K.MCB
	files/EVVD1C.MCB files/EVVD1C3F.MCB files/EVVD1E_0.MCB
	files/EVVD1K.MCB files/EVVD1K1.MCB files/EVVD1K2.MCB
	files/EVVD1K3.MCB files/EVVD1K6.MCB files/EVVD2C1F.MCB
	files/EVVD2C2F.MCB files/EVVD2C8.MCB files/EVVD2ED4.CHB
	files/EVVD2ED4.MCB files/EVVD2EE4.MCB files/EVVD2K1F.MCB
	files/EVVD2K2F.MCB files/EVVD2K8.MCB files/EVVD3C2F.MCB
	files/EVVD3C4F.MCB files/EVVD3K2.MCB files/EVVD3K4.MCB
	files/EVVD6C7F.MCB files/EVVD6K7F.MCB files/EVVG1C4F.MCB
	files/EVVG1K4F.MCB files/EVWN1C.MCB files/EVWN1K.MCB
	files/EYE.SND files/E_EDW_VO.SND files/E_EX.CHR
	files/E_EX.PAK files/E_GEP_VO.SND files/E_GRA_VO.SND
	files/E_PRO_VO.SND files/FI1C.CHR files/FI1C.PAK
	files/FLOGER.SND files/FLOWER.CHR files/FLOWER.PAK
	files/FONT.CHR files/FROG.CHR files/FROG.PAK
	files/FROG.PRG files/FROGP1.CHR files/FROGP1.PAK
	files/FRSU1C.CHB files/FRSU1C.MCB files/FRSU1E.CHB
	files/FRSU1E.MCB files/FRSU1K.CHB files/FRSU1K.MCB
	files/FS1C.CHR files/FS1C.PAK files/FUIN1C.CHB
	files/FUIN1C.MCB files/FUIN1E.CHB files/FUIN1E.MCB
	files/FUIN1K.CHB files/FUIN1K.MCB files/GAIA.CHR
	files/GAIA.PAK files/GB1T.CHR files/GB1T.PAK
	files/GBEL.CHR files/GBEL.PAK files/GBLN.CHR
	files/GBLN.PAK files/GBMK.CHR files/GBMK.PAK
	files/GBMK1C.CHB files/GBMK1C.MCB files/GBMK1E.CHB
	files/GBMK1E.MCB files/GBMK1K.CHB files/GBMK1K.MCB
	files/GDES1C.CHB files/GDES1C.MCB files/GDES1E.CHB
	files/GDES1E.MCB files/GDES1K.CHB files/GDES1K.MCB
	files/GHOST.PRG files/GHOST.SND files/GILD.CHR
	files/GILD.PAK files/GL1C.CHR files/GL1C.PAK
	files/GNSN0C.CHB files/GNSN0C.MCB files/GNSN0E.CHB
	files/GNSN0E.MCB files/GNSN0K.CHB files/GNSN0K.MCB
	files/GNSN1C.CHB files/GNSN1C.MCB files/GNSN1EA.CHB
	files/GNSN1EA.MCB files/GNSN1EB.CHB files/GNSN1EB.MCB
	files/GNSN1K.CHB files/GNSN1K.MCB files/GOAT1E.CHB
	files/GOAT1E.MCB files/GOBLIN.PRG files/GOBRIN.SND
	files/GODA.CHR files/GODA.PAK files/GODY.CHR
	files/GODY.PAK files/GOLD1C.CHB files/GOLD1C.MCB
	files/GOLD1E.CHB files/GOLD1E.MCB files/GOLD1K.CHB
	files/GOLD1K.MCB files/GOLD2C.CHB files/GOLD2C.MCB
	files/GOLD2K.CHB files/GOLD2K.MCB files/GORO.CHR
	files/GORO.PAK files/GOST.CHR files/GOST.PAK
	files/GOST2P.CHR files/GOST2P.PAK files/GRAD.CHR
	files/GRAD.PAK files/GRAD.PRG files/GRADP2.CHR
	files/GRADP2.PAK files/GRADP4.CHR files/GRADP4.PAK
	files/GRADP5.CHR files/GRADP5.PAK files/GRA_VO.SND
	files/GRDP.CHR files/GRDP.PAK files/GRIF.CHR
	files/GRIF.PAK files/GRIFON.PRG files/GS0C.CHR
	files/GS0C.PAK files/GS1C.CHR files/GS1C.PAK
	files/GS1R.CHR files/GS1R.PAK files/GURIFON.SND
	files/G_EX.CHR files/G_EX.PAK files/HEAD.CHR
	files/HEAD.PAK files/HIND.CHR files/HIND.PAK
	files/HIND.PRG files/HIND1E.CHB files/HIND1E.MCB
	files/HIND1K.CHB files/HIND1K.MCB files/HIND2C.CHB
	files/HIND2C.MCB files/HIND2E.CHB files/HIND2E.MCB
	files/HIND2K.CHB files/HIND2K.MCB files/HN1C.CHR
	files/HN1C.PAK files/HON1.CHR files/HON1.PAK
	files/HOTL1E.CHB files/HOTL1E.MCB files/HOTL2E.CHB
	files/HOTL2E.MCB files/HOTL3E.CHB files/HOTL3E.MCB
	files/HOUS1E.CHB files/HOUS1E.MCB files/HOUS2E.CHB
	files/HOUS2E.MCB files/HOUS3E.CHB files/HOUS3E.MCB
	files/HOUS4E.CHB files/HOUS4E.MCB files/HOUS5E.CHB
	files/HOUS5E.MCB files/HOUS6E.CHB files/HOUS6E.MCB
	files/HU_BGLD.CHR files/HU_BGLD.PAK files/ICED.PRG
	files/ITEM.CHR files/ITEM.PAK files/ITEM1E.CHB
	files/ITEM1E.MCB files/IVGR.CHR files/IVGR.PAK
	files/IVKN.CHR files/IVKN.PAK files/JADS.CHR
	files/JADS.PAK files/JESTELF2.CHR files/JESTELF2.PAK
	files/JESTELFA.CHR files/JESTELFA.PAK files/JESTONLY.CHR
	files/JESTONLY.PAK files/JWEL1E.CHB files/JWEL1E.MCB
	files/KAEL1C.CHB files/KAEL1C.MCB files/KAEL1K.CHB
	files/KAEL1K.MCB files/KAGE.CHR files/KAGE.PAK
	files/KAGE.PRG files/KAGE0C.CHB files/KAGE0C.MCB
	files/KAGE0E.CHB files/KAGE0E.MCB files/KAGE0K.CHB
	files/KAGE0K.MCB files/KAGE1C.CHB files/KAGE1C.MCB
	files/KAGE1EA.CHB files/KAGE1EA.MCB files/KAGE1EB.CHB
	files/KAGE1EB.MCB files/KAGE1K.CHB files/KAGE1K.MCB
	files/KAGE2C.CHB files/KAGE2C.MCB files/KAGE2E.CHB
	files/KAGE2E.MCB files/KAGE2K.CHB files/KAGE2K.MCB
	files/KAGEOU.SND files/KAIZOKU.SND files/KAJI1E.CHB
	files/KAJI1E.MCB files/KANJI.BIN files/KD1T.CHR
	files/KD1T.PAK files/KD2T.CHR files/KD2T.PAK
	files/KD3T.CHR files/KD3T.PAK files/KDV1.CHR
	files/KDV1.PAK files/KDV2.CHR files/KDV2.PAK
	files/KDV3.CHR files/KDV3.PAK files/KDVD1C.CHB
	files/KDVD1C.MCB files/KDVD1K.CHB files/KDVD1K.MCB
	files/KDVD2C.CHB files/KDVD2C.MCB files/KDVD2K.CHB
	files/KDVD2K.MCB files/KDVD3C.CHB files/KDVD3C.MCB
	files/KDVD3K.CHB files/KDVD3K.MCB files/KENT.CHR
	files/KENT.PAK files/KG0C.CHR files/KG0C.PAK
	files/KG1C.CHR files/KG1C.PAK files/KG1R.CHR
	files/KG1R.PAK files/KG2C.CHR files/KG2C.PAK
	files/KING1E.CHB files/KING1E.MCB files/KIZO.CHR
	files/KIZO.PAK files/KNIGHT.PRG files/KUBOTA.CHR
	files/KUBOTA.PAK files/KUMO.CHR files/KUMO.PAK
	files/KUMO.PRG files/KUROUNE.CHR files/KUROUNE.PAK
	files/K_AMORI.SND files/K_DOKUTU.SND files/K_DOWERF.SND
	files/K_FILE.SND files/K_FMORI.SND files/K_FUNE.SND
	files/K_HAKABA.SND files/K_IWABA.SND files/K_KMORI.SND
	files/K_MAKAI.SND files/K_MIZUUM.SND files/K_SOUGEN.SND
	files/K_TOU.SND files/K_UMI.SND files/LARV.CHR
	files/LARV.PAK files/LARV.PRG files/LAST.CHR
	files/LAST.PAK files/LAST1C.CHB files/LAST1C.MCB
	files/LAST1E.CHB files/LAST1E.MCB files/LAST1K.CHB
	files/LAST1K.MCB files/LASW2.CHR files/LASW2.PAK
	files/LGLB.CHR files/LGLB.PAK files/LGLB1C.CHB
	files/LGLB1C.MCB files/LGLB1K.CHB files/LGLB1K.MCB
	files/LGLB2C.CHB files/LGLB2C.MCB files/LGLB2K.CHB
	files/LGLB2K.MCB files/LION1E.CHB files/LION1E.MCB
	files/LL1T.CHR files/LL1T.PAK files/LOCO.CHR
	files/LOCO.PAK files/MAID.CHR files/MAID.PAK
	files/MAJO.CHR files/MAJO.PAK files/MAJO1C.CHB
	files/MAJO1C.MCB files/MAJO1E.CHB files/MAJO1E.MCB
	files/MAJO1K.CHB files/MAJO1K.MCB files/MAJO2E.CHB
	files/MAJO2E.MCB files/MAOH0C.CHB files/MAOH0C.MCB
	files/MAOH0E.CHB files/MAOH0E.MCB files/MAOH0K.CHB
	files/MAOH0K.MCB files/MAOH1C.CHB files/MAOH1C.MCB
	files/MAOH1EA.CHB files/MAOH1EA.MCB files/MAOH1EB.CHB
	files/MAOH1EB.MCB files/MAOH1K.CHB files/MAOH1K.MCB
	files/MAOH2C.CHB files/MAOH2C.MCB files/MAOH2E.CHB
	files/MAOH2E.MCB files/MAOH2K.CHB files/MAOH2K.MCB
	files/MAOU.CHR files/MAOU.PAK files/MAP_ASTL.MPB
	files/MAP_BKNG.MPB files/MAP_DNFN.MPB files/MAP_EREL.MPB
	files/MAP_FRUS.MPB files/MAP_FUIN.MPB files/MAP_GAKA.MPB
	files/MAP_GNSN.MPB files/MAP_GOLD.MPB files/MAP_HIND.MPB
	files/MAP_KAGE.MPB files/MAP_MAOH.MPB files/MAP_MORI.MPB
	files/MAP_OLD.MPB files/MAP_SOEA.MPB files/MAP_SOLD.MPB
	files/MAP_SORD.MPB files/MAP_SOWE.MPB files/MAP_VDDG.MPB
	files/MAP_VLDN.MPB files/MAP_VLGA.MPB files/MAP_WIZA.MPB
	files/MAP_YGDS.MPB files/METE.CHR files/METE.PAK
	files/MIKONID.SND files/MIX1E.CHB files/MIX1E.MCB
	files/MJ1C.CHR files/MJ1C.PAK files/MO0C.CHR
	files/MO0C.PAK files/MO1C.CHR files/MO1C.PAK
	files/MO1R.CHR files/MO1R.PAK files/MO2C.CHR
	files/MO2C.PAK files/MORI1C.CHB files/MORI1C.MCB
	files/MORI1E.CHB files/MORI1E.MCB files/MORI1K.CHB
	files/MORI1K.MCB files/MORI2C.CHB files/MORI2C.MCB
	files/MORI2E.CHB files/MORI2E.MCB files/MORI2K.CHB
	files/MORI2K.MCB files/MORI3C.CHB files/MORI3C.MCB
	files/MORI3E.CHB files/MORI3E.MCB files/MORI3K.CHB
	files/MORI3K.MCB files/MR1C.CHR files/MR1C.PAK
	files/MUR1.CHR files/MUR1.PAK files/MUR4.CHR
	files/MUR4.PAK files/MUR6.CHR files/MUR6.PAK
	files/MUR7.CHR files/MUR7.PAK files/MUR8.CHR
	files/MUR8.PAK files/MUR9.CHR files/MUR9.PAK
	files/MURA.CHR files/MURA.PAK files/MURC.CHR
	files/MURC.PAK files/MURD.CHR files/MURD.PAK
	files/MYCO.CHR files/MYCO.PAK files/MYCO.PRG
	files/NB1T.CHR files/NB1T.PAK files/NE1T.CHR
	files/NE1T.PAK files/NECR.CHR files/NECR.PAK
	files/NECRO.PRG files/NELT.CHR files/NELT.PAK
	files/NELT1C.CHB files/NELT1C.MCB files/NELT1K.CHB
	files/NELT1K.MCB files/NIGHT.SND files/NIGT_GOL.SND
	files/NING.CHR files/NING.PAK files/NISE.CHR
	files/NISE.PAK files/NISE.PRG files/NISEPO.SND
	files/NTBL.CHR files/NTBL.PAK files/NTBL1C.CHB
	files/NTBL1C.MCB files/NTBL1K.CHB files/NTBL1K.MCB
	files/OBAA.CHR files/OBAA.PAK files/OD1C.CHR
	files/OD1C.PAK files/OLD1C.CHB files/OLD1C.MCB
	files/OLD1E.CHB files/OLD1E.MCB files/OLD1K.CHB
	files/OLD1K.MCB files/ONDN2C.CHB files/ONDN2C.MCB
	files/ONDN2K.CHB files/ONDN2K.MCB files/OPENING0.EVN
	files/OP_EDOW.EVN files/OP_GRAD0.EVN files/OP_HIND.EVN
	files/OP_PORT.EVN files/OP_PURO.EVN files/PC_ABS.TXT
	files/PC_BIB.TXT files/PC_CPY.TXT files/PEOP_DAT.PRG
	files/PH1C.CHR files/PH1C.PAK files/PIRA.CHR
	files/PIRA.PAK files/PIRA_C.CHR files/PIRA_C.PAK
	files/PIRA_N.CHR files/PIRA_N.PAK files/PKBN_C.CHR
	files/PKBN_C.PAK files/PKBN_N.CHR files/PKBN_N.PAK
	files/POLT.CHR files/POLT.PAK files/POLT.PRG
	files/POLTER.SND files/PORT.CHR files/PORT.PAK
	files/PORT1C.CHB files/PORT1C.MCB files/PORT1K.CHB
	files/PORT1K.MCB files/PORT1P.CHR files/PORT1P.PAK
	files/PORT2C.CHB files/PORT2C.MCB files/PORT2K.CHB
	files/PORT2K.MCB files/PORT2P.CHR files/PORT2P.PAK
	files/PORT3C.CHB files/PORT3C.MCB files/PORT3E.CHB
	files/PORT3E.MCB files/PORT3K.CHB files/PORT3K.MCB
	files/PORT4K.CHB files/PORT4K.MCB files/PORT4P.CHR
	files/PORT4P.PAK files/POT_VO.SND files/PP1C.CHR
	files/PP1C.PAK files/PP2C.CHR files/PP2C.PAK
	files/PPHU1C.CHB files/PPHU1C.MCB files/PPHU1E.CHB
	files/PPHU1E.MCB files/PPHU1K.CHB files/PPHU1K.MCB
	files/PRAD.CHR files/PRAD.PAK files/PROS.PRG
	files/PRO_VO.SND files/PSPN1C.CHB files/PSPN1C.MCB
	files/PSPN1E.CHB files/PSPN1E.MCB files/PSPN1K.CHB
	files/PSPN1K.MCB files/PSPN2C.CHB files/PSPN2C.MCB
	files/PSPN2E.CHB files/PSPN2E.MCB files/PSPN2K.CHB
	files/PSPN2K.MCB files/PUPPET.PRG files/PURO.CHR
	files/PURO.PAK files/PUROP2.CHR files/PUROP2.PAK
	files/PUROP3.CHR files/PUROP3.PAK files/PUROP5.CHR
	files/PUROP5.PAK files/P_EX.CHR files/P_EX.PAK
	files/RALVA.SND files/RC1C.CHR files/RC1C.PAK
	files/RIYON.SND files/RL1T.CHR files/RL1T.PAK
	files/RLND.CHR files/RLND.PAK files/RLND1C.CHB
	files/RLND1C.MCB files/RLND1K.CHB files/RLND1K.MCB
	files/RLND2C.CHB files/RLND2C.MCB files/RLND2K.CHB
	files/RLND2K.MCB files/ROBE.CHR files/ROBE.PAK
	files/ROCK1C.CHB files/ROCK1C.MCB files/ROCK1E.CHB
	files/ROCK1E.MCB files/ROCK1K.CHB files/ROCK1K.MCB
	files/ROCK2C.CHB files/ROCK2C.MCB files/ROCK2E.CHB
	files/ROCK2E.MCB files/ROCK2K.CHB files/ROCK2K.MCB
	files/ROCK3C.CHB files/ROCK3C.MCB files/ROCK3E.CHB
	files/ROCK3E.MCB files/ROCK3K.CHB files/ROCK3K.MCB
	files/ROLL.CHR files/ROLL.PAK files/RYON.CHR
	files/RYON.PAK files/RYON.PRG files/RYO_EDO.SND
	files/RYO_POT.SND files/SAGN.CHR files/SAGN.PAK
	files/SAGN1C.CHB files/SAGN1C.MCB files/SAGN1K.CHB
	files/SAGN1K.MCB files/SAGN2C.CHB files/SAGN2C.MCB
	files/SAGN2K.CHB files/SAGN2K.MCB files/SAGN3C.CHB
	files/SAGN3C.MCB files/SAGN3E.CHB files/SAGN3E.MCB
	files/SAGN3K.CHB files/SAGN3K.MCB files/SAMANSA.SND
	files/SCHL1E.CHB files/SCHL1E.MCB files/SDDRVS.TSK
	files/SDOL.CHR files/SDOL.PAK files/SDOLONLY.CHR
	files/SDOLONLY.PAK files/SE1C.CHR files/SE1C.PAK
	files/SELECT.EVN files/SEME.CHR files/SEME.PAK
	files/SEME1C.CHB files/SEME1C.MCB files/SEME1E.CHB
	files/SEME1E.MCB files/SEME1K.CHB files/SEME1K.MCB
	files/SEME2C.CHB files/SEME2C.MCB files/SEME2E.CHB
	files/SEME2E.MCB files/SEME2K.CHB files/SEME2K.MCB
	files/SEME3C.CHB files/SEME3C.MCB files/SEME3K.CHB
	files/SEME3K.MCB files/SEME4E.CHB files/SEME4E.MCB
	files/SENS.CHR files/SENS.PAK files/SENSONLY.CHR
	files/SENSONLY.PAK files/SG1T.CHR files/SG1T.PAK
	files/SHIP1C.CHB files/SHIP1C.MCB files/SHIP1E.CHB
	files/SHIP1E.MCB files/SHIP1K.CHB files/SHIP1K.MCB
	files/SHIP2C.CHB files/SHIP2C.MCB files/SI1C.CHR
	files/SI1C.PAK files/SIRENE.PRG files/SIRENE.SND
	files/SKUL.CHR files/SKUL.PAK files/SKUL.PRG
	files/SKUL_4.CHR files/SKUL_4.PAK files/SLAM1E.CHB
	files/SLAM1E.MCB files/SLAM1K.CHB files/SLAM1K.MCB
	files/SLAV.CHR files/SLAV.PAK files/SLCT.CHR
	files/SLCT.PAK files/SLCT1E.CHB files/SLCT1E.MCB
	files/SLIM.CHR files/SLIM.PAK files/SLIME.PRG
	files/SLMD.CHR files/SLMD.PAK files/SM1T.CHR
	files/SM1T.PAK files/SOAD1E.CHB files/SOAD1E.MCB
	files/SOAD1K.CHB files/SOAD1K.MCB files/SOAD2C.CHB
	files/SOAD2C.MCB files/SOAD2E.CHB files/SOAD2E.MCB
	files/SOAD2K.CHB files/SOAD2K.MCB files/SOEA1C.CHB
	files/SOEA1C.MCB files/SOEA1E.CHB files/SOEA1E.MCB
	files/SOEA1K.CHB files/SOEA1K.MCB files/SOLB.CHR
	files/SOLB.PAK files/SOLDD.CHR files/SOLDD.PAK
	files/SOUNDDAT.BIN files/SOWE1C.CHB files/SOWE1C.MCB
	files/SOWE1E.CHB files/SOWE1E.MCB files/SOWE1K.CHB
	files/SOWE1K.MCB files/SPEL1E.CHB files/SPEL1E.MCB
	files/SR1C.CHR files/SR1C.PAK files/SR2C.CHR
	files/SR2C.PAK files/SRIME.SND files/STG_DAT2.PRG
	files/SW1C.CHR files/SW1C.PAK files/S_DANSIO.SND
	files/S_DOKUTU.SND files/S_ERIEL.SND files/S_FILESL.SND
	files/S_FUNE.SND files/S_HAKABA.SND files/S_KAGEOU.SND
	files/S_MAOU.SND files/S_MIZU.SND files/S_MORI.SND
	files/S_SIRO.SND files/S_TOROKO.SND files/TARNCYRA.SND
	files/TN1C.CHR files/TN1C.PAK files/TORO1C.CHB
	files/TORO1C.MCB files/TORO1E.CHB files/TORO1E.MCB
	files/TORO1K.CHB files/TORO1K.MCB files/TORO2K.MCB
	files/TORO3K.MCB files/TORUNEKO.CHR files/TORUNEKO.PAK
	files/TROC.CHR files/TROC.PAK files/TTNA1C.CHB
	files/TTNA1C.MCB files/TTNA1E.CHB files/TTNA1E.MCB
	files/TTNA1K.CHB files/TTNA1K.MCB files/TTNA2C.CHB
	files/TTNA2C.MCB files/TTNA2K.CHB files/TTNA2K.MCB
	files/UMA1.CHR files/UMA1.PAK files/UNDEAD.SND
	files/UNIC.CHR files/UNIC.PAK files/VD11.CHR
	files/VD11.PAK files/VD12.CHR files/VD12.PAK
	files/VD13.CHR files/VD13.PAK files/VD14.CHR
	files/VD14.PAK files/VD15.CHR files/VD15.PAK
	files/VD16.CHR files/VD16.PAK files/VD17.CHR
	files/VD17.PAK files/VD18.CHR files/VD18.PAK
	files/VD1R.CHR files/VD1R.PAK files/VD1T.CHR
	files/VD1T.PAK files/VD2R.CHR files/VD2R.PAK
	files/VD4R.CHR files/VD4R.PAK files/VG11.CHR
	files/VG11.PAK files/VG15.CHR files/VG15.PAK
	files/VG1R.CHR files/VG1R.PAK files/VG1T.CHR
	files/VG1T.PAK files/VG2R.CHR files/VG2R.PAK
	files/VLDN.CHR files/VLDN.PAK files/VLDN1C1F.CHB
	files/VLDN1C1F.MCB files/VLDN1C2F.CHB files/VLDN1C2F.MCB
	files/VLDN1C3F.CHB files/VLDN1C3F.MCB files/VLDN1C4F.CHB
	files/VLDN1C4F.MCB files/VLDN1C7F.CHB files/VLDN1C7F.MCB
	files/VLDN1C8F.CHB files/VLDN1C8F.MCB files/VLDN1EA1.CHB
	files/VLDN1EA1.MCB files/VLDN1EA2.CHB files/VLDN1EA2.MCB
	files/VLDN1EA4.CHB files/VLDN1EA4.MCB files/VLDN1EB2.CHB
	files/VLDN1EB2.MCB files/VLDN1EB4.CHB files/VLDN1EB4.MCB
	files/VLDN1EC2.CHB files/VLDN1EC2.MCB files/VLDN1EC4.CHB
	files/VLDN1EC4.MCB files/VLDN1ED4.CHB files/VLDN1ED4.MCB
	files/VLDN1EE4.CHB files/VLDN1EE4.MCB files/VLDN1EF4.CHB
	files/VLDN1EF4.MCB files/VLDN1EG4.CHB files/VLDN1EG4.MCB
	files/VLDN1K1F.CHB files/VLDN1K1F.MCB files/VLDN1K2F.CHB
	files/VLDN1K2F.MCB files/VLDN1K3F.CHB files/VLDN1K3F.MCB
	files/VLDN1K4F.CHB files/VLDN1K4F.MCB files/VLDN1K5F.CHB
	files/VLDN1K5F.MCB files/VLDN1K6F.CHB files/VLDN1K6F.MCB
	files/VLDN1K7F.CHB files/VLDN1K7F.MCB files/VLDN1K8F.CHB
	files/VLDN1K8F.MCB files/VLDN2C1F.CHB files/VLDN2C1F.MCB
	files/VLDN2C2F.CHB files/VLDN2C2F.MCB files/VLDN2C3F.CHB
	files/VLDN2C3F.MCB files/VLDN2C4F.CHB files/VLDN2C4F.MCB
	files/VLDN2C8F.CHB files/VLDN2C8F.MCB files/VLDN2EA1.CHB
	files/VLDN2EA1.MCB files/VLDN2EA2.CHB files/VLDN2EA2.MCB
	files/VLDN2EA4.CHB files/VLDN2EA4.MCB files/VLDN2EB2.CHB
	files/VLDN2EB2.MCB files/VLDN2EB4.CHB files/VLDN2EB4.MCB
	files/VLDN2EC2.CHB files/VLDN2EC2.MCB files/VLDN2EC4.CHB
	files/VLDN2EC4.MCB files/VLDN2ED4.CHB files/VLDN2ED4.MCB
	files/VLDN2EE4.CHB files/VLDN2EE4.MCB files/VLDN2EF4.CHB
	files/VLDN2EF4.MCB files/VLDN2EG4.CHB files/VLDN2EG4.MCB
	files/VLDN2K1F.CHB files/VLDN2K1F.MCB files/VLDN2K2F.CHB
	files/VLDN2K2F.MCB files/VLDN2K3F.CHB files/VLDN2K3F.MCB
	files/VLDN2K4F.CHB files/VLDN2K4F.MCB files/VLDN2K5F.CHB
	files/VLDN2K5F.MCB files/VLDN2K6F.CHB files/VLDN2K6F.MCB
	files/VLDN2K8F.CHB files/VLDN2K8F.MCB files/VLDN3C1F.CHB
	files/VLDN3C1F.MCB files/VLDN3C2F.CHB files/VLDN3C2F.MCB
	files/VLDN3C3F.CHB files/VLDN3C3F.MCB files/VLDN3C4F.CHB
	files/VLDN3C4F.MCB files/VLDN3C8F.CHB files/VLDN3C8F.MCB
	files/VLDN3EG4.CHB files/VLDN3EG4.MCB files/VLDN3K1F.CHB
	files/VLDN3K1F.MCB files/VLDN3K2F.CHB files/VLDN3K2F.MCB
	files/VLDN3K3F.CHB files/VLDN3K3F.MCB files/VLDN3K4F.CHB
	files/VLDN3K4F.MCB files/VLDN3K5F.CHB files/VLDN3K5F.MCB
	files/VLDN3K8F.CHB files/VLDN3K8F.MCB files/VLDN4C1F.CHB
	files/VLDN4C1F.MCB files/VLDN4C2F.CHB files/VLDN4C2F.MCB
	files/VLDN4C3F.CHB files/VLDN4C3F.MCB files/VLDN4C4F.CHB
	files/VLDN4C4F.MCB files/VLDN4C8F.CHB files/VLDN4C8F.MCB
	files/VLDN4K1F.CHB files/VLDN4K1F.MCB files/VLDN4K2F.CHB
	files/VLDN4K2F.MCB files/VLDN4K3F.CHB files/VLDN4K3F.MCB
	files/VLDN4K4F.CHB files/VLDN4K4F.MCB files/VLDN4K5F.CHB
	files/VLDN4K5F.MCB files/VLDN4K8F.CHB files/VLDN4K8F.MCB
	files/VLDN5C1F.CHB files/VLDN5C1F.MCB files/VLDN5C2F.CHB
	files/VLDN5C2F.MCB files/VLDN5C3F.CHB files/VLDN5C3F.MCB
	files/VLDN5C4F.CHB files/VLDN5C4F.MCB files/VLDN6C1F.CHB
	files/VLDN6C1F.MCB files/VLDN6C2F.CHB files/VLDN6C2F.MCB
	files/VLDN6C3F.CHB files/VLDN6C3F.MCB files/VLDN6C4F.CHB
	files/VLDN6C4F.MCB files/VLDN6C7F.CHB files/VLDN6C7F.MCB
	files/VLDN6EA1.CHB files/VLDN6EA1.MCB files/VLDN6EA2.CHB
	files/VLDN6EA2.MCB files/VLDN6EA4.CHB files/VLDN6EA4.MCB
	files/VLDN6EB2.CHB files/VLDN6EB2.MCB files/VLDN6EB4.CHB
	files/VLDN6EB4.MCB files/VLDN6EC2.CHB files/VLDN6EC2.MCB
	files/VLDN6EC4.CHB files/VLDN6EC4.MCB files/VLDN6ED4.CHB
	files/VLDN6ED4.MCB files/VLDN6EE4.CHB files/VLDN6EE4.MCB
	files/VLDN6EF4.CHB files/VLDN6EF4.MCB files/VLDN6EG4.CHB
	files/VLDN6EG4.MCB files/VLDN6K1F.CHB files/VLDN6K1F.MCB
	files/VLDN6K2F.CHB files/VLDN6K2F.MCB files/VLDN6K3F.CHB
	files/VLDN6K3F.MCB files/VLDN6K4F.CHB files/VLDN6K4F.MCB
	files/VLDN6K5F.CHB files/VLDN6K5F.MCB files/VLDN6K6F.CHB
	files/VLDN6K6F.MCB files/VLDN6K7F.CHB files/VLDN6K7F.MCB
	files/VLDN7C.CHB files/VLDN7C.MCB files/VLDN7K.CHB
	files/VLDN7K.MCB files/VLDN8C.CHB files/VLDN8C.MCB
	files/VLDN8K.CHB files/VLDN8K.MCB files/VLGA.CHR
	files/VLGA.PAK files/VLGA1C1F.CHB files/VLGA1C1F.MCB
	files/VLGA1C2F.CHB files/VLGA1C2F.MCB files/VLGA1C3F.CHB
	files/VLGA1C3F.MCB files/VLGA1C4F.CHB files/VLGA1C4F.MCB
	files/VLGA1C5F.CHB files/VLGA1C5F.MCB files/VLGA1EA1.CHB
	files/VLGA1EA1.MCB files/VLGA1EA2.CHB files/VLGA1EA2.MCB
	files/VLGA1EA3.CHB files/VLGA1EA3.MCB files/VLGA1EB1.CHB
	files/VLGA1EB1.MCB files/VLGA1EB2.CHB files/VLGA1EB2.MCB
	files/VLGA1EC2.CHB files/VLGA1EC2.MCB files/VLGA1K1F.CHB
	files/VLGA1K1F.MCB files/VLGA1K2F.CHB files/VLGA1K2F.MCB
	files/VLGA1K3F.CHB files/VLGA1K3F.MCB files/VLGA1K4F.CHB
	files/VLGA1K4F.MCB files/VLGA1K5F.CHB files/VLGA1K5F.MCB
	files/VLGA2C.CHB files/VLGA2C.MCB files/VLGA2K.CHB
	files/VLGA2K.MCB files/VOLG.CHR files/VOLG.PAK
	files/VORG.PRG files/VORGRODO.SND files/WAKAPO.CHR
	files/WAKAPO.PAK files/WARP1E.CHB files/WARP1E.MCB
	files/WARUERU.SND files/WARUGODA.SND files/WGOD.CHR
	files/WGOD.PAK files/WGOD.PRG files/WINE.CHR
	files/WINE.PAK files/WINE1C.CHB files/WINE1C.MCB
	files/WINE1K.CHB files/WINE1K.MCB files/WINE2C.CHB
	files/WINE2C.MCB files/WINE2E.CHB files/WINE2E.MCB
	files/WINE2K.CHB files/WINE2K.MCB files/WIZA1E.CHB
	files/WIZA1E.MCB files/WIZA2E.CHB files/WIZA2E.MCB
	files/WN1C.CHR files/WN1C.PAK files/WN2C.CHR
	files/WN2C.PAK files/YA0C.CHR files/YA0C.PAK
	files/YA1C.CHR files/YA1C.PAK files/YA1R.CHR
	files/YA1R.PAK files/YA_A.CHR files/YA_A.PAK
	files/YA_B.CHR files/YA_B.PAK files/YD1C.CHR
	files/YD1C.PAK files/YGDS1C.CHB files/YGDS1C.MCB
	files/YGDS1E.CHB files/YGDS1E.MCB files/YGDS1K.CHB
	files/YGDS1K.MCB files/YKAT0C.CHB files/YKAT0C.MCB
	files/YKAT0K.CHB files/YKAT0K.MCB files/YKAT1C.CHB
	files/YKAT1C.MCB files/YKAT1E.CHB files/YKAT1E.MCB
	files/YKAT1K.CHB files/YKAT1K.MCB files/YKAT2C.CHB
	files/YKAT2C.MCB files/YOUSEI.CHR files/YOUSEI.PAK
	files/ZOMBIE.PRG files/ZONB.CHR files/ZONB.PAK
)

set(evnfiles)
set(evncopyfiles)
foreach(_file ${evn_SOURCES})
	#get_filename_component(_fileext ${_file} EXT)
        #if (${_fileext} MATCHES ".EVN")
		get_filename_component(_basefile ${_file} NAME_WE)
                set(_txtfile ${SCRIPT_TRANS_DIR}/events/${_basefile}.TXT)

		add_custom_command(OUTPUT EVN/${_basefile}.EVN
			COMMAND  ${CMAKE_COMMAND} -E copy ${_file} ${CMAKE_CURRENT_BINARY_DIR}/EVN/${_basefile}.EVN
            MAIN_DEPENDENCY IP.BIN
			COMMENT "Backing up original ${_basefile}.EVN"
		)
		list(APPEND evncopyfiles EVN/${_basefile}.EVN)

		add_custom_command(OUTPUT ${_file}
			COMMAND eventeditor -i ${CMAKE_CURRENT_BINARY_DIR}/EVN/${_basefile}.EVN ${_txtfile} -o ${_file}
			DEPENDS EVN/${_basefile}.EVN eventeditor ${_txtfile}
			COMMENT "Inserting script file ${_basefile}.TXT into ${_basefile}.EVN"
		)

		list(APPEND evnfiles ${_file})
	#endif()
endforeach()


file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/EVN)
add_custom_command(OUTPUT ${buildcd_SOURCES}
	COMMAND ${ISOTOOLS} --extract=${CMAKE_CURRENT_BINARY_DIR} --iso=${CDIMAGE_CUE}
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/IP-RF.BIN ${CMAKE_CURRENT_BINARY_DIR}/IP.BIN
    MAIN_DEPENDENCY ${ISOTOOLS}
	DEPENDS ${CDIMAGE_CUE} ${CDIMAGE}
	COMMENT "Extracting CD image"
)

if (OLD_SH2_PATCH)
	add_custom_command(OUTPUT EVN/0.BIN
		COMMAND  ${CMAKE_COMMAND} -E copy files/0.BIN EVN/0.BIN
		MAIN_DEPENDENCY IP.BIN
		COMMENT "Backing up original 0.BIN"
	)

	add_custom_command(OUTPUT files/0.BIN
		COMMAND  ${CMAKE_COMMAND} -E copy EVN/0.BIN files/0.BIN
		COMMAND satcode_insert ../satcode/patch.bin files/0.BIN
		DEPENDS satcode_insert satcode EVN/0.BIN
		COMMENT "Inserting program patches to 0.bin"
	)

	add_custom_command(OUTPUT files/KANJI.BIN
		COMMAND fontgen -2 -b ${BMFONTGEN} ${FONTGEN_DIR}/pcrown.txt -o files/KANJI.BIN
		#COMMAND itemsutil -i ${SCRIPT_TRANS_DIR}/names.txt ${SCRIPT_TRANS_DIR}/items.txt files/0.BIN files/KANJI.BIN 0x2400
		DEPENDS  ${FONTGEN_DIR}/pcrown.txt ${SCRIPT_TRANS_DIR}/names.txt ${SCRIPT_TRANS_DIR}/items.txt files/0.bin
		COMMENT "Inserting names and items"
	)
else()
	add_custom_command(OUTPUT files/0.BIN
		COMMAND  ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../prog/0.BIN files/0.BIN
        DEPENDS prog
		COMMENT "Copying compiled 0.bin"
	)
endif()

add_custom_command(OUTPUT princess_crown.dsk
	COMMAND ${VCDBUILD} disc -B
	DEPENDS ${buildcd_SOURCES} ${evn_SOURCES}
	COMMENT "Generating Translated CD Image"
)

add_custom_command(OUTPUT princess_crown.xdelta
	COMMAND ${XDELTA} -e -f -s ${CDIMAGE} ${CMAKE_CURRENT_BINARY_DIR}/princess_crown.dsk princess_crown.xdelta
	DEPENDS princess_crown.dsk
	COMMENT "Generating xdelta patch"
)

add_custom_target(buildcd ALL
	DEPENDS ${evncopyfiles} ${buildcd_SOURCES} ${evn_SOURCES} princess_crown.xdelta
	SOURCES ${buildcd_SOURCES} ${evn_SOURCES}
)

# Group source files 
source_group("Event Files" REGULAR_EXPRESSION .*\\.EVN$)
source_group("Other Files" REGULAR_EXPRESSION .*\\.CHB$|.*\\.CHR$|.*\\.CLB$|.*\\.MCB$|.*\\.MPB$|.*\\.PAK$|.*\\.PRG$|.*\\.SND$|.*\\.TSK$|IP.BIN|KANJI.BIN|SOUNDDAT.BIN)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/princess_crown.xdelta DESTINATION ".")
