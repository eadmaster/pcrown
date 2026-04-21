

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
If the Japanese text is not displaying correctly in your text editor, try to force the UTF-8 encoding and reload.

> [!TIP]
> to easily test an event file, use the [in-game debug mode](https://tcrf.net/Princess_Crown_(Saturn)#Debug_Features).
> Alternatively, if using savestates, keep in mind the current event data is cached in RAM, so you must go through a loading screen to reload from disk.

See also

 - [List of event numbers and game locations](https://github.com/eadmaster/pcrown/wiki/FileStructure#files-reference)
 - Localization-related issues: https://github.com/eadmaster/pcrown/issues/87 https://github.com/eadmaster/pcrown/issues/93 https://github.com/eadmaster/pcrown/issues/5