---
title: Lateral 조인을 통한 개선 사례 두번째
author: me
date: 2024-07-09 00:00:00 +0800
categories: [Oracle,SQL개선]
tags: [뻘짓]
---

## 문제   

문제 없는데 ... 아냐 좀 느렸어 비효율이였어

## 원인   

첫번째 사례와는 다르다 첫번째 사례와는!    

```SQL 
 SELECT 결과 
   FROM 파티션테이블 A 
      , 일반테이블 B    
  WHERE A.ID = B.ID 
    AND A.일자 BETWEEN :날짜시작 AND :날짜끝
```

파티션테이블 A의 인덱스는 (일자,ID,기타등등 겁나길어) 컬럼으로 구성된 PK인덱스 한개 뿐이다   
그래서 HASH JOIN되고 있는 상황     

하지만, 실제 데이터 구성을 보면 USE_NL로 적용되는게 훨씬 탐색량이 적은 상황    
그럼 힌트를 넣으면 될까?    


## 해결

~~아니다, 이 쿼리는 박제되어 Copy&Paste 될 운명이다~~   
힌트로 강제하기 보다는 자연스럽게 쿼리로 옵티마이저의 해석을 유도하기 위해   
쿼리를 변경했다    
  
```SQL
 SELECT 결과 
   FROM 파티션테이블 A 
      , 일반테이블 B 
      , LATERAL
         (
           SELECT :날짜시작 + (LEVEL-1) AS 일자
             FROM DUAL
            WHERE B.ID IS NOT NULL 
           CONNECT BY LEVEL <= :날짜끝 - :날짜시작 + 1
         ) S
  WHERE A.ID = B.ID 
    AND A.일자 = S.일자 
    AND A.일자 BETWEEN :날짜시작 AND :날짜끝
```

로 작성하자 예상대로 일반테이블 B는 더미 일자테이블 S 와 머지조인카테시안을 먼저하고    
파티션테이블과 NL조인했다     

LATERAL을 하지 않고 그냥 머지 조인할 땐 조인 순서가 뒤바뀌어서    
영 좋지 못한 자세가 나왔었다    
~~(하지만, 뻘짓 같다)~~


