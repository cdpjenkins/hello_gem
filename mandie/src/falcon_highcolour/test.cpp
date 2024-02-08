#include <cstring>
#include <cstdint>
#include <cstdio>

uint16_t logical_screen[640*480];
uint16_t physical_screen[640*480];

int main() {
    printf("About to memset\n");

    memset(logical_screen, 0xFF, sizeof(logical_screen));
    memset(physical_screen, 0xFF, sizeof(logical_screen));

    printf("Done memset\n");
    printf("Done memset\n");
    printf("Done memset\n");
    printf("Done memset\n");
    printf("Done memset\n");
    printf("Done memset\n");
    printf("Done memset\n");
    printf("Done memset\n");
    printf("Done memset\n");
    printf("Done memset\n");
    printf("Done memset\n");
    printf("Done memset\n");
    printf("Done memset\n");
    printf("Done memset\n");
    printf("Done memset\n");
    printf("Done memset\n");
    printf("Done memset\n");
    printf("Done memset\n");
}
