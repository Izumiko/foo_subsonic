#pragma once

#include "foo_subsonic.h"
#include "subsoniclibraryscanner.h"
#include "searchQueryThread.h"
#include "ListviewHelper.h"
#include "SimpleHttpClientConfigurator.h"


class SearchDialog : public CDialogImpl<SearchDialog> {
private:
	CButton find_button;
	CEdit search_term;
	CListViewCtrl results;

	CComboBox cbResultCount;

	HACCEL    m_haccelerator;

	foo_subsonic::SubsonicLibraryScanner scanner;

	COLORREF bgColor;
	COLORREF fgColor;

public:
	enum { IDD = IDD_SEARCH };

	BEGIN_MSG_MAP(SearchDialog)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		COMMAND_ID_HANDLER_EX(IDOK, OnOk)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
		MESSAGE_HANDLER(ID_SEARCH_DONE, OnSearchDone)
		NOTIFY_HANDLER(IDC_RESULTLIST, NM_DBLCLK, OnLButtonDblClick)
	END_MSG_MAP()

	enum columns {
		artist_column,
		track_column,
		album_column,		
		duration_column
	};

	SearchDialog(COLORREF fgcolor, COLORREF bgcolor) {
		m_haccelerator = nullptr;
		fgColor = fgcolor;
		bgColor = bgcolor;
		Create(core_api::get_main_window());
	}

	
	bool OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
		static_api_ptr_t<modeless_dialog_manager>()->add(m_hWnd);

		search_term.Attach(GetDlgItem(IDC_SEARCHTERM));
		find_button = GetDlgItem(IDOK);
		results.Attach(GetDlgItem(IDC_RESULTLIST));
		
		cbResultCount = GetDlgItem(IDC_CB_RESULT_COUNT);
		
		cbResultCount.AddString(L"10");
		cbResultCount.AddString(L"20");
		cbResultCount.AddString(L"50");
		cbResultCount.AddString(L"100");
		cbResultCount.AddString(L"150");
		cbResultCount.AddString(L"200");

		cbResultCount.SelectString(0, L"200");

		auto styles = LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP;
		results.SetExtendedListViewStyle(styles, styles);

		// Adding release list columns
		listviewHelper::insert_column(results, artist_column, "Artist", 104);
		listviewHelper::insert_column(results, track_column, "Track", 110);
		listviewHelper::insert_column(results, album_column, "Album", 110);
		listviewHelper::insert_column(results, duration_column, "Duration", 50);

		results.SetBkColor(bgColor);
		results.SetTextColor(fgColor);
		results.SetTextBkColor(bgColor);
		
		search_term.SetFocus();
		
		return true;
	}

	void OnClose() {
		DestroyWindow();
	}

	void OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl) {
		DestroyWindow();
	}

	
	void OnOk(UINT uNotifyCode, int nID, CWindow wndCtl) {
		if (nID == IDOK) {

			pfc::string8 params = SimpleHttpClientConfigurator::url_encode(uGetDlgItemText(m_hWnd, IDC_SEARCHTERM));
			params << "&albumCount=0&artistCount=0&songCount=" << uGetDlgItemText(m_hWnd, IDC_CB_RESULT_COUNT);
			
			threaded_process::g_run_modeless(new service_impl_t<foo_subsonic::SearchQueryThread>(&scanner, m_hWnd, params),
				threaded_process::flag_show_progress | threaded_process::flag_show_abort, m_hWnd, "Searching Subsonic Server");
		}
	}	
	
	LRESULT OnSearchDone(UINT, WPARAM, LPARAM, BOOL&) {
		std::list<Track*>* trackList = SqliteCacheDb::getInstance()->getAllSearchResults()->getTracks();

		std::list<Track*>::iterator trackIterator;
		for (trackIterator = trackList->begin(); trackIterator != trackList->end(); trackIterator++) {
			
			char durationStr[20];
			int iDuration = atoi((*trackIterator)->get_duration());
			snprintf(durationStr, sizeof(durationStr), "%02d:%02d", iDuration / 60, iDuration % 60);

			Track* store = *trackIterator;

			listviewHelper::insert_item4(results, 0, (*trackIterator)->get_artist(), (*trackIterator)->get_title(), (*trackIterator)->get_album(), durationStr, (DWORD_PTR)store);
		}

		return 0;
	}

	LRESULT OnLButtonDblClick(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/) {

		for (INT nItem = results.GetNextItem(-1, LVNI_SELECTED); nItem >= 0; nItem = results.GetNextItem(nItem, LVNI_SELECTED))
		{
			if (nItem > -1) {
				DWORD_PTR ptr = results.GetItemData(nItem);
				auto* coreType = reinterpret_cast<CoreEntity*>(ptr);
				if (coreType->get_type() == ENTRY_TYPE_TRACK) { // Track
					if (ptr != NULL) {
						auto* track = reinterpret_cast<Track*>(ptr);
#ifdef _DEBUG
						FB2K_DebugLog() << "Got Track=" << track->get_title() << ", Artist=" << track->get_artist();
#endif
						const char* url = track->get_streamUrl().c_str();

						static_api_ptr_t<playlist_incoming_item_filter_v2>()->process_locations_async(
							pfc::list_single_ref_t<const char*>(url),
							playlist_incoming_item_filter_v2::op_flag_background,
							nullptr,
							nullptr,
							m_hWnd,
							p_notify
							);
					}
				}
			}
		}
		return 0;
	}
	
};