# 🎵 MP3 Tag Reader (C Project)

## 📌 Overview

This project reads metadata from MP3 files using C programming.
It supports ID3v2.3 tag parsing to extract information like Title and Artist.

---

## 🚀 Features

- Reads MP3 files in binary mode
- Supports ID3v2.3 parsing
- Extracts Title, Artist, Album, Year, and Genre
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

---

## ⚙️ How to Run

### Compile

```bash
gcc tag_reader.c tag_function.c -o mp3reader
```

### Run

```bash
./mp3reader
```

---

## 🎯 Future Improvements

- Add support for other ID3 versions
- Improve error handling and robustness

---

## 👩‍💻 Author

**Swati Pathak**
