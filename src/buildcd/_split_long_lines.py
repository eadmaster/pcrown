#! /usr/bin/env python
# -*- coding: utf-8 -*-

# Split long lines to avoid text overflowing due to space limitations.
# Interts "lineend" and "pause" tags in the appropriate positions, while keeping the other tags intact.
# Made by eadmaster for the Princess Crown Translation GPL Edition project  https://github.com/eadmaster/pcrown/

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
    
    
def insert_lineend_in_colored_text(s, init_lines_counter, init_char_counter):
    position = 0
    result = ""
    lines_counter = init_lines_counter
    
    # TODO: may need to loop for longer lines
    
    slice_max = s[position:position + MAX_CHARS_PER_LINE - (init_char_counter - len(s))]

    last_space_pos = slice_max.rfind(' ')
    #print(slice_max)
    
    insert_pos = position + last_space_pos
    if last_space_pos == -1:
        # just add a newline at the beginning
        insert_pos = 0
    
    # Add part before the space and insert the <lineend> tag
    lines_counter += 1
    if(lines_counter<3):
        result += s[position:insert_pos].rstrip() + "<lineend>" + s[insert_pos:].lstrip()
    else:
        result += s[position:insert_pos].rstrip() + "<pause><lineend><CC03EA>" + s[insert_pos:].lstrip()
    
    if(len(s[insert_pos:].lstrip()) > MAX_CHARS_PER_LINE):
        post_str = insert_lineend_every_x_chars(s[insert_pos:].lstrip(), lines_counter)
        result = result.replace(s[insert_pos:].lstrip(), post_str)
        return result, length_after_last_tag(result)
    else:
        return result, ( len(s[insert_pos:].lstrip()) )
        
        
        
def length_after_last_tag(s):
    tag_end = s.rfind('>')
    if tag_end == -1:
        return 0  # No closing tag found, so return 0
    return len(s) - tag_end - 1
    
    
    
    
import sys


f = open(sys.argv[1], 'r', encoding='shift_jis') 
out = open(sys.argv[2], 'w', encoding='shift_jis') 

init_lines_counter=0
init_char_counter = 0

for line in f.readlines():
    init_lines_counter = 0
    init_char_counter = 0
    if(not line.endswith("|")):
        tags = line.split(">")
        for t in tags:
            if t.startswith("<lineend"):
                init_lines_counter += 1
                init_char_counter = 0
                continue
            if t.startswith("<CC03EA"):
                init_lines_counter = 0   # new dialog, reset to 0
                init_char_counter = 0
                continue
            if t.startswith("<"):
                continue
            curr_line_text = t.split("<")[0]
                
            # handle color tags
            if t.endswith(( "<color=0001",  "<color=0005", "<color=0002")):
                init_char_counter += len(curr_line_text)
                if(init_char_counter > MAX_CHARS_PER_LINE):
                    new_line_text, init_char_counter = insert_lineend_in_colored_text(curr_line_text, init_lines_counter, init_char_counter)
                    line = line.replace(curr_line_text, new_line_text)
                    if "<CC03EA" in new_line_text:
                        init_lines_counter = 0
                        #init_lines_counter = new_line_text.count("<lineend")
                    if "<lineend" in new_line_text:
                        init_lines_counter += 1
                        #init_lines_counter += new_line_text.count("<lineend")
                        #print(t)
                        #print(init_char_counter)
                    #init_char_counter = length_after_last_tag(new_line_text)
                    #print(init_char_counter)
                continue
            else:
                init_char_counter = 0
                
            if(len(curr_line_text)<MAX_CHARS_PER_LINE):
                # nothing to change
                continue
            else:
                # add a "<lineend>" tag
                new_line_text = insert_lineend_every_x_chars(curr_line_text, init_lines_counter)
                line = line.replace(curr_line_text, new_line_text)
        # end for tags
    # end if
    out.write(line)

