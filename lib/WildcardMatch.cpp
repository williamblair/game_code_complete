#include <WildcardMatch.h>

// The following function was found on http://xoomer.virgilio.it/acantato/dev/wildcard/wildmatch.html, where it was attributed to 
// the C/C++ Users Journal, written by Mike Cornelison. It is a little ugly, but it is FAST. Use this as an excercise in not reinventing the
// wheel, even if you see gotos. 
bool WildcardMatch( const char* pattern, const char* str )
{
   int i, star;

new_segment:

   star = 0;
   if (*pattern == '*') {
      star = 1;
      do { pattern++; } while (*pattern == '*'); /* enddo */
   } /* endif */

test_match:

   for (i = 0; pattern[i] && (pattern[i] != '*'); i++) {
      //if (mapCaseTable[str[i]] != mapCaseTable[pat[i]]) {
    if (str[i] != pattern[i]) {
         if (!str[i]) return false;
         if ((pattern[i] == '?') && (str[i] != '.')) continue;
         if (!star) return false;
         str++;
         goto test_match;
      }
   }
   if (pattern[i] == '*') {
      str += i;
      pattern += i;
      goto new_segment;
   }
   if (!str[i]) return true;
   if (i && pattern[i - 1] == '*') return true;
   if (!star) return false;
   str++;
   goto test_match;

   return false; // this should never be reached
}

