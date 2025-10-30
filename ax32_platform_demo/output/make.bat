cd /d %~dp0
MakeSPIBin.exe %1 Res.bin
@echo.
@if exist F:\ (copy /y DestBin.bin F:\)

















