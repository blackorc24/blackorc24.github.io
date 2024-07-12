---
title: Lateral 조인을 통한 개선 사례
author: me
date: 2024-07-03 00:00:00 +0800
categories: [Oracle,SQL개선]
tags: [뻘짓]
---

## 문제   
 3개의 테이블이 JOIN되는중 문제가 발생했다.    
* 테이블 A는 선행테이블이 되는 테이블로 ID정보를 가지고 있다. 바인드 변수로 대상 로우수가 조정된다    

* 테이블 B는 코드성 테이블로 코드명을 조회할 때 쓰는데, C테이블에 더미 데이터가 있어 이너조인 조건으로도 쓰인다.   
 전체건수는 3000정도   
* 테이블 C는 파티션 테이블이다. 크다   

테이블 A는 C와 이너조인하고   
테이블 B도 C와 이너조인한다   
테이블 A는 바인드 변수로 대상을 선별하고   
C는 파티션 키인 일자의 범위를 바인드 변수로 사용하며 조회기간이 1년으로 무척 길어   
A와 조인되면 데이터가 많이 추출된다   

하지만 왜인지 바인드변수로 대상을 1개만 받았을땐 빨랐는데   
100개,1000개로 늘어나자 끝이 나질 않는다   


## 원인   

 이 쿼리는 A와B의 전체 데이터 수가 적어 옵티마이저가 우선 "Merge Join Cartesian"의 실행계획을 적용하여   
3000건 정도의 로우를 만들어낸뒤    
C와 NL조인하여 빠르게 처리했다.   

하지만, A의 바인드변수가 증가하여 100건이 넘자 B와의 머지 조인으로 인해 300,000로우가 만들어졌고    
이를 다시 C와 NL조인하는것은 치명적이였던것   

ANSI조인을 좋아하지만 현장에 작성된 쿼리들이 옛날것이라 오라클 문법을 사용하고 있고   
다시 그것을 그대로 Copy&Paste 해서 쓰기 때문에 오라클 문법으로 표기함   

```SQL
 SELECT a.id , a.col1, c.col1,c.col2, b.코드명
  FROM tab1 a, tab2 b, tab3 c
 WHERE a.id = c.id 
   AND c.코드id = b.코드id
   AND c.일자 between 일년
   AND a.id IN ( SELECT id   -- 테스트용 바인드 변수 대체 데이터 
                   FROM 어딘가의테이블
                  ORDER BY DBMS_RANDOM.RANDOM 
                  FETCH FIRST 110 ROWS ONLY
               )
```

## 해결

 A 와 C를 먼저 NL조인하고 B를 나중에 조인하면 그만일 것인데   
단서가 하나 있었다, B는 코드성 테이블이다.   
 그냥 조인하기엔 억울했다. 그렇다고 서브스칼라 쿼리로 작성한뒤 코드명이 없는걸 다시 제거하기에는   
불필요한 결과도 생성을 해야하는 행위가 못 마땅하지 않은가?   

```SQL
 SELECT a.id, a.col1, c.col1,c.col2, b.코드명
   FROM tab1 a
      , tab3 c
      , LATERAL( 
           SELECT /*+ RESULT_CACHE */ b.코드명 
             FROM tab2 b
           WHERE c.코드id = b.코드id 
         ) b
 WHERE a.id = c.id 
   AND c.일자 between 일년   
```

 이렇게 바꿔주었고 이것은 그냥 원본쿼리에 ```/*+ LEADING(A) USE_HASH(B) */``` 힌트를 사용한것과    
실행계획은 동일해 보였지만 실제는 더 빨리 수행됐다.   
그리고, ```/*+ RESULT_CACHE */``` 는 동작하지 않았다.   

v$result_cache_objects 를 조회했으나 역시나 없었다.   

 아무래도 조인 조건으로 결과가 생성되는 쿼리이기 때문에 적용되지 않았으리라   
스스로 빠르게 마무지 지었다   

## 참조 

[**오라클 Result Cache**](https://docs.oracle.com/en/database/oracle/oracle-database/19/tgdba/tuning-result-cache.html#GUID-60F72D26-3DD0-4DBB-9DCE-3A7634559A3C):

## 사실

Lateral Join의 특성을 활용한 쿼리가 아니라서 의미는 없는듯
인라인뷰에 a.col3 = 'TA' 같은 조건이라도 넣었으면 말이 되는데요... 

