

```
├── jap            # original Japanese script
│   ├── events     # dialogues
│   ├── items.txt  # items and status messages
│   ├── names.txt  # characters and enemies names
│   └── ...
└── eng                  # English script translation (files match 1-to-1 those in the jap folder)
    ├── events           # dialogues, parsed by the eventeditor
    ├── events_splitted  # splitted dialogues, dir populated while building (do not edit these)
    ├── items_ex.txt     # items and status messages, extended translation used in the normal and EX version, parsed by itemsutil (UTF-8-encoded)
    ├── items_lt.txt     # items and status messages, literal translation used in the LT version, parsed by itemsutil (UTF-8-encoded)
    ├── items*.txt.sjis  # items and status messages, generated while building (do not edit these)
    ├── names.txt        # characters and enemies names, parsed by itemsutil
    └── ...  # other files are unused
```

All text files are UTF-8 encoded.
If the Japanese text is not displaying correctly, force the UTF-8 encoding in your text editor and reload (encoding auto-detection fails is some cases, even on the Github frontend).

TIP: to easily test any event file, use the [in-game debug mode](https://github.com/eadmaster/pcrown/wiki/Extras#cheats).
Alternatively, if using savestates, keep in mind the current event is cached in RAM, so you must go through a loading screen to reload from disk.
