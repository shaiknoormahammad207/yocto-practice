# i.MX6ULL Yocto BSP Setup and SD Card Flashing

This guide explains how to download the NXP i.MX Yocto BSP, configure the build environment for the **i.MX6ULL 14x14 EVK**, build the Yocto image, and flash the generated image to an SD card.

## Table of Contents

* [Prerequisites](#prerequisites)
* [BSP Setup](#bsp-setup)
* [Build Yocto Image](#build-yocto-image)
* [Flash Image to SD Card](#flash-image-to-sd-card)

---

## Prerequisites

The following hardware is required:

* i.MX6ULL 14x14 EVK board
* SD card
* SD-card reader
* USB-to-TTL converter
* Micro USB cable
* Ubuntu/Linux host machine

---

# BSP Setup

## 1. Create the Workspace

Create a workspace for the i.MX6ULL Yocto BSP:

```bash
mkdir ~/imx6ull
cd ~/imx6ull
```

---

## 2. Install the `repo` Tool

The NXP i.MX BSP consists of multiple Git repositories. The `repo` tool is used to manage these repositories.

### Create a personal `bin` directory

```bash
mkdir -p ~/bin
```

### Download the `repo` launcher

```bash
curl https://storage.googleapis.com/git-repo-downloads/repo > ~/bin/repo
```

### Make `repo` executable

```bash
chmod +x ~/bin/repo
```

### Add `~/bin` to PATH

```bash
export PATH=$HOME/bin:$PATH
```

> **Note:** The above `PATH` change applies only to the current terminal session.
> To make it permanent, add the following line to `~/.bashrc`:
>
> ```bash
> export PATH=$HOME/bin:$PATH
> ```

### Verify `repo`

```bash
repo version
```

Check the location:

```bash
which repo
```

Expected:

```text
/home/<username>/bin/repo
```

For example:

```text
/home/noor/bin/repo
```

---

## 3. Download the NXP i.MX BSP

Go to the workspace:

```bash
cd ~/imx6ull
```

Initialize the NXP i.MX manifest:

```bash
repo init \
    -u https://github.com/nxp-imx/imx-manifest \
    -b imx-linux-scarthgap \
    -m imx-6.6.52-2.2.0.xml
```

Synchronize the repositories:

```bash
repo sync -j8
```

After synchronization, the workspace will contain the required Yocto layers and BSP sources.

Typical structure:

```text
~/imx6ull/
├── sources/
│   ├── poky/
│   ├── meta-openembedded/
│   ├── meta-freescale/
│   ├── meta-imx/
│   └── ...
└── ...
```

---

# Build Yocto Image

## 1. Initialize the Build Environment

For the **i.MX6ULL 14x14 EVK**:

```bash
DISTRO=fsl-imx-xwayland \
MACHINE=imx6ull14x14evk \
source imx-setup-release.sh -b build
```

This configures:

```text
DISTRO  = fsl-imx-xwayland
MACHINE = imx6ull14x14evk
BUILD   = build
```

The Yocto build directory will be:

```text
~/imx6ull/build/
```

---

## 2. Build the Image

Build the i.MX core image:

```bash
bitbake imx-image-core
```

The first build can take a significant amount of time because Yocto downloads and compiles many components.

After a successful build, the generated images are available under:

```text
build/tmp/deploy/images/imx6ull14x14evk/
```

Check the generated files:

```bash
ls build/tmp/deploy/images/imx6ull14x14evk/
```

You may see:

```text
imx-image-core-imx6ull14x14evk.rootfs.wic
imx-image-core-imx6ull14x14evk.rootfs.wic.zst
imx-image-core-imx6ull14x14evk.rootfs.tar.zst
...
```

For SD-card flashing, the important files are:

```text
*.wic
```

or:

```text
*.wic.zst
```

A `.wic` image is a complete disk image containing the partition layout and required boot/root filesystem data.

---

# Flash Image to SD Card

## 1. Identify the SD Card

Insert the SD card into the host machine and run:

```bash
lsblk
```

Example:

```text
NAME   SIZE MOUNTPOINT
sda    512G
├─sda1 500G /
└─sda2  12G

sdb     32G
├─sdb1 256M /media/noor/BOOT
└─sdb2  31G /media/noor/rootfs
```

In this example:

```text
/dev/sdb
```

is the SD card.

Its partitions are:

```text
/dev/sdb1
/dev/sdb2
```

> ⚠️ **IMPORTANT:** Do not assume that `/dev/sdb` is your SD card.
> The device name can be different on your system.
>
> Always use `lsblk` and identify the SD card by its **size and partition layout**.

---

## 2. Unmount the SD Card

If the SD-card partitions are mounted, unmount them:

```bash
sudo umount /dev/sdb1
sudo umount /dev/sdb2
```

Or:

```bash
sudo umount /dev/sdb*
```

Verify:

```bash
lsblk
```

The SD-card partitions should no longer show a `MOUNTPOINT`.

Example:

```text
NAME   SIZE MOUNTPOINT
sdb     32G
├─sdb1 256M
└─sdb2  31G
```

---

## 3. Flash a `.wic` Image

If you have an uncompressed image:

```text
imx-image-core-imx6ull14x14evk.rootfs.wic
```

flash it to the **whole SD card**:

```bash
sudo dd \
    if=imx-image-core-imx6ull14x14evk.rootfs.wic \
    of=/dev/sdb \
    bs=4M \
    status=progress \
    conv=fsync
```

After `dd` finishes:

```bash
sync
```

### `dd` Options

| Option            | Description                         |
| ----------------- | ----------------------------------- |
| `if=`             | Input file — Yocto `.wic` image     |
| `of=`             | Output device — complete SD card    |
| `bs=4M`           | Write 4 MB blocks                   |
| `status=progress` | Show write progress                 |
| `conv=fsync`      | Flush data before `dd` exits        |
| `sync`            | Ensure pending writes are completed |

### Whole Device vs Partition

**Correct:**

```bash
of=/dev/sdb
```

**Incorrect:**

```bash
of=/dev/sdb1
```

```bash
of=/dev/sdb2
```

The `.wic` image contains its own partition table, so it must be written to the **whole SD-card device**.

---

## 4. Flash a `.wic.zst` Image

If Yocto generated:

```text
imx-image-core-imx6ull14x14evk.rootfs.wic.zst
```

the image is compressed using **Zstandard**.

### Decompress the image

```bash
zstd -d imx-image-core-imx6ull14x14evk.rootfs.wic.zst
```

This creates:

```text
imx-image-core-imx6ull14x14evk.rootfs.wic
```

Now verify the SD card again:

```bash
lsblk
```

Assuming the SD card is `/dev/sdb`:

```bash
sudo dd \
    if=imx-image-core-imx6ull14x14evk.rootfs.wic \
    of=/dev/sdb \
    bs=4M \
    status=progress \
    conv=fsync
```

Then:

```bash
sync
```

---

## 5. Safely Remove the SD Card

After `dd` and `sync` have completed:

```bash
sudo eject /dev/sdb
```

The SD card can now be removed and inserted into the i.MX6ULL board.

---

## Quick Reference

```bash
# Create workspace
mkdir ~/imx6ull
cd ~/imx6ull

# Install repo
mkdir -p ~/bin
curl https://storage.googleapis.com/git-repo-downloads/repo > ~/bin/repo
chmod +x ~/bin/repo
export PATH=$HOME/bin:$PATH

# Initialize BSP
repo init \
    -u https://github.com/nxp-imx/imx-manifest \
    -b imx-linux-scarthgap \
    -m imx-6.6.52-2.2.0.xml

# Download BSP
repo sync -j8

# Initialize Yocto
DISTRO=fsl-imx-xwayland \
MACHINE=imx6ull14x14evk \
source imx-setup-release.sh -b build

# Build
bitbake imx-image-core

# Find image
ls build/tmp/deploy/images/imx6ull14x14evk/

# Check SD card
lsblk

# Unmount SD-card partitions
sudo umount /dev/sdb*

# Flash .wic image
sudo dd \
    if=<image>.wic \
    of=/dev/sdb \
    bs=4M \
    status=progress \
    conv=fsync

# Flush writes
sync

# Eject SD card
sudo eject /dev/sdb
```

