

```
├── jap            # original Japanese script
│   ├── events     # dialogues
│   ├── items.txt  # items and status messages
│   ├── names.txt  # characters and enemies names
│   └── ...
└── eng   # English script translation (files match 1-to-1 those in the jap folder)
    ├── events                   # dialogues, parsed by the eventeditor
    ├── events_splitted_35chars  # dialogues, dir populated while building
    ├── items.txt                # items and status messages, parsed by itemsutil
    ├── items.txt.sjis           # items and status messages, overwritten while building (do not edit this one)
    ├── items_ex.txt             # more detailed items and status messages used in the EX version
    ├── names.txt                # characters and enemies names, parsed by itemsutil
│   └── ...  # other files are unused
```

All text files are Shift-JIS encoded.

Events currently not parsed automatically due to bugs:
 
 - 015_00_1  https://github.com/eadmaster/pcrown/issues/30
 - 176_00_0  https://github.com/eadmaster/pcrown/issues/88
 - 041_00_1  https://github.com/eadmaster/pcrown/issues/71

TIP: to easily test these use the [in-game debug mode](https://github.com/eadmaster/pcrown/wiki/Extras#cheats). Alternatively, if using savestates, keep in mind the current event file is cached in RAM, so you must go through a black loading screen to reload from disk.
