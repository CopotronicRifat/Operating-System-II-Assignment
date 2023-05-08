To run the server program along with the client program-

1. Compile the server program:

>> gcc server.c -o server -lpthread

2. Start the server by running the compiled executable:

>> ./server

3. Open a separate terminal window or session and compile the client program:

>> gcc client.c -o client

4. Run the client program by providing the IP address of the server:

>> ./client 10.203.72.25

The client program will connect to the server and send a message. The server program will receive the message, store it in the shared file, and send a response back to the client.

You can repeat step 4 to run multiple instances of the client program on different machines or in different terminal windows. The server program will handle multiple connections concurrently.

Make sure to compile and run the server program first before running the client program.