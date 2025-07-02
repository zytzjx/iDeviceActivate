#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/HTMLtree.h>
#include <curl/curl.h>

#include <CoreFoundation/CFString.h>
#include <CoreFoundation/CFDictionary.h>
#include <CoreFoundation/CFURL.h>

#include "iTunesMobileDevice.h"

// not support idevice version
#define iVersionNotSupport	100
//need research for new Protocol
#define iErrorProtocol		101
// select carrier to active
#define iErrorSelectCarrier 102
#define iErrorDemoPhone 103
#define iErrorProblem	104
#define iErrorNetWork	105

#define iErrorTimeout  1460

#define iErrorAlreadyActive	1242
#define iErrorUnactived	1243


typedef enum {
	E_SUCCESS = 0,
	E_INCOMPLETE_INFO = -1,
	E_OUT_OF_MEMORY = -2,
	E_UNKNOWN_CONTENT_TYPE = -3,
	E_BUDDYML_PARSING_ERROR = -4,
	E_PLIST_PARSING_ERROR = -5,
	E_HTML_PARSING_ERROR = -6,
	E_UNSUPPORTED_FIELD_TYPE = -7,
	E_INTERNAL_ERROR = -255,
	E_FILE_NOT_FOUND = -1000,
	E_ICLOUD_LOCKED = 90,
	E_SIM_ERROR = 91,
	E_SELECT_CARRIER = 102,
	E_PROTOCOL_ERROR = 101,
	E_DEMO_PHONE = 103,
	E_PROBLEM = 104,
	E_NETWORK_ERROR = 105,
	E_ACTIVATION_ALREADY = 1242,
} error_t;

typedef enum {
	CLIENT_MOBILE_ACTIVATION,
	CLIENT_ITUNES
} client_type_t;

#define IDEVICE_ACTIVATION_USER_AGENT_IOS				"iOS Device Activator (MobileActivation-592.103.2)"
#define IDEVICE_ACTIVATION_USER_AGENT_ITUNES			"iTunes/12.11 (Windows; Microsoft Windows 10 Professional Edition (Build 19041)) AppleWebKit/7610.2011.1003.3 (dt:2)"
#define IDEVICE_ACTIVATION_DEFAULT_URL					"https://albert.apple.com/deviceservices/deviceActivation"
#define IDEVICE_ACTIVATION_DRM_HANDSHAKE_DEFAULT_URL	"https://albert.apple.com/deviceservices/drmHandshake"

typedef enum {
	IDEVICE_ACTIVATION_CONTENT_TYPE_URL_ENCODED,
	IDEVICE_ACTIVATION_CONTENT_TYPE_MULTIPART_FORMDATA,
	IDEVICE_ACTIVATION_CONTENT_TYPE_HTML,
	IDEVICE_ACTIVATION_CONTENT_TYPE_BUDDYML,
	IDEVICE_ACTIVATION_CONTENT_TYPE_PLIST,
	IDEVICE_ACTIVATION_CONTENT_TYPE_UNKNOWN
} content_type_t;

typedef  struct __tagRequest {
	client_type_t client_type;
	content_type_t content_type;
	std::string url;
	CFMutableDictionaryRef fields;
	~__tagRequest() {
		CF_RELEASE_CLEAR(fields);
	}
}REQUEST, *PREQUEST;

typedef struct __tagResponse {
	std::string raw_content;
	size_t raw_content_size;
	content_type_t content_type;
	std::string title;
	std::string description;
	CFDataRef activation_record;
	CFMutableDictionaryRef headers;
	CFMutableDictionaryRef fields;
	CFMutableDictionaryRef fields_require_input;
	CFMutableDictionaryRef fields_secure_input;
	CFMutableDictionaryRef labels;
	CFMutableDictionaryRef labels_placeholder;
	int is_activation_ack;
	int is_auth_required;
	int has_errors;
	void Clean() {
		raw_content.clear();
		title.clear();
		description.clear();
		raw_content_size = 0;

		CF_RELEASE_CLEAR(activation_record);
		CF_RELEASE_CLEAR(headers);
		CF_RELEASE_CLEAR(fields);
		CF_RELEASE_CLEAR(fields_require_input);
		CF_RELEASE_CLEAR(fields_secure_input);
		CF_RELEASE_CLEAR(labels);
		CF_RELEASE_CLEAR(labels_placeholder);
	}
	~__tagResponse() {
		Clean();
	}
}RESPONSE, *PRESPONSE;


class CMobileActivation
{
public:
	CMobileActivation();
	~CMobileActivation();
	int Activate();
	bool Deactivate();
	bool IsActivated(BOOL bForceRead = FALSE);
	void SetDeviceHandle(HANDLE hDevice, const std::string& udid) {
		m_hDevice = hDevice;
		m_udid = udid;
	}
	int StartSession();
	int StopSession();
	int GetVersionActivation(); 
public:
	void set_debug_level(int level);

	error_t request_new();
	error_t request_new_from_lockdownd();
	error_t drm_handshake_request_new();
	void request_free();

	void request_get_fields(CFDictionaryRef *fields);
	void request_set_fields(CFDictionaryRef fields);
	void request_set_fields_from_response();
	void request_set_field(const std::string key, const std::string value);
	void request_get_field(const std::string key, std::string &value);

	void request_get_url(std::string &url);
	void request_set_url(const std::string url);

	error_t response_new();
	error_t response_new_from_html(const std::string content);
	error_t parse_html_response();
	error_t response_to_buffer(char** buffer, size_t* size);
	void response_free();

	void response_get_field(const std::string key, std::string &value);
	void response_get_fields(CFDictionaryRef *fields);
	void response_get_label(const std::string key, std::string &value);
	void response_get_placeholder( const std::string, std::string &value);

	void response_get_title(std::string &title);
	void response_get_description(std::string &description);
	void response_get_activation_record(CFDataRef *activation_record);
	void response_get_headers(CFDictionaryRef *headers);

	int response_is_activation_acknowledged();
	int response_is_authentication_required();
	int response_field_requires_input( const char* key);
	int response_field_secure_input( const char* key);
	int response_has_errors();

	error_t send_request();

	error_t parse_raw_response();


	error_t CreateHtmlResponseFromFile(const std::string& filename, const std::string& content_type = "text/html");
private:
	int debug_level; // 0 = no debug, 1 = basic debug, 2 = verbose debug

	bool is_activated; // Activation status of the device
	std::string activation_status; // Activation status message
	uint32_t product_version = 0; // Product version of the device

	REQUEST request; // Current request being processed
	RESPONSE response; // Response received from the server
	//client_type_t activation_type;

	HANDLE m_hDevice;
	std::string m_udid;

	error_t activation_record_from_plist(CFDictionaryRef plist);
	error_t parse_buddyml_response();
	void response_add_field(const char* key, const char* value, int required_input, int secure_input);
	error_t ErrorPage(const std::string& str);
};

