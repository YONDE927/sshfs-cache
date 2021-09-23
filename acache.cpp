#include "acache.h"
#include <iostream>
#include <string>
#include <map>
using namespace std;

struct cache {
	struct fuse_operations *next_oper;
	map<string,node> *table;
	pthread_mutex_t lock;
};

static struct cache cache;

struct node {
	void *data;
};