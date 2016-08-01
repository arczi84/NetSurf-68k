#include <stdio.h>
#include <curl/curl.h>
/* For older cURL versions you will also need 
#include <curl/types.h>
#include <curl/easy.h>
*/

#include <string.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <exec/memory.h>
#include "amigaos3/misc.h"

const char build_date[] =  __DATE__ ;

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int check_version(void) {
    CURL *curl;
    FILE *fp;
	BPTR fh;
    CURLcode res;
	char *ver;
    char *url = "https://dl.dropboxusercontent.com/s/4e1i4da2vw9drk9/version.txt?dl=1";
    char outfilename[FILENAME_MAX] = "RAM:version";
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(outfilename,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
		/*Read RAm:version*/
		ver = AllocVec(10,MEMF_ANY);
		fh = Open("RAM:version",MODE_OLDFILE);
		Read(fh, ver, 10);

	//	resint = atoi(result);
				
		Printf("\n*");
		Printf(build_date);
		Printf("*\n");

        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
		Close(fh);
		FreeVec(ver);		
    }
    return 0;
}
