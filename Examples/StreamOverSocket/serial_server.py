import socket
import serial
import threading
import argparse

BUFFER_SIZE = 1024

# Parse command-line arguments
parser = argparse.ArgumentParser(description="TCP server to pipe data between a TCP client and a serial port")
parser.add_argument('-ip', '--ip-address', type=str, help="TCP server IP address", required=False,default='127.0.0.1')
parser.add_argument('-p', '--port', type=int, help="TCP server port", required=False,default=8888)
parser.add_argument('-com', '--com-port', type=str, help="Serial port name (e.g., COM1)", required=False,default='COM5')
parser.add_argument('-b', '--baudrate', type=int, help="Serial port baud rate", required=False,default='115200')
parser.add_argument('-tx', '--tx', type=str, help="file address to tx data in", required=False,default='')
parser.add_argument('-rx', '--rx', type=str, help="file address to rx data in", required=False,default='')
parser.add_argument("-v", "--verbose", help="Enable verbose mode", action="store_true")
args = parser.parse_args()

# Create a TCP socket server
tcp_server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
tcp_server_socket.bind((args.ip_address, args.port))
tcp_server_socket.listen(1)

verbos = True
if args.verbose:
    verbos = True

# def custom_decode(bytes_data):
#     special_chars = {
#         b"\n": "\\n",
#         b"\r": "\\r",
#         b"\t": "\\t",
#         b"\0": "\\0",
#         b"\x1A": "\\x1A",  # Ctrl+Z
#         # Add more special characters as needed
#     }

#     decoded_str = bytes_data.decode("utf-8")

#     for char, escape_seq in special_chars.items():
#         decoded_str = decoded_str.replace(char.decode("utf-8"), escape_seq)

#     return decoded_str

def custom_decode(byte_array):
    decoded_chars = ""
    special_chars = {
        '\n': "\\n",
        '\r': "\\r",
        '\t': "\\t",
        '\0': "\\0",
      
    }

    for byte in byte_array:
            char = chr(byte)
            if 32 <= ord(char) <= 126:  # Filter only printable ASCII characters
                decoded_chars+=(char)
            elif char in ['\n','\r','\t','\0']:
                decoded_chars += special_chars[char]
            else :
                 decoded_chars += "\\x{:02x}".format(byte)

    return decoded_chars


print(f"serial {args.com_port}:{args.baudrate}\nTCP server started on {args.ip_address}:{args.port}, Waiting for client connection...")

while True:
   
    
    # Accept a client connection
    conn, addr = tcp_server_socket.accept()
    print("Client connected. IP address:", addr)

    # Create a serial connection
    serial_port = serial.Serial(args.com_port, baudrate=args.baudrate, timeout=0.001)

     #handle file dump
    tx = None
    if args.tx: 
        tx = open(args.tx,"ab");
    rx = None
    if args.rx: 
        rx = open(args.rx,"ab");

    def read_serial_and_send():
        while True:
            try:
                # Read data from the serial port
                serial_data = serial_port.read(BUFFER_SIZE)
                if serial_data:
                    # Send data back to the client
                    conn.send(serial_data)
                    if verbos:
                        print(f"port({len(serial_data)}):", custom_decode(serial_data))
                    if tx:
                        tx.write(serial_data)
                        tx.flush();
            except:
                conn.close()
                break

    def receive_client_data():
        while True:
            try:
                # Receive data from the client
                data = conn.recv(BUFFER_SIZE)
                if not data:
                    raise
            except:
                print("Client disconnected: ",addr)
                break
            
            # Print received data
            if verbos:
                print(f"client({len(data)}):", custom_decode(data))
            if rx:
                rx.write(data)
                rx.flush();
            try:
            # Send data to the serial port
                serial_port.write(data)
            except:
                print("cannt write data to serial");
                break    

    # Start the threads
    read_thread = threading.Thread(target=read_serial_and_send)
    read_thread.start()

    receive_thread = threading.Thread(target=receive_client_data)
    receive_thread.start()
    

    # Wait for the threads to finish
    # read_thread.join()
    receive_thread.join()

    # Close the TCP connection and serial port
    print("close client and serial")
    conn.close()
    serial_port.close()
    if tx:
        tx.close()
    if rx:
        rx.close()

# Close the TCP server socket
tcp_server_socket.close()
