# Peer-hosted matchmaking server

import socket
import random
import string
import threading
import json

BUFFER = 1024
MAX_PLAYERS = 2 # This is a two-player game

TCP_PORT       = 54000 # Port for game
DISCOVERY_PORT = 54001 # Port for host discovery

class Room:
	def __init__(self, host_connection, host_address, code=None):
		self.host         = host_connection
		self.address      = host_address
		self.clients      = [host_connection]
		self.code         = code or self._generate_code()
		self.game_started = False

	def _generate_code(self, length=5) -> str:
		return ''.join(random.choices(string.ascii_uppercase + string.digits, k=length))
	
	def is_full(self) -> bool:
		return len(self.clients) >= MAX_PLAYERS
	
	def broadcast(self, sender, message):
		...

class Server:
	def __init__(self, host=None, port=54000):
		self.host = host or self.get_local_ip()
		self.port = port
		self.format = 'utf-8'
		self.rooms = {}

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
						print(f"[UDP] Discovery request from {address}")
						response = json.dumps({
							"type" : "response",
							"action" : "get_local_host",
							"payload" : { "host_ip" : self.host }
						})
						udp_socket.sendto(response.encode(self.format), address)
				except Exception as e:
					print(f"[UDP] Error: {e}", flush=True)

	def handle_connection(self, connection: socket.socket, address):
		print(f"[CONNECTION] {address} connected.", flush=True)
		current_room = None

		try:
			while True:
				data = connection.recv(BUFFER)
				if not data:
					break

				# Decode message
				try:
					msg = json.loads(data.decode(self.format))
				except json.JSONDecodeError:
					continue

				# Recieve a message from the player
				print(f"[FROM {address}]  {msg}.", flush=True)
		except ConnectionResetError:
			print(f"[DISCONNECTION] {address} disconnected abruptly", flush=True)
		finally:
			if current_room:
				# Disconnect player
				if connection in current_room.clients:
					current_room.clients.remove(connection)
				# Delete room
				if not current_room.clients:
					print(f"[ROOM CLOSED] Room ({current_room.code}) closed", flush=True)
					del self.rooms[current_room.code]

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