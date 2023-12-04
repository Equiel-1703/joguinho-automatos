set out_path=.\build\
set res_to_copy=.\res\*.bmp
set icon_res=.\res\icon.ico
set exe_path=%out_path%Sherlocka e Walter.exe

xcopy %res_to_copy% %out_path%res /Y
xcopy %icon_res% %out_path%res /Y

cl .\src\*.c user32.lib gdi32.lib /source-charset:utf-8 /Fe"%exe_path%" /Fo%out_path% /Fd%out_path% /link src\resources.res
cd %out_path%