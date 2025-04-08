# Telemetry Server (C++ Backend Recruitment Task)

This project implements a basic asynchronous REST API server in C++ using Boost.Beast and Boost.Asio.

## Features
- Asynchronous networking
- In-memory storage (no database required)
- Simple REST API:
  - `POST /paths/{event}`
  - `GET /paths/{event}/meanLength`
- JSON body parsing with [nlohmann/json](https://github.com/nlohmann/json)
- Built with CMake

## Requirements
- C++20 compiler
- Boost (system, thread, beast)
- CMake ≥ 3.16

## Setup

1. Clone repository (or copy project)
2. Install Boost libraries if missing
3. Configure and build:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

4. Run the server:

```bash
./telemetry_server
```

Server will run on `http://localhost:8080`.

## Testing

Run the provided test script:

```bash
chmod +x test_server.sh
./test_server.sh
```

Or manually use `curl` to interact with the server.

## Endpoints

### POST /paths/{event}
Save an event.

Example:

```bash
curl -X POST http://localhost:8080/paths/shopping_cart -H "Content-Type: application/json" -d '{"values":[10,20,30,40,50,60,70,80,90,100],"date":1712394000}'
```

### GET /paths/{event}/meanLength
Calculate mean duration for an event.

Example:

```bash
curl -X GET http://localhost:8080/paths/shopping_cart/meanLength -H "Content-Type: application/json" -d '{"resultUnit":"seconds"}'
```

Optional query filters:
- `startTimestamp`
- `endTimestamp`

---

## API Endpoints Summary

| Method | Path | Request Body | Response | Description |
|--------|------|--------------|----------|-------------|
| POST   | `/paths/{event}` | `{ "values": [10 numbers], "date": timestamp }` | `{}` | Save a new event data set |
| GET    | `/paths/{event}/meanLength` | `{ "resultUnit": "seconds" or "milliseconds", "startTimestamp" (optional), "endTimestamp" (optional) }` | `{ "mean": number }` | Calculate mean event duration |

---

## Future Improvements

- Input Validation: Validate that POSTed event data includes exactly 10 numerical values, and that no invalid formats are accepted.
- Input Validation: Validate event timestamps to ensure they are positive integers and fall within a realistic range (e.g., between 1970 and 2100).
- Optimize Locking: Replace std::mutex with std::shared_mutex to allow multiple concurrent GETs without blocking each other.
- Graceful Shutdown: Catch SIGINT (Ctrl+C) to cleanly shutdown the server and save state.
- Configuration Management: Load server settings (port, thread count) from a configuration file.
- Thread Pool for Heavy Tasks: Separate IO and heavy CPU task processing using dedicated thread pools.
- Optimize Data Structures: After validating the system's stability, consider switching from std::map to std::unordered_map for faster lookup times, as order preservation is not required.
