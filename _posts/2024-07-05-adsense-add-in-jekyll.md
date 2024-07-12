---
title: Jeyll에 구글 애드센스 추가하기
author: me
date: 2024-07-05 00:00:00 +0800
categories: [기타,GitHub블로그]
tags: [애드센스]
---

## 문제   

 GitHub 블로그 설치는 많은 분들이 공개해주셔서 떠듬떠듬 진행해서 성공했는데     
구글 애드센스 광고를 달고 싶다.    
격하게 달고 싶다! 

## 원인   

하지만, 컴맹인 나에겐 너무 어려운 일이다.     
아무리 검색해도 "잘 하시면 된다", "짜짠 추가완료" 라고 설명하고 있어     
분노를 삭히는 그 때에 관련 영문 블로그를 발견했다    
영어는 못하지만, 영어로 찾는 정보가 풍부하기 하다
(요즘 구글에 영어로 검색해도 찰떡 같이    
한국어로 번역해서 한국어 부터 보여주니 더 찾기가 힘들다)   

정리하자면 원인은 이렇다 

* 난 모른다 아무것도
* 잘 알려주지 않은 남 탓이다

~~남탓은 세상을 아름답게 하니까~~   

## 해결

애드센스를 하기 신청하기 전에 해야할 두가지가 있는데    
하나는 ads.txt파일 다른 하나는 스크립트 추가   
~~마지막 하나는 블로그의 컨텐츠가 풍족한가~~   

1. ads.txt   
이건 진짜 root 폴더에 넣으면 되더군요   
처음에 넣고 인식을 안해서 여기가 아닌가벼 하고    
또 열심히 구글링 했지만, 정보가 없어서 한숨 쉬는데 애드센스에서 파일 발견했다고 나옵디다   

2. 스크립트   
```html
<script async src="https://pagead2.googlesyndication.com/pagead/js/adsbygoogle.js?client="crossorigin="anonymous"></script>
```
이런 내 애드센스 아이디가 담긴 스크립트 파일은    

```
includes/advertising.html
```

이라는 파일로 새로 만들어 추가했다.(파일이름 이상하다)   

그리고, 다음 경로의 파일에   
```
_layout\post.html
```

아래의 코드를 추가해야했는데   
```html
{% raw %}
{% include advertising.html %}
{% endraw %}
```

참조한 블로그도 니 맘대로 원하는 위치에 추가하란다.    

'그게 어딘데? 내가 원하는 그곳이 어딘데?'   
분노가 치밀어 오릅니다! 짤이 떠오르는 그 때     
ChatGPT3.0 님께 물어보니   

```html
{% raw %}
<div class="content">{{ content }}</div>
{% include advertising.html %}
{% endraw %}
<div class="post-tail-wrapper text-muted">
```

이 사이에 넣으라신다.    
빙고    
  
찬양해 AI !!!!     
엔비디아 2배 레버리지 NVDL로 가즈아 !   


## 도움받은블로그

[github블로그에 애드센스 추가](https://www.lewisgavin.co.uk/Google-Analytics-Adsense/)







