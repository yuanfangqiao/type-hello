#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#define SERVER_PORT 8001

#define HTTP_RESPONSE_STATUS "HTTP/1.1 200 OK\r\n"

#define HTTP_RESPONSE_HEADER_CONTENT_TYPE "Content-Type: text/html;charset=UTF-8\r\n"
#define HTTP_RESPONSE_HEADER_CONTENT_LENGTH_PARAM "Content-Length: "
#define HTTP_RESPONSE_NEXT_LINE "\r\n"

#define HTTP_RESPONSE_BODY "\
<!DOCTYPE html>\r\n\
<html lang='zh-CN'>\r\n\
\r\n\
<head>\r\n\
    <link rel='icon' href='data:image/svg+xml,<svg xmlns=%22http://www.w3.org/2000/svg%22 viewBox=%220 0 100 100%22><text y=%22.9em%22 font-size=%2290%22>🎛圆方巧</text></svg>'>\r\n\
    <meta charset='utf-8'>\r\n\
    <meta name='viewport' content='width=device-width'>\r\n\
    <title>圆方巧</title>\r\n\
</head>\r\n\
<style type='text/css'>\r\n\
    body {\r\n\
        background-color: #bc2e26\r\n\
    }\r\n\
    .yfq-text-main {\r\n\
        color: #fdd069;\r\n\
    }\r\n\
    \r\n\
    .yfq-text-sub {\r\n\
        color: #f2d486;\r\n\
    }\r\n\
    \r\n\
    #fyq::after {\r\n\
        content: '|';\r\n\
        color: #FF7F32;\r\n\
        animation: blink 1s infinite;\r\n\
    }\r\n\
    \r\n\
    @keyframes blink {\r\n\
        from {\r\n\
            opacity: 0;\r\n\
        }\r\n\
        to {\r\n\
            opacity: 1;\r\n\
        }\r\n\
    }\r\n\
</style>\r\n\
\r\n\
<body>\r\n\
    <div>\r\n\
        <h1 align='center'>\r\n\
           <span class='yfq-text-main' id='fyq'></span> \r\n\
        </h1>\r\n\
        <h4 class='yfq-text-sub' align='center'>对未来有所期待，Looking forward to the future!</h4>\r\n\
    </div>\r\n\
\r\n\
</body>\r\n\
<script type='text/javascript'>\r\n\
    var texts = ['新年快乐！','Happy Chinese New Year!','你好，圆方巧！', 'Hello Yuanfangqiao!']\r\n\
    var text = texts[0]\r\n\
    var arrI = 0\r\n\
    var delay = 100\r\n\
    var i = 0\r\n\
    var back = false\r\n\
\r\n\
    function scrollit() {\r\n\
        let sliceI = (back ? i-- : i++)\r\n\
        fyq.innerText = text.slice(0, sliceI)\r\n\
        if ((i < 0 && back)) {\r\n\
            arrI = (arrI + 1 >= texts.length ? 0 : arrI + 1)\r\n\
            text = texts[arrI];\r\n\
            i = 0\r\n\
            back = false\r\n\
            setTimeout('scrollit()', delay * 30)\r\n\
        } else if (i > text.length && !back) {\r\n\
            back = true\r\n\
            setTimeout('scrollit()', delay * 30)\r\n\
        } else {\r\n\
            setTimeout('scrollit()', delay * (Math.floor(Math.random() * 3) + 1))\r\n\
        }\r\n\
    }\r\n\
    scrollit()\r\n\
</script>\r\n\
"

int main()
{
    struct sockaddr_in srvaddr;
    int locfd;
    int sockopt = 1;
    int res;

    /*创建一个套接字*/
    locfd = socket(AF_INET, SOCK_STREAM, 0);
    if(locfd < 0)
    {
        printf("create socket error!\n");
        return -1;
    }

    printf("socket ready!\n");

    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(SERVER_PORT);
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    setsockopt(locfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(int));

    /*bind, 将网络地址与端口绑定*/
    res = bind(locfd, (struct sockaddr *)&srvaddr, sizeof(srvaddr));
    if(res < 0)
    {
        printf("bind error!\n");
        close(locfd);
        return -1;
    }

    printf("bind ready!\n");

    /*listen, 监听端口*/
    listen(locfd, 10);

    printf("ready to accept client connnect!\n");

    while(1)
    {
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        int clifd;

        clifd = accept(locfd, (struct sockaddr *)&cliaddr, &len);
        if(clifd < 0)
        {
            printf("accept error!\n");
            close(locfd);
            return -1;
        }

        /*输出客户机的信息*/
        char *ip = inet_ntoa(cliaddr.sin_addr);

        printf("client: %s connect success!\n", ip);

        /*输出客户机请求的信息*/
        char buff[1024] = {0};
        int size = read(clifd, buff, sizeof(buff));
        printf("request total size: %d bytes\r\n", size);
        printf("client request information:\r\n");
        printf("%s\r\n", buff);


        write(clifd, HTTP_RESPONSE_STATUS, strlen(HTTP_RESPONSE_STATUS));
        
        write(clifd, HTTP_RESPONSE_HEADER_CONTENT_TYPE, strlen(HTTP_RESPONSE_HEADER_CONTENT_TYPE));
        write(clifd, HTTP_RESPONSE_HEADER_CONTENT_LENGTH_PARAM, strlen(HTTP_RESPONSE_HEADER_CONTENT_LENGTH_PARAM));

        write(clifd, HTTP_RESPONSE_HEADER_CONTENT_LENGTH_PARAM, strlen(HTTP_RESPONSE_HEADER_CONTENT_LENGTH_PARAM));
        int body_size = strlen(HTTP_RESPONSE_BODY);
        char body_str[256] = {0}; 
        sprintf(body_str,"%d",body_size);
        write(clifd, body_str, sizeof(char));
        write(clifd, HTTP_RESPONSE_NEXT_LINE, strlen(HTTP_RESPONSE_NEXT_LINE));
        write(clifd, HTTP_RESPONSE_NEXT_LINE, strlen(HTTP_RESPONSE_NEXT_LINE));

        write(clifd, HTTP_RESPONSE_BODY, strlen(HTTP_RESPONSE_BODY));

        close(clifd);
    }

    close(locfd);

    return 0;
}