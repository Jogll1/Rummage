## Message Protocol

<!-- All messages must be length-prefixed. This means the message length shall be prepended to the message.
The header packet (containing the length) must be a 4 byte uint32_t or equivalent (big endian). -->

### Structure

Messages are sent in JSON format. They include the following fields:

| Field      | Type   | Description |
|------------|--------|-------------|
| `type`     | string | "request", "response" |
| `action`   | string | The name of the action |
| `payload`  | object | Data specific to the action |

### Actions

#### get_local_host
Request payload: n/a

Response payload:
```json
{
    "host_ip" : "127.0.0.1"
}
```

Requests the IP address of an active server on the local network.
This is requested by `findLocalHost()` on the client and served by
`handle_udp_discovery()` on the server.

### create_room
Request payload: n/a

Response payload:
```json
{
    "room_code" : "XXXXX"
}
```

Asks the server to create a new room.

### join_room
Request payload: 
```json
{
    "room_code" : "XXXXX"
}
```

Response payload:
```json
{
    "connected" : true or false
}
```

Asks the server to create a room given a room code.
As long as the room isn't full, the client can join a room with a valid code.

### Example