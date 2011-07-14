#include <v8.h>
#include <pthread.h>
#include <string.h>

#include <node.h>
#include <node_buffer.h>

#define REDIS_STATIC_LIB

#define BYTE_ORDER BIG_ENDIAN

extern "C" {
	#include "redis.h"
}

using namespace v8;

class RedisArgs {
public:
	RedisArgs(char **ar, int len) {
		args = ar;
		l = len;
	}

	char **args;
	int l;
};

static int run_redis_main(eio_req *req) {
	RedisArgs *dat = static_cast<RedisArgs *>(req->data);
	redis_main(dat->l, dat->args);

	delete dat;
}

static int redis_db_save_thread(eio_req *req) {
	dsRdbSave_thread(req->data);
}

int dsRdbSaveBackground(char *filename) {
    pthread_t thread;

    eio_custom(redis_db_save_thread, EIO_PRI_DEFAULT, NULL, zstrdup(filename));
    ev_unref(EV_DEFAULT_UC);

    return REDIS_OK;
}

Handle<Value> redis_replace(const Arguments &args) {
	HandleScope scope;

	String::Utf8Value key(args[0]);
	String::Utf8Value val(args[1]);

	//dictReplace(get_dict(), *key, *val);

	return True();
}

Handle<Value> redis_lock(const Arguments &args) {
	HandleScope scope;
	Unlocker unlocker;
	aeLock();

	return Undefined();
}

Handle<Value> redis_unlock(const Arguments &args) {
	HandleScope scope;
	Unlocker unlocker;
	aeUnlock();

	return Undefined();
}

Handle<Value> redis_get(const Arguments &args) {
	HandleScope scope;

	String::Utf8Value _key(args[0]);
	char *k = *_key;

	robj *key = createStringObject(k, strlen(k));
	expireIfNeeded(&server.db[0], key);
	robj *val = lookupKeyRead(&server.db[0], key);

	if(!val) return Null();

	if(val->type != REDIS_STRING) return ThrowException(Exception::TypeError(String::New("Redis type not a string!")));

	if (val->encoding != REDIS_ENCODING_RAW) {
		val = getDecodedObject(val);

		Handle<Object> obj = node::Buffer::NewReferenceBuffer((char *)val->ptr, sdslen((char *)val->ptr));

		//this... is bad right?
		decrRefCount(val);

		return scope.Close(obj);
	}

	Handle<Object> obj = node::Buffer::NewReferenceBuffer((char *)val->ptr, sdslen((char *)val->ptr));

	return scope.Close(obj);
}

Handle<Value> redis_set(const Arguments &args) {
	HandleScope scope;

	String::Utf8Value _key(args[0]);
	String::Utf8Value _val(args[1]);
	int exp = args.Length() > 2 ? args[2]->Int32Value() : 0;
	bool nx = args[3] == True();

	robj *key = createStringObject(*_key, strlen(*_key));
	robj *val = createStringObject(*_val, strlen(*_val));

	if (lookupKeyWrite(&server.db[0],key) != NULL && nx) {
		return False();
	}

	setKey(&server.db[0],key,val);
	server.dirty++;
	if (exp) setExpire(&server.db[0],key,time(NULL)+exp);

	return True();
}

Handle<Value> run_redis(const Arguments &args) {
	HandleScope scope;

	int len = args.Length();
	char **out = new char *[len];

	for(int i = 0 ; i < len ; i++) {
		String::Utf8Value val(args[i]);
		char *str = new char[val.length() + 1];
		strcpy(str, *val);
		out[i] = str;
	}

	{
		eio_custom(run_redis_main, EIO_PRI_DEFAULT, NULL, new RedisArgs(out, len));
		ev_unref(EV_DEFAULT_UC);
	}

	return True();
}

extern "C" void init (Handle<Object> target)
{
    HandleScope scope;

    Handle<ObjectTemplate> templ = ObjectTemplate::New();

    templ->Set(String::New("run_redis"), FunctionTemplate::New(run_redis));
    templ->Set(String::New("get"), FunctionTemplate::New(redis_get));
    templ->Set(String::New("set"), FunctionTemplate::New(redis_set));
    templ->Set(String::New("lock"), FunctionTemplate::New(redis_lock));
    templ->Set(String::New("unlock"), FunctionTemplate::New(redis_unlock));

    target->Set(String::New("redis_shared"), templ->NewInstance());
}
