//
// Created by cj on 6/5/16.
//

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include <malloc.h>
#include "../utils/HeaderMap.hpp"
#include "../utils/Vector.hpp"

class HttpWebRequest {
    HeaderMap *headers;
    int sock, port;
    struct sockaddr_in sockaddress;
    char *ip, *get, *header, *rawResponse, *response;
    bool log = false, established = false;
public:

    ~HttpWebRequest() {
        disconnect();
    }

    /**
     * Construct HttpWebRequest & Splits the ip and get for later use
     */
    HttpWebRequest(char *ip, int port) {
        this->port = port;
        this->ip = new char[strlen(ip)], this->get = new char[strlen(ip)];
        grab(ip);
        headers = new HeaderMap();
        create();
    }

    /**
     * Add Headers to header map
     */
    void add(const char *key, const char *value) {
        add((char *) key, (char *) value);
    }

    /**
     * Disconnect Socket and Clear Buffers
     */
    void disconnect() {
        close(sock);
        port = 0;
        sock = 0;
        memset(ip,0,strlen(ip));
        memset(get,0, strlen(get));
        memset(header,0, strlen(header));
        delete[] ip;
        delete[] get;
        delete[] header;
        ip = nullptr;
        get = nullptr;
        header = nullptr;
        if(established) {
            memset(rawResponse,0,strlen(rawResponse));
            delete[] rawResponse;
            rawResponse = nullptr;
        }
    }

    /**
     * Header Map - Adds The Header to the HeaderMap to then be used.
     */
    void add(char *key, char *value) {
        headers->addHeader(key, value);
    }

    /**
     * Generates the header. Establishes Socket Connection Then Sends The Header To The Destination
     */
    bool establish() {
        generateHeader();
        if(log) std::cout << "Request Headers: \n" << header << std::endl << std::endl;
        if (connect(sock, (const struct sockaddr *) &sockaddress, sizeof(sockaddress)) < 0) {
            if(log) std::cout << "Unable to establish connection to: " << this->ip << std::endl;
            return false;
        } else {
            return sendMsg(header);
        }
    }

    char *getRawResponse() {
        return rawResponse;
    }

    char *getResponse() {
        return response;
    }

    /**
     * Allow Logging
     */
    void Log(bool log) {
        this->log = log;
    }

private:

    /**
     * Cleans Header (Sloppy)
     */
    void clean() {
        response = strstr(rawResponse, "\r\n\r\n");
    }

    /**
     * Reads Buffer and Auto Removes Header
     */
    bool getStr() {
        rawResponse = new char[0];
        char buffer[4];
        ssize_t rec = 0;
        while ((rec = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
            strcat(rawResponse, buffer);
        }
        clean();
    }

    /**
     * Creates Socket
     */
    bool create() {
        sockaddress.sin_family = AF_INET;
        sockaddress.sin_port = htons(port);
        sockaddress.sin_addr.s_addr = inet_addr(ip);
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }

    /**
     * Send Header to Socket
     */
    bool sendMsg(char *bytes) {
        if (send(sock, bytes, strlen(bytes), 0) < 0) {
            return false;
        } else {
            bool didSuceed = getStr();
            if(didSuceed)
                established = true;
            return false;
        }
    }

    /**
     * Generates the Header by iterating through HeaderMap
     */
    char *generateHeader() {
        const char *headerx = "GET %s HTTP/1.1\r\nHost: %s\r\n";
        char header_temp[512];
        memset(header_temp, 0, 512);
        snprintf(header_temp, sizeof(header_temp), headerx, this->get, this->ip);
        if (headers->getSize() > 0) {
            for (int i = 0; i < (*headers).getSize(); i++) {
                strcat(header_temp, (*headers)[i]->getKey());
                strcat(header_temp, ": ");
                strcat(header_temp, (*headers)[i]->getValue());
                strcat(header_temp, i != (*headers).getSize() - 1 ? "\r\n" : "\r\n\r\n");
            }
        }
        header = new char[strlen(header_temp) + 1];
        for (int i = 0; i < strlen(header_temp) + 1; i++) {
            if (i != strlen(header_temp)) {
                *(header + i) = header_temp[i];
            } else {
                *(header + i) = '\0';
            }
        }
    }

    /**
     * Compare if equal to
     */
    bool equals(char *t1, const char *t2) {
        return strncmp(t1, t2, strlen(t2)) == 0;
    }

    /**
     * Stop at Delim, Was having issue with garbage characters
     */
    int stopAt(char* ok, char delim){
        for(int i = 0;i < strlen(ok); i++){
            if(*(ok+i) == delim) return i;
        }
        return (int) strlen(ok);
    }

    /**
     * Splits the IP and the GET Request Apart
     */
    void grab(char *ip) {
        memset(this->ip, 0, strlen(ip));
        memset(this->get, 0, strlen(ip));
        char *save;
        char content[strlen(ip)];
        for (int i = 0; i < strlen(ip); i++) {
            content[i] = *(ip + i);
        }
        int occur = 0;
        for (char *p = strtok_r(content, "//", &save); p != NULL; p = strtok_r(NULL, "//", &save)) {
            if (!equals(p, "http:") && !equals(p, "https:")) {
                if (occur == 0) {
                    for (int i = 0; i < strlen(p) + 1; i++) {
                        if (i != strlen(p)) {
                            this->ip[i] = *(p + i);
                        } else {
                            this->ip[i] = '\0';
                        }
                    }
                    occur++;
                } else if (occur > 0) {
                    int size = (int) strlen(this->get);
                    for (int i = 0; i < strlen(p) + 2; i++) {
                        if (i == 0) {
                            this->get[size + i] = '/';
                        }else if (i-1 != strlen(p)) {
                            this->get[size + i] = *(p + i-1);
                        } else {
                            this->get[size + i] = '\0';
                        }
                    }
                }
            }
        }
        if(strlen(this->get) == 0){
            this->get[0] = '/';
        }
    }

};



