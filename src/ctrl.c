// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.

#include <stdio.h>
#include "ctrl.h"
#include "thumbstick.h"
#include "common.h"

// Encode a sequence of 5 directions into a single uint8.
u8 ctrl_glyph_encode(Glyph glyph) {
    u8 encoded = 0;
    u8 len = 5;
    // Determine length.
    if      (glyph[1] == 0) len = 1;
    else if (glyph[2] == 0) len = 2;
    else if (glyph[3] == 0) len = 3;
    else if (glyph[4] == 0) len = 4;
    printf("ctrl_glyph_encode len=%i ", len);
    // Initial direction.
    encoded += glyph[0] - 1;  // -1 because DIR4_NONE.
    // Termination bit.
    encoded += 1 << 1+len;
    // Remaining directions.
    for(u8 i=1; i<len; i++) {
        // Each subsequent direction is either clockwise or anticlockwise, so
        // it is encoded in a single bit.
        if (glyph[i-1] == DIR4_UP    && glyph[i] == DIR4_RIGHT) encoded += (1 << 1+i);
        if (glyph[i-1] == DIR4_RIGHT && glyph[i] == DIR4_DOWN)  encoded += (1 << 1+i);
        if (glyph[i-1] == DIR4_DOWN  && glyph[i] == DIR4_LEFT)  encoded += (1 << 1+i);
        if (glyph[i-1] == DIR4_LEFT  && glyph[i] == DIR4_UP)    encoded += (1 << 1+i);
    }
    return encoded;
}

// Decode a sequence of 5 directions from a single uint8.
void ctrl_glyph_decode(Glyph glyph, u8 encoded) {
    u8 len;
    // Determine length.
    if (encoded >> 2 == 1) len = 1;
    if (encoded >> 3 == 1) len = 2;
    if (encoded >> 4 == 1) len = 3;
    if (encoded >> 5 == 1) len = 4;
    if (encoded >> 6 == 1) len = 5;
    // Initial direction.
    glyph[0] = (encoded & 0b00000011) + 1;  // +1 because DIR4_NONE.
    // Remaining directions.
    for(u8 i=1; i<len; i++) {
        if (encoded & (1<<i+1)) {
            // Clockwise.
            if (glyph[i-1] == DIR4_UP)    glyph[i] = DIR4_RIGHT;
            if (glyph[i-1] == DIR4_RIGHT) glyph[i] = DIR4_DOWN;
            if (glyph[i-1] == DIR4_DOWN)  glyph[i] = DIR4_LEFT;
            if (glyph[i-1] == DIR4_LEFT)  glyph[i] = DIR4_UP;
        } else {
            // Anti-clockwise.
            if (glyph[i-1] == DIR4_UP)    glyph[i] = DIR4_LEFT;
            if (glyph[i-1] == DIR4_RIGHT) glyph[i] = DIR4_UP;
            if (glyph[i-1] == DIR4_DOWN)  glyph[i] = DIR4_RIGHT;
            if (glyph[i-1] == DIR4_LEFT)  glyph[i] = DIR4_DOWN;
        }
    }
}
