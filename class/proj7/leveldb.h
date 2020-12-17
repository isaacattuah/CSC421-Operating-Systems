/*
 * leveldb.h
 *
 * author: burt
 * creation: 7 nov 2019
 * last update:
 *    19 oct 2020 -bjr: merged with fuseops.h
 *    5 nov 2020 -bjr; extended LevelDB_G 
 *                     LEVEL0_SIZE set by formula
 *
 */
 
#define KEY_SIZE 16
#define VALUE_SIZE 48
#define KV_PAIR_SIZE (KEY_SIZE+VALUE_SIZE)
#define LEVEL0_N_PAIRS 256
//#define LEVEL0_N_PAIRS 32
#define LEVEL0_SIZE (LEVEL0_N_PAIRS*KV_PAIR_SIZE)
//#define LEVEL0_SIZE 16384
#define LEVEL1_MIGRATE LEVEL0_N_PAIRS

// in general, key and value correspond to the returned data from strndup
// a key with value an empty string is the same as the key not existing.

//    in the case  "echo hi > store/a ", first the OS calls create and then
//    appends the string "hi\n". so the file goes from non-existent,
//    to zero length, to the value "hi". with the convention that 
//    empty values are the same as non-existent, there is only one type
//    transition for the key.


struct KV_PAIR {
	char key[KEY_SIZE] ;
	char value[VALUE_SIZE] ;
} ;

// tomb stone and null value are not needed
#define TOMB_STONE ("\0\377")
#define NULL_VALUE ("\0\0")
#define LEVEL0_NAME "l0.dat"
#define LEVEL1_NAME "l1.dat"



struct LevelDB_G {
	int is_verbose ;
	char * mount_dir ;
	char * backing_dir ;
	int l0_id ;
	int l0_n ;
	void * l0_mm ;
	int next_index_hint ;
	int l1_id ;
} ;



int fs_open(const char *, struct fuse_file_info *) ;
int fs_read(const char *, char *, size_t, off_t, struct fuse_file_info *) ;
int fs_write(const char *, const char *, size_t, off_t,struct fuse_file_info *) ;
int fs_truncate(const char *, off_t) ;

int fs_create( const char * path, mode_t m, struct fuse_file_info * fi ) ;
int fs_unlink( const char * path ) ;

int fs_rmdir( const char * path ) ;
int fs_mkdir( const char * path, mode_t mode ) ;

int fs_getattr(const char * path, struct stat * stbuf) ;
int fs_readdir(const char * path, void * buf, 
	fuse_fill_dir_t filler, off_t offset, struct fuse_file_info * fi ) ;

// touch prefers this implemented 
int fs_utimens( const char * path, const struct timespec tv[2] );


/* end */
