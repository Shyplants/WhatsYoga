@echo off
rd /s /q Intermediate
rd /s /q Binaries
rd /s /q DerivedDataCache
rd /s /q Saved
del /q *.sln
md Saved
copy C:\Users\wlsdn\OneDrive\Desktop\Temp\Config.txt .\Saved\

:: 새로운 코드 추가 - Plugins 폴더 안의 Binaries와 Intermediate 폴더 삭제
for /d %%p in ("Plugins\*") do (
    if exist "%%p\Binaries" (
        rd /s /q "%%p\Binaries"
    )
    if exist "%%p\Intermediate" (
        rd /s /q "%%p\Intermediate"
    )
)

:: 완료 메시지
echo All specified folders have been deleted.
pause
