rd ax32xx\sd /s /q

rd bwlib\fs\make /s /q
rd bwlib\fs\src /s /q

rd multimedia\algorithm\make /s /q
rd multimedia\algorithm\src /s /q

rd multimedia\avi\make /s /q
rd multimedia\avi\src /s /q

rd multimedia\btcom\make /s /q
rd multimedia\btcom\src /s /q

del multimedia\interface\userInterface.c /q


rd multimedia\mp3\make /s /q
rd multimedia\mp3\src /s /q

del multimedia\watermark\image_watermark.c /q

rd ax32_platform_demo\Debug  /s /q
rd ax32_platform_demo\.codelite  /s /q
rd ax32_platform_demo\.clang  /s /q

pause