#! /usr/bin/env python
# -*- coding: utf-8 -*-

# Split long lines to avoid text overflowing due to space limitations.
# Interts "lineend" and "pause" tags in the appropriate positions, while keeping the other tags intact.
# Made by eadmaster for the Princess Crown Translation GPL Edition project  https://github.com/eadmaster/pcrown/

# mass convert with: 
#  for txt in *.txt ; do python _split_long_lines.py "$txt" ../events_splitted/$txt ; done


#MAX_CHARS_PER_LINE=20
#MAX_CHARS_PER_LINE=35  # 7px font + 4 coeff. spacing
MAX_CHARS_PER_LINE=45  # 5px font + 3 coeff. spacing  https://github.com/eadmaster/pcrown/issues/98

SHORT_DIALOG_WARNING_LEN=20


import sys
import re


def remove_tags(text):
    # Pattern to match anything within angle brackets, including the brackets
    text = re.sub(r'<[^>]+>', '', text)
    text = re.sub(r'<[^>]+', '', text)
    return text
    
        
def length_after_last_tag(s):
    tag_end = s.rfind('>')
    if tag_end == -1:
        return 0  # No closing tag found, so return 0
    return len(s) - tag_end - 1
    

def last_line_length(s):
    last_lineend_pos = s.rfind('<CC03EA>')
    if last_lineend_pos > 0:
        pause_pos = s.find('<pause', last_lineend_pos)
        if pause_pos > 0:
            return (pause_pos - last_lineend_pos - len('<pause>'))
    # else
    return 1000
   


def insert_lineend_every_x_chars(s, init_lines_counter=0):
    position = 0
    result = ""
    curr_lines_counter = init_lines_counter
    curr_char_counter = 0
    
    for token in s.split(" "):
        #print(token)
        #print(remove_tags(token))
           
        #if token.endswith("<lineend"):
        #    #token = token.rstrip("<lineend")
        #    curr_lines_counter += 1
        
        if (curr_char_counter + len(remove_tags(token)) + 1) < MAX_CHARS_PER_LINE:
            # just append the token
            if curr_char_counter==0:
                result += token
                curr_char_counter += len(remove_tags(token))
            else:
                result += " " + token
                curr_char_counter += len(remove_tags(token)) + 1
        else:
            # new lineend tags
            curr_lines_counter += 1
            if(curr_lines_counter < 3):
                # add a newline before the word
                result += "<lineend>" + token
            else:
                # add a pause
                result += "<pause><lineend><CC03EA>" + token
                curr_lines_counter = 0  # reset
            curr_char_counter = len(remove_tags(token))
        # endif
    # end for
    
    if last_line_length(result) < SHORT_DIALOG_WARNING_LEN:
        print("SHORT_DIALOG_WARNING: " + result)
    
    return result
    

# main

f = open(sys.argv[1], 'r', encoding='utf-8', errors='ignore') 
out = open(sys.argv[2], 'w', encoding='shift_jis') 

init_lines_counter=0


for line in f.readlines():
    init_lines_counter = 0

    if(line.endswith(">|\n")):
        tags = re.split(r'(?<!color=\w{4})>', line)
        #print(tags)
        # ['<CC03EA', '<color=0006><string=0014', '<lineend', '<color=0001>Do you need some directions?<pause', '<lineend', '<CC03EA', "The village of <color=0002>Nutsbill<color=0001> is straight to the west from this town's left exit.<pause", '<lineend', '|\n']
        # ['<CC03EA', "<color=0001>I can hear Jestonai's voice coming from somewhere...<pause", '<lineend', '<CC03EA', '<color=0004>Jestonai<lineend', '<color=0001>You who yearns for Knighthood. Defeat this monster, and prove your strength!<pause', '<lineend']
        for t in tags:
            #print(t)
            #print(init_lines_counter)
            if t.startswith("<lineend"):
                init_lines_counter += 1
                continue
            if t.startswith('|\n'):
                continue
            if t.startswith("<CC03EA"):
                init_lines_counter = 0   # new dialog, reset to 0
                continue
            if t.startswith("<pause"):
                init_lines_counter = 0   # new dialog, reset to 0
                continue
            
            # TODO: check there is always a space after "..."
        
            if(len(remove_tags(t)) < MAX_CHARS_PER_LINE):
                # nothing to change
                if t.endswith("<lineend"):
                    init_lines_counter += 1
                continue
            else:
                # add "<lineend>"/<pause> tags where needed
                #print(t)
                #print(init_lines_counter)
                new_line_text = insert_lineend_every_x_chars(t, init_lines_counter)
                line = line.replace(t, new_line_text)
            
            #if t.endswith("<lineend"):
            #    init_lines_counter += 1
            #    if init_lines_counter >= 3:
            #        # replace last "<lineend" -> pause tag
                    
        # end for tags
    # end if
    out.write(line)

