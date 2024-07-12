---
title: 바인드 변수가 너무 많은 쿼리 두번째
author: me
date: 2024-07-08 00:00:00 +0800
categories: [Oracle,SQL개선]
tags: [파싱타임개선]
---

## 문제   

 평소엔 1초도 안걸리는데 종종 쿼리가 엄청 느려진다는 요청을 받는다.   
어떤 쿼리인가 보니 바인드 변수 갯수가 그 때 그 때 마다 변하고     
추가되는 바인드 변수가 수십개에서 수천개가 사용되니 종종 다시 파싱하느라 오래 걸리는 것이다.    

## 원인   

필요한 대상을 선택하고 그 대상들에 대한 결과를 보여주는 화면이여서 선택된 대상의 키 데이터가 모두 바인드 변수로 들어와야했다.   


## 해결

오라클의 Type을 이용하면 해결 가능하다   

우선 테스트용으로 오라클에 TYPE 와 테스트 테이블을 만든다   

```SQL
CREATE OR REPLACE TYPE my_object_type AS OBJECT (
    id NUMBER
);
/

CREATE OR REPLACE TYPE my_table_type AS
    TABLE OF my_object_type;
/

CREATE TABLE test_table (
    id   NUMBER
    , name VARCHAR2(100)
);

INSERT INTO test_table VALUES (
    1
  , 'test1'
);

INSERT INTO test_table VALUES (
    2
  , 'test2'
);

INSERT INTO test_table VALUES (
    3
  , 'test3'
);

INSERT INTO test_table VALUES (
    4
  , 'test4'
);

COMMIT;
```

## 테스트-JS

NodeJS에서 간단 테스트     

```
npm install oracledb 
npm install path
```

app.js 작성    

```JS
const oracledb = require('oracledb');
const path = require('path'); 

process.env.TNS_ADMIN = path.join('tnsadmin.ora파일의경로');

const dbConfig = {
  user: 'oracleID',
  password: 'pass',
  connectString: 'MyCloudDB' 
};

async function run() {
  let connection;

  try {
    // Oracle DB 연결
    connection = await oracledb.getConnection(dbConfig);
    console.log('Successfully connected to Oracle Database');

    // 오브젝트 타입 및 테이블 타입 정의
    const MyObjectType = await connection.getDbObjectClass('MY_OBJECT_TYPE');
    const MyTableType = await connection.getDbObjectClass('MY_TABLE_TYPE');

    // 테이블 타입에 데이터 삽입
    const employees = new MyTableType([
      new MyObjectType({ ID: 1 }),
      new MyObjectType({ ID: 2 }),
      new MyObjectType({ ID: 3 })
    ]);

    // 쿼리 실행
    const result = await connection.execute(
      `SELECT id, name FROM test_table where id in ( select id from TABLE(:bindtest))`,
      { bindtest: employees }
    );

    // 결과 출력
    console.log(result.rows);

  } catch (err) {
    console.error(err);
  } finally {
    if (connection) {
      try {
        await connection.close();
      } catch (err) {
        console.error(err);
      }
    }
  }
}

run();
```

작성한뒤 터미널에서 실행   

```
node app.js
Successfully connected to Oracle Database
[ [ 1, 'test1' ], [ 2, 'test2' ], [ 3, 'test3' ] ]
```

결과까지 잘 나온다.   

## 테스트-자바 

생각해 보니 오라클 사용하는곳은 자바를 주로 사용한다 


```JAVA


import java.sql.*;
import oracle.jdbc.OracleConnection;
import oracle.sql.ARRAY;
import oracle.sql.ArrayDescriptor;
import oracle.sql.STRUCT;
import oracle.sql.StructDescriptor;

public class OracleTableTypeExample {
    public static void main(String[] args) {
        String url = "jdbc:oracle:thin:@localhost:1521/XE";
        String user = "your_username";
        String password = "your_password";

        try (Connection conn = DriverManager.getConnection(url, user, password)) {
            OracleConnection oraConn = conn.unwrap(OracleConnection.class);

            // Object Type 디스크립터 생성
            StructDescriptor objectDescriptor = StructDescriptor.createDescriptor("MY_OBJECT_TYPE", oraConn);

            // Table Type 디스크립터 생성
            ArrayDescriptor tableDescriptor = ArrayDescriptor.createDescriptor("MY_TABLE_TYPE", oraConn);

            // 데이터 준비
            Object[][] data = {
                {1, "Item1", 100},
                {2, "Item2", 200},
                {3, "Item3", 300}
            };

            // STRUCT 객체 배열 생성
            STRUCT[] structs = new STRUCT[data.length];
            for (int i = 0; i < data.length; i++) {
                structs[i] = new STRUCT(objectDescriptor, oraConn, data[i]);
            }

            // ARRAY 객체 생성
            ARRAY dataArray = new ARRAY(tableDescriptor, oraConn, structs);

            // SQL 쿼리 실행
            String sql = "SELECT * FROM your_table WHERE (id, name) IN (SELECT id, name FROM TABLE(?))";
            try (PreparedStatement pstmt = conn.prepareStatement(sql)) {
                pstmt.setArray(1, dataArray);
                
                try (ResultSet rs = pstmt.executeQuery()) {
                    while (rs.next()) {
                        // 결과 처리
                        System.out.println(rs.getInt("id") + ", " + rs.getString("name") + ", " + rs.getInt("value"));
                    }
                }
            }

            System.out.println("쿼리 실행 완료");
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
}
```

