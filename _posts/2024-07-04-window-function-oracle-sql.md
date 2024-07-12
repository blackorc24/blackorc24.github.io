---
title: 분석함수(Window Fuction)을 사용한 개선 사례
author: me
date: 2024-07-04 00:00:00 +0800
categories: [Oracle,SQL개선]
tags: [집계함수개선]
---

## 문제   

아무튼 느림

## 원인   

 인덱스가 없는데 스칼라 서브쿼리를 사용해서 분석데이터를 출력하려고 했으나     
인덱스 탐색 범위가 너무 넓어서 인덱스 사용이 오래걸렸다

문제가 된 소스 쿼리를 재연한다면 이렇다     

```SQL
SELECT A.회사코드컬럼
     , NVL( CASE WHEN A.비교일자컬럼 IS NOT NULL 
                 THEN (SELECT MAX(NVL(적용등급컬럼,'0000'))
                         FROM 등급테이블   --  인덱스는 PK인덱스(일자컬럼, 구분컬럼, 회사코드컬럼) 한개로 구성되어있다
                        WHERE 일자컬럼   = :날짜바인드 
                          AND 평가일컬럼  = A.비교일자컬럼   -- 구분컬럼은 없기 때문에 인덱스 탐색 범위가 넓어지며 인덱스컬럼이 아닌 평가일컬럼까지 조인 조건이라 느려짐
                          AND 회사코드컬럼 = A.회사코드컬럼  
                       )
                  ELSE ( SELECT MAX(NVL(적용등급컬럼,'0000'))
                           FROM 등급테이블
                          WHERE 일자컬럼  = :날짜바인드
                            AND 회사코드컬럼 = A.회사코드컬럼 
                        )

            END,  '0000' ) AS 다른등급컬럼      
   FROM ( 
         SELECT 회사코드컬럼
              , CASE WHEN 비교일자1 IS NULL     AND 비교일자2 IS NULL      THEN NULL 
                     WHEN 비교일자1 IS NOT NULL AND 비교일자2 IS NULL      THEN 비교일자1
                     WHEN 비교일자1 IS NULL     AND 비교일자2 IS NOT NULL  THEN 비교일자2
                     WHEN 비교일자1  >   비교일자2                         THEN 비교일자1
                     WHEN 비교일자1  <   비교일자2                         THEN 비교일자2
                     WHEN 비교일자1  =   비교일자2                         THEN NULL
                 END AS 비교일자컬럼      
          FROM (
                SELECT 회사코드컬럼
                     , MAX(비교일자1) AS 비교일자1
                     , MAX(비교일자2) AS 비교일자2
                  FROM (
                        SELECT 회사코드컬럼
                             , 평가일컬럼 AS 비교일자1
                             , NULL    AS 비교일자2
                          FROM 등급테이블 
                         WHERE 일자컬럼 = :날짜바인드
                           AND 구분컬럼 = '1'
                         UNION ALL   -- 이 부분도 UNION ALL 하지 않고 컬럼에서 DECODE를 사용해서 비교일자1,비교일자2를 사용해도 충분했다
                        SELECT 회사코드컬럼
                             , NULL    AS 비교일자1
                             , 평가일컬럼 AS 비교일자2
                          FROM 등급테이블
                         WHERE 일자컬럼 = :날짜바인드
                           AND 구분컬럼 = '2'  
                  )
          GROUP BY 회사코드컬럼
          )
       ) A
```


## 해결

 선행 테이블과 후행테이블 데이터가 겹치는 범위를 생각해서 미리 몽땅 뽑아낸 다음에    
가공하고 필요한 데이터만 추려내는것이 관건이다    

```SQL
 SELECT A.회사코드컬럼 
      , NVL(평가일컬럼적용등급컬러,'0000') AS 다른등급컬럼  -- 필요없지만, 있어야 여러사람이 안심한다
  FROM (
        SELECT A.*
             , MAX(적용등급컬럼) OVER(PARTITION BY 일자컬럼,회사코드컬럼,비교일자컬럼) AS 평가일컬럼적용등급컬럼 
             , ROW_NUMBER() OVER(PARTITION BY 일자컬럼,회사코드컬럼 
                                     ORDER BY 비교일자컬럼 DESC NULLS LAST,적용등급컬럼 DESC NULLS LAST,구분컬럼) AS RN
          FROM (
                SELECT A.*
                     , CASE WHEN 비교일자1 IS NULL     AND 비교일자2 IS NULL      THEN NULL 
                            WHEN 비교일자1 IS NOT NULL AND 비교일자2 IS NULL      THEN 비교일자1
                            WHEN 비교일자1 IS NULL     AND 비교일자2 IS NOT NULL  THEN 비교일자2
                            WHEN 비교일자1  >   비교일자2                         THEN 비교일자1
                            WHEN 비교일자1  <   비교일자2                         THEN 비교일자2
                            WHEN 비교일자1  =   비교일자2                         THEN NULL
                        END AS 비교일자컬럼     
                  FROM (   /* 여기서 이미 기초 데이터와 서브스칼라쿼리에서 필요한 데이터를 모두 추출한다 */
                         SELECT 일자컬럼, 회사코드컬럼, 구분컬럼,평가일컬럼,적용등급컬럼
                              , MAX((CASE WHEN 구분컬럼 = 1 THEN 평가일컬럼 END))
                                    OVER(PARTITION BY 일자컬럼,회사코드컬럼) AS 비교일자1
                              , MAX((CASE WHEN 구분컬럼 = 2 THEN 평가일컬럼 END))
                                    OVER(PARTITION BY 일자컬럼,회사코드컬럼) AS 비교일자2
                           FROM 등급테이블 A
                          WHERE 일자컬럼 = :날짜바인드
                  ) A
          ) A
         WHERE 평가일컬럼 = 비교일자컬럼 OR 비교일자컬럼 IS NULL  -- 비교일자컬럼과 평가일컬럼이 동일한 건과 비교일자 컬럼이 없는건만 추려낸다
        ) A
WHERE RN = 1  /* 비교일자컬럼이 없는건과 평가일컬럼과 비교일자컬럼이 동일한건은 적용등급컬럼이 가장 큰 하나만 남기고 제거 */

```

더 상세한 주석을 남기고 싶었으나, 여백이 부족하여 적지 않는다.





