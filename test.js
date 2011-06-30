//require('fibers');
var redis_shared = require('./redis_shared');
var sys = require('sys');
var puts = sys.puts;
var net = require('net');

var redis = new redis_shared.RedisShared();

/*
function sleep(ms) {
    var fiber = Fiber.current;
    setTimeout(function() {
        fiber.run();
    }, ms);
    yield();
}
*/

redis.start('redis', '/home/colin/Dropbox/redis_static/node_redis.conf');

var incr = 0;

var test_heart = function() {
	redis.execute(function() {
	    var v = redis.get('test');
	    
	    if(v) {
	    	//v = JSON.parse();
            //puts(v.getUTF8String());
            puts(v.getExternalAsciiString() + '');
	    }
	});
    puts('tick');
}
setInterval(test_heart, 100);   

var test_set = function() {
    con = con || net.createConnection(6379);
    con.write("set test " + Math.random() + "\r\n");
}

setInterval(test_set, 10);

var con = null;

/*
Fiber(function() {
	while(true) {
        puts((incr++) + ' tick\n');
        sleep(1000);	
	}
}).run();;
*/