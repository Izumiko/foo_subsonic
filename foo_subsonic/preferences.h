#pragma once

namespace Preferences {
	extern const GUID guid_connect_url_data;
	extern cfg_string connect_url_data;

	extern const GUID guid_username_data;
	extern cfg_string username_data;

	extern const GUID guid_password_data;
	extern cfg_string password_data;

	extern const GUID guid_check_selfsignedcerts_data;
	extern cfg_bool check_selfsignedcerts_data;

    [[maybe_unused]] extern const GUID guid_check_pass_as_hex_data;
    [[maybe_unused]] extern cfg_bool check_pass_as_hex_data;

	// Proxy Stuff
	extern const GUID guid_proxy_url_data;
	extern cfg_string proxy_url_data;

	extern const GUID guid_connect_timeout_data;
	extern cfg_int connect_timeout_data;

	extern const GUID guid_proxy_settings_no_data;
	extern cfg_bool proxy_settings_no_data;

	extern const GUID guid_proxy_settings_system_data;
	extern cfg_bool proxy_settings_system_data;

	extern const GUID guid_proxy_settings_custom_data;
	extern cfg_bool proxy_settings_custom_data;

	// CoverArt Stuff
	extern const GUID guid_coverart_size_data;
	extern cfg_int coverart_size_data;

	extern const GUID guid_coverart_download;
	extern cfg_bool coverart_download;

	extern const GUID guid_coverart_resize;
	extern cfg_bool coverart_resize;

	// Cache loading
	extern const GUID guid_load_album_cache_on_startup;
	extern cfg_bool load_album_cache_on_startup;

	extern const GUID guid_load_playlist_cache_on_startup;
	extern cfg_bool load_playlist_cache_on_startup;

}
