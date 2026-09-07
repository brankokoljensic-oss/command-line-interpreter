# Command Line Interpreter

C++ implementation of a command line interpreter (CLI) with support for pipes, I/O redirection, and batch processing. This project was developed as part of the Object-Oriented Programming course (2025/2026 academic year) at the School of Electrical Engineering, University of Belgrade.

## Features

- **Interactive command prompt** with customizable prompt string
- **Lexical analysis** with error reporting 
- **Pipeline support** — chain multiple commands with `|`
- **I/O redirection** — input (`<`), output (`>`), and append (`>>`)
- **Batch processing** — execute command scripts from files, including recursive batch calls
- **Multiple type error handling** with distinct error categories (syntax, command, file, unknown command)

## Supported Commands

| Command | Description |
|---------|-------------|
| `echo` | Pass-through |
| `wc` | Count words (`-w`), characters (`-c`)|
| `tr` | Find and replace text |
| `head` | Output first N lines |
| `time` | Display current time |
| `date` | Display current date |
| `touch` | Create an empty file |
| `truncate` | Clear file contents |
| `rm` | Delete a file |
| `batch` | Execute commands from a file |
| `prompt` | Change the command prompt |
| `exit` | Exit the interpreter |


## Usage Examples

Basic command execution:
```
$ echo "Hello, World!"
Hello, World!
$ wc -w "Lorem ipsum dolor sit amet"
5
```

Piping and redirection:
```
$ time | tr -":" "." | wc -c >output.txt
$ echo "Hello World" | wc -w
2
```

Batch execution:
```
$ batch script.txt
$ batch script.txt >output.txt
```

## License

This project is licensed under the MIT License.
