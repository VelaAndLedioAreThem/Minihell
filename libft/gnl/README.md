# Get_Next_Line
Get_Next_Line is a function commonly used in C programming to read an entire line from a file descriptor or input stream until a newline character is encountered. It is typically used in situations where you need to process a file or input stream line by line.


## Features

-   **Reads one line at a time**: Handles input line by line until the end of the file or input stream.
-   **Dynamic memory allocation**: Returns a dynamically allocated string containing the line read.
-   **Handles large files**: Efficiently manages large files or streams using a buffer to read data in chunks.
-   **Robust**: Handles various edge cases, such as empty lines, very long lines, and files without newline characters.

## Function Prototype
```
char *get_next_line(int fd);
```

### Parameters

-   `int fd`: The file descriptor of the file or input stream to read from.

### Return Value

-   Returns a `char *` pointing to the next line read from the file descriptor, including the newline character if present.
-   Returns `NULL` if the end of the file is reached or if an error occurs.

## Usage

1.  **Include the Header**: Ensure the `get_next_line.h` header file is included in your C program.

    ```
     #include "get_next_line.h"
     ```

2. **Call the Function**: Use `get_next_line` in a loop to read each line from the file or input stream.

```
int fd = open("example.txt", O_RDONLY);
char *line;

while ((line = get_next_line(fd)) != NULL)
{
    printf("%s", line);
    free(line);
}
close(fd);
```
3. **Memory Management**: The returned line is dynamically allocated and must be freed by the caller to avoid memory leaks.

```
free(line);
```
## Buffer Size

The buffer size used by `get_next_line` is defined in the implementation. If you wish to change it, modify the `BUFFER_SIZE` macro in the `get_next_line.h` file:

```
#define BUFFER_SIZE 1024
```
## Edge Cases

`get_next_line` handles the following edge cases:

-   Empty lines.
-   Files with no newline characters.
-   Very long lines that exceed the buffer size.
-   File descriptors that refer to standard input (stdin), files, or other streams.

## Error Handling

-   If `fd` is invalid or if an error occurs during memory allocation, the function returns `NULL`.
-   It is recommended to check the return value of `get_next_line` and handle errors appropriately.

## Example

Here’s a simple example to demonstrate `get_next_line`:

```
#include "get_next_line.h"

int main(void)
{
    int fd = open("example.txt", O_RDONLY);
    char *line;

    if (fd == -1)
    {
        perror("Error opening file");
        return (1);
    }

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

## Compiling

### Mandatory part
```
gcc -Wall -Wextra -Werror -I include get_next_line.c get_next_line_utils.c main.c
```
### Bonus part
```
gcc -Wall -Wextra -Werror -I include get_next_line_bonus.c get_next_line_utils_bonus.c main.c
```

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.

## Contributing

Contributions are welcome! Feel free to submit a pull request or open an issue to suggest improvements or report bugs.
