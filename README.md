# Table of Contents

1. [Introduction](#1-introduction)
2. [Getting Started](#2-getting-started)
3. [Program Structure](#3-program-structure)
4. [Linux Network Traffic](#4-linux-network-traffic)
5. [Legal and Ethical Implications](#5-legal-and-ethical-implications)
6. [License](#6-license)

# 1 Introduction

`here_is_johnny_v` is a proof of concept tool designed to demonstrate management of a Linux system by a remote
command-and-control (C2) server running the [here_is_johnny_c2](https://github.com/albert-gee/here_is_johnny_c2)
program.

---

# 2 Getting Started

## 2.1 Prerequisites

- Linux with UFW (Uncomplicated Firewall) installed.
- UFW is enabled and blocking all incoming connections.
- UFW logging level is set to high.
- The user has permission to read the UFW log file.
- The user has root privileges to use the `packet_builder` and `data_noise` components.

The Uncomplicated Firewall (UFW) is installed by default in all Ubuntu systems since 8.04 LTS and in all Debian systems
since 10. It is disabled by default and the logging level is set to low.

## 2.2 UFW Setup

To enable UFW on Debian or Ubuntu and set the logging level to high, run the following commands:

```bash
sudo ufw enable
sudo ufw logging high
```

## 2.3 Installation

To build and install `here_is_johnny_v`, follow these steps:

First, clone the repository:

```sh
git clone https://github.com/albert-gee/here_is_johnny_v.git
```

Navigate to the `here_is_johnny_v` directory:

```sh
cd here_is_johnny_v
```

Compile the program:

```sh
make
```

## 2.4 Usage

To start `here_is_johnny_v`, run:

```bash
sudo ./here_is_johnny_v
```

This will start the program, which will immediately begin monitoring the UFW log file for port-knocking sequences.

---

# 3 Program Structure

The core components of this program are:

- `crypto` provides encryption and decryption functions.
- `data_noise` hides encrypted data within crafted packets and sends it.
- `inotify_observer` monitors file system changes.
- `packet_builder` generates crafted network packets.
- `upkd` listens for port-knock sequences by monitoring the UFW log file.

The `upkd` component employs the `inotify_observer` component to monitor the UFW log file for port-knocking sequences.
The phrase `here_is_johnny` hidden within a port-knocking sequence and is used to establish a connection.

Once a connection is established, the program listens for port-knocking sequences that may contain text file data, shell
scripts, or commands from `here_is_johnny_c2`.

The text file data is saved to a file in the current directory using the `file_operations.c` utility. The results of the
execution of the shell scripts and commands are exfiltrated using the `data_noise` component which utilizes
the `packet_builder` component to craft network packets.

## 3.1 crypto

The `crypto` component provides functions for encrypting and decrypting data using the Advanced Encryption Standard (
AES) algorithm in Cipher Block Chaining (CBC) mode. AES is a symmetric key encryption algorithm used to secure data. It
has a fixed block size of 128 bits and a key size of 128, 192, or 256 bits. For this project, we use a 128-bit key for
encryption and decryption.

The component includes functions for setting keys and initialization vectors (IVs), encrypting and decrypting data, and
handling key and IV storage. For demo purposes, the keys are hard-coded.

The program utilizes the `aes_cbc_encrypt` function in order to encrypt data in the `data_noise` component.

The program utilizes the `aes_cbc_decrypt` function in order to decrypt port-knocking sequences in the `upkd` component.

## 3.2 data_noise

The `data_noise` component is designed to demonstrate transfer of data to an external location covertly by hiding it in 
headers of network packets.

### 3.2.1 Overview

The component encrypts 15-byte-chunks of data using the `crypto` component and embeds the encrypted data in the headers
of crafted TCP packets using the `packet_builder` component. The packets are sent over the network with a delay between
each packet to avoid detection.

### 3.2.2 Data Encoding

Similar to the `upk` component, the `data_noise` implements the Structure Preserving Storage Pattern by encoding data
into network packet headers and sending them over the network. The `here_is_johnny_c2` utilizes a packet sniffer to 
capture these packets and extract the hidden data.

This approach relies on the TCP/IP model for hiding data. Some fields are more suitable for hiding data due to their
size, variability, and less strict validation by networking equipment. The following list provides an analysis of
various fields for their suitability in covert channels:

- **Ethernet Source MAC Address (6 bytes)**
    - **Pros**: Relatively large and can be manipulated.
    - **Cons**: MAC addresses are often validated and logged by network devices, making changes detectable.
- **Ethernet Destination MAC Address (6 bytes)**
    - **Pros**: Similar to source MAC, but often more scrutinized.
    - **Cons**: Tampering can cause misrouting and network issues.
- **IP Source Address (4 bytes)**
    - **Pros**: Can be changed within certain networks.
    - **Cons**: Source IP addresses are often validated and logged, making changes detectable.
- **IP Destination Address (4 bytes)**
    - **Pros**: Not a good candidate as tampering can cause packet misrouting.
    - **Cons**: Heavily validated and essential for correct routing (not a problem for Packet Sniffer).
- **TCP Source Port (2 bytes)**
    - **Pros**: Often variable and can be manipulated.
    - **Cons**: Some port numbers are reserved and changes can be detected.
- **TCP Destination Port (2 bytes)**
    - **Pros**: Good candidate for covert communication with a server protected by a firewall.
    - **Cons**: Some port numbers are reserved and changes can be detected (not a problem for covert communication).
- **TCP Sequence Number (4 bytes)**
    - **Pros**: Large field, often variable.
    - **Cons**: Essential for data integrity and order, tampering can disrupt communication (not a problem for crafted
      packets).
- **TCP Acknowledgment Number (4 bytes)**
    - **Pros**: Similar to sequence number.
    - **Cons**: Essential for reliable communication, tampering can cause issues (not a problem for crafted packets).
- **TCP Flags (2 bytes)**
    - **Pros**: Limited variability, tampering can be detected easily.
    - **Cons**: Essential for TCP communication states (not a problem for crafted packets).
- **TCP Window Size (2 bytes)**
    - **Pros**: Variable, can be manipulated slightly.
    - **Cons**: Important for flow control, large changes can be detected.
- **TCP Checksum (2 bytes)**
    - **Pros**: Essential for error-checking, tampering detectable.
    - **Cons**: Modifying this field without adjusting data will cause packet rejection.
- **TCP Urgent Pointer (2 bytes)**
    - **Pros**: Rarely used, can be manipulated.
    - **Cons**: Detection possible if used by application layer protocols.

**Note**: Flooding the network with forged packets can overwhelm a server.

The `data_noise` component encodes data into the following fields of packet headers:

- IP source address, (4 bytes)
- TCP source port, (2 bytes)
- TCP destination port, (2 bytes)
- TCP sequence number, (4 bytes)
- TCP acknowledgment number (4 bytes).

The `libnet` library allows us to craft packets that appear legitimate and compliant with standard protocols,
potentially evading detection by network security mechanisms such as firewalls and intrusion detection systems (IDS).

## 3.3 inotify_observer

**`inotify`** (inode notify) is a Linux kernel subsystem designed to monitor file system changes and report these
changes to applications. The `inotify` API enables the monitoring of individual files or entire directories. When a
directory is monitored, `inotify` can provide events for both the directory itself and the files contained within it.
The `inotify` API uses fewer file descriptors compared to alternatives, making it compatible with established select and
poll interfaces.

The `inotify_observer` component utilizes the `inotify` API to monitor file system events, such as file creation,
modification, and deletion. It follows the **Observer design pattern**, where an object (the **subject**) maintains a
list of dependents (the **observers**) and notifies them of state changes. This pattern is ideal for scenarios where
changes in one part of the system need to be communicated to other parts.

The functionality of the `inotify_observer` component is divided into two main files:

- **`inotify_observer.h`**: Defines the `InotifyObserver` structure, which includes the `handleEvent` function for
  processing `inotify` events.
- **`inotify_subject.h`**: Manages file system events using `inotify`, maintaining a list of `inotify` watches, and 
  notifying observers of changes.

The `inotify_observer` component creates a new thread to monitor file system events. This thread reads `inotify` events
and calls the `handleEvent` function for each event. The `handleEvent` function processes the event and notifies
observers if the event is relevant.

This component is used to monitor the UFW log file for changes. When the log file is modified, the `handleEvent`
function is called to process the new log entries.

## 3.4 packet_builder

The `packet_builder` component uses the `libnet` library to construct custom network packets. `libnet` is a high-level
API used in the construction and injection of network packets. It provides a simplified interface for packet creation,
manipulation, and transmission, making it easier to implement packet-related functionality in applications.

The `packet_builder` component includes functions for setting network interface, creating Ethernet frames, creating IP
packets, and creating TCP packets. These functions allow the program to craft network packets with custom headers and
payloads, which can be used for various purposes, such as hiding data or performing port-knocking sequences.

The program uses the `packet_builder` component to create TCP packets with custom headers and payloads. These packets
are used to hide encrypted data and send it over the network.

## 3.5 upkd

The `upkd` component demonstrates a port-knocking daemon that listens for port-knocking sequences by monitoring the UFW
log file. Port-knocking is a security technique used to protect network services from unauthorized access. It involves
sending a series of connection attempts to closed ports in a specific sequence. Once the correct sequence is received,
the firewall opens the desired port, allowing the client to connect.

The `upkd` component includes the following functions:

- `start_monitoring`: Starts monitoring the UFW log file for port-knocking sequences.
- `stop_monitoring`: Stops monitoring the UFW log file.
- `handle_event`: Processes `inotify` events and checks for port-knocking sequences.
- `process_log_entry`: Parses log entries and extracts port-knocking data.
- `validate_sequence`: Validates the port-knocking sequence and decrypts the data.

The `upkd` component utilizes the `inotify_observer` component to monitor the UFW log file for changes. When a log entry
is detected, the `handle_event` function processes the entry and checks for port-knocking sequences. If a valid sequence
is found, the program decrypts the data using the `crypto` component and processes it accordingly.

---

# 4 Linux Network Traffic

This section explains how Linux manages network traffic, crucial for understanding the covert communication system in
this project. It covers the network interface controller (NIC), network stack, sockets, firewall, and Berkeley Packet
Filter (BPF).

## 4.1 NIC and Network Stack

When a NIC receives an Ethernet frame, it checks if the frame's destination MAC address matches its own or is a
broadcast or multicast address. If there's a match, the NIC generates an interrupt request to the CPU, signaling that
data is ready to be processed. The CPU copies the data from the NIC to system memory, and the NIC may use Direct Memory
Access (DMA) to transfer the data directly into the system's memory, reducing CPU involvement.

The NIC can also be put in promiscuous mode if its driver supports it, allowing the NIC to receive all Ethernet packets
on the network, including those addressed to other hosts.

The network stack within the Linux kernel examines the EtherType field of the Ethernet frame to determine the
encapsulated protocol and dispatches the frame to the appropriate protocol handler. Each protocol handler ensures the
packet isn't corrupt according to its specification.

Internet protocol handlers, such as IPv4 or IPv6, check if the packet is intended for the host and perform checksum
calculations to validate data integrity. They strip off headers and pass the payload to a transport protocol handler,
usually TCP, UDP, or QUIC. Transport layer protocols use port numbers to provide end-to-end communication services for
applications. This process continues until the data reaches the application layer, where it's processed by the
user-level application.

## 4.2 Sockets

In Linux, sockets function similarly to file I/O, using descriptors provided by the system to manage resources such as
files or network connections. This allows for familiar system calls like `read()`, `write()`, and `close()` to be used
with both files and sockets. When a socket is created using a system function like `socket()`, the system assigns it a
socket descriptor—an integer used to manage and reference the socket in all subsequent data transfer operations. Each
socket is uniquely identified by a combination of an IP address and a port number.

The operating system handles the encapsulation of data written to a socket by adding the necessary headers and trailers
for network transmission. On the receiving end, the OS strips these headers and trailers to present the raw data to the
application.

The Berkeley socket API, widely adopted across many operating systems including Linux, supports several socket types:

- Datagram Sockets (`SOCK_DGRAM`) used for connectionless communication (UDP).
- Stream Sockets (`SOCK_STREAM`) used for connection-oriented communication (TCP).
- Raw Sockets (`SOCK_RAW`) provide access to lower-level protocols and allow custom packet creation.

Raw sockets enable applications to construct packets and send them directly to the network interface, offering detailed
control over packet contents. This bypasses some of the processing typically handled by the host's TCP/IP stack,
offering more control to the user. However, raw sockets do not completely bypass the TCP/IP stack for all operations;
they usually still interact with the stack for some sending and receiving operations.

## 4.3 Firewall

A firewall monitors and controls incoming and outgoing network traffic based on ports, IP addresses, and protocols. It
uses an access-control list (ACL) to decide which packets to inspect and what action to take. There are three basic
actions for handling packets:

- Silent discard (default)
- Discard with an Internet Control Message Protocol (ICMP) or TCP reset response to the sender
- Forwarding to the next hop

The Linux kernel provides a system called Netfilter, enabling packet filtering, network address translation, and port
translation functionalities. It can also be used for packet manipulation, including altering TOS/DSCP/ECN bits in the IP
header. Netfilter consists of hooks within the kernel, allowing kernel modules to register callback functions with the
networking stack. These functions, applied as filtering and modification rules, are triggered for every packet passing
through the respective hook.

`iptables` has been a user-space utility program for Netfilter, allowing users to configure firewall rules.
However, `nftables` is replacing `iptables` with `nft` as the user space utility. As of Ubuntu 21.10, `nftables` has
become the default backend for the firewall, offering improved functionality and flexibility in managing network
filtering rules.

The Uncomplicated Firewall (UFW) is a frontend for `iptables` or `nftables`. It is available by default in all Ubuntu
installations since 8.04 LTS and in all Debian installations since 10.

## 4.4 Berkeley Packet Filter

The Berkeley Packet Filter (BPF) is a packet filter used at the operating system level, providing a raw interface to the
data link layer. It enables the capture and filtering of packets based on user-defined criteria. Linux Socket
Filtering (LSF) is derived from the Berkeley Packet Filter, and in this report, LSF is used interchangeably with BPF.
BPF enables packet capture before Netfilter applies its filtering rules.

BPF has two main components:

- Network tap collects copies of packets from the network device driver and delivers them to a listening application.
- Packet filter is a user-defined program which analyzes the intercepted packets based on specified criteria and
  determines whether a packet is relevant to a listening application.

The Berkeley Packet Filter enables packet capture prior to Netfilter applying its filtering rules, providing visibility
into all network traffic, including packets that may be allowed or blocked by higher-level filtering rules.

---

# 5 Legal and Ethical Implications

Using programs like `here_is_johnny_v` for unauthorized access or control of computer systems is illegal and
unethical. Such activities can result in legal consequences and damage to the targeted systems and networks. This
program is intended for educational purposes and lawful use only. Users must obtain explicit permission from system
owners before deploying or testing this software on any network.

This project demonstrates the technical aspects of covert systems and network penetration techniques, highlighting the
legal and ethical considerations involved. It serves an educational purpose, illustrating the risks and implications of
deploying such systems in real-world scenarios. By promoting responsible and ethical use of technology, potential risks
can be mitigated, and the rights and privacy of individuals in the digital age can be safeguarded.

---

# 6 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
