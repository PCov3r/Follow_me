/* Il faut installer la librairie curl sur RPi avec 'sudo apt-get install libcurl4-openssl-dev' */

#include "api.h"

size_t write_callback_func(void *buffer,size_t size,size_t nmemb,void *userp) // Fonction de callback
{
    char **response_ptr =  (char**)userp;

    *response_ptr = strndup(buffer, (size_t)(size *nmemb));
    return ((size_t)(size *nmemb));

}

char* activateOverride() { // Fonction à éxecuter pour passer du mode manuel au mode auto et inversement

	CURL *curl;
  	CURLcode res;
  	char *response = NULL;
	char *url = "localhost:5000/m_override";

  	curl = curl_easy_init();
  	if(curl) {
    	curl_easy_setopt(curl, CURLOPT_URL, url);

    	/* example.com is redirected, so we tell libcurl to follow redirection */
    	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_func);

    	/* passing the pointer to the response as the callback parameter */
    	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);


    	/* Perform the request, res will get the return code */
    	res = curl_easy_perform(curl);
    	/* Check for errors */
    	if(res != CURLE_OK)  fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

    	curl_easy_cleanup(curl);

    }
    return(response);
}


char* sendCmd(int x, int y, int zoom){ // Fonction pour envoyer les positions au serveur

	CURL *curl;
  	CURLcode res;
  	char *response = NULL;
  	char url[105] = "localhost:5000/m_control/?x=";
  	char xpos[5];
  	char ypos[5];
  	char zoom_ctrl[3];

  	sprintf(xpos,"%d",x);
	sprintf(ypos,"%d",y);
	sprintf(zoom_ctrl,"%d",zoom);

  	strcat(url, xpos);
  	strcat(url, "&y=");
  	strcat(url, ypos);
  	strcat(url, "&zoom=");
  	strcat(url, zoom_ctrl);

  	curl = curl_easy_init();
  	if(curl) {
    	curl_easy_setopt(curl, CURLOPT_URL, url);

    	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_func);
    	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    	//Perform the request, res will get the return code 
    	res = curl_easy_perform(curl);

    	//Check for errors
    	if(res != CURLE_OK)  fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

    	curl_easy_cleanup(curl);
    }
	return(response);
}
