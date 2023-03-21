#include <errno.h>
#include <string.h>
#include "file_reader.h"

struct disk_t *disk_open_from_file(const char *volume_file_name) {
    if (volume_file_name == NULL) {
        errno = EFAULT;
        return NULL;
    }

    struct disk_t *disk = calloc(1, sizeof(struct disk_t));
    if (disk == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    disk->file_ptr = fopen(volume_file_name, "rb");
    if (disk->file_ptr == NULL) {
        free(disk);
        errno = ENOENT;
        return NULL;
    }

    return disk;
}

int disk_read(struct disk_t *pdisk, int32_t first_sector, void *buffer, int32_t sectors_to_read) {
    if (pdisk == NULL || first_sector < 0 || buffer == NULL || sectors_to_read <= 0) {
        errno = EFAULT;
        return -1;
    }
    if (fseek(pdisk->file_ptr, first_sector * 512, SEEK_SET)) {
        errno = ERANGE;
        return -1;
    }
    size_t a = fread(buffer, 512, sectors_to_read, pdisk->file_ptr);
    if (a != (size_t) sectors_to_read) {
        errno = ERANGE;
        return -1;
    }
    return sectors_to_read;
}

int disk_close(struct disk_t *pdisk) {
    if (pdisk == NULL) {
        errno = EFAULT;
        return -1;
    }
    if (pdisk->file_ptr != NULL) {
        fclose(pdisk->file_ptr);
    }
    free(pdisk);
    return 0;
}

struct volume_t *fat_open(struct disk_t *pdisk, uint32_t first_sector) {
    if (pdisk == NULL) {
        errno = EFAULT;
        return NULL;
    }
    struct volume_t *boot_volume = (struct volume_t *) calloc(1, sizeof(struct volume_t));
    if (boot_volume == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    boot_volume->disk = pdisk;

    boot_volume->super_t = (struct fat_super_t *) calloc(1, sizeof(struct fat_super_t));
    if (boot_volume->super_t == NULL) {
        fat_close(boot_volume);
        free(boot_volume);
        errno = ENOMEM;
        return NULL;
    }

    struct fat_super_t *super_sector = boot_volume->super_t;

    if (disk_read(pdisk, (int32_t) first_sector, super_sector, 1) == -1) {
        fat_close(boot_volume);
        errno = EINVAL;
        return NULL;
    }

    if (super_sector->bytes_per_sector == 0) {
        fat_close(boot_volume);
        errno = EINVAL;
        return NULL;
    }

    set_volume_fields(boot_volume, super_sector);
    void *first_fat = calloc(super_sector->sectors_per_fat * super_sector->bytes_per_sector, sizeof(uint8_t));
    void *second_fat = calloc(super_sector->sectors_per_fat * super_sector->bytes_per_sector, sizeof(uint8_t));

    if (disk_read(boot_volume->disk, boot_volume->fat1_start, first_fat,
                  super_sector->sectors_per_fat) != super_sector->sectors_per_fat) {
        fat_close(boot_volume);
        errno = EINVAL;
        free(first_fat);
        free(second_fat);
        return NULL;
    }
    if (disk_read(boot_volume->disk, boot_volume->fat2_start, second_fat,
                  super_sector->sectors_per_fat) != super_sector->sectors_per_fat) {
        fat_close(boot_volume);
        errno = EINVAL;
        free(first_fat);
        free(second_fat);
        return NULL;
    }

    if (memcmp(first_fat, second_fat, super_sector->sectors_per_fat * super_sector->bytes_per_sector) != 0) {
        fat_close(boot_volume);
        errno = EINVAL;
        free(first_fat);
        free(second_fat);
        return NULL;
    }
    free(second_fat);
    boot_volume->FAT = first_fat;

    return boot_volume;
}

int set_volume_fields(struct volume_t *volume, struct fat_super_t *super_t) {
    volume->fat1_start = super_t->reserved_sectors;
    volume->fat2_start = volume->fat1_start + super_t->sectors_per_fat;

    volume->root_start = volume->fat1_start + 2 * super_t->sectors_per_fat;
    volume->sectors_per_root =
            ((super_t->root_dir_capacity * 32) + (super_t->bytes_per_sector - 1)) / super_t->bytes_per_sector;
    //(super_t->bytes_per_sector - 1) guarantees rounding

    volume->data_start = volume->root_start + volume->sectors_per_root;

    if (super_t->logical_sectors16 == 0)
        volume->available_clusters =
                (super_t->logical_sectors32 - super_t->reserved_sectors - 2 * super_t->sectors_per_fat -
                 volume->sectors_per_root) / super_t->sector_per_cluster;
    else {
        volume->available_clusters =
                (super_t->logical_sectors16 - super_t->reserved_sectors - 2 * super_t->sectors_per_fat -
                 volume->sectors_per_root) / super_t->sector_per_cluster;
    }
    volume->available_bytes = volume->available_clusters * super_t->sector_per_cluster * super_t->bytes_per_sector;

    volume->fat_entry_count = volume->available_clusters + 2;
    volume->whole_fat_size = super_t->sectors_per_fat * super_t->bytes_per_sector;
    return 0;
}

int fat_close(struct volume_t *pvolume) {
    if (pvolume == NULL) {
        errno = EFAULT;
        return -1;
    }
    if (pvolume->FAT != NULL) {
        free(pvolume->FAT);
    }
    if (pvolume->super_t != NULL) {
        free(pvolume->super_t);
    }
    free(pvolume);
    return 0;
}

struct clusters_chain_t *get_chain_fat12(const void *const buffer, size_t size, uint16_t first_cluster) {
    if (buffer == NULL || size == 0)
        return NULL;
    struct clusters_chain_t *chain = (struct clusters_chain_t *) calloc(1, sizeof(struct clusters_chain_t));
    uint16_t val = first_cluster;
    const struct bitset8 *const buff_tmp = buffer;
    while (1) {
        uint16_t *tmp = realloc(chain->clusters, (chain->size + 1) * sizeof(uint16_t));
        if (!tmp) {
            free(chain->clusters);
            free(chain);
            return NULL;
        }
        chain->clusters = tmp;
        chain->clusters[chain->size] = val;
        chain->size++;

        uint16_t control = 0;
        uint8_t one, two;
        one = *((uint8_t *) buff_tmp + (int) (val + (val >> 1)));
        two = *((uint8_t *) buff_tmp + (int) (val + (val >> 1)) + 1);
        if (val % 2) {
            control |= ((two << 4) & 0x0ff0);
            control |= ((one >> 4) & 0x000f);
        } else {
            control |= ((two << 8) & 0x0f00);
            control |= (one & 0x00ff);
        }

        if (control >= 0x002 && control <= 0xFEF) {
            val = control;
        } else {
            return chain;
        }
    }
}

struct file_t *file_open(struct volume_t *pvolume, const char *file_name) {
    if (pvolume == NULL || file_name == NULL) {
        errno = EFAULT;
        return NULL;
    }
    struct dir_t *dir = dir_open(pvolume, "\\");
    struct dir_entry_t entry;
    int found = 0;
    while (dir_read(dir, &entry) == 0) {
        if (strcmp(entry.name, file_name) == 0) {
            found = 1;
            break;
        }
    }
    dir_close(dir);
    if (!found) {
        errno = ENOENT;
        return NULL;
    }
    if (entry.is_directory) {
        errno = EISDIR;
        return NULL;
    }
    struct file_t *file = (struct file_t *) calloc(1, sizeof(struct file_t));
    if (file == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    file->volume = pvolume;
    file->position = 0;
    file->size = entry.size;
    file->chain = get_chain_fat12(pvolume->FAT, pvolume->whole_fat_size * pvolume->super_t->bytes_per_sector,
                                  entry.first_cluster);
    return file;
}

int file_close(struct file_t *stream) {
    if (stream == NULL) {
        errno = EFAULT;
        return -1;
    }
    if(stream->chain->clusters != NULL){
        free(stream->chain->clusters);
    }
    if(stream->chain != NULL){
        free(stream->chain);
    }
    free(stream);
    return 0;
}

size_t file_read(void *ptr, size_t size, size_t nmemb, struct file_t *stream) {
    if (ptr == NULL || size == 0 || nmemb == 0 || stream == NULL) {
        errno = EFAULT;
        return -1;
    }
    int cluster_size = stream->volume->super_t->sector_per_cluster * stream->volume->super_t->bytes_per_sector;
    int curr_cluster = (stream->position / cluster_size);
    char *curr_cluster_content = calloc(cluster_size, sizeof(char));
    int32_t curr_cluster_start = stream->volume->data_start + (stream->chain->clusters[curr_cluster] - 2) *
                                                              stream->volume->super_t->sector_per_cluster;
    int err = disk_read(stream->volume->disk, curr_cluster_start, curr_cluster_content,
                        stream->volume->super_t->sector_per_cluster);
    if (err == -1) {
        errno = ERANGE;
        return -1;
    }

    int written_count = 0;
    for (size_t i = 0; i < size * nmemb; i++) {
        int curr_cluster_check = (stream->position / cluster_size);
        if (i >= stream->size || stream->position >= stream->size) break;
        if (curr_cluster_check != curr_cluster) {
            curr_cluster = curr_cluster_check;
            curr_cluster_start = stream->volume->data_start + (stream->chain->clusters[curr_cluster] - 2) *
                                                              stream->volume->super_t->sector_per_cluster;
            err = disk_read(stream->volume->disk, curr_cluster_start, curr_cluster_content,
                            stream->volume->super_t->sector_per_cluster);
            if (err == -1) {
                errno = ERANGE;
                return -1;
            }
        }
        *((char *) ptr + i) = curr_cluster_content[stream->position % cluster_size];
        stream->position++;
        written_count++;
    }
    free(curr_cluster_content);
    return written_count / size;
}

int32_t file_seek(struct file_t *stream, int32_t offset, int whence) {
    if (stream == NULL) {
        errno = EFAULT;
        return -1;
    }
    switch (whence) {
        case SEEK_SET:
            stream->position = offset;
            break;
        case SEEK_END:
            stream->position = stream->size + offset;
            break;
        case SEEK_CUR:
            stream->position = stream->position + offset;
            break;
        default:
            errno = EINVAL;
            return -1;
    }

    return stream->position;
}

struct dir_t *dir_open(struct volume_t *pvolume, const char *dir_path) {
    if (pvolume == NULL || dir_path == NULL || strcmp(dir_path, "\\") != 0) {
        errno = EFAULT;
        return NULL;
    }
    struct dir_t *dir = (struct dir_t *) calloc(1, sizeof(struct dir_t));
    dir->sectors_in_dir = calloc(pvolume->sectors_per_root, pvolume->super_t->bytes_per_sector);

    if (disk_read(pvolume->disk, pvolume->root_start, dir->sectors_in_dir, pvolume->sectors_per_root) !=
        pvolume->sectors_per_root) {
        dir_close(dir);
        errno = ENOENT;
        return NULL;
    }
    return dir;
}

int dir_read(struct dir_t *pdir, struct dir_entry_t *pentry) {
    if (pdir == NULL || pentry == NULL) {
        errno = EFAULT;
        return -1;
    }
    while (1) {
        struct dir_all_data *dir_all = (struct dir_all_data *) ((unsigned long) (pdir->sectors_in_dir) +
                                                                pdir->position * 32);
        if ((unsigned char) dir_all->name[0] == 0xe5) {
            pdir->position++;
            continue;
        }
        if ((unsigned char) dir_all->name[0] == 0x00) {
            break;
        }
        pentry->first_cluster = dir_all->first_cluster_low_bits;
        pentry->is_archived = dir_all->is_archive;
        pentry->is_readonly = dir_all->is_read_only;
        pentry->is_system = dir_all->is_system;
        pentry->is_hidden = dir_all->is_hidden;
        pentry->is_directory = dir_all->is_directory;
        pentry->size = dir_all->size;
        char *ptr_f_name = pentry->name;
        filename_fix(ptr_f_name, dir_all->name, dir_all->ext);

        pdir->position++;
        return 0;
    }

    return 1;
}

int dir_close(struct dir_t *pdir) {
    if (pdir == NULL) {
        errno = EFAULT;
        return -1;
    }
    if (pdir->sectors_in_dir != NULL)
        free(pdir->sectors_in_dir);

    free(pdir);
    return 0;
}

void filename_fix(char *filename, const char *name, const char *ext) {
    strcpy(filename, name);
    int i;
    for (i = 0; i < 13; ++i) {
        if (filename[i] == ' ') {
            break;
        }
    }
    int ext_len = 0;

    for (int e = 0; e < 3 && ext[e] != ' ' && ext[e] != '\0'; e++) ext_len++;
    if(i == 13){
        filename[11 - ext_len] = '.';
        for (int j = 12 - ext_len, ext_ind = 0; j < 12; ++j, ext_ind++) {
            filename[j] = ext[ext_ind];
        }
        filename[13] = '\0';
        return;
    }
    if (ext[0] != ' ') {
        filename[i] = '.';
        int j = i + 1;
        for (int ext_ind = 0; ext_ind < ext_len; ext_ind++) {
            filename[j] = ext[ext_ind];
            j++;
        }
        filename[j] = '\0';
        return;
    }
    for (i = 0; i < 13; ++i) {
        if (filename[i] == ' ') {
            filename[i] = '\0';
            break;
        }
    }
}



