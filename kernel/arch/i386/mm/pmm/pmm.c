#include "pmm.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "pmm_init_stage1.h"
#include "../../useful/useful_things.h"
#include "../memorylayout.h"
#include "dllist.h"

size_t free_pages_count = 0;

void pmm_init(uintptr_t mbi_addr)
{
    init_memorylayout_variables();
    pmm_init_stage1(mbi_addr);
}