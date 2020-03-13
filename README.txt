We have implemented 3 programs, namely, client.c, proxy_server.c and dns_server.c

To compile the 3 programs, open 3 different terminal windows and run the below commands, one in each window:

	$ gcc dns_server.c -o dns_server
	$ gcc proxy_server.c -o proxy_server
	$ gcc client.c -o client

Now, there will be 3 executable files dns_server, proxy_server, and client, that will be created.

While running dns_server.c we have to input the DNS server's port, running proxy_server.c requires us to input the Proxy Server's port, and while running client.c, we have to input the IP address and the Port of the proxy server. 

For simplicity, we can consider all the 3 programs to run on the local host, i.e. 127.0.0.1

We need to execute the programs in the order dns_server.c, proxy_server.c and client.c . We can choose some non-restricted port numbers for these programs, and we've taken ports 4200 and 4201 as an example.

To execute the 3 programs, we need to run the following commands, in the respective windows:

	$ ./dns_server 4200
	$ ./proxy_server 4201
	$ ./client 127.0.0.1 4201

This assigns the port number 4200 to the dns server, and 4201 to the proxy server, and tells the client program, the IP address(127.0.0.1) and the port number(4201) of the proxy server.

Next, we need to input the request(either IP address or Domain name) and the type of request(0 for requesting IP address, 1 for requesting Domain name) in the client program(Eg. www.google.com 0). 

If the proxy cache contains an entry corresponding to the requested domain name/IP address, the proxy server program sends back the data to the client program. 

If the proxy cache doesn't contain an entry corresponding to the requested domain name/IP address, then the proxy server will ask the user to input the DNS server's IP address and port number, which in our example are 127.0.0.1 and 4200 respectivly. After establishing a connection with the DNS server, the proxy server will now forward the request to the DNS server, which will check if there is an entry corresponding to the request. If there doesn't exist any entry, the DNS server sends a message to the proxy server saying that it can't service the request, which the proxy server will forward to the client program.

If a corresponding entry exists, the DNS server will send the required data to the proxy server, which will now update it's proxy cache using the FIFO algorithm, and then it forwards the data to the client program.

We have implemented the proxy cache and the DNS database as text programs with the names proxy_cache.txt and dns.txt

The messages are also passed according to the specified conditions. A request for the IP address from the client will be sent as a char array with the 1st element indicating the request type(Eg. '18.8.8.8' is a request asking for the domain name corresponding to the IP address 8.8.8.8, which, in our case, is www.google.com). A message from the proxy server or dns server to the client is a char array, in which the 1st element corresponds to the request type, 0 if we're able to service the request, 1 if there is no corresponding entry in the database.
