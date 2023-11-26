set out_path="./build/"
set res_path="./res/*.bmp"
set exe_path=%out_path%sherlocka_and_walter.exe

xcopy %res_path% %out_path%%res_path% /E /Y

cl ./src/*.c user32.lib gdi32.lib /source-charset:utf-8 /Fe%exe_path% /Fo%out_path% /Fd%out_path% /Zi
cd %out_path%