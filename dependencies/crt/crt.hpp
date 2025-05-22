#pragma once
#include "core/uefi_include.hpp"

namespace crt {
	char chrlwr( char );
	int strlen( const char* );
	int strcmp( const char*, const char* );
	int stricmp( const char*, const char* );
	char* strcpy( char*, char* );
	const char* strstr( char const*, char const* );
	char* strcat( char*, const char* );
	int wcslen( const wchar_t* );
	int wcscmp( const wchar_t*, const wchar_t*);
}