set out_path=.\build\
set res_to_copy=.\res\*.bmp
set exe_path=%out_path%sherlocka_and_walter.exe

xcopy %res_to_copy% %out_path%res /Y

cl .\src\*.c user32.lib gdi32.lib /source-charset:utf-8 /Fe%exe_path% /Fo%out_path% /Fd%out_path%
cd %out_path%