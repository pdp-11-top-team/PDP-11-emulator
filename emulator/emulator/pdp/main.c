//
//  main.c
//  emulator.c
//
//  Created by Jenny on 05.10.15.
//  Copyright © 2015 com.mipt. All rights reserved.
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "emulator.c"

int main(void) {
    emu_init();
    emu_run();
    
    return 0;
}