#!/bin/bash

OUTPUT_FILE="code.txt"

# Clear existing output file
> "$OUTPUT_FILE"

# Find and process all C source and header files
find . -type f \( -name "*.c" -o -name "*.h" \) -print0 | while IFS= read -r -d $'\0' file; do
    # Add file header
    echo -e "\n\n### START OF FILE: $file ###\n" >> "$OUTPUT_FILE"
    # Append file content
    cat "$file" >> "$OUTPUT_FILE"
    echo -e "\n### END OF FILE: $file ###" >> "$OUTPUT_FILE"
done

echo "All .c and .h files have been combined into $OUTPUT_FILE"