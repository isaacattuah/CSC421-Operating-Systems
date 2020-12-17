#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<assert.h>
#define FUSE_USE_VERSION 30
#include<fuse.h>
#include<errno.h>

#include "leveldb.h"


/*
 * program to implement leveldb server
 *
 * author: bjr
 * created: 9 nov 2019
 * last-update: 10 nov 2019
 *     19 oct 2020
 */

extern int errno ; 
struct LevelDB_G leveldb_g ;

#define USAGE_MESSAGE "Usage: %s -v _directory_ _virtual_disk_name_\n"
#define PROG_NAME "leveldbd"


static struct fuse_operations fuse_oper = {
	// call backs for fuse
	.getattr = fs_getattr,
	.readdir = fs_readdir,
	.mkdir   = fs_mkdir,
	.rmdir   = fs_rmdir,
	.create  = fs_create,
	.unlink  = fs_unlink,
	.utimens = fs_utimens,
	.truncate = fs_truncate,
	.open    = fs_open,
	.write   = fs_write,
	.read    = fs_read,
} ;

int print_usage(void) {
	printf(USAGE_MESSAGE,PROG_NAME) ;
	return 0 ;
}

void * mmap_level0_file(struct LevelDB_G * ldb, char * filename) {
	struct stat stat_struct ;
	char * path ;
	
	path = (char *)malloc(strlen(ldb->backing_dir)+strlen(filename)+2) ;
	strcat(strcat(strcat(path,ldb->backing_dir),"/"),filename) ;
	// todo, prepend ldb->backing_dir
	
	ldb->l0_id = open(path, O_RDWR ) ;
	free(path) ;
	
	if ( ldb->l0_id==-1 ) {
		perror("l0 file") ;
		return NULL ;
	}
	assert( fstat(ldb->l0_id,&stat_struct)!=-1 ) ;
	assert( stat_struct.st_size=LEVEL0_SIZE) ;

	// memory map  the L0 file
	ldb->l0_mm = mmap(NULL, stat_struct.st_size , 
			PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, ldb->l0_id, 0 ) ;
	if ( ldb->l0_mm == MAP_FAILED ) {
		perror("Error in mmap") ;
		return NULL ;
	}
	return ldb->l0_mm ;
}

int main(int argc, char * argv[]) {
	int ch ;
	
	while ((ch = getopt(argc, argv, "v")) != -1) {
		switch(ch) {
		case 'v':
			leveldb_g.is_verbose ++ ;
			break ;
		case '?':
		default:
			return( print_usage()) ;
		}
	}
	argc -= optind;
	argv += optind;
	if ( argc!=2 ) {
		return( print_usage()) ;
	}

	leveldb_g.mount_dir = strdup(argv[0]) ;
	leveldb_g.backing_dir = strdup(argv[1]) ;
	
	assert(mmap_level0_file(&leveldb_g, LEVEL0_NAME)) ;

	if ( leveldb_g.is_verbose )
	{
		char * argv_fuse[5]  = { "", "-d", "-s", leveldb_g.mount_dir, NULL } ;
		return fuse_main(4, argv_fuse, &fuse_oper, NULL) ;	
	}
	else {
		char * argv_fuse[4]  = { "", "-s", leveldb_g.mount_dir, NULL } ;
		return fuse_main(3, argv_fuse, &fuse_oper, NULL) ;	
	}
	
}
