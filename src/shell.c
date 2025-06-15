#include "../include/kernel.h"

#define COMMAND_BUFFER_SIZE 256
static char command_buffer[COMMAND_BUFFER_SIZE];
static int buffer_pos = 0;

// Function to initialize the shell
void shell_initialize() {
    terminal_writestring("KFS Shell v1.0\n");
    terminal_writestring("Type 'help' for available commands\n");
    terminal_writestring("> ");
}

// Process a command
void shell_process_command(const char* cmd) {
    // Extract the first word as the command
    char command[32] = {0};
    char arg[32] = {0};
    int i = 0, j = 0;
    
    // Parse command and argument
    while (cmd[i] && cmd[i] != ' ' && i < 31) {
        command[i] = cmd[i];
        i++;
    }
    command[i] = '\0';
    
    // Skip spaces
    while (cmd[i] && cmd[i] == ' ') {
        i++;
    }
    
    // Get argument if any
    while (cmd[i] && j < 31) {
        arg[j] = cmd[i];
        i++;
        j++;
    }
    arg[j] = '\0';
    
    if (strcmp(command, "help") == 0) {
        terminal_writestring("Available commands:\n");
        terminal_writestring("  help         - Display this help message\n");
        terminal_writestring("  stack        - Print the kernel stack\n");
        terminal_writestring("  push <hex>   - Push a value onto the stack\n");
        terminal_writestring("  pop          - Pop a value from the stack\n");
        terminal_writestring("  clear        - Clear the screen\n");
        terminal_writestring("  reboot       - Reboot the system\n");
        terminal_writestring("  halt         - Halt the system\n");
        terminal_writestring("  shutdown     - Shutdown the system\n");
    } else if (strcmp(command, "stack") == 0) {
        print_kernel_stack();
    } else if (strcmp(command, "push") == 0) {
        if (arg[0] == '\0') {
            terminal_writestring("Error: push requires a hexadecimal value\n");
        } else {
            // Convert hex string to integer
            uint32_t value = 0;
            i = 0;
            
            // Skip "0x" prefix if present
            if (arg[0] == '0' && (arg[1] == 'x' || arg[1] == 'X')) {
                i = 2;
            }
            
            while (arg[i] != '\0') {
                value <<= 4;  // Multiply by 16
                
                if (arg[i] >= '0' && arg[i] <= '9') {
                    value += arg[i] - '0';
                } else if (arg[i] >= 'a' && arg[i] <= 'f') {
                    value += arg[i] - 'a' + 10;
                } else if (arg[i] >= 'A' && arg[i] <= 'F') {
                    value += arg[i] - 'A' + 10;
                } else {
                    terminal_writestring("Error: Invalid hexadecimal value\n");
                    value = 0;
                    break;
                }
                i++;
            }
            
            if (value != 0 || (arg[0] == '0' && arg[1] == '\0')) {
                stack_push(value);
                terminal_writestring("Pushed 0x");
                
                // Print the value in hex
                for (int j = 7; j >= 0; j--) {
                    uint8_t nibble = (value >> (j * 4)) & 0xF;
                    char hex_char = nibble < 10 ? '0' + nibble : 'A' + (nibble - 10);
                    terminal_putchar(hex_char);
                }
                
                terminal_writestring(" onto the stack\n");
            }
        }
    } else if (strcmp(command, "pop") == 0) {
        if (stack_is_empty()) {
            terminal_writestring("Error: Stack is empty\n");
        } else {
            uint32_t value = stack_pop();
            terminal_writestring("Popped 0x");
            
            // Print the value in hex
            for (int j = 7; j >= 0; j--) {
                uint8_t nibble = (value >> (j * 4)) & 0xF;
                char hex_char = nibble < 10 ? '0' + nibble : 'A' + (nibble - 10);
                terminal_putchar(hex_char);
            }
            
            terminal_writestring(" from the stack\n");
        }
    } else if (strcmp(command, "clear") == 0) {
        terminal_initialize();
    } else if (strcmp(command, "reboot") == 0) {
        terminal_writestring("Rebooting...\n");
        // Send reset command to keyboard controller
        outb(0x64, 0xFE);
    } else if (strcmp(command, "halt") == 0) {
        terminal_writestring("System halted\n");
        // Halt the CPU
        __asm__ volatile("hlt");
    } else if (strcmp(command, "shutdown") == 0) {
        terminal_writestring("Shutting down...\n");
        
        // Try ACPI shutdown first (if available)
        outw(0xB004, 0x2000);
        
        // If ACPI shutdown fails, try APM shutdown
        outw(0x604, 0x2000);
        
        // If all else fails, just halt the CPU
        terminal_writestring("Shutdown failed, halting CPU\n");
        __asm__ volatile("cli; hlt");
    } else if (command[0] != '\0') {
        terminal_writestring("Unknown command: ");
        terminal_writestring(command);
        terminal_writestring("\n");
    }
    
    terminal_writestring("> ");
}

// Handle keyboard input for the shell
void shell_handle_input(char c) {
    if (c == '\n') {
        // Process the command when Enter is pressed
        terminal_putchar('\n');
        command_buffer[buffer_pos] = '\0';
        shell_process_command(command_buffer);
        buffer_pos = 0;
    } else if (c == '\b' && buffer_pos > 0) {
        // Handle backspace
        buffer_pos--;
        terminal_putchar('\b');
        terminal_putchar(' ');
        terminal_putchar('\b');
    } else if (c >= ' ' && c <= '~' && buffer_pos < COMMAND_BUFFER_SIZE - 1) {
        // Add printable characters to the buffer
        command_buffer[buffer_pos++] = c;
        terminal_putchar(c);
    }
}

// Helper function to compare strings
int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
