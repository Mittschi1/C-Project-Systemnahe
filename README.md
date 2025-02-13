# wordcount-thws - Multi-threaded Word Count

wordcount-thws is a multi-threaded implementation of the Unix wc command made for a student project. It can process multiple files in parallel and count lines, words, bytes, and ASCII characters.

## Features

- Count lines, words, bytes, and ASCII characters
- Configurable output options
- Process multiple files
- Multi-threaded file processing
- Piping on stdin and stdout

## Prerequisites

To build and run wordcount-thws, you need:

- GCC compiler 
- Linux operating system
- make utility

## Building the Program

1. Clone or download the repository
2. Navigate to the project directory
3. Run make:

```bash
make
```
or:

```bash
make all
```

This will create the `wordcount-thws` executable in the project directory. 

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
./wordcount-thws [OPTION]... [FILE]...
```

### Options

- `-l, --lines`: Print line count
- `-w, --words`: Print word count
- `-c, --bytes`: Print byte count
- `-m, --chars`: Print character count
- `--help`: Display help message

If no options are specified, wordcount-thws displays line, word, and byte counts.
If no file is specified, wordcount-thws reads from standard input.

### Examples

Count lines, words, and bytes in a single file:
```bash
./wordcount-thws file.txt
```

Count only lines in multiple files:
```bash
./wordcount-thws -l file1.txt file2.txt file3.txt
```

Count words and characters in files:
```bash
./wordcount-thws -w -m *.txt
```

Read from standard input:
```bash
echo "Hello, World!" | ./wordcount-thws
```
Write to standard output:
```bash
./wordcount-thws file.txt > output.txt
```

## Output Format

For each file, wordcount-thws displays counts in the following order (if selected):
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

## Error Handling

wordcount-thws handles various error conditions:
- File not found
- Permission denied
- Memory allocation failures
- Invalid options

Error messages are printed to stderr.


