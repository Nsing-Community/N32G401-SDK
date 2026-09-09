.\SREC\srec_cat .\Objects\N32G401_SelfTest.hex -intel -crop 0x08000000 0x0800FF00 -fill 0x00 0x08000000 0x0800FF00 ^
-crc32-l-e 0x0800FF00 -o .\Objects\N32G401_SelfTest_CRC.hex -intel
