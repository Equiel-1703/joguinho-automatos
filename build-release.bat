set out_path="./build/"
set exe_path=%out_path%"sherlocka_and_walter.exe"

xcopy "./res" %out_path% /E /I /Y

cl ./src/*.c user32.lib gdi32.lib /Fe%exe_path% /Fo%out_path% /Fd%out_path%
cd %out_path%