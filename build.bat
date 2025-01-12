@echo off

SET DEBUG=0
SET IFLAGS=/Iinclude
SET CFLAGS=/FeRandomSteamGameLauncher /nologo /W4 /wd4152 /wd4029 /D_CRT_SECURE_NO_WARNINGS /GR /EHa /Oi /fp:fast /FC /link /INCREMENTAL:NO /opt:ref

where cl.exe > nul 2>&1
if %errorlevel% neq 0 (call setup.bat)

if "%DEBUG%" == "1" (
  cl src/main.c %IFLAGS% /Od /MTd /Zi /Fm /DDEBUG=1 %CFLAGS% 1>&2
) else (
  cl src/main.c %IFLAGS% /Ox /MT /DDEBUG=0 %CFLAGS% 1>&2
)
