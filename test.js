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
var ready = false;

function test_heart() {
	if(!ready) return;
	
	redis.execute(function() {
	    var v = redis.get('test'+parseInt(Math.random() * 300000));
	    
	    if(v) {
	    	//v = JSON.parse();
            	puts(v.getUTF8String());
            	//puts(v.getExternalAsciiString() + '');
	    }
	    
	    redis.set("test"+parseInt(Math.random() * 300000), parseInt(Math.random() * 300000), 10, true);
	});
    //puts('tick');
}


function test_set() {
	if(!ready) return;
	
    con = con || net.createConnection(6379);
    //con.write("set test " + Math.random() + "\r\n");
    con.write("incr test"+parseInt(Math.random() * 300000)+"\r\n");
}


setTimeout(function() {
    puts('tickstart');
    ready = true;
    //setInterval(test_heart, 11);  
    //setInterval(test_set, 10);
}, 2000);

setInterval(test_heart, 1);  
setInterval(test_set, 0);
setInterval(test_heart, 1);  
setInterval(test_set, 0);
setInterval(test_heart, 1);  
setInterval(test_set, 0);
setInterval(test_heart, 1);  
setInterval(test_set, 0);
setInterval(test_heart, 1);  
setInterval(test_set, 0);

var con = null;
puts('tickstartxxxxxxx');
/*
Fiber(function() {
	while(true) {
        puts((incr++) + ' tick\n');
        sleep(1000);	
	}
}).run();;
*/
