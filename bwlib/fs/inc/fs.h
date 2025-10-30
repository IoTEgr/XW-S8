

#ifndef  FS_H
#define  FS_H

#include "ffconf.h"
#include "ff.h"


#define  FS_CFG_NODE_NUM           4   // support max open file number at the same time

typedef  int        FHANDLE;

extern FATFS work_fatfs;



int f_unlink_ext_init(void);

int  f_unlink_ext_service(void);

int  f_unlink_ext(const TCHAR *filename);

int fs_nodeinit(void);

int fs_mount(int dev);

int fs_init(void);

int open(const TCHAR *name,INT32U op);

int close(int fd);

int read(int fd,void *buff,UINT len);

int write(int fd,const void *buff,UINT len);

int lseek(int fd,INT32S offset,INT32U point);

int fs_merge(int fd1,int fd2);

int fs_bound(int fd1,int fd2);

int fs_readsector(int fd,void *buffer,int sectors);

int fs_writesector(int fd,void *buffer,int sectors);

INT32U fs_getsector(int fd,INT32U offset,int from);

INT32U fs_getclustersize(void);

INT32U fs_getcluster(int fd,INT32U cluster_no);

int fs_fastseek_init(int fd);

INT32U fs_size(int fd);

INT32U fs_size2(int fd);

INT32U fs_tell(int fd);

INT32U fs_free_size(void);
int fs_sync(int fd);

int Ascii2Tchar(const char *ascStr, TCHAR *tchStr, int len);
int Tchar2Ascii(const TCHAR *tchStr, char *ascStr, int len);
int tchNCopy(TCHAR *dst, int dst_len, const TCHAR *src);
int tchLen(const TCHAR *tchStr);


#endif

