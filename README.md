## What I Learned and Why I Loved the ft_irc Project 💬✨

### 🧠 Key Concepts and Skills Acquired

- **Low-Level Network Programming (C++98)**
  Built a full-featured IRC server from scratch using raw socket programming and TCP/IP protocols, following the C++98 standard.

- **Non-Blocking I/O with Poll**
  Handled multiple simultaneous client connections using `poll()` to manage read/write/listen operations in a single loop without blocking.

- **IRC Protocol Essentials**
  Implemented essential IRC features such as:
  - Nickname and username authentication
  - Channel creation and joining
  - Public and private messaging
  - Channel broadcasting to all users

- **IRC Channel Management**
  Developed operator-level controls, including:
  - `KICK`, `INVITE`, `TOPIC`, `MODE` commands
  - Channel modes like invite-only, topic-lock, key-protected, user limits, and operator privileges

- **Robust Command Parsing**
  Learned to handle fragmented input over TCP and reconstruct complete IRC commands from partial packets using buffer management.

- **Secure Server Architecture**
  Ensured server stability by avoiding crashes and segmentation faults, and correctly managing file descriptors in non-blocking mode.

- **Makefile & Code Structure**
  Organized the project with a clean, maintainable architecture and a fully functional Makefile with standard rules (`all`, `clean`, `fclean`, `re`).

- **Testing with Real Clients**
  Used real IRC clients (like irssi or HexChat) to connect to the server and validate functionality — just like in production systems.

---

### ❤️ Why This Project Was So Engaging

- **It Was a Real Server**
  This wasn’t a simulation — it was a fully operational server that real IRC clients could connect to. That brought a great sense of accomplishment.

- **Solidified My Understanding of Network Layers**
  I now have a clear mental model of how TCP/IP communication works, how clients connect, and how data flows in real-world applications.

- **Challenging and Rewarding**
  Debugging low-level behavior and ensuring non-blocking operations taught me patience, precision, and the value of clean architecture.

- **Expanded My Comfort Zone in C++**
  Working within C++98 constraints forced me to better understand memory management, object-oriented design, and standard libraries.

- **Foundation for Future Networking Projects**
  Now I feel more prepared to tackle other real-time systems — like chat apps, multiplayer games, and microservice communication.

---

🔥 Completing `ft_irc` gave me the confidence to say: I can build a protocol-based network service from scratch, understand the internals, and debug it like a pro.
