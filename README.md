# Concurrent-HTTP-Server
Thread-based concurrent HTTP Server that supports GET PUT POST AND File System requests

# NOTE ABOUT SERVICE 

* The default local port of this server is 4221
* 
# Testing

* Extract URL path for HTTP server :
  An example request for the HTTP service :
  
```
GET /index.html HTTP/1.1\r\nHost: localhost:4221\r\nUser-Agent: curl/7.64.1\r\nAccept: */*\r\n\r\n
```

Testing the GET Method for URL Path from Codecrafters' description : 

* Running server : 

```
mkdir build

cd build 

cmake ..

make 

./server

```


* Open a new terminal and send a random string as URL path to the 4221 port service : 

```
curl -v http://localhost:4221/abcdefg
```

&nbsp; &nbsp; - Expected Response :

```
HTTP/1.1 404 Not Found\r\n\r\n
```

* Send an empty string as URL path to the 4221 port service : 

```
curl -v http://localhost:4221/abcdefg
```

&nbsp; &nbsp; - Expected Response :

```
HTTP/1.1 200 OK\r\n\r\n
```


