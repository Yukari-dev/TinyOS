#define WIDTH 320
#define HEIGHT 200


void put_pixel(int x, int y, unsigned char color){
    unsigned char* screen = (unsigned char*)0xA0000;
    screen[y * 320 + x] = color;
}

void main(){
    for(int y = 0; y < HEIGHT; y++){
        for(int x = 0; x < WIDTH; x++){
            put_pixel(x, y, 0x03);
        }
    }
    while (1);
}

