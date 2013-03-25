 /* -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*- */
// vim: ft=cpp:expandtab:ts=8:sw=4:softtabstop=4:

#ifndef BACKUP_MANAGER_H
#define BACKUP_MANAGER_H

#ident "Copyright (c) 2012-2013 Tokutek Inc.  All rights reserved."
#ident "$Id$"

#include "backup.h"
#include "backup_directory.h"
#include "file_description.h"
#include <sys/types.h>
#include <vector>
#include <pthread.h>

class backup_manager
{
private:
    bool m_doing_backup;
    bool m_doing_copy;

    // TODO: Make this an array or vector of directories.
    backup_directory m_dir;
    file_descriptor_map m_map;
    pthread_mutex_t m_mutex; // Used to serialize multiple backup operations.
    int m_capture_error;
    
    volatile unsigned long m_throttle;

public:
    backup_manager();
    int do_backup(backup_poll_fun_t poll_fun, void *poll_extra, backup_error_fun_t error_fun, void *error_extra) __attribute__((warn_unused_result));
    int add_directory(const char *source_dir, const char *dest_dir,
                      backup_poll_fun_t poll_fun, void *poll_extra, backup_error_fun_t error_fun, void *error_extra)  __attribute__((warn_unused_result));

    // Methods used during interposition:
    void create(int fd, const char *file);
    void open(int fd, const char *file, int oflag);
    void close(int fd);
    ssize_t write(int fd, const void *buf, size_t nbyte); // Actually performs the write on fd (so that a lock can be obtained).
    void pwrite(int fd, const void *buf, size_t nbyte, off_t offset);
    ssize_t read(int fd, void *buf, size_t nbyte);        // Actually performs the read (so a lock can be obtained).
    off_t   lseek(int fd, size_t nbyte, int whence);      // Actually performs the seek (so a lock can be obtained).
    void rename(const char *oldpath, const char *newpath);
    void ftruncate(int fd, off_t length);
    void truncate(const char *path, off_t length);
    void mkdir(const char *pathname);
    
    void set_throttle(unsigned long bytes_per_second); // This is thread-safe.
    unsigned long get_throttle(void);                 // This is thread-safe.

private:
    backup_directory* get_directory(int fd);
    backup_directory* get_directory(const char *file);
};

#endif // End of header guardian.
