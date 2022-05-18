#pragma once

#include <stdio.h>
#include <random>
#include <windows.h>
#include <Wincrypt.h>
#include "foo_subsonic.h"
#include "simplehttpclient.h"
#include "logindlg.h"

class SimpleHttpClientConfigurator {

public:

	/*
	Checks if all required preferences were configured before, and if they are "correct".
	*/
	static BOOL check_preferences() {
		if (Preferences::connect_url_data.is_empty()) {
			console::error("URL cannot be empty!");
			return FALSE;
		}
		else { // validate url
			pfc::string url = Preferences::connect_url_data;
			url = url.toLower();
			if (!url.startsWith("http://") && !url.startsWith("https://")) {
				console::error("Url has to start with http:// or https://");
				return FALSE;
			}

			if (Preferences::proxy_settings_custom_data) {
				url = Preferences::proxy_url_data;
				url = url.toLower();
				if (!url.startsWith("http://") && !url.startsWith("https://") && !url.startsWith("socks://") && !url.startsWith("socks5://")) {
					console::error("Proxy-Address has to start with http:// or socks5://");
					return FALSE;
				}
			}

		}

		return TRUE;
	}

	/*
	Create SimpleHttpClientConfig using parameters found in foobar config.
	*/
	static BOOL createSimpleHttpClientConfigFromPreferences(SimpleHttpClientConfig &cliConfig) {
		if (!SimpleHttpClientConfigurator::check_preferences()) {
			return FALSE;
		}

		if (Preferences::proxy_settings_custom_data) {

			pfc::stringcvt::string_wide_from_utf8 proxyHost(Preferences::proxy_url_data);

			cliConfig.setProxyUrl(proxyHost);
			cliConfig.useProxy = PROXY_CUSTOM;
		}
		else if (Preferences::proxy_settings_system_data) {
			cliConfig.useProxy = PROXY_SYSTEM;
		}
		else {
			cliConfig.useProxy = PROXY_OFF;
		}

		cliConfig.timeoutInSec = Preferences::connect_timeout_data;
		cliConfig.disableCertVerify = Preferences::check_selfsignedcerts_data;

		return TRUE;
	}

	/*
	Turn char to Hex-representation.
	*/
	static char to_hex(char c) {
		return c < 0xa ? '0' + c : 'a' - 0xa + c;
	}

	/*
	Encode a URL (which means mask all none ASCII characters).
	*/
	static pfc::string8 url_encode(const char *in) {
		pfc::string8 out;
		out.prealloc(strlen(in) * 3 + 1);

		for (const char *tmp = in; *tmp != '\0'; tmp++) {
			auto c = static_cast<unsigned char>(*tmp);
			if (isalnum(c)) {
				out.add_char(c);
			}
			else if (isspace(c)) {
				out.add_char('+');
			}
			else {
				out.add_char('%');
				out.add_char(SimpleHttpClientConfigurator::to_hex(c >> 4));
				out.add_char(SimpleHttpClientConfigurator::to_hex(c % 16));
			}
		}
		return out;
	}

	/*
	Generate a random alphanumeric string (salt) of length @length
	*/
	static std::string random_salt(size_t length)
	{
		auto randchar = []() -> char
		{
			const char charset[] =
				"0123456789"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz";
			const size_t max_index = (sizeof(charset) - 1);
			return charset[rand() % max_index];
		};
		std::string str(length, 0);
		std::generate_n(str.begin(), length, randchar);
		return str;
	}

	/*
	Generate the MD5 hash for a given @input string
	*/
	static std::string MD5(std::string input)
	{
		HCRYPTPROV CryptProv;
		HCRYPTHASH CryptHash;
		BYTE BytesHash[33];
		DWORD dwHashLen;
		std::string result;
		
		if (CryptAcquireContext(&CryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET))
		{
			if (CryptCreateHash(CryptProv, CALG_MD5, 0, 0, &CryptHash))
			{
				if (CryptHashData(CryptHash, (BYTE*)input.c_str(), input.length(), 0))
				{
					if (CryptGetHashParam(CryptHash, HP_HASHVAL, BytesHash, &dwHashLen, 0))
					{
						result.clear();
						std::string hexcharset = "0123456789abcdef";
						for (int j = 0; j < 16; j++)
						{
							result += hexcharset.substr(((BytesHash[j] >> 4) & 0xF), 1);
							result += hexcharset.substr(((BytesHash[j]) & 0x0F), 1);
						}
					}
				}
			}
		}
		CryptDestroyHash(CryptHash);
		CryptReleaseContext(CryptProv, 0);
		return result;
	}

	/*
	Build the request URL required for subsonic.
	This will build the URL using the configured server and add the required parameters like client (c), user (u), salt (s) and auth token (t).
	*/
	static pfc::string8 buildRequestUrl(const char* restMethod, pfc::string8 urlparams) {

		if (Preferences::password_data.is_empty() || Preferences::username_data.is_empty()) {
			LoginDialog *dlg = new LoginDialog();
			dlg->DoModal(core_api::get_main_window(), LPARAM(0));
		}

		std::string pass = Preferences::password_data.c_str();
		std::string salt = random_salt(12);
		std::string token = MD5(pass + salt);

		pfc::string8 url;
		url << Preferences::connect_url_data;
		url << "/rest/";
		url << restMethod;
		url << ".view";
		url << "?v=1.13.0";
		url << "&c=" << COMPONENT_SHORT_NAME;
		url << "&u=" << url_encode(Preferences::username_data);
		url << "&s=" << salt.c_str();
		url << "&t=" << token.c_str();

		if (strlen(urlparams) > 0) {
			url << "&" << urlparams;
		}

		return url;
	}

};