#include "dcache.h"
#include <iostream>
#include <string>
#include <map>
#include <pthread.h>

#ifndef __CACHE__
#include "cache.h"
#endif

using namespace std;

//extern "C"
//read write の時の last access
//cache と 全体lock
//open時にファイル全体をディスクに書き込み。close時にdirty check。
// passthroughにsftpを加える。open close write
// sshfsをpassthrough化,sftpが呼ばれないようになる。

struct DataNode {
	string data;
};

struct DataCache {
	fuse_operations *next_oper;
	map<string,DataNode*> table;
	pthread_mutex_t lock;
	//書き込み制御・タイムアウト
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

static void dcache_purge(const char *path)
{
	//write dirty bit によって書き込み命令を送る。
	auto itr = dcache.table.find(path);
    if(itr!=dcache.table.end()){
        dcache.table.erase(path);
    }
}

void dcache_invalidate(const char *path){
	pthread_mutex_lock(&dcache.lock);
	dcache_purge(path);
	pthread_mutex_unlock(&dcache.lock);
}

static int dcache_read(const char *path, char *rbuf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
	int err;
	(void) path;
	// cacheで検索&rbufに適切な量を書き込み

	//それまでにreturnできなければ普通にsshfs_readを呼ぶ
	err = dcache.next_oper->read(path, rbuf, size,offset,fi);
	// add cache
	return err;
}

static int dcache_w_rite(const char *path, const char *buf, size_t size,
                       off_t offset, struct fuse_file_info *fi)
{
	
}

static int dcache_write(const char *path, const char *buf, size_t size,
                       off_t offset, struct fuse_file_info *fi)
{
	int res = dcache_w_rite(path,buf,size,offset,fi);
	if (res < 0){
		res = dcache.next_oper->write(path, buf, size, offset, fi);
	}
	if (res >= 0){
		dcache_invalidate(path);
	}
	return res;
}
