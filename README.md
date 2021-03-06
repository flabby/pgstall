## Intro
* **A midlleware used to stall greenplum' Insert commands and load data by gpload**
* a simple ETL tool with some limitations;
* tested with postgre-jdbc 9.4 and php-pgsql
* support simple Postgre Frontend/Backend protocol
  - Query Message
  - Extended Message

## Usage

### Compile

* install dependencies
* GCC 4.7+ to support c++11

```powershell
git clone https://github.com/Qihoo360/gpstall
git submodule update --init
make
# for release: make __REL=1
```

### Run
* copy output/lib/ to /usr/local/gpstall/lib/ if needed;
```powershell
Usage:
  ./gpstall [-h] [-c conf/file]
    -h            -- show this help
    -c conf/file  -- config file
```

### Configuration

```powershell
######################
### gpstall conf
######################
local_ip : 127.0.0.1
local_port : 8001
worker_num : 8

## csv file size (Bytes)
# file_size : 400

## Seconds
load_interval : 120
flush_interval : 1800

# data_path : ./data
# load_script : bin/load.sh
# conf_script : bin/gpload.yaml.ori
daemon_mode : false

######################
### Glog conf
######################
log_path : ./logs
# 0, log; 1, warning; 2, error; 3, fatal error.
minloglevel : 0
# Stdout stderr size, not the glog file size
#maxlogsize : 1800

######################
### Greenplum conf
######################
gp_user : user
passwd : passwd
gp_host : 127.0.0.1
gp_port : 15432

######################
### gpfdist conf
######################
gpd_host : 127.0.0.1
gpd_port : 8081
error_limit : 50000
```

### Client example

```powershell
$ psql -h 127.0.0.1 -p 8001 -U user test_db
Password for user user:
psql (8.3.23, server gpstall0.1)
WARNING: psql version 8.3, server version 0.0.
Some psql features might not work.
Type "help" for help.

test_db=# INSERT INTO test_table("name", "id") VALUES ('zhang', 1);
INSERT 0 1

test_db=# stall_status;
NOTICE:
  start_time: 20170920192532
  service_status: ONLINE
  conn_num: 1
  QPS: 0
  gpload_failed_num: 2
  
```

## Limitation

* 1 We only support simple Insert command.
`insert into table_name (attribute_A, attribute_B, ...) values (value_A_1, value_B_1, ...), (value_A_2, value_B_2, ...);`
* 2 Client is forced to use password to login;

## Contact Us
email: g-infra-bada@list.qihoo.net
