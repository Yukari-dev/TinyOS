// VGA text mode base address
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
unsigned short* videomem = (unsigned short*)0xB8000;

// Clear screen with spaces (black background, light grey text)
void clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        videomem[i] = 0x07;  // ' ' (space) with attribute 0x07
    }
}

// Print a string starting at current position (simple version)
void print(const char* str) {
    static int pos = 0;  // keeps track of where we are on screen

    while (*str) {
        if (*str == '\n') {
            // Move to next line
            pos += VGA_WIDTH;
            pos -= pos % VGA_WIDTH;  // align to start of line
        } else {
            videomem[pos] = (*str) | (0x07 << 8);  // char + white on black
            pos++;
        }
        str++;
    }

    // Simple wrap-around (optional)
    if (pos >= VGA_WIDTH * VGA_HEIGHT) {
        pos = 0;
    }
}

void main(){
    clear_screen();
    print("Welcome to T-OS");
    while(1);
}
