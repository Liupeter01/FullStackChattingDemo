# FullStackChattingDemo
## Description

FullStackChattingDemo using C++17 standard

## Client

### Libraries

using QT6(Q::Widgets Qt::Network)

![](./assets/client_main.png)

![](./assets/register_page.png)

## Server

### Libraries

boost-1.84  (beast asio uuid mysql), grpc-1.50.2, hiredis, jsoncpp, ada(url parsing), spdlog

### Functions

/get_test(GET method): system function test.

/get_verification(POST method): User sends get CPATCHA request to server. server using GRPC protocol to communicate with NodeJS server and generate and store uuid in Redis DB.

/post_registration(POST method): User post registration request to server. server store info into DB.

![](./assets/server.png)

### Submodule -- Verification Server(NodeJS)

import ioredis, grpc-js proto-loader, nodemailer, uuidv4 libraries to the project

Sending verification code to server
![](./assets/verification.png)

 ![](./assets/result.png)



## Requirements

The project is self-contained almost all dependencies on both Windows and Linux/Unix-like systems.

### Main Server(C++)

main server using config.ini to store parameters

```ini
[GateServer]
port = 8080
[VerificationServer]
host=127.0.0.1
port = 65500
[MySQL]
username=root
password=123456
database=chatting
host=localhost
port=3307
timeout=60          #timeoutsetting seconds
[Redis]
host=127.0.0.1
port=6379
password=123456
```

### Verification Server(Nodejs)

verification server using verification-server/config.json to store parameters

```json
{
    "email":{
        "host": "please set to your email host name",
        "port": "please set to your email port",
        "username": "please set to your email address",
        "password": "please use your own authorized code"
    },
	"mysql": {
		"host": "your ip",
		  "port": "your port",
		  "password":  "your password"
	},
	"redis": {
		"host": "your ip",
		"port": "your port",
		"password": "your password"
	}
}
```

### Redis Server

1. Create a volume on host machine

   ```bash
   #if you are using windows, please download WSL2
   mkdir -p /path/to/redis/{conf,data} 
   ```

2. Download Redis configuration file

   ```bash
   #download redis.conf
   cd /path/to/redis/conf
   wget http://download.redis.io/redis-stable/redis.conf
   ```

3. Configuration file setting

   ```ini
   # bind 127.0.0.1 -::1
   # Protected mode is a layer of security protection, in order to avoid that
   # Redis instances left open on the internet are accessed and exploited.
   # When protected mode is on and the default user has no password, the server
   # only accepts local connections from the IPv4 address (127.0.0.1), IPv6 address
   # (::1) or Unix domain sockets.
   # By default protected mode is enabled. You should disable it only if
   # you are sure you want clients from other hosts to connect to Redis
   # even if no authentication is configured.
   protected-mode no
   
   # Accept connections on the specified port, default is 6379 (IANA #815344).
   # If port 0 is specified Redis will not listen on a TCP socket.
   port 6379
   
   # Unix socket.
   # Specify the path for the Unix socket that will be used to listen for
   # incoming connections. There is no default, so Redis will not listen
   # on a unix socket when not specified.
   # unixsocket /run/redis.sock
   # unixsocketperm 700
   # Close the connection after a client is idle for N seconds (0 to disable)
   timeout 0
   
   # TCP keepalive.
   # If non-zero, use SO_KEEPALIVE to send TCP ACKs to clients in absence
   # of communication. This is useful for two reasons:
   # 1) Detect dead peers.
   # 2) Force network equipment in the middle to consider the connection to be alive.
   # On Linux, the specified value (in seconds) is the period used to send ACKs.
   # Note that to close the connection the double of the time is needed.
   # On other kernels the period depends on the kernel configuration.
   #
   # A reasonable value for this option is 300 seconds, which is the new
   # Redis default starting with Redis 3.2.1.
   tcp-keepalive 300
   
   # Specify the log file name. Also the empty string can be used to force
   # Redis to log on the standard output. Note that if you use standard
   # output for logging but daemonize, logs will be sent to /dev/null
   logfile "your_log_file.log"
   
   # Set the number of databases. The default database is DB 0, you can select
   # a different one on a per-connection basis using SELECT <dbid> where
   # dbid is a number between 0 and 'databases'-1
   databases 16
   
   ############################## APPEND ONLY MODE ###############################
   # By default Redis asynchronously dumps the dataset on disk. This mode is
   # good enough in many applications, but an issue with the Redis process or
   # a power outage may result into a few minutes of writes lost (depending on
   # the configured save points).
   # The Append Only File is an alternative persistence mode that provides
   # much better durability. For instance using the default data fsync policy
   # (see later in the config file) Redis can lose just one second of writes in a
   # dramatic event like a server power outage, or a single write if something
   # wrong with the Redis process itself happens, but the operating system is
   # still running correctly.
   # AOF and RDB persistence can be enabled at the same time without problems.
   # If the AOF is enabled on startup Redis will load the AOF, that is the file
   # with the better durability guarantees.
   # Note that changing this value in a config file of an existing database and
   # restarting the server can lead to data loss. A conversion needs to be done
   # by setting it via CONFIG command on a live server first.
   # Please check https://redis.io/topics/persistence for more information.
   appendonly no
   
   requirepass ??????

4. Create Redis container

   ```bash
   #Pull the official docker image from Docker hub
   	docker pull redis:7.2.4
   	
   #create container
   docker run \
   --restart always \
   -p 16379:6379 --name redis \
   --privileged=true \
   -v /path/to/redis/conf/redis.conf:/etc/redis/redis.conf \
   -v /path/to/redis/data:/data:rw \
   -d redis:7.2.4 redis-server /etc/redis/redis.conf \
   --appendonly yes
   --requirepass "??????"
   ```

5. Entering Redis container

   ```bash
   #entering redis
   	docker exec -it redis bash
   	
   # login redis db
   	redis-cli
   ```

   

### MySQL Server

1. Create a volume on host machine

   ```bash
   #if you are using windows, please download WSL2
   mkdir -p /path/to/mysql/{conf,data} 
   ```

2. Create a configuration file(my.cnf)

   ```bash
   touch /path/to/mysql/conf/my.cnf	#create
   vim /path/to/mysql/conf/my.cnf		#write config file(you could use other editing tools!)
   ```

3. Configuration file setting

   ```ini
   # For advice on how to change settings please see
   # http://dev.mysql.com/doc/refman/8.0/en/server-configuration-defaults.html
   [mysqld]
   #
   # Remove leading # and set to the amount of RAM for the most important data
   # cache in MySQL. Start at 70% of total RAM for dedicated server, else 10%.
   # innodb_buffer_pool_size = 128M
   #
   # Remove leading # to turn on a very important data integrity option: logging
   # changes to the binary log between backups.
   # log_bin
   #
   # Remove leading # to set options mainly useful for reporting servers.
   # The server defaults are faster for transactions and fast SELECTs.
   # Adjust sizes as needed, experiment to find the optimal values.
   # join_buffer_size = 128M
   # sort_buffer_size = 2M
   # read_rnd_buffer_size = 2M
   
   # Remove leading # to revert to previous value for default_authentication_plugin,
   # this will increase compatibility with older clients. For background, see:
   # https://dev.mysql.com/doc/refman/8.0/en/server-system-variables.html#sysvar_default_authentication_plugin
   # default-authentication-plugin=mysql_native_password
   skip-host-cache
   skip-name-resolve
   datadir=/var/lib/mysql
   socket=/var/run/mysqld/mysqld.sock
   secure-file-priv=/var/lib/mysql-files
   user=mysql
   character-set-server=utf8
   max_connections=200
   max_connect_errors=10
   
   pid-file=/var/run/mysqld/mysqld.pid
   [client]
   socket=/var/run/mysqld/mysqld.sock
   default-character-set=utf8
   !includedir /etc/mysql/conf.d/
   ```

4. Create MySQL container

   **If you intended to pass a host directory, please use absolute path.**

   ```bash
   #Pull the official docker image from Docker hub
   	docker pull mysql:8.0
   	
   #Start a mysql server instance
   	 docker run --restart=on-failure:3 -d \
   	-v /path/to/mysql/conf:/etc/mysql \
       -v /path/to/mysql/data:/var/lib/mysql \
   	-e MYSQL_ROOT_PASSWORD="your_password" \
   	-p 3307:3306 --name "your_container_name"
       mysql:8.0
   ```

5. Entering mysql and login DB

   ```bash
   #entering mysql
   	docker exec -it "your_container_name" bash
   	
   # login mysql db
   #-u: root by default
   #-p password
   	mysql -uroot -p"your_password" 
   ```

   

## Developer Quick Start

### Platform Support
Windows, Linux, MacOS(Intel & Apple Silicon M)

### Download FullStackChattingDemo

```bash
git clone https://github.com/Liupeter01/FullStackChattingDemo
```

### Compile Client

1. For Windows

   ```bash
   #please import client dir to qt creator
   ```

2. For MacOS/Linux

   ```bash
   #It might take a long time to download dependency libraies!!!!
   cd FullStackChattingDemo/client
   cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
   cmake --build build --parallel [x]
   ```

### Compile Server

```bash
cd FullStackChattingDemo/server
git submodule update --init
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel [x]
```

### How to Execute

1. Execute Main Server Program

   ```bash
   ./server/build/ChattingServer
   ```

2. Activate Redis and MySQL service

3. Execute Verification Server(Nodejs)

   ```bash
   cd server/verification-server
   node index.js
   ```

4. Execute Client

   ```bash
   ./client/build/ChattingServer
   ```

### Error Handling
1. SyntaxError: Unexpected token  in JSON at position 0
   ```bash
   SyntaxError: Unexpected token  in JSON at position 0
       at JSON.parse (<anonymous>)
   ```

   Solving
   please change your encoding method to UTF-8, especially for VSCode user

   Referring Url
   https://stackoverflow.com/questions/55960919/nodejs-syntaxerror-unexpected-token-in-json-at-position-0

   
   
2. undefined symbol upb_alloc_global
   ```cmake
   set(protobuf_BUILD_LIBUPB OFF)
   ```

   Referring Url
   https://github.com/grpc/grpc/issues/35794

   
   
3. fatal error: 'unicode/locid.h' 'unicode/ucnv.h' file not found (usually happened on MacOS)
   Download icu 74.1
   ```bash
   wget https://github.com/unicode-org/icu/releases/download/release-74-1/icu4c-74_1-src.tgz
   ```

   Compile and Install
   ```bash
   git clone https://github.com/unicode-org/icu.git
   mkdir icu4c-build
   cd icu4c-build
   ./runConfigureICU Linux
   make check
   ```

   set cmake variable
   ```cmake
   cmake -Bbuild -DCMAKE_INCLUDE_PATH=/usr/local/include
   cmake --build build --parallel x
   ```

   Referring Url
   https://unicode-org.github.io/icu/userguide/icu4c/build.html

   
   
4. boringssl undefined win32
   ```cmake
   set(OPENSSL_NO_ASM ON)
   ```

   Referring Url
   https://github.com/grpc/grpc/issues/16376

   
   
5. Handling gRPC issue
   Issue description
   
   ```bash
   CMake Error: install(EXPORT "protobuf-targets" ...) includes target "libprotobuf-lite" which requires target "absl_node_hash_map" that is not in any export set.
   ```
   
   Problem Solving
   ```cmake
   set(ABSL_ENABLE_INSTALL ON)
   ```
   
   Referring Url
    https://github.com/protocolbuffers/protobuf/issues/12185
    https://github.com/protocolbuffers/protobuf/issues/12185#issuecomment-1594685860
   
   
   
6. E No address added out of total 1 resolved

   you have to start the main server first and then open nodejs service
