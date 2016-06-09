#include <iostream>
#include "net/HttpWebRequest.hpp"


int main(){
    HttpWebRequest http((char *) "http://xxx.xx.xxx.xx/get/whatever.php", 80);
    http.Log(true);
    http.add("Email","I'm a savage");
    http.add("User-Agent","Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2228.0 Safari/537.36");
    if(http.establish()){
        std::cout << http.getResponse() << std::endl;
        http.disconnect();
    }else{
        printf("Well.... that failed.");
    }
    return 0;
}
