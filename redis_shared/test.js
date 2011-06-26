require('fibers');
var redis_shared = require('./redis_shared');
var sys = require('sys');
var puts = sys.puts;

var redis = new redis_shared.RedisShared();

function sleep(ms) {
    var fiber = Fiber.current;
    setTimeout(function() {
        fiber.run();
    }, ms);
    yield();
}

var incr = 0;

function test_heart() {
    puts((incr++) + ' tickh\n');
    setTimeout(test_heart, 1000);   
}
test_heart();

Fiber(function() {
	while(true) {
        puts((incr++) + ' tick\n');
        sleep(1000);	
	}
}).run();;


setTimeout(function() {
    //sleep(1000);
    puts('starting redis\n');
    redis.start('redis', './redis.conf');
}, 900);//.run();;