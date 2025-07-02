#include "stdafx.h"
#include "MobileActivation.h"
#include "iDeviceUtil.h"
#include "iTunesMobileDevice.h"
#include <regex>  

extern _iTunesMobileDevice_t g_iTunesMobileDevice;

CMobileActivation::CMobileActivation() :m_hDevice(nullptr) {
	curl_global_init(CURL_GLOBAL_ALL);
}

CMobileActivation::~CMobileActivation() {
	curl_global_cleanup();
}

int CMobileActivation::StartSession() {
	ENTER_FUNC_BEGIN;
	int err = ERROR_SUCCESS;
	err = g_iTunesMobileDevice.AMDeviceConnect(m_hDevice);
	if (err == ERROR_SUCCESS)
	{
		if (g_iTunesMobileDevice.AMDeviceIsPaired(m_hDevice))
		{
			logIt("Connect AMDeviceIsPaired\n");
			// paired on this pc
			err = g_iTunesMobileDevice.AMDeviceValidatePairing(m_hDevice);
			if (err != ERROR_SUCCESS)
			{
				err = g_iTunesMobileDevice.AMDevicePair(m_hDevice);
			}
		}
		else
		{
			// not paired on this pc
			err = g_iTunesMobileDevice.AMDevicePair(m_hDevice);
		}
	}
	else
	{
		logIt("Connect Error:%d\n", err);
	}
	err = g_iTunesMobileDevice.AMDeviceStartSession(m_hDevice);

	ENTER_FUNC_ENDRET(err);
	return err;
}

int CMobileActivation::StopSession() {
	ENTER_FUNC_BEGIN;
	int err;
	try
	{
		err = g_iTunesMobileDevice.AMDeviceStopSession(m_hDevice);
		err = g_iTunesMobileDevice.AMDeviceDisconnect(m_hDevice);
	}
	catch (...)
	{
	}
	ENTER_FUNC_END;
	return err;
}

int CMobileActivation::GetVersionActivation() {
	ENTER_FUNC_BEGIN;
	int err = ERROR_SUCCESS;
	CFTypeRef version = g_iTunesMobileDevice.AMDeviceCopyValue(m_hDevice, nullptr, CFSTR("ProductVersion"));
	if (version && CFGetTypeID(version) == CFStringGetTypeID()) {
		CFStringRef versionStr = (CFStringRef)version;
		char versionBuffer[64];
		int vers[3] = { 0, 0, 0 };
		CFStringGetCString(versionStr, versionBuffer, sizeof(versionBuffer), kCFStringEncodingUTF8);
		logIt("Activation Version: %s\n", versionBuffer);
		if (strlen(versionBuffer) > 0 && sscanf_s(versionBuffer, "%d.%d.%d", &vers[0], &vers[1], &vers[2]) >= 2) {
			product_version = ((vers[0] & 0xFF) << 16) | ((vers[1] & 0xFF) << 8) | (vers[2] & 0xFF);
		}
		CF_RELEASE_CLEAR(version);
	}
	else {
		logIt("Failed to get activation version\n");
		err = E_INCOMPLETE_INFO;
	}

	CFTypeRef activatestate = g_iTunesMobileDevice.AMDeviceCopyValue(m_hDevice, nullptr, CFSTR("ActivationState"));
	if (activatestate && CFGetTypeID(activatestate) == CFStringGetTypeID()) {
		CFStringRef activatestatestr = (CFStringRef)activatestate;
		char activatestatebuf[64];
		CFStringGetCString(activatestatestr, activatestatebuf, sizeof(activatestatebuf), kCFStringEncodingUTF8);
		logIt("Activation status: %s\n", activatestatebuf);
		if (strlen(activatestatebuf) > 0) {
			activation_status = activatestatebuf;
		}
		CF_RELEASE_CLEAR(activatestate);
	}
	else {
		logIt("Failed to get activation state\n");
		err = E_INCOMPLETE_INFO;
	}

	ENTER_FUNC_ENDRET(err);
	return err;
}


CFPropertyListRef recordToPropertyList(CFDataRef record) {
	if (!record) {
		return nullptr;
	}

	CFErrorRef error = nullptr;
	CFPropertyListRef propertyList = CFPropertyListCreateWithData(
		CFAllocatorGetDefault(),
		record,
		kCFPropertyListMutableContainersAndLeaves,
		nullptr,
		&error
	);

	if (error) {
		CF_RELEASE_CLEAR(error);
		return nullptr;
	}

	return propertyList;
}

int CMobileActivation::Activate() {
	ENTER_FUNC_BEGIN;
	int err = ERROR_SUCCESS;
	GetVersionActivation();
	int session_mode = 0;
	CFTypeRef ainfo = g_iTunesMobileDevice.AMDeviceCreateActivationInfo(m_hDevice, nullptr, &err);
	if (product_version >= 0x0A0000 || err != ERROR_SUCCESS) {
		logIt("Activation not supported for iOS 10 and later\n");
		session_mode = 1;
	}
	if (session_mode) {
		CFTypeRef sessioninfo = g_iTunesMobileDevice.AMDeviceCreateActivationSessionInfo(m_hDevice, &err);
		StopSession();
		if (err != ERROR_SUCCESS) {
			logIt("Failed to create activation session info: %d\n", err);
			return false;
		}
		if (debug_level > 0)
			CFShow(sessioninfo); // debug output

		drm_handshake_request_new();
		request_set_fields((CFDictionaryRef)sessioninfo);
		CF_RELEASE_CLEAR(sessioninfo);

		if (debug_level > 0) {
			CFShow(request.fields); // debug output
		}
		send_request();
		request_new();

		CFDictionaryRef handshake_response = nullptr;
		response_get_fields(&handshake_response);
		if (debug_level > 0) {
			CFShow(handshake_response); // debug output            
		}
		StartSession();
		CFDataRef handshakeResponseData = nullptr;
		if (handshake_response && CFGetTypeID(handshake_response) == CFDictionaryGetTypeID()) {
			handshakeResponseData = CFPropertyListCreateData(
				CFAllocatorGetDefault(),
				handshake_response,
				kCFPropertyListXMLFormat_v1_0,
				0,
				nullptr
			);
			if (handshakeResponseData) {
				logIt("Converted handshake_response to CFDataRef successfully.\n");
			}
			else {
				logIt("Failed to convert handshake_response to CFDataRef.\n");
			}
		}
		else {
			logIt("handshake_response is not a valid CFDictionary.\n");
		}

		CFTypeRef plCAI = g_iTunesMobileDevice.AMDeviceCreateActivationInfo(m_hDevice, handshakeResponseData, &err);
		StopSession();
		if (err != ERROR_SUCCESS) {
			logIt("Failed to create activation info: %d\n", err);
			goto cleanup;
		}
		if (debug_level > 0)
			CFShow(plCAI); // debug output
		if (plCAI && CFGetTypeID(plCAI) == CFDictionaryGetTypeID()) {
			CFMutableDictionaryRef fields = CFDictionaryCreateMutable(CFAllocatorGetDefault(), 0,
				&g_iTunesMobileDevice.my_kCFTypeDictionaryKeyCallBacks, &g_iTunesMobileDevice.my_kCFTypeDictionaryValueCallBacks);
			CFDictionarySetValue(fields, CFSTR("activation-info"), plCAI);
			request_set_fields(fields);
		}
		std::string response_title;
		std::string response_description;

		while (TRUE) {
			if (send_request() != E_SUCCESS) {
				fprintf(stderr, "Failed to send request or retrieve response.\n");
				// Here response might have some content that could't be correctly interpreted (parsed)
				// by the library. Printing out the content could help to identify the cause of the error.
				err = EXIT_FAILURE;
				goto cleanup;
			}

			if (response_has_errors()) {
				fprintf(stderr, "Activation server reports errors.\n");

				response_get_title(response_title);
				if (!response_title.empty()) {
					fprintf(stderr, "\t%s\n", response_title.c_str());
				}

				response_get_description(response_description);
				if (!response_description.empty()) {
					fprintf(stderr, "\t%s\n", response_description.c_str());
				}
				err = EXIT_FAILURE;
				goto cleanup;

			}

			CFDataRef record = nullptr;
			response_get_activation_record(&record);
			if (debug_level > 0 && record) {
				logIt("record:\n");
				CFShow(record); // debug output
			}
			if (record) {
				if (!g_args.bFMIChecked) {
					err = ERROR_SUCCESS;
					break;
				}
				StartSession();
				if (session_mode) {
					CFDictionaryRef headers = nullptr;
					response_get_headers(&headers);

					if (debug_level > 0 && headers) {
						logIt("Headers:\n");
						CFShow(headers); // debug output
					}

					if (E_SUCCESS != (err = g_iTunesMobileDevice.AMDeviceActivateWithOptions(m_hDevice, record, headers))) {
						CF_RELEASE_CLEAR(headers);
						fprintf(stderr, "Failed to activate device with record.\n");
						err = EXIT_FAILURE;
						goto cleanup;
					}
					
					CF_RELEASE_CLEAR(headers);
				}
				else {
					if (E_SUCCESS != (err = g_iTunesMobileDevice.AMDeviceActivate(m_hDevice, (CFMutableDictionaryRef)record))) {
						fprintf(stderr, "Failed to activate device with record.\n");
						err = EXIT_FAILURE;
						goto cleanup;
					}
				}

				// set ActivationStateAcknowledged if we succeeded
				if (E_SUCCESS != (err = g_iTunesMobileDevice.AMDeviceSetValue(m_hDevice, NULL, CFSTR("ActivationStateAcknowledged"), g_iTunesMobileDevice.my_kCFBooleanTrue))) {
					fprintf(stderr, "Failed to set ActivationStateAcknowledged on device.\n");
					err = EXIT_FAILURE;
					goto cleanup;
				}
				break;
			}
			else {
				if (response_is_activation_acknowledged()) {
					printf("Activation server reports that device is already activated.\n");
					err = EXIT_SUCCESS;
					goto cleanup;
				}

				response_get_title(response_title);
				if (!response_title.empty()) {
					fprintf(stderr, "Server reports:\n%s\n", response_title.c_str());
				}

				response_get_description(response_description);
				if (!response_description.empty()) {
					fprintf(stderr, "Server reports:\n%s\n", response_description.c_str());
				}
				CFDictionaryRef fields = nullptr;
				response_get_fields(&fields);


				request_free();
				if (request_new() != E_SUCCESS) {
					fprintf(stderr, "Could not create new request.\n");
					err = EXIT_FAILURE;
					goto cleanup;
				}

				request_set_fields_from_response();
				response_free();
			}

		}
	}

cleanup:
	ENTER_FUNC_ENDRET(err);
	return err;
}

bool CMobileActivation::Deactivate() {
	g_iTunesMobileDevice.AMDeviceDeactivate(m_hDevice);
	return true;
}

bool CMobileActivation::IsActivated(BOOL bForceRead) {
	if (bForceRead || activation_status.empty()) {
		int err = GetVersionActivation();
		if (err != ERROR_SUCCESS) {
			logIt("Failed to get activation version: %d\n", err);
			return false;
		}
	}
	/*
	* _CFStringMakeConstantString("ActivationState");
_CFStringMakeConstantString("Unactivated");
_CFStringMakeConstantString("Activated");
_CFStringMakeConstantString("FactoryActivated");
_CFStringMakeConstantString("SoftActivation");
_CFStringMakeConstantString("MismatchedIMEI");
_CFStringMakeConstantString("MismatchedICCID");
_CFStringMakeConstantString("MissingSIM");
_CFStringMakeConstantString("Unlocked");
_CFStringMakeConstantString("WildcardActivated");
_CFStringMakeConstantString("Expired");
	*/
	if (activation_status == "Unactivated" || activation_status == "Unlocked") {
		is_activated = false;
	}
	else {
		is_activated = true;
	}
	return is_activated;
}

void CMobileActivation::set_debug_level(int level) {
	debug_level = level;
}

error_t CMobileActivation::request_new() {
	request.client_type = CLIENT_MOBILE_ACTIVATION;
	request.content_type = IDEVICE_ACTIVATION_CONTENT_TYPE_URL_ENCODED;
	request.url = IDEVICE_ACTIVATION_DEFAULT_URL;
	if (request.fields != nullptr) {
		CF_RELEASE_CLEAR(request.fields);
	}
	request.fields = CFDictionaryCreateMutable(CFAllocatorGetDefault(), 0,
		&g_iTunesMobileDevice.my_kCFTypeDictionaryKeyCallBacks, &g_iTunesMobileDevice.my_kCFTypeDictionaryValueCallBacks);
	return E_SUCCESS;
}

error_t CMobileActivation::request_new_from_lockdownd() {
	Boolean has_telephony_capability = 0;
	Boolean has_mobile_equipment_id = 0;
	CFMutableDictionaryRef fields = CFDictionaryCreateMutable(CFAllocatorGetDefault(), 0,
		&g_iTunesMobileDevice.my_kCFTypeDictionaryKeyCallBacks, &g_iTunesMobileDevice.my_kCFTypeDictionaryValueCallBacks);

	// add InStoreActivation
	CFDictionaryAddValue(fields, CFSTR("InStoreActivation"), g_iTunesMobileDevice.my_kCFBooleanFalse);

	// get a bunch of information at once
	CFTypeRef info = g_iTunesMobileDevice.AMDeviceCopyValue(m_hDevice, nullptr, nullptr);
	if (!info || CFGetTypeID(info) != CFDictionaryGetTypeID()) {
		if (debug_level > 0)
			fprintf(stderr, "%s: Unable to get basic information from lockdownd\n", __func__);
		CF_RELEASE_CLEAR(fields);
		return E_INCOMPLETE_INFO;
	}
	if (debug_level > 0)
		CFShow(info); // debug output
	// add AppleSerialNumber
	CFTypeRef  node = CFDictionaryGetValue((CFDictionaryRef)info, CFSTR("SerialNumber"));
	if (!node || CFGetTypeID(node) != CFStringGetTypeID()) {
		if (debug_level > 0)
			fprintf(stderr, "%s: Unable to get SerialNumber from lockdownd\n", __func__);
		CF_RELEASE_CLEAR(fields);
		CF_RELEASE_CLEAR(info);
		return E_INCOMPLETE_INFO;
	}
	CFDictionarySetValue(fields, CFSTR("AppleSerialNumber"), CFStringCreateCopy(CFAllocatorGetDefault(), (CFStringRef)node));
	node = nullptr;


	// check if device has telephone capability
	node = CFDictionaryGetValue((CFDictionaryRef)info, CFSTR("TelephonyCapability"));
	if (!node || CFGetTypeID(node) != CFBooleanGetTypeID()) {
		has_telephony_capability = false;
	}
	else {
		has_telephony_capability = CFBooleanGetValue((CFBooleanRef)node);
	}
	node = nullptr;

	if (has_telephony_capability) {
		// add IMEI
		node = CFDictionaryGetValue((CFDictionaryRef)info, CFSTR("InternationalMobileEquipmentIdentity"));
		if (!node || CFGetTypeID(node) != CFStringGetTypeID()) {
			has_mobile_equipment_id = false;
		}
		else {
			CFDictionarySetValue(fields, CFSTR("IMEI"), CFStringCreateCopy(CFAllocatorGetDefault(), (CFStringRef)node));
			has_mobile_equipment_id = true;
		}
		node = nullptr;

		// add MEID
		node = CFDictionaryGetValue((CFDictionaryRef)info, CFSTR("MobileEquipmentIdentifier"));
		if (!node || CFGetTypeID(node) != CFStringGetTypeID()) {
			if (debug_level > 0)
				fprintf(stderr, "%s: Unable to get MEID from lockdownd\n", __func__);
			if (!has_mobile_equipment_id) {
				CF_RELEASE_CLEAR(fields);
				CF_RELEASE_CLEAR(info);
				return E_INCOMPLETE_INFO;
			}
		}
		else {
			CFDictionarySetValue(fields, CFSTR("MEID"), CFStringCreateCopy(CFAllocatorGetDefault(), (CFStringRef)node));
		}
		node = nullptr;

		// add IMSI
		node = CFDictionaryGetValue((CFDictionaryRef)info, CFSTR("InternationalMobileSubscriberIdentity"));
		if (!node || CFGetTypeID(node) != CFStringGetTypeID()) {
			if (debug_level > 0)
				fprintf(stderr, "%s: Unable to get IMSI from lockdownd\n", __func__);
		}
		else {
			CFDictionarySetValue(fields, CFSTR("IMSI"), CFStringCreateCopy(CFAllocatorGetDefault(), (CFStringRef)node));
		}
		node = nullptr;

		// add ICCID
		node = CFDictionaryGetValue((CFDictionaryRef)info, CFSTR("IntegratedCircuitCardIdentity"));
		if (!node || CFGetTypeID(node) != CFStringGetTypeID()) {
			if (debug_level > 0)
				fprintf(stderr, "%s: Unable to get ICCID from lockdownd\n", __func__);
		}
		else {
			CFDictionarySetValue(fields, CFSTR("ICCID"), CFStringCreateCopy(CFAllocatorGetDefault(), (CFStringRef)node));
		}
		node = NULL;
	}
	CF_RELEASE_CLEAR(info);
	info = nullptr;

	// add activation-info
	node = g_iTunesMobileDevice.AMDeviceCopyValue(m_hDevice, nullptr, CFSTR("ActivationInfo"));
	if (!node || CFGetTypeID(node) != CFDictionaryGetTypeID()) {
		fprintf(stderr, "%s: Unable to get ActivationInfo from lockdownd\n", __func__);
		CF_RELEASE_CLEAR(fields);
		return E_INCOMPLETE_INFO;
	}
	CFDictionarySetValue(fields, CFSTR("activation-info"), CFDictionaryCreateCopy(CFAllocatorGetDefault(), (CFDictionaryRef)node));
	CF_RELEASE_CLEAR(node);
	node = nullptr;

	if (debug_level > 0)
		CFShow(fields); // debug output

	request.client_type = CLIENT_MOBILE_ACTIVATION;
	request.content_type = IDEVICE_ACTIVATION_CONTENT_TYPE_MULTIPART_FORMDATA;
	request.url = IDEVICE_ACTIVATION_DEFAULT_URL;
	request.fields = fields;

	return E_SUCCESS;
}

error_t CMobileActivation::drm_handshake_request_new() {
	request.client_type = CLIENT_MOBILE_ACTIVATION;
	request.content_type = IDEVICE_ACTIVATION_CONTENT_TYPE_PLIST;
	request.url = IDEVICE_ACTIVATION_DRM_HANDSHAKE_DEFAULT_URL;
	request.fields = CFDictionaryCreateMutable(CFAllocatorGetDefault(), 0,
		&g_iTunesMobileDevice.my_kCFTypeDictionaryKeyCallBacks, &g_iTunesMobileDevice.my_kCFTypeDictionaryValueCallBacks);
	return E_SUCCESS;
}

void CMobileActivation::request_free() {
	CF_RELEASE_CLEAR(request.fields);
}

void CMobileActivation::request_get_fields(CFDictionaryRef* fields) {
	if (fields == nullptr) {
		return;
	}
	if (request.fields != nullptr) {
		*fields = CFDictionaryCreateCopy(CFAllocatorGetDefault(), request.fields);
	}
	else {
		*fields = nullptr;
	}
}

void CMobileActivation::request_set_fields(CFDictionaryRef fields) {
	if (fields == nullptr) {
		return;
	}
	if (request.fields == nullptr) {
		request.fields = CFDictionaryCreateMutable(CFAllocatorGetDefault(), 0,
			&g_iTunesMobileDevice.my_kCFTypeDictionaryKeyCallBacks, &g_iTunesMobileDevice.my_kCFTypeDictionaryValueCallBacks);
	}

	if (request.content_type == IDEVICE_ACTIVATION_CONTENT_TYPE_URL_ENCODED) {
		CFDictionaryApplyFunction(fields, [](const void* key, const void* value, void* context) {
			if (CFGetTypeID(value) != CFStringGetTypeID()) {
				(static_cast<PREQUEST>(context))->content_type = IDEVICE_ACTIVATION_CONTENT_TYPE_MULTIPART_FORMDATA;

			}
			}, &request);

	}
	CFDictionaryApplyFunction(fields, [](const void* key, const void* value, void* context) {
		CFDictionarySetValue(static_cast<CFMutableDictionaryRef>(context), key, value);
		}, request.fields);
}

void CMobileActivation::request_set_fields_from_response() {
	CFDictionaryRef response_fields = nullptr;
	response_get_fields(&response_fields);
	if (response_fields) {
		request_set_fields(response_fields);
		CF_RELEASE_CLEAR(response_fields);
	}
}
void CMobileActivation::request_set_field(const std::string key, const std::string value) {
	if (key.empty() || value.empty()) {
		return;
	}
	if (request.fields == nullptr) {
		request.fields = CFDictionaryCreateMutable(CFAllocatorGetDefault(), 0,
			&g_iTunesMobileDevice.my_kCFTypeDictionaryKeyCallBacks, &g_iTunesMobileDevice.my_kCFTypeDictionaryValueCallBacks);
	}
	CFStringRef cf_key = CFStringCreateWithCString(CFAllocatorGetDefault(), key.c_str(), kCFStringEncodingUTF8);
	CFStringRef cf_value = CFStringCreateWithCString(CFAllocatorGetDefault(), value.c_str(), kCFStringEncodingUTF8);
	CFDictionarySetValue(request.fields, cf_key, cf_value);
}


void CMobileActivation::request_get_field(const std::string key, std::string& value) {
	if (key.empty() || request.fields == nullptr) {
		value.clear();
		return;
	}
	CFStringRef cf_key = CFStringCreateWithCString(CFAllocatorGetDefault(), key.c_str(), kCFStringEncodingUTF8);
	CFTypeRef cf_value = CFDictionaryGetValue(request.fields, cf_key);
	if (cf_value) {
		if (CFGetTypeID(cf_value) == CFStringGetTypeID()) {
			char buffer[1024];
			if (CFStringGetCString((CFStringRef)cf_value, buffer, sizeof(buffer), kCFStringEncodingUTF8)) {
				value = buffer;
			}
			else {
				value.clear();
			}
		}
		else {
			CFDataRef xml_data = CFPropertyListCreateXMLData(CFAllocatorGetDefault(), cf_value);
			if (xml_data) {
				const UInt8* data_ptr = CFDataGetBytePtr(xml_data);
				CFIndex data_length = CFDataGetLength(xml_data);
				value.assign(reinterpret_cast<const char*>(data_ptr), data_length);
				CF_RELEASE_CLEAR(xml_data);
			}
			else {
				value.clear();
			}
		}
	}
	else {
		value.clear();
	}
	CF_RELEASE_CLEAR(cf_key);
}


void CMobileActivation::request_get_url(std::string& url) {
	url = request.url;
}

void CMobileActivation::request_set_url(const std::string url) {
	if (url.empty()) {
		return;
	}
	request.url = url;
}

error_t CMobileActivation::response_new() {
	response.Clean();
	response.content_type = IDEVICE_ACTIVATION_CONTENT_TYPE_UNKNOWN;

	response.headers = CFDictionaryCreateMutable(CFAllocatorGetDefault(), 0,
		&g_iTunesMobileDevice.my_kCFTypeDictionaryKeyCallBacks, &g_iTunesMobileDevice.my_kCFTypeDictionaryValueCallBacks);
	response.fields = CFDictionaryCreateMutable(CFAllocatorGetDefault(), 0,
		&g_iTunesMobileDevice.my_kCFTypeDictionaryKeyCallBacks, &g_iTunesMobileDevice.my_kCFTypeDictionaryValueCallBacks);
	response.fields_require_input = CFDictionaryCreateMutable(CFAllocatorGetDefault(), 0,
		&g_iTunesMobileDevice.my_kCFTypeDictionaryKeyCallBacks, &g_iTunesMobileDevice.my_kCFTypeDictionaryValueCallBacks);
	response.fields_secure_input = CFDictionaryCreateMutable(CFAllocatorGetDefault(), 0,
		&g_iTunesMobileDevice.my_kCFTypeDictionaryKeyCallBacks, &g_iTunesMobileDevice.my_kCFTypeDictionaryValueCallBacks);
	response.labels = CFDictionaryCreateMutable(CFAllocatorGetDefault(), 0,
		&g_iTunesMobileDevice.my_kCFTypeDictionaryKeyCallBacks, &g_iTunesMobileDevice.my_kCFTypeDictionaryValueCallBacks);
	response.labels_placeholder = CFDictionaryCreateMutable(CFAllocatorGetDefault(), 0,
		&g_iTunesMobileDevice.my_kCFTypeDictionaryKeyCallBacks, &g_iTunesMobileDevice.my_kCFTypeDictionaryValueCallBacks);
	response.is_activation_ack = 0;
	response.is_auth_required = 0;
	response.has_errors = 0;
	return E_SUCCESS;
}

error_t CMobileActivation::response_new_from_html(const std::string content) {
	if (content.empty()) {
		return E_INTERNAL_ERROR;
	}
	response.raw_content = content;
	response.raw_content_size = content.size();
	response.content_type = IDEVICE_ACTIVATION_CONTENT_TYPE_HTML;

	error_t result = parse_html_response();
	if (result != E_SUCCESS) {
		response_free();
		return result;
	}
	// This is a placeholder for actual HTML parsing logic
	// In a real implementation, you would use an HTML parser to extract the necessary information
	return E_SUCCESS;
}

error_t CMobileActivation::ErrorPage(const std::string& str) {  
	std::regex carrierRegex("Choose Your Carrier to Continue", std::regex_constants::icase);  
	std::regex demoRegex("Demo Registration", std::regex_constants::icase);
	std::regex problemRegex("There is a problem with your iPhone", std::regex_constants::icase);
	if (std::regex_search(str, carrierRegex)) {
		return E_SELECT_CARRIER;  
	}  
	if (std::regex_search(str, demoRegex)) {
		return E_DEMO_PHONE;
	}
	if (std::regex_search(str, problemRegex)) {
		return E_PROBLEM;
	}
	return E_SIM_ERROR;  
}

error_t CMobileActivation::parse_html_response() {
	error_t result = E_SUCCESS;
	xmlDocPtr doc = NULL;
	xmlXPathContextPtr context = NULL;
	xmlXPathObjectPtr xpath_result = NULL;

	if (response.content_type != IDEVICE_ACTIVATION_CONTENT_TYPE_HTML)
		return E_UNKNOWN_CONTENT_TYPE;

	doc = xmlReadMemory(response.raw_content.c_str(), response.raw_content_size, "ideviceactivation.xml", NULL, XML_PARSE_RECOVER | XML_PARSE_NOERROR);
	if (!doc) {
		result = E_HTML_PARSING_ERROR;
		goto cleanup;
	}

	context = xmlXPathNewContext(doc);
	if (!context) {
		result = E_HTML_PARSING_ERROR;
		goto cleanup;
	}

	// check for authentication required
	if (xpath_result) {
		xmlXPathFreeObject(xpath_result);
		xpath_result = NULL;
	}
	xpath_result = xmlXPathEvalExpression((const xmlChar*)"//input[@name='isAuthRequired' and @value='true']", context);
	if (!xpath_result) {
		result = E_INTERNAL_ERROR;
		goto cleanup;
	}

	if (xpath_result->nodesetval && xpath_result->nodesetval->nodeNr) {
		response.is_auth_required = 1;
		result = E_ICLOUD_LOCKED;
		goto cleanup;
	}
	// Select carrier info
	xpath_result = xmlXPathEvalExpression((const xmlChar*)"//div[@id='IPAJingleServiceSwapSelectCarrierPage']", context);
	if (!xpath_result) {
		result = E_INTERNAL_ERROR;
		goto cleanup;
	}
	if (xpath_result->nodesetval && xpath_result->nodesetval->nodeNr) {
		xmlNodePtr node = xpath_result->nodesetval->nodeTab[0];
		xmlChar* content = xmlNodeGetContent(node);
		if (content) {
			response.description = (const char*)content;
			xmlFree(content);
		}
		result = E_SELECT_CARRIER;
		goto cleanup;
	}
	//html report error
	xpath_result = xmlXPathEvalExpression((const xmlChar*)"//div[@id='IPAJingleEndPointErrorPage']", context);
	if (!xpath_result) {
		result = E_INTERNAL_ERROR;
		goto cleanup;
	}
	if (xpath_result->nodesetval && xpath_result->nodesetval->nodeNr) {
		xmlBufferPtr plistNodeBufferA = xmlBufferCreate();
		if (htmlNodeDump(plistNodeBufferA, doc, xpath_result->nodesetval->nodeTab[0]) == -1) {
			result = E_HTML_PARSING_ERROR;
			goto cleanup;
		}
		htmlDocPtr doc = htmlReadMemory((const char*)plistNodeBufferA->content, strlen((const char*)plistNodeBufferA->content), NULL, NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
		if (doc) {
			xmlXPathContextPtr ctx = xmlXPathNewContext(doc);
			if (ctx) {
				// 提取 <h1>
				xmlXPathObjectPtr xpath_h1 = xmlXPathEvalExpression((const xmlChar*)"//h1", ctx);
				if (xpath_h1 && xpath_h1->nodesetval && xpath_h1->nodesetval->nodeNr > 0) {
					xmlNodePtr h1node = xpath_h1->nodesetval->nodeTab[0];
					xmlChar* h1content = xmlNodeGetContent(h1node);
					if (h1content) {
						response.title = (const char*)h1content;
						xmlFree(h1content);
						result = ErrorPage(response.title);
					}
				}
				if (xpath_h1) xmlXPathFreeObject(xpath_h1);

				xmlXPathObjectPtr xpath_p = xmlXPathEvalExpression((const xmlChar*)"//p[string-length(normalize-space())>0]", ctx);
				if (xpath_p && xpath_p->nodesetval && xpath_p->nodesetval->nodeNr > 0) {
					xmlNodePtr pnode = xpath_p->nodesetval->nodeTab[0];
					xmlChar* pcontent = xmlNodeGetContent(pnode);
					if (pcontent) {
						response.description = (const char*)pcontent;
						xmlFree(pcontent);
					}
				}
				if (xpath_p) xmlXPathFreeObject(xpath_p);

				xmlXPathFreeContext(ctx);
			}
			xmlFreeDoc(doc);
		}
		if (plistNodeBufferA)
			xmlBufferFree(plistNodeBufferA);
		goto cleanup;
	}

	// check for plist content
	xpath_result = xmlXPathEvalExpression((const xmlChar*)"//script[@type='text/x-apple-plist']/plist", context);
	if (!xpath_result) {
		result = E_INTERNAL_ERROR;
		goto cleanup;
	}

	if (xpath_result->nodesetval && xpath_result->nodesetval->nodeNr) {
		//plist_t plist = NULL;
		xmlBufferPtr plistNodeBuffer = xmlBufferCreate();
		if (htmlNodeDump(plistNodeBuffer, doc, xpath_result->nodesetval->nodeTab[0]) == -1) {
			result = E_HTML_PARSING_ERROR;
			goto local_cleanup;
		}
		CFDataRef xmlData = CFDataCreate(CFAllocatorGetDefault(),
			reinterpret_cast<const UInt8*>((const char*)plistNodeBuffer->content),
			static_cast<CFIndex>(plistNodeBuffer->use));
		CFPropertyListRef plist = NULL;
		if (xmlData) {
			CFErrorRef error = NULL;
			plist = CFPropertyListCreateWithData(CFAllocatorGetDefault(),
				xmlData,
				kCFPropertyListMutableContainersAndLeaves,
				NULL,
				&error);
			CF_RELEASE_CLEAR(xmlData);
			if (error) {
				// Handle error if needed
				CF_RELEASE_CLEAR(error);
				result = E_PLIST_PARSING_ERROR;
				goto local_cleanup;
			}
		}

		if (plist == NULL) {
			result = E_PLIST_PARSING_ERROR;
			goto local_cleanup;
		}

		result = activation_record_from_plist((CFDictionaryRef)plist);
		CF_RELEASE_CLEAR(plist);

local_cleanup:
		if (plistNodeBuffer)
			xmlBufferFree(plistNodeBuffer);
		goto cleanup;
	}

	response.has_errors = 1;

cleanup:
	if (xpath_result)
		xmlXPathFreeObject(xpath_result);
	if (context)
		xmlXPathFreeContext(context);
	if (doc)
		xmlFreeDoc(doc);

	return result;
}

error_t CMobileActivation::response_to_buffer(char** buffer, size_t* size) {
	if (!buffer || !size)
		return E_INTERNAL_ERROR;

	char* tmp_buffer = (char*)malloc(sizeof(char) * response.raw_content_size);
	if (!tmp_buffer) {
		return E_OUT_OF_MEMORY;
	}

	memcpy(tmp_buffer, response.raw_content.c_str(), response.raw_content_size);

	*buffer = tmp_buffer;
	*size = response.raw_content_size;

	return E_SUCCESS;
}

void CMobileActivation::response_free() {
	response.Clean();
}

void CMobileActivation::response_get_field(const std::string key, std::string& value) {
	if (key.empty()) {
		value.clear();
		return;
	}
	CFStringRef cf_key = CFStringCreateWithCString(CFAllocatorGetDefault(), key.c_str(), kCFStringEncodingUTF8);
	CFTypeRef cf_value = CFDictionaryGetValue(response.fields, cf_key);
	if (cf_value && CFGetTypeID(cf_value) == CFStringGetTypeID()) {
		char buffer[1024];
		if (CFStringGetCString((CFStringRef)cf_value, buffer, sizeof(buffer), kCFStringEncodingUTF8)) {
			value = buffer;
		}
		else {
			value.clear();
		}
	}
	else {
		value.clear();
	}
}

void CMobileActivation::response_get_fields(CFDictionaryRef* fields) {
	if (fields == nullptr) {
		return;
	}
	if (response.fields != nullptr) {
		*fields = CFDictionaryCreateCopy(CFAllocatorGetDefault(), response.fields);
	}
	else {
		*fields = nullptr;
	}
}

void CMobileActivation::response_get_label(const std::string key, std::string& value) {
	if (key.empty() || response.labels == nullptr) {
		value.clear();
		return;
	}
	CFStringRef cf_key = CFStringCreateWithCString(CFAllocatorGetDefault(), key.c_str(), kCFStringEncodingUTF8);
	CFTypeRef cf_value = CFDictionaryGetValue(response.labels, cf_key);
	if (cf_value && CFGetTypeID(cf_value) == CFStringGetTypeID()) {
		char buffer[1024];
		if (CFStringGetCString((CFStringRef)cf_value, buffer, sizeof(buffer), kCFStringEncodingUTF8)) {
			value = buffer;
		}
		else {
			value.clear();
		}
	}
	else {
		value.clear();
	}
	CF_RELEASE_CLEAR(cf_key);
}

void CMobileActivation::response_get_placeholder(const std::string key, std::string& value) {
	if (key.empty() || response.labels_placeholder == nullptr) {
		value.clear();
		return;
	}
	CFStringRef cf_key = CFStringCreateWithCString(CFAllocatorGetDefault(), key.c_str(), kCFStringEncodingUTF8);
	CFTypeRef cf_value = CFDictionaryGetValue(response.labels_placeholder, cf_key);
	if (cf_value && CFGetTypeID(cf_value) == CFStringGetTypeID()) {
		char buffer[1024];
		if (CFStringGetCString((CFStringRef)cf_value, buffer, sizeof(buffer), kCFStringEncodingUTF8)) {
			value = buffer;
		}
		else {
			value.clear();
		}
	}
	else {
		value.clear();
	}
	CF_RELEASE_CLEAR(cf_key);
}

void CMobileActivation::response_get_title(std::string& title) {
	title = response.title;
}

void CMobileActivation::response_get_description(std::string& description) {
	description = response.description;
}

void CMobileActivation::response_get_activation_record(CFDataRef* activation_record) {

	if (activation_record == nullptr) {
		return;
	}
	if (response.activation_record != nullptr) {
		*activation_record = CFDataCreateCopy(CFAllocatorGetDefault(), response.activation_record);
	}
	else {
		*activation_record = nullptr;
	}
}

void CMobileActivation::response_get_headers(CFDictionaryRef* headers) {
	if (headers == nullptr) {
		return;
	}
	if (response.headers != nullptr) {
		CFDictionaryRef hh = CFDictionaryCreateCopy(CFAllocatorGetDefault(), response.headers);
		CFMutableDictionaryRef mhh = CFDictionaryCreateMutable(CFAllocatorGetDefault(), 0,
			&g_iTunesMobileDevice.my_kCFTypeDictionaryKeyCallBacks, &g_iTunesMobileDevice.my_kCFTypeDictionaryValueCallBacks);
		CFDictionaryAddValue(mhh, CFSTR("ActivationResponseHeaders"), hh);
		//CFRelease(hh);
		*headers = mhh;
	}
	else {
		*headers = nullptr;
	}
}

int CMobileActivation::response_is_activation_acknowledged() {
	return response.is_activation_ack;
}

int CMobileActivation::response_is_authentication_required() {
	return response.is_auth_required;
}

int CMobileActivation::response_field_requires_input(const char* key) {
	if (key == nullptr || response.fields_require_input == nullptr) {
		return 0;
	}
	CFStringRef cf_key = CFStringCreateWithCString(CFAllocatorGetDefault(), key, kCFStringEncodingUTF8);
	CFTypeRef cf_value = CFDictionaryGetValue(response.fields_require_input, cf_key);
	CF_RELEASE_CLEAR(cf_key);
	if (cf_value && CFGetTypeID(cf_value) == CFBooleanGetTypeID()) {
		return CFBooleanGetValue((CFBooleanRef)cf_value) ? 1 : 0;
	}
	return 0;
}

int CMobileActivation::response_field_secure_input(const char* key) {
	if (key == nullptr || response.fields_secure_input == nullptr) {
		return 0;
	}
	CFStringRef cf_key = CFStringCreateWithCString(CFAllocatorGetDefault(), key, kCFStringEncodingUTF8);
	CFTypeRef cf_value = CFDictionaryGetValue(response.fields_secure_input, cf_key);
	CF_RELEASE_CLEAR(cf_key);
	if (cf_value && CFGetTypeID(cf_value) == CFBooleanGetTypeID()) {
		return CFBooleanGetValue((CFBooleanRef)cf_value) ? 1 : 0;
	}
	return 0;

}

int CMobileActivation::response_has_errors() {
	return response.has_errors;
}

static size_t idevice_activation_write_callback(char* data, size_t size, size_t nmemb, void* userdata)
{
	PRESPONSE response = (PRESPONSE)userdata;
	const size_t total = size * nmemb;

	if (total != 0) {
		response->raw_content.append(data, total);
		response->raw_content_size += total;
	}

	return total;
}

static size_t idevice_activation_header_callback(void* data, size_t size, size_t nmemb, void* userdata)
{
	PRESPONSE response = (PRESPONSE)userdata;
	const size_t total = size * nmemb;
	if (total != 0) {
		char* header = (char*)malloc(total + 1);
		char* value = NULL;
		char* p = NULL;
		memcpy(header, data, total);
		header[total] = '\0';

		p = strchr(header, ':');
		if (p) {
			*p = '\0';
			p++;
			while (*p == ' ') {
				p++;
			}
			if (*p != '\0') {
				value = p;
				while (*p != '\0' && *p != '\n' && *p != '\r') {
					p++;
				}
				*p = '\0';
			}
		}
		if (value) {
			if (strncmp(header, "Content-Type", 12) == 0) {
				if (strncmp(value, "text/xml", 8) == 0) {
					response->content_type = IDEVICE_ACTIVATION_CONTENT_TYPE_PLIST;
				}
				else if (strncmp(value, "application/xml", 15) == 0) {
					response->content_type = IDEVICE_ACTIVATION_CONTENT_TYPE_PLIST;
				}
				else if (strncmp(value, "application/x-buddyml", 21) == 0) {
					response->content_type = IDEVICE_ACTIVATION_CONTENT_TYPE_BUDDYML;
				}
				else if (strncmp(value, "text/html", 9) == 0) {
					response->content_type = IDEVICE_ACTIVATION_CONTENT_TYPE_HTML;
				}
			}
			//plist_dict_set_item(response->headers, header, plist_new_string(value));
			auto cf_header = CFDictionaryCreateMutableCopy(CFAllocatorGetDefault(), 0, response->headers);
			CFDictionaryAddValue(cf_header, CFStringCreateWithCString(CFAllocatorGetDefault(), header, kCFStringEncodingUTF8),
				CFStringCreateWithCString(CFAllocatorGetDefault(), value, kCFStringEncodingUTF8));
			CF_RELEASE_CLEAR(response->headers);
			response->headers = cf_header;
		}
		free(header);
	}
	return total;
}

static char* urlencode(const char* buf)
{
	static const signed char conv_table[256] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	};

	unsigned int i;
	int count = 0;
	for (i = 0; i < strlen(buf); i++) {
		if (conv_table[(int)buf[i]]) {
			count++;
		}
	}
	int newsize = strlen(buf) + count * 2 + 1;
	char* res = (char*)malloc(newsize);
	int o = 0;
	for (i = 0; i < strlen(buf); i++) {
		if (conv_table[(int)buf[i]]) {
			sprintf_s(&res[o], 200, "%%%02X", (unsigned char)buf[i]);
			o += 3;
		}
		else {
			res[o] = buf[i];
			o++;
		}
	}
	res[o] = '\0';

	return res;
}

static int idevice_activation_curl_debug_callback(CURL* handle, curl_infotype type, char* data, size_t size, void* userptr)
{
	switch (type) {
	case CURLINFO_TEXT:
		fprintf(stderr, "* ");
		break;
	case CURLINFO_DATA_IN:
	case CURLINFO_HEADER_IN:
		fprintf(stderr, "< ");
		break;
	case CURLINFO_DATA_OUT:
	case CURLINFO_HEADER_OUT:
		fprintf(stderr, "> ");
		break;
	case CURLINFO_SSL_DATA_IN:
	case CURLINFO_SSL_DATA_OUT:
		return 0;
	default:
		return 0;
	}

	fwrite(data, 1, size, stderr);
	if (size > 0 && data[size - 1] != '\n') {
		fprintf(stderr, "\n");
	}
	return 0;
}

struct FormAddContext {
	struct curl_httppost** pForm;
	struct curl_httppost** pLast;
};

static void CFDictToCurlForm(const void* key, const void* value, void* context) {
	if (!key || !value || !context) return;

	const CFStringRef cfKey = (CFStringRef)key;
	FormAddContext* ctx = (FormAddContext*)context;

	// Convert CFString key to C string
	char keyBuf[256];
	if (!CFStringGetCString(cfKey, keyBuf, sizeof(keyBuf), kCFStringEncodingUTF8)) {
		return;
	}

	char* svalue = nullptr;
	if (CFGetTypeID(value) == CFStringGetTypeID()) {
		// Value is a string
		const CFStringRef cfValue = (CFStringRef)value;
		CFIndex maxLen = CFStringGetMaximumSizeForEncoding(CFStringGetLength(cfValue), kCFStringEncodingUTF8) + 1;
		svalue = (char*)malloc(maxLen);
		if (svalue && CFStringGetCString(cfValue, svalue, maxLen, kCFStringEncodingUTF8)) {
			// ok
		}
		else {
			free(svalue);
			svalue = nullptr;
		}
	}
	else {
		// Value is not a string, serialize to XML
		CFDataRef xmlData = CFPropertyListCreateXMLData(CFAllocatorGetDefault(), value);
		if (xmlData) {
			CFIndex len = CFDataGetLength(xmlData);
			svalue = (char*)malloc(len + 1);
			if (svalue) {
				memcpy(svalue, CFDataGetBytePtr(xmlData), len);
				svalue[len] = '\0';
			}
			CF_RELEASE_CLEAR(xmlData);
		}
	}

	if (svalue) {
		curl_formadd(ctx->pForm, ctx->pLast,
			CURLFORM_COPYNAME, keyBuf,
			CURLFORM_COPYCONTENTS, svalue,
			CURLFORM_END);
		free(svalue);
	}
}


error_t CMobileActivation::send_request() {
	error_t result = E_SUCCESS;
	if (request.fields == nullptr || request.url.empty()) {
		return E_INTERNAL_ERROR;
	}

	CURL* handle = curl_easy_init();
	struct curl_httppost* form = NULL;
	struct curl_slist* slist = NULL;
	if (!handle) {
		result = E_INTERNAL_ERROR;
		goto cleanup;
	}

	switch (request.client_type) {
	case CLIENT_MOBILE_ACTIVATION:
		curl_easy_setopt(handle, CURLOPT_USERAGENT, IDEVICE_ACTIVATION_USER_AGENT_IOS);
		break;
	case CLIENT_ITUNES:
		curl_easy_setopt(handle, CURLOPT_USERAGENT, IDEVICE_ACTIVATION_USER_AGENT_ITUNES);
		break;
	default:
		result = E_INTERNAL_ERROR;
		goto cleanup;
	}

	//char* key = NULL;
	//char* svalue = NULL;
	//plist_t value_node = NULL;

	if (request.content_type == IDEVICE_ACTIVATION_CONTENT_TYPE_MULTIPART_FORMDATA) {
		struct curl_httppost* last = NULL;
		FormAddContext ctx = { &form, &last };
		CFDictionaryApplyFunction(request.fields, CFDictToCurlForm, &ctx);
		curl_easy_setopt(handle, CURLOPT_HTTPPOST, form);
	}
	else if (request.content_type == IDEVICE_ACTIVATION_CONTENT_TYPE_URL_ENCODED) {
		char* postdata = (char*)malloc(sizeof(char));
		postdata[0] = '\0';
		CFIndex count = CFDictionaryGetCount(request.fields);
		const void** keys = (const void**)malloc(sizeof(void*) * count);
		const void** values = (const void**)malloc(sizeof(void*) * count);

		CFDictionaryGetKeysAndValues(request.fields, keys, values);

		for (CFIndex i = 0; i < count; ++i) {
			CFStringRef keyref = (CFStringRef)keys[i];
			CFTypeRef value = values[i];

			if (CFGetTypeID(value) != CFStringGetTypeID()) {
				free(postdata);
				result = E_UNSUPPORTED_FIELD_TYPE;
				goto cleanup;
			}
			char svalue[2048] = { 0 };
			char key[1024] = { 0 };
			if (!CFStringGetCString(keyref, key, sizeof(key), kCFStringEncodingUTF8)) {
				free(postdata);
				result = E_UNSUPPORTED_FIELD_TYPE;
				goto cleanup;
			}
			if (CFStringGetCString((CFStringRef)value, svalue, sizeof(svalue), kCFStringEncodingUTF8)) {
				char* value_encoded = urlencode(svalue);
				if (value_encoded) {
					const size_t new_size = strlen(postdata) + strlen(key) + strlen(value_encoded) + 3;
					postdata = (char*)realloc(postdata, new_size);
					sprintf_s(&postdata[strlen(postdata)], 200, "%s=%s&", key, value_encoded);
					free(value_encoded);
				}
			}
		}

		free(keys);
		free(values);

		const size_t postdata_len = strlen(postdata);
		if (postdata_len > 0)
			postdata[postdata_len - 1] = '\0';

		curl_easy_setopt(handle, CURLOPT_POST, 1);
		curl_easy_setopt(handle, CURLOPT_POSTFIELDS, postdata);
		curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, strlen(postdata));
	}
	else if (request.content_type == IDEVICE_ACTIVATION_CONTENT_TYPE_PLIST) {
		char* postdata = NULL;
		uint32_t postdata_len = 0;
		//plist_to_xml(request->fields, &postdata, &postdata_len);
		CFDataRef xmlData = CFPropertyListCreateXMLData(CFAllocatorGetDefault(), request.fields);
		if (xmlData) {
			postdata_len = CFDataGetLength(xmlData);
			postdata = (char*)malloc(postdata_len + 1);
			if (postdata) {
				memcpy(postdata, CFDataGetBytePtr(xmlData), postdata_len);
				postdata[postdata_len] = '\0';
			}
			CF_RELEASE_CLEAR(xmlData);
		}
		curl_easy_setopt(handle, CURLOPT_POST, 1);
		curl_easy_setopt(handle, CURLOPT_POSTFIELDS, postdata);
		curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, postdata_len);
		slist = curl_slist_append(NULL, "Content-Type: application/x-apple-plist");
		slist = curl_slist_append(slist, "Accept: application/xml");
		curl_easy_setopt(handle, CURLOPT_HTTPHEADER, slist);
	}
	else {
		result = E_INTERNAL_ERROR;
		goto cleanup;
	}


	result = response_new();
	if (result != E_SUCCESS) {
		goto cleanup;
	}

	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &idevice_activation_write_callback);
	curl_easy_setopt(handle, CURLOPT_HEADERDATA, &response);
	curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, &idevice_activation_header_callback);
	curl_easy_setopt(handle, CURLOPT_URL, request.url.c_str());
	curl_easy_setopt(handle, CURLOPT_FORBID_REUSE, 1);
	curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);

	// enable communication debugging
	if (debug_level > 0) {
		curl_easy_setopt(handle, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(handle, CURLOPT_DEBUGFUNCTION, idevice_activation_curl_debug_callback);
	}

	CURLcode res = curl_easy_perform(handle);
	if (res != CURLE_OK) {
		logIt("curl_easy_perform failed: %s\n", curl_easy_strerror(res));
	}

	result = parse_raw_response();
	if (result != E_SUCCESS) {
		goto cleanup;
	}


	result = E_SUCCESS;

cleanup:
	if (form)
		curl_formfree(form);
	if (slist)
		curl_slist_free_all(slist);
	if (handle)
		curl_easy_cleanup(handle);

	return result;
}

error_t CMobileActivation::parse_raw_response()
{
	switch (response.content_type)
	{
	case IDEVICE_ACTIVATION_CONTENT_TYPE_PLIST:
	{
		error_t result = E_SUCCESS;

		CFDataRef xmlData = CFDataCreate(CFAllocatorGetDefault(),
			reinterpret_cast<const UInt8*>(response.raw_content.c_str()),
			static_cast<CFIndex>(response.raw_content.size()));
		CFPropertyListRef plist = NULL;
		if (xmlData) {
			CFErrorRef error = NULL;
			plist = CFPropertyListCreateWithData(CFAllocatorGetDefault(),
				xmlData,
				kCFPropertyListMutableContainersAndLeaves,
				NULL,
				&error);
			CF_RELEASE_CLEAR(xmlData);
			if (error) {
				// Handle error if needed
				CF_RELEASE_CLEAR(error);
			}
		}

		if (plist == NULL) {
			return E_PLIST_PARSING_ERROR;
		}

		/* check if this is a reply to drmHandshake request */
		if (CFDictionaryContainsKey((CFDictionaryRef)plist, CFSTR("HandshakeResponseMessage"))) {
			result = E_SUCCESS;
		}
		else {
			result = activation_record_from_plist((CFDictionaryRef)plist);
		}

		CF_RELEASE_CLEAR(response.fields);
		response.fields = (CFMutableDictionaryRef)plist;

		return result;
	}
	case IDEVICE_ACTIVATION_CONTENT_TYPE_BUDDYML:
		return parse_buddyml_response();
	case IDEVICE_ACTIVATION_CONTENT_TYPE_HTML:
		return parse_html_response();
	default:
		return E_UNKNOWN_CONTENT_TYPE;
	}

	return E_SUCCESS;
}

error_t CMobileActivation::activation_record_from_plist(CFDictionaryRef plist)
{
	CFTypeRef record = CFDictionaryGetValue(plist, CFSTR("ActivationRecord"));
	if (record != nullptr) {
		CFTypeRef ack_received = CFDictionaryGetValue((CFDictionaryRef)record, CFSTR("ack-received"));
		if (ack_received) {
			Boolean val = false;
			CFBooleanGetValue((CFBooleanRef)ack_received);
			if (val) {
				response.is_activation_ack = 1;
			}
		}
		response.activation_record = CFDataCreate(CFAllocatorGetDefault(),
			reinterpret_cast<const UInt8*>(response.raw_content.c_str()),
			static_cast<CFIndex>(response.raw_content_size));
	}
	else {
		CFTypeRef activation_node = CFDictionaryGetValue(plist, CFSTR("iphone-activation"));
		if (!activation_node) {
			activation_node = CFDictionaryGetValue(plist, CFSTR("device-activation"));
		}
		if (!activation_node) {
			return E_PLIST_PARSING_ERROR;
		}
		CFTypeRef ack_received = CFDictionaryGetValue((CFDictionaryRef)activation_node, CFSTR("ack-received"));
		if (ack_received) {
			Boolean val = false;
			CFBooleanGetValue((CFBooleanRef)ack_received);
			if (val) {
				response.is_activation_ack = 1;
			}

		}
		record = CFDictionaryGetValue((CFDictionaryRef)activation_node, CFSTR("activation-record"));
		if (record) {
			response.activation_record = CFDataCreateCopy(CFAllocatorGetDefault(), (CFDataRef)record);
		}
	}
	return E_SUCCESS;
}

error_t CMobileActivation::parse_buddyml_response()
{
	error_t result = E_SUCCESS;
	xmlDocPtr doc = NULL;
	xmlXPathContextPtr context = NULL;
	xmlXPathObjectPtr xpath_result = NULL;
	int i = 0;

	if (response.content_type != IDEVICE_ACTIVATION_CONTENT_TYPE_BUDDYML)
		return E_UNKNOWN_CONTENT_TYPE;

	doc = xmlReadMemory(response.raw_content.c_str(), response.raw_content_size, "ideviceactivation.xml", NULL, XML_PARSE_NOERROR);
	if (!doc) {
		result = E_BUDDYML_PARSING_ERROR;
		goto cleanup;
	}

	context = xmlXPathNewContext(doc);
	if (!context) {
		result = E_BUDDYML_PARSING_ERROR;
		goto cleanup;
	}

	// check for an error
	// <navigationBar> appears directly under <xmlui> only in case of an error
	xpath_result = xmlXPathEvalExpression((const xmlChar*)"/xmlui/navigationBar/@title", context);
	if (!xpath_result) {
		result = E_INTERNAL_ERROR;
		goto cleanup;
	}

	if (xpath_result->nodesetval && xpath_result->nodesetval->nodeNr) {
		xmlChar* content = xmlNodeListGetString(doc, xpath_result->nodesetval->nodeTab[0]->xmlChildrenNode, 1);
		if (content) {
			response.title = (const char*)content;
			xmlFree(content);
		}

		response.has_errors = 1;
		goto cleanup;
	}

	// check for activation ack
	if (xpath_result) {
		xmlXPathFreeObject(xpath_result);
		xpath_result = NULL;
	}
	xpath_result = xmlXPathEvalExpression((const xmlChar*)"/xmlui/clientInfo[@ack-received='true']", context);
	if (!xpath_result) {
		result = E_INTERNAL_ERROR;
		goto cleanup;
	}

	if (xpath_result->nodesetval && xpath_result->nodesetval->nodeNr) {
		// existing activation_acknowledged
		response.is_activation_ack = 1;
		goto cleanup;
	}

	// retrieve response title
	if (xpath_result) {
		xmlXPathFreeObject(xpath_result);
		xpath_result = NULL;
	}
	xpath_result = xmlXPathEvalExpression((const xmlChar*)"/xmlui/alert/@title", context);
	if (!xpath_result) {
		result = E_INTERNAL_ERROR;
		goto cleanup;
	}

	if (xpath_result->nodesetval && xpath_result->nodesetval->nodeNr) {
		// incorrect credentials
		// <alert> exists only in case of incorrect credentials
		xmlChar* content = xmlNodeListGetString(doc, xpath_result->nodesetval->nodeTab[0]->xmlChildrenNode, 1);
		if (content) {
			response.title = (const char*)content;
			xmlFree(content);
		}
	}
	else {
		if (xpath_result) {
			xmlXPathFreeObject(xpath_result);
			xpath_result = NULL;
		}
		xpath_result = xmlXPathEvalExpression((const xmlChar*)"/xmlui/page/navigationBar/@title", context);
		if (!xpath_result) {
			result = E_INTERNAL_ERROR;
			goto cleanup;
		}

		if (!xpath_result->nodesetval) {
			result = E_BUDDYML_PARSING_ERROR;
			goto cleanup;
		}
		xmlChar* content = xmlNodeListGetString(doc, xpath_result->nodesetval->nodeTab[0]->xmlChildrenNode, 1);
		if (content) {
			response.title = (const char*)content;
			xmlFree(content);
		}
	}

	// retrieve response description
	if (xpath_result) {
		xmlXPathFreeObject(xpath_result);
		xpath_result = NULL;
	}
	xpath_result = xmlXPathEvalExpression((const xmlChar*)"/xmlui/page/tableView/section/footer[not (@url)]", context);
	if (!xpath_result) {
		result = E_INTERNAL_ERROR;
		goto cleanup;
	}
	if (!xpath_result->nodesetval) {
		xmlXPathFreeObject(xpath_result);
		xpath_result = xmlXPathEvalExpression((const xmlChar*)"/xmlui/page/tableView/section[@footer and not(@footerLinkURL)]/@footer", context);
		result = E_INTERNAL_ERROR;
		goto cleanup;
	}

	if (xpath_result->nodesetval) {
		char* response_description = (char*)malloc(sizeof(char));
		response_description[0] = '\0';

		for (i = 0; i < xpath_result->nodesetval->nodeNr; i++) {
			xmlChar* content = xmlNodeListGetString(doc, xpath_result->nodesetval->nodeTab[i]->xmlChildrenNode, 1);
			if (content) {
				const size_t len = strlen(response_description);
				response_description = (char*)realloc(response_description, len + xmlStrlen(content) + 2);
				sprintf_s(&response_description[len], 100, "%s\n", (const char*)content);
				xmlFree(content);
			}
		}

		if (strlen(response_description) > 0) {
			// remove the last '\n'
			response_description[strlen(response_description) - 1] = '\0';
			response.description = response_description;
		}
		else {
			free(response_description);
		}
	}

	// retrieve input fields
	if (xpath_result) {
		xmlXPathFreeObject(xpath_result);
		xpath_result = NULL;
	}

	xpath_result = xmlXPathEvalExpression((const xmlChar*)"/xmlui/page//editableTextRow", context);
	if (!xpath_result) {
		result = E_INTERNAL_ERROR;
		goto cleanup;
	}

	if (xpath_result->nodesetval) {
		for (i = 0; i < xpath_result->nodesetval->nodeNr; i++) {
			xmlChar* id = xmlGetProp(xpath_result->nodesetval->nodeTab[i], (const xmlChar*)"id");
			if (!id) {
				result = E_BUDDYML_PARSING_ERROR;
				goto cleanup;
			}
			int secure_input = 0;
			xmlChar* secure = xmlGetProp(xpath_result->nodesetval->nodeTab[i], (const xmlChar*)"secure");
			if (secure) {
				if (!strcmp((const char*)secure, "true")) {
					secure_input = 1;
				}
				xmlFree(secure);
			}

			response_add_field((const char*)id, "", 1, secure_input);

			xmlChar* label = xmlGetProp(xpath_result->nodesetval->nodeTab[i], (const xmlChar*)"label");
			if (label) {
				//plist_dict_set_item(response->labels, (const char*)id, plist_new_string((const char*)label));
				CFStringRef cf_label = CFStringCreateWithCString(CFAllocatorGetDefault(), (const char*)label, kCFStringEncodingUTF8);
				CFDictionaryAddValue(response.labels, __CFStringMakeConstantString((const char*)id), cf_label);
				xmlFree(label);
			}
			xmlChar* placeholder = xmlGetProp(xpath_result->nodesetval->nodeTab[i], (const xmlChar*)"placeholder");
			if (placeholder) {
				//plist_dict_set_item(response->labels_placeholder, (const char*)id, plist_new_string((const char*)placeholder));
				CFStringRef cf_placeholder = CFStringCreateWithCString(CFAllocatorGetDefault(), (const char*)placeholder, kCFStringEncodingUTF8);
				CFDictionaryAddValue(response.labels_placeholder, __CFStringMakeConstantString((const char*)id), cf_placeholder);
				xmlFree(placeholder);
			}

			xmlFree(id);
		}
	}

	// retrieve server info fields
	if (xpath_result) {
		xmlXPathFreeObject(xpath_result);
		xpath_result = NULL;
	}

	xpath_result = xmlXPathEvalExpression((const xmlChar*)"/xmlui/serverInfo/@*", context);
	if (!xpath_result) {
		result = E_INTERNAL_ERROR;
		goto cleanup;
	}

	if (xpath_result->nodesetval) {
		for (i = 0; i < xpath_result->nodesetval->nodeNr; i++) {
			xmlChar* content = xmlNodeGetContent(xpath_result->nodesetval->nodeTab[i]);
			if (content) {
				if (!xmlStrcmp(xpath_result->nodesetval->nodeTab[i]->name, (const xmlChar*)"isAuthRequired")) {
					response.is_auth_required = 1;
				}

				response_add_field(
					(const char*)xpath_result->nodesetval->nodeTab[i]->name, (const char*)content, 0, 0);
				xmlFree(content);
			}
		}
	}

	if (CFDictionaryGetCount(response.fields) == 0) {
		response.has_errors = 1;
	}

cleanup:
	if (xpath_result)
		xmlXPathFreeObject(xpath_result);
	if (context)
		xmlXPathFreeContext(context);
	if (doc)
		xmlFreeDoc(doc);

	return result;
}

void CMobileActivation::response_add_field(const char* key, const char* value, int required_input, int secure_input)
{
	//plist_dict_set_item(response->fields, key, plist_new_string(value));
	if (response.fields == nullptr) {
		response.fields = CFDictionaryCreateMutable(CFAllocatorGetDefault(), 0,
			&g_iTunesMobileDevice.my_kCFTypeDictionaryKeyCallBacks, &g_iTunesMobileDevice.my_kCFTypeDictionaryValueCallBacks);
	}
	CFStringRef cf_key = CFStringCreateWithCString(CFAllocatorGetDefault(), key, kCFStringEncodingUTF8);
	CFStringRef cf_value = CFStringCreateWithCString(CFAllocatorGetDefault(), value, kCFStringEncodingUTF8);
	CFDictionarySetValue(response.fields, cf_key, cf_value);
	CF_RELEASE_CLEAR(cf_key);
	CF_RELEASE_CLEAR(cf_value);


	if (required_input) {
		//plist_dict_set_item(response->fields_require_input, key, plist_new_bool(1));
		CFDictionarySetValue(response.fields_require_input, cf_key, g_iTunesMobileDevice.my_kCFBooleanTrue);
	}
	if (secure_input) {
		//plist_dict_set_item(response->fields_secure_input, key, plist_new_bool(1));
		CFDictionarySetValue(response.fields_secure_input, cf_key, g_iTunesMobileDevice.my_kCFBooleanTrue);
	}
}


#include <fstream> // Add this include to resolve incomplete type "std::ifstream" error  
#include <sstream> // Add this include to resolve incomplete type "std::ostringstream" error
error_t CMobileActivation::CreateHtmlResponseFromFile(const std::string& filename, const std::string& content_type) {  
  error_t result = E_SUCCESS;  
  std::ifstream file(filename, std::ios::in | std::ios::binary);  

  if (!file.is_open()) {  
      logIt("Failed to open file: %s\n", filename.c_str());  
      return E_FILE_NOT_FOUND;  
  }  

  std::ostringstream contentStream;  
  contentStream << file.rdbuf();  
  response.raw_content = contentStream.str();  
  response.raw_content_size = response.raw_content.size();  
  response.content_type = IDEVICE_ACTIVATION_CONTENT_TYPE_HTML;  

  file.close(); 



  return result;  
}
