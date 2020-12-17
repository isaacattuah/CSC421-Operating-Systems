#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<assert.h>

#define FUSE_USE_VERSION 30
#include<fuse.h>
#include<errno.h>

#include "leveldb.h"

/*
 * fuseops subroutines
 *
 * author: bjr
 * created: 9 nov 2019
 * last-update: 10 nov 2019
 *    19 oct 2020 -bjr: 
 *
 *   template for the students code
 * 
 * student name:
 * last-update:
 *
 */
 
 
 
extern struct LevelDB_G leveldb_g ;
extern int errno ; 


char * path_to_key(char * key, const char * path) {
	// key must be char[KEY_SIZE]

	assert(path[0]=='/') ;
	strncpy(key,path+1,KEY_SIZE) ;
	return key ;
}

int lookup_key_v(char * match_key) {
	// returns the index in the L0 table where
	// match_key is found, or -1 if not found
	int found = -1 ;
	
	
	//
	// ...
	//
		
	return found ;
}



struct KV_PAIR * add_key_pair( const char * path, const char value[], int value_size) {
	struct KV_PAIR * kv_pair = (struct KV_PAIR *) leveldb_g.l0_mm ;	
	int key_index = 0 ;
	// adds a key pair to the end of the L0 table. 
	// the key is extracted from path, and the value is in the value[] array.
	// assume value is a string but it can be larger than VALUE_SIZE.

	//
	// ...
	//
	
	return  &kv_pair[key_index] ;
}



/* 
 * fuse functions
 *
 * fs_getattr needs code
 *
 */


int fs_read(const char * path, char * buf, size_t size, off_t offset, 
			struct fuse_file_info * fi) {
	struct KV_PAIR * kv_pair = (struct KV_PAIR *) leveldb_g.l0_mm ;
	char key[KEY_SIZE] ;
	int key_index ;

	assert(size>=VALUE_SIZE) ;	
	if (leveldb_g.is_verbose)  printf("fs_read (%s,%d): key: %s\n", __FILE__, __LINE__, path) ;
	
	path_to_key(key,path) ;
	key_index = lookup_key_v(key) ;
	if (key_index==-1) {
		buf[0] = '\0' ;
		return 0 ;
	}
	strncpy(buf, kv_pair[key_index].value,VALUE_SIZE) ;
	return strnlen(kv_pair[key_index].value,VALUE_SIZE) ;
}


int fs_write(const char * path, const char * buf, size_t size, off_t offset,
			struct fuse_file_info * fi) {
	struct KV_PAIR * kv ;
	if (leveldb_g.is_verbose) printf("fs_write (%s,%d): path=|%s|\n", __FILE__, __LINE__, path) ;
		
	kv = add_key_pair( path, buf,  size );
	
	if (leveldb_g.is_verbose) printf("fs_write (%s,%d): new key-value added\n", __FILE__, __LINE__) ;
			
	//return strnlen(kv->value,VALUE_SIZE) ;
	//thanks to my csc421-211 class for this change; if the value is longer that VALUE_SIZE,
	//  just tell a white lie that all of it was stored; else fs_write will be called again
	//  with the unstored characters.
	return size ;
}

int fs_unlink( const char * path ) {
	char key[KEY_SIZE] ;
	struct KV_PAIR * kv ;
	char buf[1] = {'\0'} ;
	
	if (leveldb_g.is_verbose) printf("fs_unlink (%s,%d): key: %s\n", __FILE__, __LINE__, path) ;

	kv = add_key_pair( path, buf,  1 );
	return 0 ;
}

int fs_truncate(const char * path, off_t offset) {
	// truncate to zero gets used for a zero length write
	struct KV_PAIR * kv ;
	char buf[1] = { '\0' } ;
	if (leveldb_g.is_verbose) printf("fs_truncate (%s,%d): path: %s %d\n", __FILE__, __LINE__, path, (int)offset) ; 
	kv = add_key_pair( path, buf,  1 );
	return 0 ;
}


int fs_getattr(const char * path, struct stat * stbuf) {
	int k ;
	char key[KEY_SIZE] ;
	char buf[VALUE_SIZE] ;
	int size ;
	if (leveldb_g.is_verbose) printf("fs_getattr (%s,%d): path: %s\n", __FILE__, __LINE__, path) ; 
	
	memset(stbuf, 0, sizeof(struct stat)) ;
	if ( strcmp(path,"/") == 0 ) {
			stbuf->st_mode = S_IFDIR | 0777 ;
			stbuf->st_nlink = 2 ;
			stbuf->st_size =  0 ;
			return 0 ;
	}
	
	stbuf->st_mode = 0777 | S_IFREG  ;
	stbuf->st_nlink = 1 ;
	size = 0 ;
	
	//
	// ..
	// 
	
	stbuf->st_size = size ;
	return 0 ;

}

int fs_readdir(const char * path, void * buf, 
	fuse_fill_dir_t filler, off_t offset, struct fuse_file_info * fi ) {

	// not really implemented, there are various things you can decide to 
	// do here. Here is an idea of a pseudo-file system that gives you sub-dir's
	// for each level of the database, plus a sub-dir for asking configuration
	// questions, encoded in file requests

	if (strcmp(path,"/")) return -ENOENT ;
	filler(buf,"level0",NULL,0) ;
	filler(buf,"level1",NULL,0) ;
	filler(buf,"level2",NULL,0) ;
	filler(buf,"system",NULL,0) ;

	return 0 ;
}

int fs_create( const char * path, mode_t m, struct fuse_file_info * fi ) {
	if (leveldb_g.is_verbose) printf("fs_create (%s,%d): path: %s\n", __FILE__, __LINE__, path) ; 
	// piping such as echo > file call create then stat's.
	// if we don't implement create than stat must return OK as if file exists
	return 0 ;
}

int fs_open(const char * path, struct fuse_file_info * fi) {
	if (leveldb_g.is_verbose) printf("fs_open (%s,%d): key: %s\n", __FILE__, __LINE__, path ) ;
	return 0 ;
}

// touch prefers this implemented 
int fs_utimens( const char * path, const struct timespec tv[2] ) {
	return 0 ;
}

int fs_mkdir( const char * path, mode_t mode ) {
	// not needed in my-leveldb
	return 0 ;
}

int fs_rmdir( const char * path ) {
	// not needed in my-leveldb
	return 0 ;
}


