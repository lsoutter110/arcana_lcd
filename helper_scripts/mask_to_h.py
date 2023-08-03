import sys
from PIL import Image
from numpy import asarray

file_base = """\
#include <avr/pgmspace.h>
"""
mask_base = """
const uint8_t {mask_data_name}[] PROGMEM = {{{mask_data}
}};
PGMMask {mask_name} = {{{mask_data_name}, {w}, {h}}};
"""

def main():
    if(len(sys.argv) < 3):
        print("Expects arguments [output] [input] ([input] ...)")
        return
    
    file_str = file_base
    for image_path in sys.argv[2:]:
        try:
            img = Image.open(image_path)
        except:
            print(f"Image '{image_path}' failed to open! Skipping...")
            continue
        mask_data = ""
        buf = 0
        bufi = 7
        for row in asarray(img):
            mask_data += "\n\t"
            for pix in row:
                if(any(pix)):
                    buf |= 1<<bufi
                bufi -= 1
                if(bufi < 0):
                    mask_data += f"{buf:#0{4}x},"
                    buf = 0
                    bufi = 7
        if(bufi != 7):
            mask_data += f"{buf:#0{4}x},"
        
        w, h = img.size
        file_str += mask_base.format(**{
            "mask_name": image_path.partition('.')[0],
            "mask_data_name": image_path.partition('.')[0].upper() + "_DATA",
            "mask_data": mask_data,
            "w": w, "h": h
        })

    try:
        file_out = open(sys.argv[1], "w")
    except:
        print(f"Failed to open file '{sys.argv[1]}'!")
        return
    file_out.write(file_str)

if(__name__ == "__main__"):
    main()