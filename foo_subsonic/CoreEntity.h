#pragma once

#include "foo_subsonic.h"

#define ENTRY_TYPE_TRACK	0
#define ENTRY_TYPE_ALBUM	1
#define ENTRY_TYPE_PLAYLIST 3
#define ENTRY_TYPE_ARTIST   4

class CoreEntity {
	STRING_MEMBER(id)
    MEMBER(int, type)
public:
	explicit CoreEntity(int _type) {
		type = _type;
	};
	~CoreEntity() = default;;
};
