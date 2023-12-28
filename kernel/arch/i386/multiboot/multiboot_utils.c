#include "multiboot_utils.h"

typedef struct multiboot_tag_load_base_addr multiboot_tag_load_base_addr_t;

void multiboot_print_mbi_info(uint32_t mbi_addr)
{
    printf_("MBI addr: 0x%p\n", mbi_addr);
    printf_("MBI size: %u\n", *(uint32_t*)(mbi_addr));
    printf_("Tags:\n");
    // Skip total_size and reserved fields
    size_t offset = 8;
    // The end tag has been reached
    bool end_tag_reached = false;

    while (!end_tag_reached)
    {
        // every tag start at 8-bytes aligned address
        if ((mbi_addr + offset) % MULTIBOOT_TAG_ALIGN) {
            offset += (MULTIBOOT_TAG_ALIGN - ((mbi_addr + offset) % MULTIBOOT_TAG_ALIGN));
        }
        // Get tag
        uint32_t* tag_ptr = (uint32_t*)(mbi_addr + offset);
        uint32_t tag_type = *tag_ptr;
        printf_("type: %u\n", tag_type);
        switch (tag_type)
        {
        case MULTIBOOT_TAG_TYPE_END:
            {
                multiboot_tag_t* end_tag_ptr = (multiboot_tag_t*)tag_ptr;
                printf_("size: %u\n", end_tag_ptr->size);
            }
            end_tag_reached = true;
            break;
        case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
            {
                multiboot_tag_string_t* string_tag_ptr = (multiboot_tag_string_t*)tag_ptr;
                printf_("size: %u\n", string_tag_ptr->size);
                printf_("bootloader name: %s\n", string_tag_ptr->string);
                offset += string_tag_ptr->size;
            }
            break;
        case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR:
            {
                multiboot_tag_load_base_addr_t* load_base_tag_ptr = (multiboot_tag_load_base_addr_t*)tag_ptr;
                printf_("size: %u\n", load_base_tag_ptr->size);
                printf_("load_base_addr: 0x%p\n", load_base_tag_ptr->load_base_addr);
                offset += load_base_tag_ptr->size;
            }
            break;
        default:
            {
                //printf_("[ERROR] Unknown multiboot info tag type: %u\n", tag_type);
                //printf_("size: %u\n", ((multiboot_tag_t*)tag_ptr)->size);
                offset += ((multiboot_tag_t*)tag_ptr)->size;
            }
            break;
        }
    }
}
