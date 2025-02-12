# budgetWC - Multi-threaded Word Count Utility

budgetWC is a high-performance, multi-threaded implementation of the classic word count utility. It can process multiple files in parallel and count lines, words, bytes, and ASCII characters.

## Features

- Multi-threaded file processing for improved performance
- Count lines, words, bytes, and ASCII characters
- Process multiple files in parallel
- Configurable output options
- Thread-safe operations

## Prerequisites

To build and run budgetWC, you need:

- GCC compiler (version 4.8 or later)
- POSIX-compliant operating system (Linux, Unix)
- pthread library
- make utility

## Building the Program

1. Clone or download the repository
2. Navigate to the project directory
3. Run make:

```bash
make
```

This will create the `budgetWC` executable in the project directory.

To build with debug information:
```bash
make debug
```

To run tests:
```bash
make test
```

To clean build files:
```bash
make clean
```

## Usage

Basic usage:
```bash
./budgetWC [OPTION]... [FILE]...
```

### Options

- `-l, --lines`: Print line count
- `-w, --words`: Print word count
- `-c, --bytes`: Print byte count
- `-m, --chars`: Print character count
- `--help`: Display help message

If no options are specified, budgetWC displays line, word, and byte counts.
If no file is specified, budgetWC reads from standard input.

### Examples

Count lines, words, and bytes in a single file:
```bash
./budgetWC file.txt
```

Count only lines in multiple files:
```bash
./budgetWC -l file1.txt file2.txt file3.txt
```

Count words and characters in files:
```bash
./budgetWC -w -m *.txt
```

Read from standard input:
```bash
echo "Hello, World!" | ./budgetWC
```

## Output Format

For each file, budgetWC displays counts in the following order (if selected):
1. Line count
2. Word count
3. Byte count
4. Character count
5. Filename

When processing multiple files, a total line is added at the end.

Example output:
```
   3    10   100 file1.txt
   5    20   200 file2.txt
   8    30   300 total
```

## Performance

The program uses a thread pool to process multiple files concurrently. The number of worker threads is defined in `threadpool.h` and can be adjusted based on your system's capabilities.

Default configuration:
- Number of worker threads: 4
- Maximum work queue size: 100

## Error Handling

budgetWC handles various error conditions:
- File not found
- Permission denied
- Memory allocation failures
- Invalid options

Error messages are printed to stderr with appropriate exit codes.

## Contributing

Feel free to submit issues, fork the repository, and create pull requests for any improvements.

## License

This project is licensed under the MIT License - see the LICENSE file for details.
# C-Project-Systemnahe-Programmierung git init git add README.md
# C-Project-Systemnahe
# C-Project-Systemnahe
# C-Project-Systemnahe
