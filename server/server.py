# Peer-hosted matchmaking server

import socket
import random
import string
import threading
import json

BUFFER = 1024
PORT = 5050
FORMAT = 'utf-8'

SERVER = socket.gethostbyname(socket.gethostname()) # Server IP
ADDRESS = (SERVER, PORT)

MAX_PLAYERS = 2 # This is a two-player game

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

# Map room codes to room objects
rooms = {}

def handle_connection(connection: socket.socket, address):
	print(f"[CONNECTION] {address} connected.", flush=True)
	current_room = None

	try:
		while True:
			data = connection.recv(BUFFER)
			if not data:
				break

			# Decode message
			try:
				msg = json.loads(data.decode(FORMAT))
			except json.JSONDecodeError:
				continue

			# Recieve a message from the player
			# { "action" : "host", "join", "move_tile", "end_go" etc. }
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
				del rooms[current_room.code]

		connection.close()

def start_server():
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
		server.bind(ADDRESS)
		server.listen(MAX_PLAYERS) # Only accept a certain number of connections
		print(f"[LISTENING] Server is listening on {SERVER}:{PORT}.")

		while True:
			connection, address = server.accept()

		 	# Start handle_connection in a new thread
			thread = threading.Thread(target=handle_connection, args=(connection, address), daemon=True)
			thread.start()

if __name__ == "__main__":
	start_server()