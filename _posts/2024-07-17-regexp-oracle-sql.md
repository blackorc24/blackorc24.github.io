---
title: 오라클 정규표현식으로 쿼리문장 편집
author: me
date: 2024-07-17 00:00:00 +0800
categories: [Oracle,SQL활용]
tags: [정규표현식]
---

## 문제   

데이터 이관을 하다보면 여러 테이블 혹은 거기에서 파생되는 SQL을 일괄적으로 편집할 필요가 있다    
이번엔 로컬에서 테스트한 데이터 이행 쿼리에 DBLINK를 붙여야 했던 과거 기록 

## 원인   

늘 계획은 바뀌는 법이니까

## 해결

```SQL
WITH T AS
(
SELECT '(SELECT A.NAME , B.COMP FROM TXT_FFT.TABLE_A A, FAT.TABLE_B B WHERE A.KEY = B.KEY)' TXT FROM DUAL 
-- 변경이 필요한 대상 쿼리, 예제 용으로 이리 적었지만, 이미 많이 작성된 쿼리에서 테이블명에 DBLINK명을 붙이는 작업이다
)
SELECT REGEXP_REPLACE(TXT,'(TXT_\S*\.)([^ ]+)','\1\2@DBLINK')   /*
FROM T
```

* (TXT_\S*\.) : TXT_로 시작하고 \S* 아무 대문자 단어를 만나서 . 으로 끝나는 문자열을 버퍼 1에 저장한다
* ([^ ]+) : 공백을 제외한 모든 문자를 버퍼 2에 저장한다 
* Replace 할 대상문자 전체를 '\1\2@DBLINK' :  버퍼1 을 붙여넣고 버퍼2를 붙여넣고 @DBLINK 를 추가하여 대체한다


이렇게 하면 특정 문자열 패턴만 원하는데로 변경이 가능하다   

