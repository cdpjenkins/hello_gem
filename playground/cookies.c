#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <mint/sysbind.h>

#define S_INQUIRE     0xFFFF
#define S_OSNAME      0x0000
#define S_OSXNAME     0x0001
#define S_OSVERSION   0x0002
#define S_OSBUILDDATE 0x0004
#define S_GETCOOKIE   0x0008

#define SSYSTEM_IS_SUPPORTED 0

typedef struct
{
    uint32_t id;
    uint32_t value;
} Cookie;

int main(int argc, char** argv) {
    uint32_t result;
    uint16_t i;

    result = Ssystem(S_INQUIRE, 0, 0);
    printf("S_INQUIRE: %08X\n", result);

    if (result == SSYSTEM_IS_SUPPORTED) {
        result = Ssystem(S_OSNAME, 0, 0);
        printf("S_OSNAME:      %.4s\n", &result);

        result = Ssystem(S_OSXNAME, 0, 0);
        printf("S_OSXNAME:     %.4s\n", &result);

        {
            uint8_t char_summary;
            uint8_t patch_level;
            uint8_t minor;
            uint8_t major;

            result = Ssystem(S_OSVERSION, 0, 0);
            char_summary = result & 0xFF;
            patch_level = (result >> 8) & 0xFF;
            minor = (result >> 16) & 0xFF;
            major = (result >> 24) & 0xFF;
            printf("S_OSVERSION:   %d.%d.%d%c\n", major, minor, patch_level, 0);
        }    

        result = Ssystem(S_OSBUILDDATE, 0, 0);
        printf("S_OSBUILDDATE: %d-%d-%d\n", result & 0xFFFF, (result >> 16) & 0xFF, (result >> 24) & 0xFF);
    
        for (i = 0; i < 100; i++) {
            uint32_t cookie_id = Ssystem(S_GETCOOKIE, i, 0);
            printf("cookie_id: %08X\n", cookie_id);
            if (cookie_id != 0) {
                uint32_t value = Ssystem(S_GETCOOKIE, cookie_id, 0);
                printf("%8d %.4s %08X\n", i, &cookie_id, value);
            } else {
                // TODO bit of an evil HACK
                break;
            }
        }
    } else {
        Cookie *cookiejar;

        cookiejar = (Cookie *)(Setexc(0x05A0/4, (const void (*)(void))-1));

        if (cookiejar) {
            for(i=0 ; cookiejar[i].id ; i++) {
                if (cookiejar[i].id) {
                    printf("%8d %.4s %08X\n", i, &cookiejar[i].id, cookiejar[i].value);
                }
            }
        } else {
            printf("We totally can't get any cookies\n");
        }
    }

    printf("Done. Press any key to exit.\n");
    Cconin();
}
