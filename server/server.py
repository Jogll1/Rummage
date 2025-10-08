# Peer-hosted matchmaking server

import socket
import random
import string
import threading
import json
import struct

BUFFER = 1024
MAX_PLAYERS = 2        # This is a two-player game

TCP_PORT       = 54000 # Port for game
DISCOVERY_PORT = 54001 # Port for host discovery

class Room:
	def __init__(self, host_connection: socket.socket, host_address, code=None):
		self.host         = host_connection
		self.address      = host_address
		self.clients      = [host_connection]
		self.code         = code or self.generate_code()
		self.game_started = False

	@staticmethod
	def generate_code(length=5) -> str:
		return ''.join(random.choices(string.ascii_uppercase, k=length))
	
	def is_full(self) -> bool:
		return len(self.clients) >= MAX_PLAYERS
	
	def broadcast(self, server, message: dict):
		for client in self.clients:
			server.send_message_tcp(client, message)

class Server:
	def __init__(self, host=None, port=54000):
		self.host = host or self.get_local_ip()
		self.port = port
		self.format = 'utf-8'
		self.rooms = {}

	def print_rooms(self):
		print(f"Rooms:")
		for key in self.rooms:
			print(f"- Code: {self.rooms[key].code}")
			print(f"- Connections: {self.rooms[key].clients}")
			print("")

	def get_local_ip():
		# From https://stackoverflow.com/a/28950776
		s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		s.settimeout(0)
		try:
			s.connect(('10.254.254.254', 1))
			ip = s.getsockname()[0]
		except Exception:
			ip = '127.0.0.1'
		finally:
			s.close()
		return ip
	
	def check_json_key(self, json, key: str, value):
		return key in json and json[key] == value
	
	def is_valid_message(self, message_json, check_payload):
		# Check if a message follows the protocol in protocol.md
		return 'type' in message_json and 'action' in message_json and (not check_payload or 'payload' in message_json)

	def send_message_tcp(self, connection: socket.socket, message_dict: dict):
		message_bytes = json.dumps(message_dict).encode(self.format)
		length_prefix = struct.pack('>I', len(message_bytes)) # 4 byte length header

		connection.sendall(length_prefix + message_bytes)

	def recv_all(self, sock: socket.socket, n):
		# From https://stackoverflow.com/a/17668009
		data = bytearray()
		while len(data) < n:
			packet = sock.recv(n - len(data))
			if not packet:
				return None
			data.extend(packet)
		return data

	def recv_message_tcp(self, sock: socket.socket):
		# Read length prefix
		raw_len = self.recv_all(sock, 4)
		if not raw_len:
			return None
		len = struct.unpack('>I', raw_len)[0]

		# Read message
		return self.recv_all(sock, len)

	def handle_udp_discovery(self):
		with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as udp_socket:
			udp_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
			udp_socket.bind(("0.0.0.0", DISCOVERY_PORT))  # Listen on all interfaces

			print(f"[UDP] Listening for discovery requests on {DISCOVERY_PORT}.", flush=True)

			while True:
				try:
					# Receive message
					message, address = udp_socket.recvfrom(BUFFER)
					if not message:
						continue
					
					# Decode the message
					try:
						message = json.loads(message.decode(self.format))
					except json.JSONDecodeError:
						continue

					if self.check_json_key(message, 'type', 'request') and self.check_json_key(message, 'action', 'get_local_host'):
						print(f"[UDP] Discovery request from {address}.")
						response = json.dumps({
							"type" : "response",
							"action" : "get_local_host",
							"payload" : { "host_ip" : self.host }
						})
						udp_socket.sendto(response.encode(self.format), address)
				except Exception as e:
					print(f"[UDP] Error: {e}", flush=True)

	def handle_client_message(self, message_json, from_connection: socket.socket, from_address):
		if not self.is_valid_message(message_json, False):
			return
		
		# Requests
		if self.check_json_key(message_json, 'type', 'request'):
			# create_room
			if self.check_json_key(message_json, 'action', 'create_room'):
				room = Room(from_connection, from_address)

				while room.code in self.rooms:
					room.code = Room.generate_code()

				self.rooms[room.code] = room

				print(f"[ROOM {room.code}] Room created.", flush=True)

				# Respond
				self.send_message_tcp(from_connection, {
					"type" : "response",
					"action" : "create_room",
					"payload" : { 
						"status" : "success",
						"room_code" : room.code 
					}
				})

				return
			
			# join_room
			if self.check_json_key(message_json, 'action', 'join_room') and 'payload' in message_json:
				connected = False
				if 'room_code' in message_json['payload']:
					code = message_json['payload']['room_code'].upper()

					if code in self.rooms and not self.rooms[code].is_full():
						self.rooms[code].clients.append(from_connection)
						print(f"[ROOM {code}] {from_address} joined room.", flush=True)
						connected = True

				# Response
				self.send_message_tcp(from_connection, {
					"type" : "response",
					"action" : "join_room",
					"payload" : { 
						"status" : "success" if connected else "fail" 
					}
				})

				return
			
			# start_game
			if self.check_json_key(message_json, 'action', 'start_game') and 'payload' in message_json:
				if 'room_code' in message_json['payload']:
					code = message_json['payload']['room_code'].upper()

					if code in self.rooms and self.rooms[code].is_full():
						room: Room = self.rooms[code]
						print(f"[ROOM {code}] Game started.", flush=True)
						room.game_started = True
						room.broadcast(self, {
							"type" : "notification",
							"action" : "game_started",
							"payload" : {}
						})

				return

	def handle_connection(self, connection: socket.socket, address):
		print(f"[CONNECTION] {address} connected.", flush=True)

		try:
			# Handle messages
			while True:
				# Recieve a message from the player
				message = self.recv_message_tcp(connection)
				if not message:
					print("[ERROR] Invalid message received.")
					break

				# Decode message
				try:
					message = json.loads(message.decode(self.format))
				except json.JSONDecodeError:
					continue

				print(f"[FROM {address}]  {message}.", flush=True)

				self.handle_client_message(message, connection, address)
		except ConnectionResetError:
			print(f"[DISCONNECTION] {address} disconnected abruptly", flush=True)
		finally:
			# Remove player from room
			to_delete = []
			for key, room in self.rooms.items():
				if connection in room.clients:
					room.clients.remove(connection)
					
				# Delete room if it is empty
				if not room.clients:
					to_delete.append(key)

			# Delete rooms after loop
			for key in to_delete:
				print(f"[ROOM {key}] Room closed.")
				del self.rooms[key]

			connection.close()

	def start(self):
		# Start UDP discovery in background thread
		udp_thread = threading.Thread(target=self.handle_udp_discovery, daemon=True)
		udp_thread.start()

		# Run threads for client connections
		with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
			server.bind((self.host, self.port))
			server.listen(MAX_PLAYERS) # Only accept a certain number of connections
			print(f"[LISTENING] Server is listening on {self.host}:{self.port}.", flush=True)

			while True:
				connection, address = server.accept()

				# Start handle_connection in a new thread
				thread = threading.Thread(target=self.handle_connection, args=(connection, address), daemon=True)
				thread.start()

if __name__ == "__main__":
	server = Server(socket.gethostbyname(socket.gethostname()))
	server.start()