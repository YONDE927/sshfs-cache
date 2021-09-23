#include "acache.h"
#include <iostream>
#include <string>
#include <map>
#include <pthread.h>

using namespace std;


struct DataNode {
	string data;
};

struct DataCache {
	fuse_operations *next_oper;
	map<string,DataNode*> table;
	pthread_mutex_t lock;
};

DataCache dcache;

static DataNode* dcache_lookup(const char *path){
	auto itr = dcache.table.find((string)path);
	if(itr!=dcache.table.end()){
		DataNode* dnode = dcache.table[(string)path];
	}else{
		return NULL;
	}
}

static DataNode* dcache_get(const char *path)
{
	DataNode* dnode= dcache_lookup(path);
	
	if(dnode==NULL){
		dcache.table[(string)path]=dnode;
	}
	return dnode;
}

static int dcache_read(const char *path, char *rbuf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
	int err;
	(void) path;
	// cacheで検索&rbufに適切な量を書き込み

	//それまでにreturnできなければ普通にsshfs_readを呼ぶ
	err = dcache.next_oper->read(path, rbuf, size,offset,fi);
}