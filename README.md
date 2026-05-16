# 🎵 MP3 Tag Reader & Editor (C Project)

## 📌 Overview

This project reads and edits metadata from MP3 files using C programming.
It supports ID3v2.3 tag parsing to extract and modify information like Title, Artist, Album, Year, Genre, Track, and Comment via a command-line interface.

---

## 🚀 Features

- Reads MP3 files in binary mode
- Supports ID3v2.3 parsing
- Extracts Title, Artist, Album, Year, Genre, Track, and Comment
- Edits individual ID3v2.3 tags via CLI flags
- Demonstrates low-level file handling and parsing
- Handles missing tags gracefully (no infinite loop)
- Validates ID3 version before parsing

---

## 🛠️ Concepts Used

- File handling (binary mode)
- Bit manipulation
- Byte-level parsing
- Memory management
- Structures in C
- Parsing variable-length frames using tag size
- Handling edge cases in binary file parsing
- Preventing infinite loops using boundary checks
- Command-line argument parsing

---

## ⚙️ How to Run

### Compile

```bash
gcc tag_reader.c tag_function.c -o mp3reader
```

### Run

**View tags:**
```bash
./mp3reader -v <mp3filename>
```

**Edit a tag:**
```bash
./mp3reader -e -[flag] "new value" <mp3filename>
```

---

## 🖥️ CLA Options

| Flag | Description |
|------|-------------|
| `-v` | Prints version info / displays tag data |
| `-e` | Edit mode — must be followed by a tag flag |
| `-t` | Modifies the **Title** tag |
| `-T` | Modifies the **Track** tag |
| `-a` | Modifies the **Artist** tag |
| `-A` | Modifies the **Album** tag |
| `-y` | Modifies the **Year** tag |
| `-c` | Modifies the **Comment** tag |
| `-g` | Modifies the **Genre** tag |
| `-h` | Displays help info |

### Examples

```bash
# Display tags
./mp3reader -v song.mp3

# Edit the title
./mp3reader -e -t "New Title" song.mp3

# Edit the artist
./mp3reader -e -a "Artist Name" song.mp3

# Edit the year
./mp3reader -e -y "2024" song.mp3
```

---

## 🎯 Future Improvements

- Add support for other ID3 versions
- Improve error handling and robustness
- Support batch editing across multiple files

---

## 👩‍💻 Author

**Swati Pathak**