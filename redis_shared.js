var redis_shared = require("./build/default/redis_shared").redis_shared;

exports.RedisShared = function() {
    this.start = function() {
    	redis_shared.run_redis.apply(redis_shared, arguments);
    };
    
    this.get = function(key) {
    	return redis_shared.get.apply(redis_shared, arguments);
    };
    
    this.set = function(key, value, exp, nx) {
        return redis_shared.set.apply(redis_shared, arguments);
    };
    
    this.execute = function(f) {
    	redis_shared.lock();
    	
    	try {
    		f();
    	} finally {
    		redis_shared.unlock();
    	}
    }
};