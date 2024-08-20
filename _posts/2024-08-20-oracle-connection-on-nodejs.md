---
title: 오라클 자율운영데이터베이스 백엔드에서 접속
author: me
date: 2024-08-20 00:00:00 +0800
categories: [Oracle,NODEJS]
tags: []
---

## 개요   

nodejs와 python을 사용해서 오라클 프리티어 DB에 접속하려면 Wallet 파일을 쓰거나    
[Oracle Autonomous Database 접속시 TNSNAMES.ora](https://docs.oracle.com/ko/solutions/oci-iot-open-source-backend/set-and-configure1.html#GUID-4B6DB264-662A-4ECB-8756-855974031309) 을 사용해서     
접속하라고 안내하고 있는데     

TNSNAMES.ora 파일을 안만들고 그냥 내용을 변수에 저장해서 쓰면 되는것을 자꾸 잊어서 기록한다    




## TNSNAMES.ora 사용하여 접속

```js
const oracledb = require('oracledb');
const path = require('path'); // path 모듈을 import합니다.

process.env.TNS_ADMIN = path.join('~/.oracle/network/admin');

const dbConfig = {
  user: 'scott',
  password: 'tiger',
  connectString: 'MyCloudDB'  // TNS 이름을 지정합니다
};
```

아 세상 간단 
 
## 변수에 접속정보 저장하여 접속

```python
import oracledb

dsn = "(description= (retry_count=20)(retry_delay=3)(address=(protocol=tcps)(port=1521)(host=adb.ap-oraclecloud.com))(connect_data=(service_name=g91_myclouddb_high.adb.oraclecloud.com))(security=(ssl_server_dn_match=yes)))"


def get_connection():
    
    connection = oracledb.connect(
        user="scott",
        password="tigre",
        dsn=dsn
    )

```


## 끝
