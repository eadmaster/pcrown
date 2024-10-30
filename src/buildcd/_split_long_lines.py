#! /usr/bin/env python
# -*- coding: utf-8 -*-

# mass convert with: 
#  for txt in *.txt ; do python _split_long_lines.py "$txt" ../events_splitted_35chars/$txt ; done


#MAX_CHARS_PER_LINE=20
MAX_CHARS_PER_LINE=35


def insert_lineend_every_x_chars(s, init_lines_counter=0):
    position = 0
    result = ""
    lines_counter = init_lines_counter

    while position < len(s):
        # Get the next MAX_CHARS_PER_LINE characters slice
        slice_max = s[position:position + MAX_CHARS_PER_LINE]
        
        # Find the last space in this slice
        last_space_pos = slice_max.rfind(' ')
        
        #if last_space_pos == -1 or (len(slice_max)-s.find(slice_max) <= MAX_CHARS_PER_LINE):
        if last_space_pos == -1 or (len(slice_max)< MAX_CHARS_PER_LINE):
            # If no space found, just append the remaining string
            result += s[position:]
            break
        
        # Adjust the space position to the full string index
        insert_pos = position + last_space_pos
        
        # Add part before the space and insert the <lineend> tag
        lines_counter += 1
        if(lines_counter<3):
            result += s[position:insert_pos] + "<lineend>"
        else:
            result += s[position:insert_pos] + "<pause><lineend><CC03EA>"
            lines_counter = 0  # reset
        
        # Move the position after the space
        position = insert_pos + 1
    
    
    return result
    
    

import sys


f = open(sys.argv[1], 'r', encoding='shift_jis') 
out = open(sys.argv[2], 'w', encoding='shift_jis') 

init_lines_counter=0

for line in f.readlines():
    init_lines_counter = 0
    if(not line.endswith("|")):
        tags = line.split(">")
        for t in tags:
            if t.startswith("<lineend"):
                init_lines_counter += 1
            if t.startswith("<CC03EA"):
                init_lines_counter = 0   # new dialog, reset to 0
            if t.startswith("<"):
                continue
            curr_line_text = t.split("<")[0]
            if(len(curr_line_text)<MAX_CHARS_PER_LINE):
                # nothing to change
                continue
            else:
                # add a "<lineend>" tag
                new_line_text = insert_lineend_every_x_chars(curr_line_text, init_lines_counter)
                line = line.replace(curr_line_text, new_line_text)
    out.write(line)

    
