set out_path="./build/"

cl ./src/*.c user32.lib /Fe%out_path% /Fo%out_path% /Fd%out_path% /Zi