# FAT-12-File-System-Reader

A FAT-12 disk image reader developed for the "Operating Systems" classes that allows you to read the contents of files located in the root directory.

# USAGE
```
int main() {
    char *filecontent = (char *)calloc(10240, 1);
    
    struct disk_t* disk = disk_open_from_file("example_fat12_volume.img");
    
    struct volume_t* volume = fat_open(disk, 0);
    
    struct file_t* file = file_open(volume, "MONEY");
    
    size_t size = file_read(filecontent, 1, 10240, file);
    
    printf("%s",filecontent);
    
    free(filecontent);

    file_close(file);
    fat_close(volume);
    disk_close(disk);
    return 0;
}
```
