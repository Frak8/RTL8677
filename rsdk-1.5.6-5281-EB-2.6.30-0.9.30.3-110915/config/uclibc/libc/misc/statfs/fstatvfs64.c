/* Copyright (C) 1998, 1999, 2000 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1998.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <_lfs_64.h>

#include <errno.h>
#include <mntent.h>
#include <paths.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/statvfs.h>


/* Experimentally off - libc_hidden_proto(memset) */
/* Experimentally off - libc_hidden_proto(strcmp) */
/* Experimentally off - libc_hidden_proto(strsep) */
libc_hidden_proto(setmntent)
libc_hidden_proto(getmntent_r)
libc_hidden_proto(endmntent)

#undef stat
#define stat stat64
#if defined __UCLIBC_LINUX_SPECIFIC__
libc_hidden_proto(fstatfs64)
libc_hidden_proto(fstat64)
#endif
libc_hidden_proto(stat64)

int fstatvfs64 (int fd, struct statvfs64 *buf)
{
    struct statfs64 fsbuf;
    struct stat64 st;
#if !defined __UCLIBC_LINUX_SPECIFIC__
    int ret;
    struct statvfs buf32;

    ret = fstatvfs (fd, &buf32);
    if (ret == 0) {
      fsbuf.f_bsize = buf32.f_bsize;
      fsbuf.f_frsize = buf32.f_frsize;
      fsbuf.f_blocks = buf32.f_blocks;
      fsbuf.f_bfree = buf32.f_bfree;
      fsbuf.f_bavail = buf32.f_bavail;
      fsbuf.f_files = buf32.f_files;
      fsbuf.f_ffree = buf32.f_ffree;
      if (sizeof (fsbuf.f_fsid) == sizeof(buf32.f_fsid))
	memcpy (&fsbuf.f_fsid, &buf32.f_fsid, sizeof(fsbuf.f_fsid));
      /* and if not, then you could approximate or whatever.. */
      fsbuf.f_namelen = buf32.f_namemax;
    } else
      return ret;
#else
    /* Get as much information as possible from the system.  */
    if (fstatfs64 (fd, &fsbuf) < 0)
	return -1;
#endif
#define STAT(st) fstat64 (fd, st)
#include "internal_statvfs.c"

    /* We signal success if the statfs call succeeded.  */
    return 0;
}
