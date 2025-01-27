

```
├── jap            # original Japanese script
│   ├── events     # dialogues
│   ├── items.txt  # items and status messages
│   ├── names.txt  # characters and enemies names
│   └── ...
└── eng                  # English script translation (files match 1-to-1 those in the jap folder)
    ├── events           # dialogues, parsed by the eventeditor
    ├── events_splitted  # splitted dialogues, dir populated while building (do not edit these)
    ├── items.txt        # items and status messages, parsed by itemsutil (UTF-8-encoded)
    ├── items_ex.sed     # items and status messages, changes used in the EX version (UTF-8-encoded)
    ├── items*.txt.sjis  # items and status messages, generated while building (do not edit these)
    ├── names.txt        # characters and enemies names, parsed by itemsutil
    └── ...  # other files are unused
```

All text files are Shift-JIS encoded, with the exception of `items.txt`.

Currently these two events have size restrictions due to bugs:
 
 - 015_00_1.TXT  https://github.com/eadmaster/pcrown/issues/30
 - 176_00_0.TXT  https://github.com/eadmaster/pcrown/issues/88

TIP: to easily test any event file, use the [in-game debug mode](https://github.com/eadmaster/pcrown/wiki/Extras#cheats).
Alternatively, if using savestates, keep in mind the current event is cached in RAM, so you must go through a loading screen to reload from disk.
