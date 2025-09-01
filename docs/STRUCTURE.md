```
RippleEffectEngine/
├── .gitignore
├── LICENSE
├── README.md
├── CMakeLists.txt
│
├── docs/
│   └── STRUCTURE.md
│
├── lib/
│   └── Logitech/
│       ├── include/
│       │   └── LogitechLedEnginesWrapper.h
│       └── lib/
│           └── LogitechLed.x64.lib
│
├── vendor/
│   └── Logitech/
│       ├── LogitechLed.h
│       ├── LogitechLed.lib
│
├── include/
│   ├── Core/
│   │   ├── Effects/
│   │   │   ├── IEffect.h
│   │   │   └── RippleEffect.h
│   │   ├── Keyboard/
│   │   │   ├── KeyCodes.h
│   │   │   ├── Key.h
│   │   │   └── Keyboard.h
│   │   ├── Lighting/
│   │   │   └── LightingManager.h
│   │   └── Util/
│   │       ├── Color.h
│   │       └── Position.h
│   │
│   └── Hardware/
│       ├── IHardware.h
│       ├── Simulator.h
│       └── LogitechLed.h
│
└── src/
    ├── Core/
    │   ├── Effects/
    │   │   └── RippleEffect.cpp
    │   ├── Keyboard/
    │   │   ├── Key.cpp
    │   │   └── Keyboard.cpp
    │   └── Lighting/
    │       └── LightingManager.cpp
    │
    ├── Hardware/
    │   ├── Simulator.cpp
    │   └── LogitechLed.cpp
    │
    ├── main.ino
    └── main.cpp
