# Plume

A lightweight anime CLI written in C++.

Plume searches for anime from the terminal using the Kitsu API.

## Overall use

You can just scrape and search anime from the command line, like something like
ani-cli or mov-cli.

## Usage

```bash
plume "Komi Can't Communicate"
```

## Dependencies

C++20 compiler

cmake

libcurl

nlohmann/json

```bash
cmake -S . -B build
```

```bash
cmake --build build
```

```bash
./build/plume "Komi"
```
