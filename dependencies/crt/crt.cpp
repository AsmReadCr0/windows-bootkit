#include "crt.hpp"

#define to_lower( chr ) ( ( chr >= 'A' && chr <= 'Z' ) ? ( chr + 32 ) : chr )

namespace crt {
    char chrlwr( char c ) {
        if ( c >= ( 'A' ) && c <= ( 'Z' ) ) return c - ( 'A' ) + ( 'a' );
        return c;
    }

    int strlen( const char* str ) {
        int chr = ( 0 );
        if ( str ) {
            for ( ; *str != ( 0 ); ++str ) ++chr;
        }
        return chr;
    }

    int strcmp( const char* cs, const char* ct ) {
        if ( cs && ct ) {
            while ( *cs == *ct ) {
                if ( *cs == ( 0 ) && *ct == ( 0 ) ) return ( 0 );
                if ( *cs == ( 0 ) || *ct == ( 0 ) ) break;
                cs++;
                ct++;
            }

            return *cs - *ct;
        }

        return ( -1 );
    }

    int stricmp( const char* cs, const char* ct ) {
        if ( cs && ct ) {
            while ( chrlwr( *cs ) == chrlwr( *ct ) ) {
                if ( *cs == ( 0 ) && *ct == ( 0 ) ) return ( 0 );
                if ( *cs == ( 0 ) || *ct == ( 0 ) ) break;
                cs++;
                ct++;
            }
            return chrlwr( *cs ) - chrlwr( *ct );
        }
        return -1;
    }

    char* strcpy( char* dst, char* src ) {
        char* ret = dst;
        while ( *src ) *dst++ = *src++;
        *dst = ( 0 );
        return ret;
    }

    const char* strstr( char const* str, char const* sub_str ) {
        const char* bp = sub_str;
        const char* back_pos;

        while ( *str != ( 0 ) && str != 0 && sub_str != 0 ) {
            back_pos = str;
            while ( chrlwr( *back_pos++ ) == chrlwr( *sub_str++ ) ) {
                if ( *sub_str == ( 0 ) ) {
                    return ( char* )( back_pos - strlen( bp ) );
                }
            }
            ++str;
            sub_str = bp;
        }

        return 0;
    }

    char* strcat( char* dest, const char* src ) {
        if ( ( dest == 0 ) || ( src == 0 ) )
            return dest;

        while ( *dest != ( 0 ) )
            dest++;

        while ( *src != ( 0 ) ) {
            *dest = *src;
            dest++;
            src++;
        }

        *dest = ( 0 );
        return dest;
    }

    int wcslen( const wchar_t* s ) {
        int cnt = ( 0 );

        if ( !s )
            return ( 0 );
        for ( ; *s != ( 0 ); ++s )
            ++cnt;

        return cnt;
    }

    int wcscmp( const wchar_t* s1, const wchar_t* s2) {
        while (*s1 && (*s1 == *s2))
        {
            s1++;
            s2++;
        }
        return (int)(*s1) - (int)(*s2);
    }
}