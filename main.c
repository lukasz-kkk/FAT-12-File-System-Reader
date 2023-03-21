#include <stdio.h>
#include "file_reader.h"

int main() {
    char *filecontent = (char *)calloc(10241, 1);

    struct disk_t* disk = disk_open_from_file("example_fat12_volume.img");
    if (disk == NULL){
        disk_close(disk);
        printf("Fat file could not be found.");
        return 1;
    }
    struct volume_t* volume = fat_open(disk, 0);
    if(volume == NULL){
        fat_close(volume);
        disk_close(disk);
        printf("Image file is not a valid FAT volume.");
        return 2;
    }
    struct file_t* file = file_open(volume, "MONEY");
    if (file == NULL){
        fat_close(volume);
        disk_close(disk);
        puts("The file does not exist.");
        return 3;
    }
    size_t size = file_read(filecontent, 1, 10240, file);

    printf("%s",filecontent);
    free(filecontent);

    file_close(file);
    fat_close(volume);
    disk_close(disk);
    return 0;
}




