#pragma once

#include "foo_subsonic.h"

class LoginDialog : public CDialogImpl<LoginDialog> {
private:
	CEdit txt_username;
	CEdit txt_password;
public:

	enum { IDD [[maybe_unused]] = IDD_PASSWORD_DLG };

	BEGIN_MSG_MAP(LoginDialog)
		COMMAND_ID_HANDLER_EX(IDOK, OnOk)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_SHOWWINDOW(OnShow)
	END_MSG_MAP()
	
	LoginDialog() = default;

	void OnClose() {
		DestroyWindow();
	}

	void OnShow(BOOL wParam, int lParam) {
		txt_username.SetFocus();
	}

	bool OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
		txt_username.Attach(GetDlgItem(IDC_TXT_USERNAME));
		txt_password.Attach(GetDlgItem(IDC_TXT_PASSWORD));
		
		if (!Preferences::username_data.is_empty()) {
			uSetWindowText(txt_username, Preferences::username_data);
		}
		if (!Preferences::password_data.is_empty()) {
			uSetWindowText(txt_password, Preferences::password_data);
		}

		return true;
	}

	void OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl) {
		EndDialog(1);
	}


	void OnOk(UINT uNotifyCode, int nID, CWindow wndCtl) {
		if (nID == IDOK) {
			Preferences::username_data = uGetDlgItemText(m_hWnd, IDC_TXT_USERNAME);
			Preferences::password_data = uGetDlgItemText(m_hWnd, IDC_TXT_PASSWORD);
		}
		//CDialogImpl::DestroyWindow();
		EndDialog(0);
	}

	
};