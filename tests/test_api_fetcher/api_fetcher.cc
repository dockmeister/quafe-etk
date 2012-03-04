/* Eve API Fetcher test */

#include <glibmm-2.4/glibmm.h>
#include <curl/curl.h>
#include <sstream>
#include <iostream>

using namespace Glib;

//enum EAPI_
struct EAPI_Key {
	ustring keyID;
	ustring vCode;

	std::stringstream so;

	const char* make_postfields() {
		ustring p = "keyID=" + keyID + "&vCode=" + vCode;
		return p.c_str();
	}

};
static size_t write_data(void *contents, size_t size, size_t nmemb, void *eapi_) {
	EAPI_Key *m_key = static_cast<EAPI_Key *>(eapi_);

	ustring buf = ustring(static_cast<char *>(contents), size * nmemb);

	m_key->so.write(buf.c_str(), (std::streamsize) buf.size());

	return size * nmemb;
}

int main(int argc, char **argv) {
	EAPI_Key m_key = { "0", "0" };

	CURL *curl;
	CURLcode res;
	std::cout << m_key.make_postfields() << std::endl;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		/* First set the URL that is about to receive our POST. This URL can
		 just as well be a https:// URL if that is what should receive the
		 data. */
		curl_easy_setopt(curl, CURLOPT_URL, "http://api.eveonline.com/account/APIKeyInfo.xml.aspx");
		/* Now specify the POST data */
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "keyID=0&vCode=0");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &m_key);

		char ebuf[CURL_ERROR_SIZE];
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, ebuf);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);

		/* always cleanup */
		curl_easy_cleanup(curl);

		std::cout << "contents" << m_key.so.str() << std::endl;
	}
	return 0;
}

