# Concurrent-HTTP-Server
Thread-based concurrent HTTP Server that supports GET PUT POST AND File System requests inspired from Codecrafters 
HTTP Network Server 

# NOTE ABOUT SERVICE 

* The default local port of this server is 4221

# Testing

* Extract URL path for HTTP server :
  An example request for the HTTP service :
  
```
GET /index.html HTTP/1.1\r\nHost: localhost:4221\r\nUser-Agent: curl/7.64.1\r\nAccept: */*\r\n\r\n
```

* Running cmake server : 

```
mkdir build

cd build 

cmake ..

make 

./server

```

Testing the GET Method for URL Path from Codecrafters' description : 


✅ Open a new terminal and send a random string as URL path to the 4221 port service : 

```
curl -v http://localhost:4221/abcdefg
```

&nbsp; &nbsp; - Expected Response :

```
HTTP/1.1 404 Not Found\r\n\r\n
```

✅ Send an empty string as URL path to the 4221 port service : 

```
curl -v http://localhost:4221/abcdefg
```

&nbsp; &nbsp; - Expected Response :

```
HTTP/1.1 200 OK\r\n\r\n
```

✅ Support POST request header and concurrent client connections : 

```
$ (sleep 3 && printf "GET / HTTP/1.1\r\n\r\n") | nc localhost 4221 &
$ (sleep 3 && printf "GET / HTTP/1.1\r\n\r\n") | nc localhost 4221 &
$ (sleep 3 && printf "GET / HTTP/1.1\r\n\r\n") | nc localhost 4221 &
```

✅ Read from file directory : 
```
./server --directory /tmp/

```

✅ Support gzip compression optimization : 

```
$ curl -v -H "Accept-Encoding: gzip" http://localhost:4221/echo/abc | hexdump -C
```

✅ Support close connection and persistent connections
