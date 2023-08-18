.global _fast_draw_strip

_fast_draw_strip:
    movem.l d0-d7/a0-a3, -(sp)

    move.l 52(sp), a0
    move.l 56(sp), a1

    movem.l (a0)+, d0-d7/a2-a3
    movem.l d0-d7/a2-a3, (0 * 640 / 8)(a1)
    movem.l d0-d7/a2-a3, (1 * 640 / 8)(a1)
    movem.l d0-d7/a2-a3, (2 * 640 / 8)(a1)
    movem.l d0-d7/a2-a3, (3 * 640 / 8)(a1)
    movem.l d0-d7/a2-a3, (4 * 640 / 8)(a1)
    movem.l d0-d7/a2-a3, (5 * 640 / 8)(a1)
    movem.l d0-d7/a2-a3, (6 * 640 / 8)(a1)

    movem.l a0@+, %d0-%d7/%a2-%a3
    movem.l d0-d7/a2-a3, (40 + 0 * 640 / 8)(a1)
    movem.l d0-d7/a2-a3, (40 + 1 * 640 / 8)(a1)
    movem.l d0-d7/a2-a3, (40 + 2 * 640 / 8)(a1)
    movem.l d0-d7/a2-a3, (40 + 3 * 640 / 8)(a1)
    movem.l d0-d7/a2-a3, (40 + 4 * 640 / 8)(a1)
    movem.l d0-d7/a2-a3, (40 + 5 * 640 / 8)(a1)
    movem.l d0-d7/a2-a3, (40 + 6 * 640 / 8)(a1)

    movem.l (sp)+,  d0-d7/a0-a3

    rts
