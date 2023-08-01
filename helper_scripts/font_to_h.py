import sys
from math import ceil
from PIL import Image
from numpy import asarray

file_base = """\
#include <avr/pgmspace.h>
"""
font_base = """
const uint8_t {font_data_name}[] PROGMEM = {{{font_data}
}};
PGMFont {font_name} = {{{font_data_name}, {bytes_per_char}, {w}, {h}, {x_space}, {y_space}}};
"""

def main():
    if(len(sys.argv) < 5):
        print("Expects arguments [output] [input] [x space] [y space]")
        return
    
    image_path = sys.argv[2]
    file_str = file_base
    try:
        img = Image.open(image_path)
    except:
        print(f"Image '{image_path}' failed to open!")
        return
    
    w = int(img.size[0]/12)
    h = int(img.size[1]/8)

    font_data = ""
    img_arr = asarray(img)
    char_i = 32
    for y in range(0, img.size[1], h):
        for x in range(0, img.size[0], w):
            buf = 0
            bufi = 7
            font_data += "\n\t"
            for yr in range(y, y+h):
                for xr in range(x, x+w):
                    if(any(img_arr[yr][xr])):
                        buf = buf | 1<<bufi
                    bufi -= 1
                    if(bufi < 0):
                        font_data += f"{buf:#0{4}x},"
                        buf = 0
                        bufi = 7
            if(bufi != 7):
                font_data += f"{buf:#0{4}x},"
            if(char_i != 127):
                font_data += " // '" + chr(char_i) + "'"
            else:
                font_data += " // unknown"
            char_i += 1

    file_str += font_base.format(**{
        "font_name": image_path.partition('.')[0],
        "font_data_name": image_path.partition('.')[0].upper() + "_DATA",
        "font_data": font_data,
        "w": w, "h": h,
        "bytes_per_char": int(ceil(w*h/8)),
        "x_space": int(sys.argv[3]),
        "y_space": int(sys.argv[4]),
    })

    try:
        file_out = open(sys.argv[1], "w")
    except:
        print(f"Failed to open file '{sys.argv[1]}'!")
        return
    file_out.write(file_str)

if(__name__ == "__main__"):
    main()