@echo off

SET DEBUG=1
SET IFLAGS=/Iinclude
SET CFLAGS=/FeRandomSteamGameLauncher /nologo /W4 /wd4152 /wd4029 /D_CRT_SECURE_NO_WARNINGS ^
/GR /EHa /Oi /fp:fast /Fm /FC /link /INCREMENTAL:NO /opt:ref ^
user32.lib gdi32.lib opengl32.lib

where cl.exe > nul 2>&1
if %errorlevel% neq 0 (call setup.bat)

if "%DEBUG%" == "1" (
  cl src/main.c %IFLAGS% /Od /MTd /Zi /DDEBUG=1 %CFLAGS%
) else (
  cl src/main.c %IFLAGS% /Ox /MT /DDEBUG=0 %CFLAGS%
)
