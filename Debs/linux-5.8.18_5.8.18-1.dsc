Format: 1.0
Source: linux-5.8.18
Binary: linux-image-5.8.18, linux-libc-dev, linux-headers-5.8.18, linux-image-5.8.18-dbg
Architecture: amd64
Version: 5.8.18-1
Maintainer: root <root@parallels-Parallels-Virtual-Platform>
Homepage: http://www.kernel.org/
Testsuite: autopkgtest
Testsuite-Triggers: @builddeps@, build-essential, bzr, fakeroot, gcc-multilib, gdb, git, python, python2
Build-Depends: bc, rsync, kmod, cpio, bison, flex | flex:native, libssl-dev:native
Package-List:
 linux-headers-5.8.18 deb kernel optional arch=amd64
 linux-image-5.8.18 deb kernel optional arch=amd64
 linux-image-5.8.18-dbg deb debug optional arch=amd64
 linux-libc-dev deb devel optional arch=amd64
Checksums-Sha1:
 5e291e0e14b9056639270b1b5bcef7a8114f589b 186558703 linux-5.8.18_5.8.18.orig.tar.gz
 5e0921157151b252191b3b6b5491e45965cfeecf 7197963 linux-5.8.18_5.8.18-1.diff.gz
Checksums-Sha256:
 160057234dc2e9d8cf4c3bb571010be1c7a56ef1eb8ee4683be3ca21829d382a 186558703 linux-5.8.18_5.8.18.orig.tar.gz
 d1344be1c3b53c4d714fa929583119cc1d7afcf9e652dd07eb40a637dba2112c 7197963 linux-5.8.18_5.8.18-1.diff.gz
Files:
 9bd02905f07f5e366935e30848d8a975 186558703 linux-5.8.18_5.8.18.orig.tar.gz
 fd831771bc97591463b3ebd0a0102126 7197963 linux-5.8.18_5.8.18-1.diff.gz
