---
title: IN절 조건이 너무 많은 쿼리 첫번째
author: me
date: 2024-06-21 00:00:00 +0800
categories: [Oracle,SQL개선]
tags: [파싱타임개선]
---

## 문제   

 평소엔 1초도 안걸리는데 종종 쿼리가 엄청 느려진다는 요청을 받는다.   
어떤 쿼리인가 보니 IN절에 상수가 육천개가 사용되니 파싱할 때마다 오래 걸리는 것이다.    

## 원인   

조건이 되는 대상을 정해놓고 쓰는 쿼리였다.
너무 많다 보니 ```컬럼 IN ('조건','조건'...)``` 으로 너무 많이 쓰면 사용 제한의 오류가 발생하기 때문에 

```(1,컬럼) IN ((1,'조건'),(1,'조건') ... )``` 처럼 꼼수를 사용했었다

## 해결

XML형태로 IN조건을 입력받아서 테이블 형태로 가공한다

```SQL
WITH xmldata AS ( 
    SELECT XMLTYPE(    TO_CLOB('<values>') -- 데이터 시작
                    || TO_CLOB('<v>data1</v><v>data2</v>')
                    || TO_CLOB('<v>data3</v><v>data4</v>') 
                    || TO_CLOB('</values>') -- 데이터 종료 
            ) xml_data 
      FROM DUAL
)
SELECT *
  FROM TAB1 A
 WHERE A.COL1 IN (
                 SELECT EXTRACTVALUE(COLUMN_VALUE,'/v')
                   FROM TABLE(
                              XMLSEQUENCE(
                                          EXTRACT(
                                                 (SELECT xml_data 
                                                    FROM xmldata
                                                  )
                                                  ,'/values/v'
                                                 ) --EXTRACT
                                         ) --XMLSEQUENCE
                              ) --TABLE
                 )
```

 변수를 xml_bind_01 와 xml_bind_02 두개를 썼는데 varchar타입의 길이 제한으로 인해   
더 길게 입력하기 위해서 두개를 썼다. 실제론 7개정도 사용  

IN조건으로 쓰일 대상은 ```<values>``` 로 시작해서 ```<v>데이터</v>``` 로 변수 데이터를 채우고   
마지막은 ```</values>```로 닫아주면 된다.   

```xml_bind_01``` 에는 ```<v>data1</v><v>data2</v>```       
```xml_bind_02``` 에는 ```<v>data3</v><v>data4</v>```    

이런식으로 채워서 하나의 문장으로 보고 태그를 잘 닫아주면 된다   
 

물론 변수를 한개만 쓰고 안쓰는건 NULL로 채워도 그만이다.  


변수를 여러개로 쓰고 싶지 않다면   

```ALTER SYSTEM SET max_string_size = EXTENDED SCOPE = SPFILE;```   

의 파라메터를 설정해서 VARCHAR2의 4000바이트 한계를 확장시키는 방법도 있다. 


## 활용

개발자에게 connection pool의 직접적인 접근 권한이 없다는   
답변을 받아서 오라클 Object Type이 아닌 이 XMLType의 사용을 권했다    
상수 부분을 바인드 변수로 변경만 하면 끝   
효과가 좋다고 한다   

