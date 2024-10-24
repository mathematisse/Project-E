import os

def remove_lines_from_file(file_path, text_to_remove):
    # Read the content of the file
    with open(file_path, 'r') as file:
        lines = file.readlines()

    # Filter out lines containing the specific text
    lines = [line for line in lines if text_to_remove not in line]

    # Write the filtered content back to the file
    with open(file_path, 'w') as file:
        file.writelines(lines)

def process_directory(directory, text_to_remove):
    # Walk through directory and all subdirectories
    for root, _, files in os.walk(directory):
        for file in files:
            file_path = os.path.join(root, file)
            # Check if it's a file that should be processed
            if file.endswith(('.cpp', '.hpp', '.h', '.c', '.cc')):  # You can adjust the file types as necessary
                print(f'Processing file: {file_path}')
                remove_lines_from_file(file_path, text_to_remove)

if __name__ == "__main__":
    # Set the directory and the text pattern to remove
    directory_to_process = "."
    text_to_remove = "&other) = default;"
    
    # Start processing the directory
    process_directory(directory_to_process, text_to_remove)
