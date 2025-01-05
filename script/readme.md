

```
├── jap            # original Japanese script
│   ├── events     # dialogues
│   ├── items.txt  # items and status messages
│   ├── names.txt  # characters and enemies names
│   └── ...
└── eng   # English script translation (files match 1-to-1 those in the jap folder)
    ├── events                   # dialogues, parsed by the eventeditor
    ├── events_splitted_35chars  # dialogues, empty dir auto-populated while building
    ├── items.txt                # items and status messages, parsed by itemsutil
    ├── names.txt                # characters and enemies names, parsed by itemsutil
│   └── ...  # other files are unused
```

All text files are Shift-JIS encoded.

Events currently not parsed automatically due to bugs:
 
 - 015_00_1  https://github.com/eadmaster/pcrown/issues/30
 - 176_00_0  https://github.com/eadmaster/pcrown/issues/88
 - 041_00_1  https://github.com/eadmaster/pcrown/issues/71