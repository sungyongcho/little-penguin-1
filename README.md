# little-penguin-1

> Linux kernel module development — kernel-space programming, char and misc devices, sysfs / debugfs / procfs interfaces.

## Overview

A series of ten assignments inside the Linux kernel: submitting an upstream coding-style patch, writing loadable modules, exposing entries through procfs / debugfs, building character and misc device drivers, and reacting to USB hot-plug events. Modules are built against a custom kernel and tested under QEMU for safe iteration.

This project was built as part of the 42 school cybersecurity / kernel track · Score: 100/100.

## Tech Stack

| Layer | Technologies |
|-------|-------------|
| Language | C |
| Target | Linux kernel (in-tree module API) |
| Build | kbuild, Makefiles |
| Run | QEMU with custom kernel image |

## Key Features

- **Loadable kernel modules**: `__init` / `__exit` hooks, license/author/description metadata, `printk` logging
- **Misc and character devices**: `miscdevice` with `file_operations`, custom `read` / `write` implementations
- **debugfs interface**: `debugfs_create_*` for exposing kernel state (`id`, `jiffies`, free-form strings)
- **procfs interface**: custom `/proc/...` entries for kernel introspection (e.g., mount-table dump)
- **USB hot-plug detection**: udev rule + kernel module reacting to keyboard plug/unplug events
- **Kernel-source patch**: coding-style patch submitted against an in-tree kernel source file

## Architecture

```
little-penguin-1/
├── assignment-00/       # coding-style patch (config + kern.log)
├── assignment-01/       # hello-world loadable module
├── assignment-02/       # patch — kernel-source modification
├── assignment-03/       # sleep / wake behavior with pr_info logging
├── assignment-04/       # USB keyboard hot-plug detection (udev rule + module)
├── assignment-05/       # misc device "fortytwo" with read/write
├── assignment-06/       # kern.log-only configuration assignment
├── assignment-07/       # debugfs hierarchy with multiple entries
├── assignment-08/       # misc device "reverse" — string-reversal interface
├── assignment-09/       # /proc/mymounts — procfs mount-table dump
└── test/                # kernel image and test scaffolding
```

## Getting Started

### Prerequisites

A POSIX environment with:

```bash
gcc                     # for kernel module builds
make                    # via kbuild
qemu-system-x86_64      # for testing in a VM
linux-headers           # matching the target kernel
```

### Build & test

Each assignment is independent:

```bash
cd assignment-XX
make
sudo insmod <module>.ko
sudo dmesg | tail
sudo rmmod <module>
```

## What This Demonstrates

- **Kernel-space programming**: writing C code that runs at ring 0 using the kernel's own APIs — no libc, no userspace facilities.
- **Driver scaffolding**: structuring loadable modules with init/exit, registering character / misc devices, and exposing state through procfs / sysfs / debugfs.
- **Build & deploy loop**: integrating with kbuild, building against a target kernel, loading and unloading without crashing the system.

## License

This project was built as part of the 42 school curriculum.

---

*Part of [sungyongcho](https://github.com/sungyongcho)'s project portfolio.*
