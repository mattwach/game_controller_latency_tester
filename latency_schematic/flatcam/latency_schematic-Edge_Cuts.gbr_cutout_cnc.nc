(G-CODE GENERATED BY FLATCAM v8.994 - www.flatcam.org - Version Date: 2020/11/7)

(Name: latency_schematic-Edge_Cuts.gbr_cutout_cnc)
(Type: G-code from Geometry)
(Units: MM)

(Created on Friday, 24 February 2023 at 19:47)

(Specially modified by mattwach to work with the Sainsmart 3080 that I have.)

(TOOL DIAMETER: 2.4 mm)
(Feedrate_XY: 120.0 mm/min)
(Feedrate_Z: 60.0 mm/min)
(Feedrate rapids 1500.0 mm/min)

(Z_Cut: -1.7 mm)
(DepthPerCut: 0.6 mm <=>3 passes)
(Z_Move: 2.0 mm)
(Z Start: None mm)
(Z End: 5.0 mm)
(X,Y End: 0.0000, 0.0000 mm)
(Steps per circle: 64)
(Preprocessor Geometry: default)

(X range:   -1.3500 ...   59.7700  mm)
(Y range:  -28.6550 ...    1.3500  mm)

(Spindle Speed: 10000.0 RPM)
G21
G90
G17
G94


G01 F120.00

M5             
G00 Z15.0000
G00 X0.0000 Y0.0000                
G00 Z15.0000
        
M03 S10000.0
G01 F120.00
G00 X-1.3485 Y-27.3688
G01 F60.00
G01 Z-0.6000
G01 F120.00
G01 X-1.3500 Y-27.3050 F120.00
G01 X-1.3500 Y-0.0000 F120.00
G01 X-1.3485 Y0.0638 F120.00
G01 X-1.3350 Y0.2005 F120.00
G01 X-1.3101 Y0.3256 F120.00
G01 X-1.2719 Y0.4525 F120.00
G01 X-1.2193 Y0.5794 F120.00
G01 X-1.1566 Y0.6961 F120.00
G01 X-1.0858 Y0.8022 F120.00
G01 X-0.9986 Y0.9084 F120.00
G01 X-0.9048 Y1.0019 F120.00
G01 X-0.8061 Y1.0828 F120.00
G01 X-0.6961 Y1.1566 F120.00
G01 X-0.5750 Y1.2214 F120.00
G01 X-0.4525 Y1.2719 F120.00
G01 X-0.3256 Y1.3101 F120.00
G01 X-0.1957 Y1.3357 F120.00
G01 X-0.0687 Y1.3482 F120.00
G01 X0.0000 Y1.3500 F120.00
G01 X58.4200 Y1.3500 F120.00
G01 X58.4887 Y1.3482 F120.00
G01 X58.6157 Y1.3357 F120.00
G01 X58.7504 Y1.3089 F120.00
G01 X58.8771 Y1.2702 F120.00
G01 X58.9950 Y1.2214 F120.00
G01 X59.1161 Y1.1566 F120.00
G01 X59.2222 Y1.0858 F120.00
G01 X59.3248 Y1.0019 F120.00
G01 X59.4219 Y0.9048 F120.00
G01 X59.5028 Y0.8061 F120.00
G01 X59.5766 Y0.6961 F120.00
G01 X59.6414 Y0.5750 F120.00
G01 X59.6919 Y0.4525 F120.00
G01 X59.7301 Y0.3256 F120.00
G01 X59.7557 Y0.1957 F120.00
G01 X59.7682 Y0.0687 F120.00
G01 X59.7700 Y-0.0000 F120.00
G01 X59.7700 Y-27.3050 F120.00
G01 X59.7682 Y-27.3737 F120.00
G01 X59.7557 Y-27.5007 F120.00
G01 X59.7289 Y-27.6354 F120.00
G01 X59.6902 Y-27.7621 F120.00
G01 X59.6414 Y-27.8800 F120.00
G01 X59.5766 Y-28.0011 F120.00
G01 X59.5028 Y-28.1111 F120.00
G01 X59.4219 Y-28.2098 F120.00
G01 X59.3248 Y-28.3069 F120.00
G01 X59.2261 Y-28.3878 F120.00
G01 X59.1161 Y-28.4616 F120.00
G01 X58.9950 Y-28.5264 F120.00
G01 X58.8725 Y-28.5769 F120.00
G01 X58.7456 Y-28.6151 F120.00
G01 X58.6157 Y-28.6407 F120.00
G01 X58.4887 Y-28.6532 F120.00
G01 X58.4200 Y-28.6550 F120.00
G01 X0.0000 Y-28.6550 F120.00
G01 X-0.0687 Y-28.6532 F120.00
G01 X-0.2005 Y-28.6400 F120.00
G01 X-0.3304 Y-28.6139 F120.00
G01 X-0.4571 Y-28.5752 F120.00
G01 X-0.5794 Y-28.5243 F120.00
G01 X-0.6961 Y-28.4616 F120.00
G01 X-0.8061 Y-28.3878 F120.00
G01 X-0.9084 Y-28.3036 F120.00
G01 X-1.0019 Y-28.2098 F120.00
G01 X-1.0858 Y-28.1072 F120.00
G01 X-1.1592 Y-27.9969 F120.00
G01 X-1.2214 Y-27.8800 F120.00
G01 X-1.2719 Y-27.7575 F120.00
G01 X-1.3101 Y-27.6306 F120.00
G01 X-1.3357 Y-27.5007 F120.00
G01 X-1.3485 Y-27.3688 F120.00
G00 X-1.3485 Y-27.3688
G01 F60.00
G01 Z-1.2000
G01 F120.00
G01 X-1.3357 Y-27.5007 F120.00
G01 X-1.3101 Y-27.6306 F120.00
G01 X-1.2719 Y-27.7575 F120.00
G01 X-1.2214 Y-27.8800 F120.00
G01 X-1.1592 Y-27.9969 F120.00
G01 X-1.0858 Y-28.1072 F120.00
G01 X-1.0019 Y-28.2098 F120.00
G01 X-0.9084 Y-28.3036 F120.00
G01 X-0.8061 Y-28.3878 F120.00
G01 X-0.6961 Y-28.4616 F120.00
G01 X-0.5794 Y-28.5243 F120.00
G01 X-0.4571 Y-28.5752 F120.00
G01 X-0.3304 Y-28.6139 F120.00
G01 X-0.2005 Y-28.6400 F120.00
G01 X-0.0687 Y-28.6532 F120.00
G01 X0.0000 Y-28.6550 F120.00
G01 X58.4200 Y-28.6550 F120.00
G01 X58.4887 Y-28.6532 F120.00
G01 X58.6157 Y-28.6407 F120.00
G01 X58.7456 Y-28.6151 F120.00
G01 X58.8725 Y-28.5769 F120.00
G01 X58.9950 Y-28.5264 F120.00
G01 X59.1161 Y-28.4616 F120.00
G01 X59.2261 Y-28.3878 F120.00
G01 X59.3248 Y-28.3069 F120.00
G01 X59.4219 Y-28.2098 F120.00
G01 X59.5028 Y-28.1111 F120.00
G01 X59.5766 Y-28.0011 F120.00
G01 X59.6414 Y-27.8800 F120.00
G01 X59.6902 Y-27.7621 F120.00
G01 X59.7289 Y-27.6354 F120.00
G01 X59.7557 Y-27.5007 F120.00
G01 X59.7682 Y-27.3737 F120.00
G01 X59.7700 Y-27.3050 F120.00
G01 X59.7700 Y-0.0000 F120.00
G01 X59.7682 Y0.0687 F120.00
G01 X59.7557 Y0.1957 F120.00
G01 X59.7301 Y0.3256 F120.00
G01 X59.6919 Y0.4525 F120.00
G01 X59.6414 Y0.5750 F120.00
G01 X59.5766 Y0.6961 F120.00
G01 X59.5028 Y0.8061 F120.00
G01 X59.4219 Y0.9048 F120.00
G01 X59.3248 Y1.0019 F120.00
G01 X59.2222 Y1.0858 F120.00
G01 X59.1161 Y1.1566 F120.00
G01 X58.9950 Y1.2214 F120.00
G01 X58.8771 Y1.2702 F120.00
G01 X58.7504 Y1.3089 F120.00
G01 X58.6157 Y1.3357 F120.00
G01 X58.4887 Y1.3482 F120.00
G01 X58.4200 Y1.3500 F120.00
G01 X0.0000 Y1.3500 F120.00
G01 X-0.0687 Y1.3482 F120.00
G01 X-0.1957 Y1.3357 F120.00
G01 X-0.3256 Y1.3101 F120.00
G01 X-0.4525 Y1.2719 F120.00
G01 X-0.5750 Y1.2214 F120.00
G01 X-0.6961 Y1.1566 F120.00
G01 X-0.8061 Y1.0828 F120.00
G01 X-0.9048 Y1.0019 F120.00
G01 X-0.9986 Y0.9084 F120.00
G01 X-1.0858 Y0.8022 F120.00
G01 X-1.1566 Y0.6961 F120.00
G01 X-1.2193 Y0.5794 F120.00
G01 X-1.2719 Y0.4525 F120.00
G01 X-1.3101 Y0.3256 F120.00
G01 X-1.3350 Y0.2005 F120.00
G01 X-1.3485 Y0.0638 F120.00
G01 X-1.3500 Y-0.0000 F120.00
G01 X-1.3500 Y-27.3050 F120.00
G01 X-1.3485 Y-27.3688 F120.00
G00 X-1.3485 Y-27.3688
G01 F60.00
G01 Z-1.7000
G01 F120.00
G01 X-1.3500 Y-27.3050 F120.00
G01 X-1.3500 Y-0.0000 F120.00
G01 X-1.3485 Y0.0638 F120.00
G01 X-1.3350 Y0.2005 F120.00
G01 X-1.3101 Y0.3256 F120.00
G01 X-1.2719 Y0.4525 F120.00
G01 X-1.2193 Y0.5794 F120.00
G01 X-1.1566 Y0.6961 F120.00
G01 X-1.0858 Y0.8022 F120.00
G01 X-0.9986 Y0.9084 F120.00
G01 X-0.9048 Y1.0019 F120.00
G01 X-0.8061 Y1.0828 F120.00
G01 X-0.6961 Y1.1566 F120.00
G01 X-0.5750 Y1.2214 F120.00
G01 X-0.4525 Y1.2719 F120.00
G01 X-0.3256 Y1.3101 F120.00
G01 X-0.1957 Y1.3357 F120.00
G01 X-0.0687 Y1.3482 F120.00
G01 X0.0000 Y1.3500 F120.00
G01 X58.4200 Y1.3500 F120.00
G01 X58.4887 Y1.3482 F120.00
G01 X58.6157 Y1.3357 F120.00
G01 X58.7504 Y1.3089 F120.00
G01 X58.8771 Y1.2702 F120.00
G01 X58.9950 Y1.2214 F120.00
G01 X59.1161 Y1.1566 F120.00
G01 X59.2222 Y1.0858 F120.00
G01 X59.3248 Y1.0019 F120.00
G01 X59.4219 Y0.9048 F120.00
G01 X59.5028 Y0.8061 F120.00
G01 X59.5766 Y0.6961 F120.00
G01 X59.6414 Y0.5750 F120.00
G01 X59.6919 Y0.4525 F120.00
G01 X59.7301 Y0.3256 F120.00
G01 X59.7557 Y0.1957 F120.00
G01 X59.7682 Y0.0687 F120.00
G01 X59.7700 Y-0.0000 F120.00
G01 X59.7700 Y-27.3050 F120.00
G01 X59.7682 Y-27.3737 F120.00
G01 X59.7557 Y-27.5007 F120.00
G01 X59.7289 Y-27.6354 F120.00
G01 X59.6902 Y-27.7621 F120.00
G01 X59.6414 Y-27.8800 F120.00
G01 X59.5766 Y-28.0011 F120.00
G01 X59.5028 Y-28.1111 F120.00
G01 X59.4219 Y-28.2098 F120.00
G01 X59.3248 Y-28.3069 F120.00
G01 X59.2261 Y-28.3878 F120.00
G01 X59.1161 Y-28.4616 F120.00
G01 X58.9950 Y-28.5264 F120.00
G01 X58.8725 Y-28.5769 F120.00
G01 X58.7456 Y-28.6151 F120.00
G01 X58.6157 Y-28.6407 F120.00
G01 X58.4887 Y-28.6532 F120.00
G01 X58.4200 Y-28.6550 F120.00
G01 X0.0000 Y-28.6550 F120.00
G01 X-0.0687 Y-28.6532 F120.00
G01 X-0.2005 Y-28.6400 F120.00
G01 X-0.3304 Y-28.6139 F120.00
G01 X-0.4571 Y-28.5752 F120.00
G01 X-0.5794 Y-28.5243 F120.00
G01 X-0.6961 Y-28.4616 F120.00
G01 X-0.8061 Y-28.3878 F120.00
G01 X-0.9084 Y-28.3036 F120.00
G01 X-1.0019 Y-28.2098 F120.00
G01 X-1.0858 Y-28.1072 F120.00
G01 X-1.1592 Y-27.9969 F120.00
G01 X-1.2214 Y-27.8800 F120.00
G01 X-1.2719 Y-27.7575 F120.00
G01 X-1.3101 Y-27.6306 F120.00
G01 X-1.3357 Y-27.5007 F120.00
G01 X-1.3485 Y-27.3688 F120.00
G00 Z2.0000
M05
G00 Z2.0000
G00 Z5.00
G00 X0.0 Y0.0

