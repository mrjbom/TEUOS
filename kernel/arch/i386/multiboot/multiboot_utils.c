#include "multiboot_utils.h"
#include "../useful/useful_things.h"
#include <stdio.h>

bool multiboot_info_is_valid(uintptr_t mbi_addr, bool panic)
{
    uint32_t mbi_total_size = *(uint32_t*)(mbi_addr);
    if (mbi_total_size == 0) {
        if (panic) {
            kpanic("MULTIBOOT", "MBI total_size is 0!", __FILE__, __LINE__);
        }
        return false;
    }

    // Skip total_size and reserved fields
    uintptr_t current_tag_addr = mbi_addr + 8;
    uint32_t mbi_size_counter = 0 + 8;
    // The end tag has been reached
    bool end_tag_reached = false;

    while (mbi_size_counter < mbi_total_size) {
        // Every tag start at 8-bytes aligned address
        mbi_size_counter += useful_align_up(&current_tag_addr, MULTIBOOT_INFO_ALIGN);

        // Get tag
        multiboot_tag_t* current_tag_ptr = (multiboot_tag_t*)(current_tag_addr);

        if (current_tag_ptr->type == MULTIBOOT_TAG_TYPE_END) {
            end_tag_reached = true;
            mbi_size_counter += current_tag_ptr->size;
            break;
        }
        // Go to next tag
        current_tag_addr += current_tag_ptr->size;
        mbi_size_counter += current_tag_ptr->size;
    }
    if (!end_tag_reached) {
        if (panic) {
            kpanic("MULTIBOOT", "All the MBI memory was read, but the END TAG was not found!", __FILE__, __LINE__);
        }
        return false;
    }
    if (end_tag_reached && mbi_size_counter != mbi_total_size) {
        if (panic) {
            kpanic("MULTIBOOT", "The END TAG has been reached, but the size of the read MBI memory is not equal to the total size of the MBI!", __FILE__, __LINE__);
        }
        return false;
    }
    return true;
}

multiboot_tag_t* multiboot_get_tag_ptr(uintptr_t mbi_addr, uint32_t tag_type)
{
    bool end_tag_reached = false;
    // Skip total_size and reserved fields
    uintptr_t current_tag_addr = mbi_addr + 8;
    while (!end_tag_reached) {
        // Every tag start at 8-bytes aligned address
        useful_align_up(&current_tag_addr, MULTIBOOT_INFO_ALIGN);

        // Get tag
        multiboot_tag_t* current_tag_ptr = (multiboot_tag_t*)(current_tag_addr);

        if (current_tag_ptr->type == tag_type) {
            // Tag was found
            return current_tag_ptr;
        }
        if (current_tag_ptr->type == MULTIBOOT_TAG_TYPE_END) {
            // Tag not found
            return NULL;
        }
        // Go to next tag
        current_tag_addr += current_tag_ptr->size;
    }
    // Unreachable
    return NULL;
}

void multiboot_print_mbi_info(uintptr_t mbi_addr)
{
    printf_serial("multiboot info start:\n");
    printf_serial("MBI addr: 0x%p\n", mbi_addr);
    uint32_t total_size = *(uint32_t*)(mbi_addr);
    printf_serial("MBI size: %u\n", total_size);
    printf_serial("Tags:\n");
    // Skip total_size and reserved fields
    uintptr_t current_tag_addr = mbi_addr + 8;
    uint32_t size_counter = 0 + 8;
    // The end tag has been reached
    bool end_tag_reached = false;

    while (!end_tag_reached) {
        // every tag start at 8-bytes aligned address
        size_counter += useful_align_up(&current_tag_addr, MULTIBOOT_INFO_ALIGN);
        
        // Get tag
        multiboot_tag_t* tag_ptr = (multiboot_tag_t*)(current_tag_addr);
        uint32_t tag_type = tag_ptr->type;
        switch (tag_type)
        {
        case MULTIBOOT_TAG_TYPE_END:
            {
                //printf_serial("-TAG-\n");
                //printf_serial("type: MULTIBOOT_TAG_TYPE_END\n");
                //printf_serial("size: %u\n", tag_ptr->size);
            }
            end_tag_reached = true;
            break;
        case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
            {
                //multiboot_tag_string_t* string_tag_ptr = (multiboot_tag_string_t*)tag_ptr;
                //printf_serial("-TAG-\n");
                //printf_serial("type: MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME\n");
                //printf_serial("size: %u\n", string_tag_ptr->size);
                //printf_serial("string: %s\n", string_tag_ptr->string);
            }
            break;
        case MULTIBOOT_TAG_TYPE_MMAP:
            {
                multiboot_tag_mmap_t* mmap_tag_ptr = (multiboot_tag_mmap_t*)tag_ptr;
                //printf_serial("-TAG-\n");
                //printf_serial("type: MULTIBOOT_TAG_TYPE_MMAP\n");
                //printf_serial("size: %u\n", mmap_tag_ptr->size);
                //printf_serial("entry_size: %u\n", mmap_tag_ptr->entry_size);
                //printf_serial("entry_version: %u\n", mmap_tag_ptr->entry_version);
                // Subtract the size of the variables going before the entries
                size_t entries_num = (mmap_tag_ptr->size - 16) / mmap_tag_ptr->entry_size;
                
                // View every entry
                uint64_t total_memory_size = 0;
                for (uint32_t entry_index = 0; entry_index < entries_num; ++entry_index) {
                    uint32_t offset = 16 + (mmap_tag_ptr->entry_size * entry_index);
                    uintptr_t mmap_entry_tag_addr = ((uint32_t)tag_ptr) + offset;
                    //printf_serial("mmap_entry[%u]:\n", entry_index);
                    multiboot_mmap_entry_t* mmap_entry_tag_ptr = (multiboot_mmap_entry_t*)mmap_entry_tag_addr;
                    printf_serial("base_addr: 0x%llx\n", mmap_entry_tag_ptr->addr);
                    printf_serial("lenght: %llu (0x%llx)\n", mmap_entry_tag_ptr->len, mmap_entry_tag_ptr->len);
                    //printf_ega("0x%llx (%llu) ", mmap_entry_tag_ptr->addr, mmap_entry_tag_ptr->len);
                    total_memory_size += mmap_entry_tag_ptr->len;
                    switch (mmap_entry_tag_ptr->type)
                    {
                    case MULTIBOOT_MEMORY_AVAILABLE:
                        //printf_ega("AVAIL\n");
                        printf_serial("type: MULTIBOOT_MEMORY_AVAILABLE\n");
                        break;
                    case MULTIBOOT_MEMORY_RESERVED:
                        //printf_ega("RES\n");
                        printf_serial("type: MULTIBOOT_MEMORY_RESERVED\n");
                        break;
                    case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
                        //printf_ega("ACPI\n");
                        printf_serial("type: MULTIBOOT_MEMORY_ACPI_RECLAIMABLE\n");
                        break;
                    case MULTIBOOT_MEMORY_NVS:
                        //printf_ega("NVS\n");
                        printf_serial("type: MULTIBOOT_MEMORY_NVS\n");
                        break;
                    case MULTIBOOT_MEMORY_BADRAM:
                        //printf_ega("BAD\n");
                        printf_serial("type: MULTIBOOT_MEMORY_BADRAM\n");
                        break;
                    default:
                        //printf_ega("ERROR\n");
                        printf_serial("type: ERROR!\n");
                        break;
                    }
                }
                //printf_ega("%u\n", total_memory_size);
                printf_serial("total_memory_size %llu\n", total_memory_size);
            }
            break;
        case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR:
            {
                //multiboot_tag_load_base_addr_t* load_base_tag_ptr = (multiboot_tag_load_base_addr_t*)tag_ptr;
                //printf_serial("-TAG-\n");
                //printf_serial("type: MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR\n");
                //printf_serial("size: %u\n", load_base_tag_ptr->size);
                //printf_serial("load_base_addr: 0x%p\n", load_base_tag_ptr->load_base_addr);
            }
            break;
        default:
            {
                //printf_serial("Unknown multiboot info tag type: %u\n", tag_ptr->type);
                //printf_serial("size: %u\n", tag_ptr->size);
            }
            break;
        }
        current_tag_addr += tag_ptr->size;
        size_counter += tag_ptr->size;
    }
    if (size_counter != total_size) {
        kpanic("MULTIBOOT INFO PRINT", "The size of all processed tags is not equal to the size contained in the multiboot info", __FILE__, __LINE__);
    }
    printf_serial("multiboot info end\n");
}
