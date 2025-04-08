import struct

# script to parse PAK files, based on https://github.com/eadmaster/pcrown/wiki/PAKFormat

def parse_pak(file_path):
    with open(file_path, "rb") as f:
        # Read the ID string (4 bytes)
        file_id = f.read(4).decode("ascii")
        
        if file_id not in ("unkn", "OVER"):
            raise ValueError("Invalid file ID: " + file_id)
        
        # Read the file size (4 bytes, big-endian unsigned int)
        file_size = struct.unpack(">I", f.read(4))[0]
        
        # Read offsets (each 4 bytes, big-endian unsigned int)
        texture_offset = struct.unpack(">I", f.read(4))[0]
        sprite_offset = struct.unpack(">I", f.read(4))[0]
        sprite_group_offset = struct.unpack(">I", f.read(4))[0]
        
        parsed_data = {
            "File ID": file_id,
            "File Size": file_size,
            "Texture Offset": texture_offset,
            "Sprite Offset": sprite_offset,
            "Sprite Group Offset": sprite_group_offset,
        }
        
        print(parsed_data)
        
        # Read and print texture information structure (8 bytes each)
        f.seek(texture_offset)
        print("\nTexture Information Structure:")
        offset = texture_offset
        count = 0
        while offset < sprite_offset:
            record = f.read(8)
            if len(record) < 8:
                break
            
            address, width, height, index = struct.unpack(">IBBH", record)
            print(f"[{count:04X}] Offset 0x{offset:X}: Address=0x{address:08X}, Width={width}, Height={height}, Index={index}")
            
            offset += 8
            count += 1
        
        # Read and print sprite information structure (12 bytes each)
        f.seek(sprite_offset)
        print("\nSprite Information Structure:")
        offset = sprite_offset
        count = 0
        while offset < sprite_group_offset:
            record = f.read(12)
            if len(record) < 12:
                break
                
            index, coords, origin_flags = struct.unpack(">HQH", record)
            print(f"[{count}] Offset 0x{offset:X}: Index={index:04X}, Coords=0x{coords:016X}, Origin Flags=0x{origin_flags:04X}")
            
            offset += 12
            count += 1
        
        return parsed_data




import sys
parse_pak(sys.argv[1])


