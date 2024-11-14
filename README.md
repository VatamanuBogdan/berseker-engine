# Berseker Engine

## Overview

**Berseker Engine** is a game engine that focuses on collision detection. Currently, it uses **OpenGL** as the primary graphic API but it was designed to be extendable to **DirectX** and **Metal**.

![Berseker Engine Preview](documentation/berseker-preview.gif)

## Getting Started

### Clone the project

Clone the project recursive in order to init its submodule: 

```bash
git clone --recursive https://github.com/VatamanuBogdan/berseker-engine.git
```

If you already cloned the repository without `--recursive` run the following command inside the project root directory:

```bash
git submodule update --init --recursive
```

### Load utility scripts

Before starting to deploy the project you have to load the [bootstrap.sh](scripts/bootstrap.sh) script in order to load Bersker's utilities:

```bash
source scripts/bootstrap.sh
```

### Build

To build the project you have to run `bersker_build` command followed by the build type inside the project root directory:

```bash
berseker_build <release/debug>
```

### Clean

To clean the project you have to run `berseker_cleanup` inside the project root directory:

```bash
berseker_cleanup
```

### Run

To run the project you have to run `berseker_run` inside the project root directory:

```bash
berseker_run
```

### Reload shaders

Every time when you define a new shader inside `resources/shader` directory run `berseker_reload_shaders` in order to make it visible to the resource loader:

```bash
berseker_reload_shaders
```
