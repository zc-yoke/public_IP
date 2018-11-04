#include <iostream>
# include<thread>
#include "curl.h"
//#include <curl/curl.h>
#include <windows.h>
#include <fstream>
#include<string.h>
#include <unistd.h>
#include <time.h>
using namespace std;
char cip[256];//ip.cip.cc
char ipip[256];//myip.ipip.net/ip
char ipinfo[256];//ipinfo.io/ip
char icanhazip[256];//icanhazip.com

static  size_t process_cip(void *buffer, size_t sizeLength, size_t nmemb, void *user_p)
{
    char* temp=(char*)user_p;
    memcpy(temp,buffer,256);

    for(int i=0; i<256; ++i)
    {
        if(temp[i]=='\n')
            temp[i]=0;
    }
    return sizeLength*nmemb;
}
static  size_t process_ipip(void *buffer, size_t sizeLength, size_t nmemb, void *user_p)
{
    char* temp=(char*)user_p;
    memcpy(temp,buffer,256);
    temp=&(temp[7]);
    //get the length
    int length=strlen(temp);
    char tempA[64]= {0};
    memcpy(tempA,temp,length-2);
    strcpy(temp,tempA);

    return sizeLength*nmemb;
}
size_t process_ipinfo(void *buffer, size_t sizeLength, size_t nmemb, void *user_p)
{
    char* temp=(char*)user_p;
    memcpy(temp,buffer,256);

    for(int i=0; i<256; ++i)
    {
        if(temp[i]=='\n')
            temp[i]=0;
    }

    return sizeLength*nmemb;
}
static size_t process_icanhazip(void *buffer, size_t sizeLength, size_t nmemb, void *user_p)
{
    char* temp=(char*)user_p;
    memcpy(temp,buffer,256);

    for(int i=0; i<256; ++i)
    {
        if(temp[i]=='\n')
            temp[i]=0;
    }

    return sizeLength*nmemb;
}


static void perform(CURL * handle_)
{
    curl_easy_perform(handle_);
}



int main(int argc, char **argv)
{
    ofstream qtycount("qtycount.txt");
    ofstream changeIP("changeIPLog.txt");
    long long counter=0;
    // initialize libcurl
    CURLcode return_code;
    return_code = curl_global_init(CURL_GLOBAL_ALL);
    if (CURLE_OK != return_code)
    {
        cerr << "init libcurl failed." << endl;
        return -1;
    }

    //handle ip.cip.cc
    CURL *cip_handle = curl_easy_init();
    if (cip_handle==NULL)
    {
        cerr << "get a easy handle failed." << endl;
        curl_global_cleanup();
        return -1;
    }
    curl_easy_setopt(cip_handle, CURLOPT_URL,"ip.cip.cc");
    curl_easy_setopt(cip_handle, CURLOPT_WRITEFUNCTION, &process_cip);
    curl_easy_setopt(cip_handle, CURLOPT_WRITEDATA, cip);
    curl_easy_setopt(cip_handle, CURLOPT_TIMEOUT, 3);


    //handle myip.ipip.net/ip
    CURL *ipip_handle = curl_easy_init();
    if (ipip_handle==NULL)
    {
        cerr << "get a easy handle failed." << endl;
        curl_global_cleanup();
        return -1;
    }
    curl_easy_setopt(ipip_handle, CURLOPT_URL,"myip.ipip.net/ip");
    curl_easy_setopt(ipip_handle, CURLOPT_WRITEFUNCTION, &process_ipip);
    curl_easy_setopt(ipip_handle, CURLOPT_WRITEDATA, ipip);
    curl_easy_setopt(ipip_handle, CURLOPT_TIMEOUT, 3);


    //handle ipinfo.io/ip
    CURL *ipinfo_handle = curl_easy_init();
    if (ipinfo_handle==NULL)
    {
        cerr << "get a easy handle failed." << endl;
        curl_global_cleanup();
        return -1;
    }
    curl_easy_setopt(ipinfo_handle, CURLOPT_URL,"ipinfo.io/ip");
    curl_easy_setopt(ipinfo_handle, CURLOPT_WRITEFUNCTION, &process_ipinfo);
    curl_easy_setopt(ipinfo_handle, CURLOPT_WRITEDATA, ipinfo);
    curl_easy_setopt(ipinfo_handle, CURLOPT_TIMEOUT, 3);


    //handle icanhazip.com
    CURL *icanhazip_handle = curl_easy_init();
    if (icanhazip_handle==NULL)
    {
        cerr << "get a easy handle failed." << endl;
        curl_global_cleanup();
        return -1;
    }
    curl_easy_setopt(icanhazip_handle, CURLOPT_URL,"icanhazip.com");
    curl_easy_setopt(icanhazip_handle, CURLOPT_WRITEFUNCTION, &process_icanhazip);
    curl_easy_setopt(icanhazip_handle, CURLOPT_WRITEDATA, icanhazip);
    curl_easy_setopt(icanhazip_handle, CURLOPT_TIMEOUT,3);



    //claim a variant to store the result
    char** result=new char*[4];
    for(int i=0; i<4; ++i)
    {
        result[i]=new char[64];
        memset(result[i],0,64);
    }

    int* qty=new int[4];
    qty[0]=1;
    qty[1]=1;
    qty[1]=1;
    qty[1]=1;

    memset(cip,0,256);
    memset(ipip,0,256);
    memset(ipinfo,0,256);
    memset(icanhazip,0,256);

    char oldIp[64]={1};

    while(true)
    {
        //mark a new task begin
        cout<<"=================="<<endl;
        thread t_cip(perform,cip_handle);
        t_cip.detach();

        thread t_ipip(perform,ipip_handle);
        t_ipip.detach();

        thread t_icanhazip(perform,icanhazip_handle);
        t_icanhazip.detach();


        thread t_ipinfo(perform,ipinfo_handle);
        t_ipinfo.detach();


        sleep(5);

        //Find the ip that appears the most
        strcpy(result[0],cip);
        strcpy(result[1],ipip);
        strcpy(result[2],ipinfo);
        strcpy(result[3],icanhazip);


        for(int i=1; i<4; ++i)
        {
            for(int k=0; k<i&&k>=0; --k)
            {
                if(strlen(result[k])==0)
                {
                    qty[k]=0;//to ensure result come out when there only a ip was get from web
                    continue;
                }
                if(strcmp(result[i],result[k])==0)
                {
                    qty[k]=qty[k]+1;
                    memset(result[i],0,64);
                    break;
                }
            }
        }


        //rank the result
        int maxQty=0;
        int flag=0;
        for(int k=0; k<4; ++k)
        {
            if(qty[k]>maxQty)
            {
                maxQty=qty[k];
                flag=k;
                break;
            }
        }
        cout<<"the most is "<<result[flag]<<endl;
        ++counter;

        if(counter%100==0)
        {
            qtycount<<counter<<endl;
        }
        //compare the new ip between the old ip,if the ip has changed,write the log into file
        if(strcmp(oldIp,result[flag])!=0)
        {
            time_t t = time(0);
            char tmp[64];
            strftime( tmp, sizeof(tmp), "%Y/%m/%d %X",localtime(&t) );
            changeIP<<"ip changed at "<<tmp<<" from "<<oldIp<<" to "<<result[flag]<<endl;
            strcpy(oldIp,result[flag]);
        }

        //reset the data,ready to new loop
        for(int i=0; i<4; ++i)
        {
            qty[i]=1;
            memset(result[i],0,64);
        }

        memset(cip,0,256);
        memset(ipip,0,256);
        memset(ipinfo,0,256);
        memset(icanhazip,0,256);
    }
    curl_easy_cleanup(cip_handle);
    curl_global_cleanup();
    return 0;
}
