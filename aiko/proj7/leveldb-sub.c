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
	struct KV_PAIR * kv_pair = (struct KV_PAIR *) leveldb_g.l0_mm ;
	int i;
	for (i = 0; i < LEVEL0_N_PAIRS ; i++){
	  if ( * match_key == * kv_pair[i].key){
	      found = i;
	    }
	}
	
	//
	// ...
	//
		
	return found ;
}

struct KV_PAIR * sort (struct KV_PAIR * l0, int n){
  int i, j, min;
  char temp[KEY_SIZE];
  
  for (i= 0; i < n-1; i++){
    min = i;
    strncpy(temp, l0[i].key, KEY_SIZE);
    for (j = i + 1; j < n ; j++){
      if(strncmp(temp, l0[j].key, KEY_SIZE) > 0){
	strncpy(temp, l0[j].key, KEY_SIZE);
	min = j;
      }
    }

    if (min != i){
      char val[VALUE_SIZE];
      strncpy(temp, l0[i].key, KEY_SIZE);
      strncpy(l0[i].key, l0[min].key, KEY_SIZE);
      strncpy(l0[min].key, temp, KEY_SIZE);
      strncpy(val, l0[i].value, VALUE_SIZE);
      strncpy(l0[i].value, l0[min].value, VALUE_SIZE);
      strncpy(l0[min].value, val, VALUE_SIZE);
    }
  }
  return l0;
  
}


struct KV_PAIR * l0_to_l1_migrate(struct KV_PAIR * l0){
    remove("kv-tables/l1.dat");
    leveldb_g.l1_id = open("kv-tables/l1.dat", O_RDWR | O_CREAT, 0664);
    struct KV_PAIR * l1 = l0;
    int i;
    for (i = 0; i < LEVEL0_N_PAIRS; i++){
      strncpy(l1[i].key, l0[i].key, KEY_SIZE);
      strncpy(l1[i].value, l0[i].value, VALUE_SIZE);
    }

     for (i = 0; i < LEVEL0_N_PAIRS; i++){
       if (i != lookup_key_v(l1[i].key)){
	 strncpy(l1[i].key, TOMB_STONE, KEY_SIZE);
	 strncpy(l1[i].value, TOMB_STONE, VALUE_SIZE);
       }
    }
     l1 = sort(l1, LEVEL1_MIGRATE);

      for (i = 0; i < LEVEL0_N_PAIRS; i++){
	write(leveldb_g.l1_id, l1[i].key, KEY_SIZE);
        write(leveldb_g.l1_id, l1[i].value, VALUE_SIZE);
    }
      return l1;
    
  }



struct KV_PAIR * add_key_pair( const char * path, const char value[], int value_size) {
	struct KV_PAIR * kv_pair = (struct KV_PAIR *) leveldb_g.l0_mm ;	
	int key_index = 0 ;
	char key[KEY_SIZE];
	path_to_key(key,path);
	int i;
	// adds a key pair to the end of the L0 table. 
	// the key is extracted from path, and the value is in the value[] array.
	// assume value is a string but it can be larger than VALUE_SIZE.

	while(key_index < LEVEL0_N_PAIRS){
	  if (leveldb_g.l0_n - 1 <= leveldb_g.next_index_hint){
	    struct KV_PAIR * l1;
	    l1 = l0_to_l1_migrate(kv_pair);
	    for (i = 0; i < LEVEL0_N_PAIRS; i++){
	      strncpy(kv_pair[i].key, NULL_VALUE, KEY_SIZE);
	      strncpy(kv_pair[i].value, NULL_VALUE, VALUE_SIZE);
	    }
	  }

	  if (*kv_pair[key_index].key == 0){
	    strcpy(kv_pair[key_index].key, key);
	    strncpy(kv_pair[key_index].value, value, value_size);
	    leveldb_g.next_index_hint = key_index;
	    return &kv_pair[key_index];
	  }
	  key_index++;
	}
	  return &kv_pair[key_index];
	
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
	return strnlen(kv->value,VALUE_SIZE) ;
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
	path_to_key(key,path);
	k = lookup_key_v(key);
	struct KV_PAIR * kv_pair = (struct KV_PAIR *) leveldb_g.l0_mm ;

	if(k == -1){
	  size = 0;
	}
	else{
	  strncpy(buf, kv_pair[k].value, VALUE_SIZE);
	  strncpy(key, kv_pair[k].key, KEY_SIZE);
	  size = strlen(buf) + strlen(key) - 1 ;
	  
	}
	
	
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


