# Minishell

![42 School Badge](https://img.shields.io/badge/42-School-000000?style=for-the-badge&logo=42&logoColor=white)
![C Badge](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Build](https://img.shields.io/github/actions/workflow/status/herom-s/minishell/build.yml?branch=master&style=for-the-badge)
![Norminette](https://img.shields.io/github/actions/workflow/status/herom-s/minishell/norminette.yml?branch=master&label=norminette&style=for-the-badge)

A minimal shell implementation in C, recreating basic functionalities of bash. This project is part of the 42 School curriculum and serves as an introduction to process creation, file descriptors, and Unix system calls.

## 📋 Table of Contents

- [About](#about)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Testing](#testing)
- [Project Structure](#project-structure)
- [Implementation Details](#implementation-details)
- [Built-in Commands](#built-in-commands)
- [Technical Requirements](#technical-requirements)
- [Resources](#resources)
- [Contributing](#contributing)
- [Author](#author)

## 🎯 About

Minishell is a simple shell implementation that mimics the behavior of bash. The project challenges students to understand:

- **Process management** and system calls
- **Signal handling** and process control
- **Environment variables** and their manipulation
- **File descriptor** management and redirection
- **Parsing** and command execution
- **Memory management** in complex scenarios

This project bridges the gap between basic C programming and understanding how shells work at a system level.

## ✨ Features

### Core Functionality
- ✅ Display a prompt when waiting for commands
- ✅ Command history (working history between sessions)
- ✅ Search and launch executables (using PATH or relative/absolute paths)
- ✅ Handle quotes (single `'` and double `"`)
- ✅ Redirections:
  - `<` : input redirection
  - `>` : output redirection
  - `<<` : here-doc (read until delimiter)
  - `>>` : output redirection in append mode
- ✅ Pipes (`|`) to connect command outputs to inputs
- ✅ Environment variable expansion (`$VAR`)
- ✅ Special variable `$?` for last exit status
- ✅ Signal handling:
  - `ctrl-C` : display new prompt
  - `ctrl-D` : exit shell
  - `ctrl-\` : do nothing

### Built-in Commands
- `echo` with option `-n`
- `cd` with relative or absolute path
- `pwd` with no options
- `export` with no options
- `unset` with no options
- `env` with no options or arguments
- `exit` with no options

## 🚀 Installation

### Prerequisites
- GCC or Clang compiler
- Make
- Unix-like system (Linux, macOS)
- CMocka (for testing - automatically installed by Makefile)

### Build Instructions

1. **Clone the repository with submodules:**
```bash
git clone --recursive https://github.com/herom-s/minishell.git
cd minishell
```

2. **Build the project:**
```bash
make
```

3. **Build with bonus features (if available):**
```bash
make bonus
```

4. **Clean build artifacts:**
```bash
make clean    # Remove object files
make fclean   # Remove object files and executable
make re       # Rebuild everything
```

## 💻 Usage

### Basic Usage

Run the shell:
```bash
./minishell
```

### Example Commands

```bash
# Simple command execution
minishell$ ls -la

# Pipes
minishell$ cat file.txt | grep "pattern" | wc -l

# Redirections
minishell$ echo "Hello, World!" > output.txt
minishell$ cat < input.txt > output.txt

# Here-doc
minishell$ cat << EOF
> line 1
> line 2
> EOF

# Environment variables
minishell$ echo $HOME
minishell$ export MY_VAR=42
minishell$ echo $MY_VAR

# Exit status
minishell$ ls /nonexistent
minishell$ echo $?

# Built-ins
minishell$ cd /tmp
minishell$ pwd
minishell$ export PATH=/usr/bin:/bin
minishell$ env
minishell$ exit
```

## 🧪 Testing

The project includes comprehensive unit tests using CMocka.

### Running Tests

```bash
# Install CMocka and run all tests
make test

# Clean test artifacts
make fclean
```

### Test Coverage

The test suite covers:
- ✅ Main function structure and behavior
- ✅ Output validation
- ✅ Argument handling
- ✅ Return values
- ✅ String manipulation
- ✅ Memory management

## 📁 Project Structure

```
minishell/
├── src/                  # Source files
│   └── main.c           # Entry point
├── src_bonus/           # Bonus implementation (if applicable)
├── inc/                 # Header files
│   └── minishell.h     # Main header
├── obj/                 # Object files (generated)
├── tests/               # Test files
│   └── test_main.c     # Unit tests
├── libft/              # Custom C library (submodule)
├── cmocka/             # Testing framework (auto-installed)
├── Makefile            # Build configuration
└── README.md           # This file
```

## 🔧 Implementation Details

### Architecture

The minishell follows a modular architecture:

1. **Lexer**: Tokenizes input into meaningful elements
2. **Parser**: Builds a syntax tree from tokens
3. **Expander**: Handles variable expansion and quote removal
4. **Executor**: Runs commands and manages processes
5. **Built-ins**: Implements shell built-in commands

### Key Components

#### Signal Handling
```c
// Handles ctrl-C, ctrl-D, and ctrl-\ appropriately
// Different behavior in interactive vs non-interactive mode
```

#### Environment Variables
```c
// Manages a local copy of environment variables
// Implements export, unset, and variable expansion
```

#### Process Management
```c
// Creates child processes for external commands
// Manages pipes and file descriptors
// Handles process termination and exit status
```

## 🛠️ Built-in Commands

### `echo`
```bash
minishell$ echo -n "Hello, 42!"  # No trailing newline
minishell$ echo "Hello, 42!"     # With newline
```

### `cd`
```bash
minishell$ cd /tmp              # Absolute path
minishell$ cd ..                # Relative path
minishell$ cd                   # Go to HOME
```

### `pwd`
```bash
minishell$ pwd
/current/working/directory
```

### `export`
```bash
minishell$ export VAR=value
minishell$ export              # List all exported variables
```

### `unset`
```bash
minishell$ unset VAR
```

### `env`
```bash
minishell$ env                 # Display environment variables
```

### `exit`
```bash
minishell$ exit                # Exit with last status
minishell$ exit 42             # Exit with specific code
```

## 📋 Technical Requirements

### Compilation
- **Compiler**: GCC or Clang
- **Flags**: `-Wall -Wextra -Werror`
- **Standard**: C99 or later
- **Norm**: Must follow 42 School's Norminette

### Memory Management
- No memory leaks (except from readline)
- All heap-allocated memory must be properly freed
- Proper handling of file descriptors

### Error Handling
- Robust error handling for system calls
- Appropriate error messages
- Proper cleanup on errors

### External Functions Allowed
- `readline`, `rl_clear_history`, `rl_on_new_line`
- `rl_replace_line`, `rl_redisplay`, `add_history`
- `printf`, `malloc`, `free`, `write`
- `access`, `open`, `read`, `close`
- `fork`, `wait`, `waitpid`, `wait3`, `wait4`
- `signal`, `sigaction`, `sigemptyset`, `sigaddset`
- `kill`, `exit`, `getcwd`, `chdir`
- `stat`, `lstat`, `fstat`, `unlink`
- `execve`, `dup`, `dup2`, `pipe`
- `opendir`, `readdir`, `closedir`
- `strerror`, `perror`, `isatty`, `ttyname`
- `ttyslot`, `ioctl`, `getenv`, `tcsetattr`
- `tcgetattr`, `tgetent`, `tgetflag`, `tgetnum`
- `tgetstr`, `tgoto`, `tputs`

## 📚 Resources

### Documentation
- [GNU Bash Manual](https://www.gnu.org/software/bash/manual/)
- [POSIX Shell Standard](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)
- [Advanced Programming in the UNIX Environment](https://www.amazon.com/Advanced-Programming-UNIX-Environment-3rd/dp/0321637739)

### Useful Tools
- [Valgrind](https://valgrind.org/) - Memory leak detection
- [GDB](https://www.gnu.org/software/gdb/) - Debugging
- [strace](https://strace.io/) - System call tracing

### Testing
- [minishell_tester](https://github.com/LucasKuhn/minishell_tester)
- [42_minishell_tester](https://github.com/thallard/minishell_tester)

## 🤝 Contributing

This is a 42 School project and should be completed individually. However, after evaluation:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/improvement`)
3. Commit your changes (`git commit -am 'Add improvement'`)
4. Push to the branch (`git push origin feature/improvement`)
5. Create a Pull Request

## 👨‍💻 Author

**herom-s** - *42 School Student*

- GitHub: [@herom-s](https://github.com/herom-s)
- 42 Intranet: `hermarti`

## 📄 License

This project is part of the 42 School curriculum. Feel free to use it as a reference, but please don't copy it directly for your own project.

## 🎓 42 School

This project is part of the 42 School curriculum. 42 is a future-proof computer science training program that teaches students through peer-to-peer learning and project-based education.

## 🙏 Acknowledgments

- Thanks to all the 42 students who shared their knowledge
- The GNU Bash team for excellent documentation
- The open-source community for various testing tools

---

**Note:** If you are a current 42 student, I encourage you to implement your own version rather than copying code. The learning process is more valuable than the final result. Understanding how shells work at a deep level will serve you throughout your career.

*"As we say in the sewer, if you're not prepared to go all the way, don't put your boots on in the first place."* - Ed Norton
