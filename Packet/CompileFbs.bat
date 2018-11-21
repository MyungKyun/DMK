flatc.exe -c --no-prefix playerInfo.fbs
flatc.exe -n playerInfo.fbs

copy playerInfo_generated.h .\Generated\