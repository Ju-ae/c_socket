# multi_echo_server

## 멀티 에코 서버 순서
bind
listen
accept
recv/send
close

## 부모 프로세스와 자식 프로세스의 차이
<li> 부모 프로세스 : listen소켓(server소켓)을 가지고 연결 요청을 받아들인다.
<li> 자식 프로세스 : connect소켓(client소켓)을 가지고 데이터를 보내고 받는다. while문 안에서 처리한다.
