# UDPPingerServer.py
# We will need the following module to generate randomized lost packets
import random
from socket import *
import time
import sys

print(sys.argv)
# have macros for port numbers and ping
HOST_NAME = sys.argv[1]
PORT_NUMBER = int(sys.argv[2])
sequence_no = 1
while sequence_no <= 10:
    # Create a UDP socket
    clientSocket = socket(AF_INET, SOCK_DGRAM)

    # set socket time out of 1 second
    clientSocket.settimeout(1)
    time_val = time.time()
    message = f"| ping | send time: {sequence_no} | time value: {time_val} |"  # Fixed message format

    try:
        clientSocket.sendto(message.encode(), (HOST_NAME, PORT_NUMBER))
        send_time = time.time()  # Record the time when the message was sent
        data, server = clientSocket.recvfrom(1024)
        receive_time = time.time()  # Record the time when the response was received
        elapsed = receive_time - send_time  # Calculate round-trip time
        print(data, f"RTT: {elapsed:.4f}s")  # Print response and round-trip time
    except timeout:
        print('REQUEST TIMED OUT')

    sequence_no += 1

clientSocket.close()  # Close the socket after the loop
