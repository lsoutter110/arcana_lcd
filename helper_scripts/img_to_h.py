import sys
from PIL import Image
from numpy import asarray

file_base = """\
#include <avr/pgmspace.h>
"""
sprite_base = """
const uint8_t {sprite_data_name}[] PROGMEM = {{{sprite_data}
}};
PGMSprite {sprite_name} = {{{sprite_data_name}, {w}, {h}}};
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
        sprite_data = ""
        for row in asarray(img):
            sprite_data += "\n\t"
            for pix in row:
                hexh = pix[0]&0xF8 | pix[1]>>5
                hexl = (pix[1]<<3)&0xE0 | pix[2]>>3
                sprite_data += f"{hexh:#0{4}x},{hexl:#0{4}x},"
        w, h = img.size
        file_str += sprite_base.format(**{
            "sprite_name": image_path.partition('.')[0],
            "sprite_data_name": image_path.partition('.')[0].upper() + "_DATA",
            "sprite_data": sprite_data,
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