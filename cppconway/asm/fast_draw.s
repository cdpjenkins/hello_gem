.global _fast_draw_strip

_fast_draw_strip:
    movem.l %d0-%d7/%a0-%a3, %sp@-

    move.l %sp@(52), %a0
    move.l %sp@(56), %a1

    movem.l %a0@+, %d0-%d7/%a2-%a3
    movem.l %d0-%d7/%a2-%a3, %a1@(0 * 640 / 8)
    movem.l %d0-%d7/%a2-%a3, %a1@(1 * 640 / 8)
    movem.l %d0-%d7/%a2-%a3, %a1@(2 * 640 / 8)
    movem.l %d0-%d7/%a2-%a3, %a1@(3 * 640 / 8)
    movem.l %d0-%d7/%a2-%a3, %a1@(4 * 640 / 8)
    movem.l %d0-%d7/%a2-%a3, %a1@(5 * 640 / 8)
    movem.l %d0-%d7/%a2-%a3, %a1@(6 * 640 / 8)

    movem.l %a0@+, %d0-%d7/%a2-%a3
    movem.l %d0-%d7/%a2-%a3, %a1@(40 + 0 * 640 / 8)
    movem.l %d0-%d7/%a2-%a3, %a1@(40 + 1 * 640 / 8)
    movem.l %d0-%d7/%a2-%a3, %a1@(40 + 2 * 640 / 8)
    movem.l %d0-%d7/%a2-%a3, %a1@(40 + 3 * 640 / 8)
    movem.l %d0-%d7/%a2-%a3, %a1@(40 + 4 * 640 / 8)
    movem.l %d0-%d7/%a2-%a3, %a1@(40 + 5 * 640 / 8)
    movem.l %d0-%d7/%a2-%a3, %a1@(40 + 6 * 640 / 8)

    movem.l %sp@+,  %d0-%d7/%a0-%a3

    rts
