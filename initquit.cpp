#include "foo_subsonic.h"
#include "sqliteCacheDb.h"

class myinitquit : public initquit {
public:
	void on_init() override {
		console::print("Subsonic component: on_init()");		
		SqliteCacheDb::getInstance();
	}
	void on_quit() override {
		console::print("Subsonic component: on_quit()");
	}
};


[[maybe_unused]] static initquit_factory_t<myinitquit> g_myinitquit_factory;
