var redis_shared = require("./build/default/redis_shared").redis_shared;

exports.RedisShared = function() {
    this.start = function() {
    	redis_shared.run_redis.apply(redis_shared, arguments);
    };
};