# LowCodeForITK
This is master thesis work focused on developing low code implementation for ITK library.

---

# Installing dependecies

### QT5
Download [qt for open source development](https://www.qt.io/download-open-source?hsCtaTracking=9f6a2170-a938-42df-a8e2-a9f0b1d6cdce%7C6cb0de4f-9bb5-4778-ab02-bfb62735f3e5)

Using MaintenanceTool install QT5 (I am using 5.15.2)

---

Before further actions download git submodules
```bash
git submodule update --init --recursive
```

### QuickQanava
Using cmake build QuickQanava project

```bash
cd .../LowCodeForITK/externals/QuickQanava

cmake -B ./build -S . 
```

Follow cmake instructions regarding missing libraries path. You may need to add QT and Qt5_DIR to your system PATH variable or add it with *-D* cmake flag.  

Build it with your IDE or with
```bash

cmake --build ./build 
```

### ITK