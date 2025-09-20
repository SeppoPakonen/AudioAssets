@echo off
REM Simple static server for the generated site under /www
REM Defaults: host 127.0.0.1, port 8080, root = /www
REM Usage:
REM   scripts\serve_www.bat              ^<— serves /www
REM   scripts\serve_www.bat repo-root    ^<— serves repository root (then open /www/)

setlocal ENABLEDELAYEDEXPANSION

set ROOT=%~dp0..
set SERVE_DIR=%ROOT%\www
set HOST=127.0.0.1
set PORT=8080
set OPENPATH=/

if /I "%1"=="repo-root" (
  set SERVE_DIR=%ROOT%
  set OPENPATH=/www/
)

REM Generate site if serving www and it seems missing
if exist "%ROOT%\www\index.html" (
  REM ok
) else (
  python "%ROOT%\scripts\generate_www.py" 2>nul 1>nul
)

set URL=http://%HOST%:%PORT%%OPENPATH%
echo [serve_www] Serving: %SERVE_DIR%
echo [serve_www] URL:     %URL%

where npx >nul 2>&1
if errorlevel 1 goto try_python

REM Start npx http-server
start "http-server" cmd /c npx http-server "%SERVE_DIR%" -p %PORT% -a %HOST% -c-1
goto :open

:try_python
echo [serve_www] npx not found; trying Python http.server
pushd "%SERVE_DIR%"
start "http.server" cmd /c python -m http.server %PORT%
popd

:open
REM Open default browser
start "" "%URL%"
endlocal
exit /b 0

