#ifndef UNTITLED56_FILE_READER_H
#define UNTITLED56_FILE_READER_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef uint64_t lba_t;

struct dir_all_data {
    char name[8];
    char ext[3];
    struct {
        uint8_t is_read_only: 1;
        uint8_t is_hidden: 1;
        uint8_t is_system: 1;
        uint8_t is_volume: 1;
        uint8_t is_directory: 1;
        uint8_t is_archive: 1;
        uint8_t is_unused: 2;
    } __attribute__ (( packed ));
    uint8_t reserved;
    uint8_t creation_time_tenths_of_second;
    struct {
        uint8_t hour: 5;
        uint8_t minutes: 6;
        uint8_t seconds: 5;
    } __attribute__ (( packed ));
    struct {
        uint8_t year: 7;
        uint8_t month: 4;
        uint8_t day: 5;
    } __attribute__ (( packed ));
    struct {
        uint8_t year_last_access: 7;
        uint8_t month_last_access: 4;
        uint8_t day_last_access: 5;
    } __attribute__ (( packed ));
    uint16_t first_cluster_high_bits;
    struct {
        uint8_t hour_last_modified: 5;
        uint8_t minutes_last_modified: 6;
        uint8_t seconds_last_modified: 5;
    } __attribute__ (( packed ));
    struct {
        uint8_t year_last_modified: 7;
        uint8_t month_last_modified: 4;
        uint8_t day_last_modified: 5;
    } __attribute__ (( packed ));
    uint16_t first_cluster_low_bits;
    uint32_t size;
} __attribute__ (( packed ));

struct dir_entry_t {
    char name[13];
    uint32_t size;
    unsigned char is_archived: 1;
    unsigned char is_readonly: 1;
    unsigned char is_system: 1;
    unsigned char is_hidden: 1;
    unsigned char is_directory: 1;
    uint16_t first_cluster;
};

struct disk_t {
    FILE *file_ptr;
};

struct fat_super_t {
    uint8_t jump_code[3];
    char oem_name[8];
    uint16_t bytes_per_sector;
    uint8_t sector_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_count;
    uint16_t root_dir_capacity;
    uint16_t logical_sectors16;
    uint8_t media_type;
    uint16_t sectors_per_fat;
    uint16_t chs_sectors_per_track;
    uint16_t chs_tracks_per_cylinder;
    uint32_t hidden_sectors;
    uint32_t logical_sectors32;
    uint8_t media_id;
    uint8_t chs_head;
    uint8_t ext_bpb_signature;
    uint32_t serial_number;
    char volume_label[11];
    char fsid[8];
    uint8_t boot_code[448];
    uint16_t magic;
}__attribute__((packed));

struct volume_t {
    struct fat_super_t *super_t;
    struct disk_t *disk;
    uint8_t *FAT;
    uint16_t whole_fat_size;
    uint32_t fat_entry_count;
    uint16_t fat1_start;
    lba_t fat2_start;

    int32_t root_start;
    uint16_t sectors_per_root;

    int32_t data_start;

    lba_t available_clusters;
    uint64_t available_bytes;


};

struct file_t {
    struct clusters_chain_t* chain;
    uint16_t position;
    uint32_t size;
    struct volume_t *volume;
};

struct dir_t{
    void* sectors_in_dir;
    int position;
};

struct clusters_chain_t {
    uint16_t *clusters;
    int size;
};

struct disk_t *disk_open_from_file(const char *volume_file_name);

int disk_read(struct disk_t *pdisk, int32_t first_sector, void *buffer, int32_t sectors_to_read);

int disk_close(struct disk_t *pdisk);

struct volume_t *fat_open(struct disk_t *pdisk, uint32_t first_sector);

int set_volume_fields(struct volume_t *volume, struct fat_super_t *super_t);

struct clusters_chain_t *get_chain_fat12(const void *const buffer, size_t size, uint16_t first_cluster);

int fat_close(struct volume_t *pvolume);

struct file_t *file_open(struct volume_t *pvolume, const char *file_name);

int file_close(struct file_t *stream);

size_t file_read(void *ptr, size_t size, size_t nmemb, struct file_t *stream);

int32_t file_seek(struct file_t *stream, int32_t offset, int whence);

struct dir_t *dir_open(struct volume_t *pvolume, const char *dir_path);

int dir_read(struct dir_t *pdir, struct dir_entry_t *pentry);

struct dir_entry_t *read_directory_entry(const char *filename);

int dir_close(struct dir_t *pdir);

struct dir_entry_t *read_directory_entry(const char *filename);

void filename_fix(char *filename, const char *name, const char* ext);

#endif
