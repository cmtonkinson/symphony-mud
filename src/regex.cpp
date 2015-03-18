
#include "regex.h"

std::map<std::pair<std::string,int>,RegexInfo*> Regex::_cache;
std::list<std::map<std::pair<std::string,int>,RegexInfo*>::iterator> Regex::_cache_meta;
#ifdef REGEX_THREAD_SAFE
pthread_mutex_t Regex::_mutex;
pthread_mutexattr_t Regex::_attributes;
#endif

//////////////////////////////////////////////////////////////////////////  REGEX EXCEPTION  //////////////////////////////////////////////////////////////////////////
RegexException::RegexException(void) {
  return;
}

RegexException::RegexException(const std::string& file, const unsigned long& line, const std::string& extra) {
  char* foo = new char[extra.size()+100];
  sprintf(foo, "Regex exception %s line %lu: %s\n", file.c_str(), line, extra.c_str());
  setMessage(foo);
  return;
}

RegexException::RegexException(const RegexException& ref) {
  setMessage(ref.getMessage());
  return;
}

RegexException::~RegexException(void) {
  return;
}

//////////////////////////////////////////////////////////////////////////  REGEX INFO  //////////////////////////////////////////////////////////////////////////
RegexInfo::RegexInfo(std::pair<std::string,int>* pair, const bool& study): _pattern(pair->first), _options(pair->second) {
  pcre* foo = NULL;
  const char* errptr = NULL;
  int erroffset = 0;

  if ((foo = pcre_compile(pair->first.c_str(), pair->second, &errptr, &erroffset, NULL)) == NULL) {
    throw RegexException(__FILE__, __LINE__, errptr);
  }

  code(foo);

  if (study) {
    extra(pcre_study(code(), 0, &errptr));
  }
  return;
}

RegexInfo::~RegexInfo(void) {
  pcre_free(code());
  return;
}

//////////////////////////////////////////////////////////////////////////  REGEX  //////////////////////////////////////////////////////////////////////////
Regex::Regex(void) {
  return;
}

Regex::~Regex(void) {
  return;
}
#ifdef REGEX_THREAD_SAFE
void Regex::cleanup_thread_stuff(void) {
  pthread_mutexattr_destroy(&_attributes);
  pthread_mutex_destroy(&_mutex);
  return;
}

void Regex::init(void) {
  pthread_mutexattr_init(&_attributes);
  pthread_mutexattr_settype(&_attributes, PTHREAD_MUTEX_NORMAL);
  pthread_mutex_init(&_mutex, &_attributes);
  atexit(Regex::cleanup_thread_stuff);
  return;
}
#endif

unsigned int Regex::min(const unsigned int& a, const unsigned int& b) {
  return ((a < b) ? a : b);
}

RegexInfo* Regex::get(const std::string& pattern, const int& options, const bool& study) {
  std::map<std::pair<std::string,int>,RegexInfo*>::iterator it;
  std::pair<std::string,int> pair(pattern, options);
  std::map<std::pair<std::string,int>,RegexInfo*>::iterator dead;
  RegexInfo* ri = NULL;

  #ifdef REGEX_THREAD_SAFE
  pthread_mutex_lock(&_mutex);
  #endif
  if ((it = _cache.find(pair)) == _cache.end()) {
    /* Cache Miss */
    // Create a new cache entry
    ri = new RegexInfo(&pair, study);
    // Push the cache entry onto the 'top' of the 'stack'...
    _cache.insert(std::make_pair(pair, ri));
    _cache_meta.push_front(_cache.find(pair));
    // Drop the oldest element of the cache ('bottom' of the 'stack') if the cache has reached it's max size...
    if (_cache_meta.size() > CACHE_SIZE) {
      dead = _cache_meta.back();
      _cache.erase(dead);
      delete dead->second;
      _cache_meta.pop_back();
    }
    #ifdef REGEX_THREAD_SAFE
    pthread_mutex_unlock(&_mutex);
    #endif
    return ri;
  }

  /* Cache Hit */
  // Re-place the cache entry at the 'top' of the 'stack'...
  _cache_meta.remove(it);
  _cache_meta.push_front(it);
  #ifdef REGEX_THREAD_SAFE
  pthread_mutex_unlock(&_mutex);
  #endif
  return it->second;
}

bool Regex::execute(pcre* code, pcre_extra* extra, const std::string& subject, int& offset, int* ovector) {
  int matches = 0;

  matches = pcre_exec(code, extra, subject.c_str(), subject.size(), offset, 0, ovector, OVECTOR_SIZE);

  // A value lower than -1 means a PCRE error, so we throw...
  if (matches < -1) {
    throw RegexException(__FILE__, __LINE__, "Most likely related to a bad option or a malformed pattern.");
  }

  // A value of -1 just means no match was made...
  if (matches == -1) {
    return false;
  }

  // A value of zero means that pcre_exec ran out of space in the ovector...
  if (matches == 0) {
    throw RegexException(__FILE__, __LINE__, "It looks like pcre_exec() ran out of space in ovector for substring capturing. Try increasing MAX_SUBPATTERNS.");
  }

  // Point offset to the last character of the entire match...
  offset = ovector[1];

  return true;
}

bool Regex::match(const std::string& pattern, const std::string& subject, int options, bool study) {
  RegexInfo* ri = get(pattern, options, study);
  int ovector[OVECTOR_SIZE];
  int offset = 0;
  memset(ovector, 0, sizeof(ovector));
  return execute(ri->code(), ri->extra(), subject, offset, ovector);
}

bool Regex::match(const std::string& pattern, const std::string& subject, std::string& match, int options, bool study) {
  RegexInfo* ri = get(pattern, options, study);
  int ovector[OVECTOR_SIZE];
  int offset = 0;

  match.clear();
  memset(ovector, 0, sizeof(ovector));

  if (!execute(ri->code(), ri->extra(), subject, offset, ovector)) {
    return false;
  }

  if (ovector[3]) {
    match.assign(subject.substr(ovector[2], ovector[3]-ovector[2]));
  } else {
    match.assign(subject.substr(ovector[0], ovector[1]-ovector[0]));
  }

  return true;
}

bool Regex::match(const std::string& pattern, const std::string& subject, std::vector<std::string>& matches, int options, bool study) {
  RegexInfo* ri = get(pattern, options, study);
  int ovector[OVECTOR_SIZE];
  int offset = 0;

  matches.clear();

  while (true) {
    memset(ovector, 0, sizeof(ovector));
    if (!execute(ri->code(), ri->extra(), subject, offset, ovector)) {
      return false;
    }
    if (ovector[3]) {
      matches.push_back(subject.substr(ovector[2],ovector[3]-ovector[2]));
    }
  }

  return true;
}

bool Regex::match(const std::string& pattern, const std::string& subject, match_list& matches, int options, bool study) {
  std::pair<std::string,std::vector<std::string> > foo;
  RegexInfo* ri = get(pattern, options, study);
  int ovector[OVECTOR_SIZE];
  int offset = 0;
  int a = 0, b = 0;

  matches.clear();

  while (true) {
    memset(ovector, 0, sizeof(ovector));
    foo.first.clear();
    foo.second.clear();
    if (!execute(ri->code(), ri->extra(), subject, offset, ovector)) {
      break;
    }
    foo.first = subject.substr(ovector[0],ovector[1]-ovector[0]);
    a = 2;
    b = 3;
    // Iterate over captured subpatterns and save them to the vector...
    while (ovector[b]) {
      foo.second.push_back(subject.substr(ovector[a],ovector[b]-ovector[a]));
      a += 2;
      b += 2;
    }
    matches.push_back(foo);
  }

  return true;
}

bool Regex::replace(const std::string& pattern, const std::string& replacement, std::string& subject, int options, bool study) {
  RegexInfo* ri = get(pattern, options, study);
  int ovector[OVECTOR_SIZE];
  int offset = 0;
  int rep_size = replacement.size();
  bool got_at_least_one = false;

  while (true) {
    memset(ovector, 0, sizeof(ovector));
    if (!execute(ri->code(), ri->extra(), subject, offset, ovector)) {
      // No match was made, so of course nothing could be captured...
      if (!got_at_least_one) {
        return false;
      }
      break;
    }
    got_at_least_one = true;
    // Kill the matched text...
    subject.erase(ovector[0], ovector[1]-ovector[0]);
    // Replace it with the replacement text...
    subject.insert(ovector[0], replacement);
    // Adjust the offset for starting the next search...
    offset += rep_size-(ovector[1]-ovector[0]);
  }

  return true;
}

bool Regex::replace(const std::string& pattern, const std::string& replacement, const std::string& subject, std::string& dest, int options, bool study) {
  dest = subject;
  return replace(pattern, replacement, dest, options, study);
}

unsigned int Regex::count(const std::string& needle, const std::string& haystack) {
  unsigned int count = 0;
  size_t start = 0;
  size_t end = 0;

  while ((end = haystack.find(needle, start)) != std::string::npos) {
    start = end+1;
    count++;
  }

  return count;
}

std::string Regex::ltrim(const std::string& target) {
  std::string dest = target;

  /// Remove whitespace from the begining of the string...
  while (isspace(dest[0])) {
    dest.erase(0, 1);
  }

  return dest;
}

std::string Regex::rtrim(const std::string& target) {
  std::string dest = target;

  /// Remove whitespace from the end of the string...
  while (isspace(dest[dest.size()-1])) {
    dest.erase(dest.end()-1);
  }

  return dest;
}

std::string Regex::trim(const std::string& target) {
  std::string dest = target;

  /// Remove whitespace from the begining of the string...
  if (dest.size()) {
    while (isspace(dest[0])) {
      dest.erase(0, 1);
    }
  }

  /// Remove whitespace from the end of the string...
  if (dest.size()) {
    while (isspace(dest[dest.size()-1])) {
      dest.erase(dest.end()-1);
    }
  }

  return dest;
}

std::string Regex::strip(const std::string& target, const char& kill) {
  std::string dest = target;
  size_t pos = 0;

  while ((pos = dest.find(kill)) != std::string::npos) {
    dest.erase(pos, 1);
  }

  return dest;
}

std::string Regex::lower(const std::string& target) {
  std::string dest;

  for (std::string::const_iterator it = target.begin(); it != target.end(); it++) {
    dest.append(1, tolower(*it));
  }

  return dest;
}

std::string Regex::upper(const std::string& target) {
  std::string dest;

  for (std::string::const_iterator it = target.begin(); it != target.end(); it++) {
    dest.append(1, toupper(*it));
  }

  return dest;
}

std::vector<std::string> Regex::explode(const std::string& delimiter, const std::string& target, bool trim_substrings) {
  std::vector<std::string> matches;
  size_t start = 0;
  size_t end = 0;

  // This while-loop will capture the first n-1 substrings (where n is the
  // total number of substrings to be captured)...
  while (target.size() >= delimiter.size() && (end = target.find(delimiter, start)) != std::string::npos) {
    end = target.find(delimiter, start);
    matches.push_back(target.substr(start, end-start));
    start = end+1;
  }

  // This last bit captures the nth substring...
  if (target.size() >= delimiter.size()) {
    matches.push_back(target.substr(target.rfind(delimiter) + delimiter.size()));
  }

  // Do we want to trim() our resulting substrings?
  if (trim_substrings) {
    for (std::vector<std::string>::iterator it = matches.begin(); it != matches.end(); it++) {
      *it = trim(*it);
    }
  }

  return matches;
}

std::vector<std::string> Regex::explode_multi(const std::string& delimiters, const std::string& target, bool trim_substrings) {
  std::vector<std::string> matches;
  size_t start = 0;
  size_t end = 0;

  // This while-loop will capture the first n-1 substrings (where n is the
  // total number of substrings to be captured)...
  while ((end = target.find_first_of(delimiters, start)) != std::string::npos) {
    end = target.find_first_of(delimiters, start);
    matches.push_back(target.substr(start, end-start));
    start = end+1;
  }

  // This last bit captures the nth substring...
  matches.push_back(target.substr(target.find_last_of(delimiters) + 1));

  // Do we want to trim() our resulting substrings?
  if (trim_substrings) {
    for (std::vector<std::string>::iterator it = matches.begin(); it != matches.end(); it++) {
      *it = trim(*it);
    }
  }

  return matches;
}

std::string Regex::implode(const std::string& glue, const std::vector<std::string>& pieces) {
  std::string output;
  for (std::vector<std::string>::const_iterator it = pieces.begin(); it != pieces.end(); ++it) {
    output.append(*it).append(glue);
  }
  if (!pieces.empty()) {
    output.resize(output.size() - glue.size());
  }
  return output;
}

std::string Regex::implode(const std::string& glue, const std::set<std::string>& pieces) {
  std::string output;
  for (std::set<std::string>::const_iterator it = pieces.begin(); it != pieces.end(); ++it) {
    output.append(*it).append(glue);
  }
  if (!pieces.empty()) {
    output.resize(output.size() - glue.size());
  }
  return output;
}

bool Regex::strPrefix(const std::string& prefix, const std::string& str) {

  if (prefix.empty() || str.empty() || prefix.length() > str.length()) {
    return false;
  }

  for (unsigned u = 0; u < prefix.length(); ++u) {
    if (tolower(prefix[u]) != tolower(str[u])) {
      return false;
    }
  }

  return true;
}

std::vector<std::string> Regex::split(const std::string& src, unsigned short n) {
  std::string input = Regex::trim(src);
  std::string current;
  std::vector<std::string> pieces;
  size_t pos = 0;
  bool quote = false; // whether or not we're "inside" of single quotes

  if (n == 0 || input.empty()) {
    pieces.push_back(input);
  } else {
    while (pieces.size() < n && pos < input.size()) {
      switch (input[pos]) {
        case ' ':
          if (quote) {
            current.append(1, ' ');
          } else {
            pieces.push_back(current);
            current.clear();
          }
          break;
        case '\'':
          current.append(1, '\'');
          quote = !quote;
          break;
        default:
          current.append(1, input[pos]);
          break;
      }
      ++pos;
    }
    if (!current.empty()) {
      pieces.push_back(current);
    }
    if (pos < input.size()) {
      pieces.push_back(input.substr(pos));
    }
  }

  return pieces;
}

bool Regex::isMisspelling(const std::string& attempt, const std::string& correct) {
  std::string temp = attempt;
  std::string::iterator temp_begin = temp.begin();
  std::string::iterator temp_end = temp.end();
  std::vector<std::string> buffer;
  std::vector<std::string>::iterator buffer_end;
  std::string regex("^\\s*?(?:");
  std::string s1, s2, s3;
  unsigned u = 0;

  // Fill the buffer with all of the different subpatterns...
  for (std::string::iterator it = temp_begin; it != temp_end; ++it, ++u) {
    buffer.push_back(temp.substr(0, u).append(".?").append(temp.substr(u)));
    buffer.push_back(temp.substr(0, u).append(".?").append(temp.substr(u+1)));
    // Now for our 'swapped' character case...
    if (it+1 != temp_end) {
      s3.assign(temp_begin, it);
      s3 += *(it+1);
      s3 += *it;
      s3.append(temp.substr(u+2));
      buffer.push_back(s3);
    }
  }

  // One last subpattern (a dot (".") appended to attempt)...
  buffer.push_back(temp.append(".?"));

  // Put all the strings in the vector together (concatenated with pipes) into one single regex...
  buffer_end = buffer.end();
  for (std::vector<std::string>::iterator it = buffer.begin(); it != buffer_end; ++it) {
    if (it+1 != buffer_end) {
      regex.append(*it).append("|");
    } else {
      regex.append(*it).append(")\\s*?$");
    }
  }

  // Return the value of the regex matching the correct string...
  return match(regex, correct);
}

unsigned int Regex::levenshtein(const std::string& string1, const std::string& string2) {
  unsigned int distance = 0, cost = 0, cell_above = 0, cell_left = 0, cell_diagonal = 0;
  unsigned int length1 = string1.length();
  unsigned int length2 = string2.length();
  unsigned int** matrix = NULL;

  // Bail out early if one of the strings are empty...
  if (!length1 || !length2) {
    return 0;
  }

  // Create and initialize the matrix...
  matrix = new unsigned int*[length1+1];
  for (unsigned int ui = 0; ui <= length1; ui++) {
    matrix[ui] = new unsigned int[length2+1];
  }
  for (unsigned int i=1; i <= length1; i++) {
    matrix[i][0] = i;
  }
  for (unsigned int j=1; j <= length2; j++) {
    matrix[0][j] = j;
  }

  // Iterate through the matrix and calculate each value...
  for (unsigned int i=1; i <= length1; i++) {
    for (unsigned int j=1; j <= length2; j++) {
      cost = (string1[i-1] == string2[j-1]) ? 0 : 1;
      cell_above = matrix[i-1][j];
      cell_left = matrix[i][j-1];
      cell_diagonal = matrix[i-1][j-1];
      matrix[i][j] = min(min(cell_above + 1, cell_left + 1), cell_diagonal + cost);
    }
  }

  // Save our final value and clean up after ourselves...
  distance = matrix[length1][length2];
  delete * matrix;

  return distance;
}

std::string Regex::encodeEmail(const std::string& target) {
  std::string dest;

  for (std::string::const_iterator it = target.begin(); it != target.end(); ++it) {
    switch(*it) {
      case 'A': dest.append("&#065;"); break;
      case 'a': dest.append("&#097;"); break;
      case 'B': dest.append("&#066;"); break;
      case 'b': dest.append("&#098;"); break;
      case 'C': dest.append("&#067;"); break;
      case 'c': dest.append("&#099;"); break;
      case 'D': dest.append("&#068;"); break;
      case 'd': dest.append("&#100;"); break;
      case 'E': dest.append("&#069;"); break;
      case 'e': dest.append("&#101;"); break;
      case 'F': dest.append("&#070;"); break;
      case 'f': dest.append("&#102;"); break;
      case 'G': dest.append("&#071;"); break;
      case 'g': dest.append("&#103;"); break;
      case 'H': dest.append("&#072;"); break;
      case 'h': dest.append("&#104;"); break;
      case 'I': dest.append("&#073;"); break;
      case 'i': dest.append("&#105;"); break;
      case 'J': dest.append("&#074;"); break;
      case 'j': dest.append("&#106;"); break;
      case 'K': dest.append("&#075;"); break;
      case 'k': dest.append("&#107;"); break;
      case 'L': dest.append("&#076;"); break;
      case 'l': dest.append("&#108;"); break;
      case 'M': dest.append("&#077;"); break;
      case 'm': dest.append("&#109;"); break;
      case 'N': dest.append("&#078;"); break;
      case 'n': dest.append("&#110;"); break;
      case 'O': dest.append("&#079;"); break;
      case 'o': dest.append("&#111;"); break;
      case 'P': dest.append("&#080;"); break;
      case 'p': dest.append("&#112;"); break;
      case 'Q': dest.append("&#081;"); break;
      case 'q': dest.append("&#113;"); break;
      case 'R': dest.append("&#082;"); break;
      case 'r': dest.append("&#114;"); break;
      case 'S': dest.append("&#083;"); break;
      case 's': dest.append("&#115;"); break;
      case 'T': dest.append("&#084;"); break;
      case 't': dest.append("&#116;"); break;
      case 'U': dest.append("&#085;"); break;
      case 'u': dest.append("&#117;"); break;
      case 'V': dest.append("&#086;"); break;
      case 'v': dest.append("&#118;"); break;
      case 'W': dest.append("&#087;"); break;
      case 'w': dest.append("&#119;"); break;
      case 'X': dest.append("&#088;"); break;
      case 'x': dest.append("&#120;"); break;
      case 'Y': dest.append("&#089;"); break;
      case 'y': dest.append("&#121;"); break;
      case 'Z': dest.append("&#090;"); break;
      case 'z': dest.append("&#122;"); break;
      case '0': dest.append("&#048;"); break;
      case '1': dest.append("&#049;"); break;
      case '2': dest.append("&#050;"); break;
      case '3': dest.append("&#051;"); break;
      case '4': dest.append("&#052;"); break;
      case '5': dest.append("&#053;"); break;
      case '6': dest.append("&#054;"); break;
      case '7': dest.append("&#055;"); break;
      case '8': dest.append("&#056;"); break;
      case '9': dest.append("&#057;"); break;
      case '_': dest.append("&#095;"); break;
      case '-': dest.append("&#045;"); break;
      case '@': dest.append("&#064;"); break;
      case '.': dest.append("&#046;"); break;
      default:  dest.append("&nbsp;"); break;
    }
  }

  return dest;
}

std::string Regex::decodeEmail(const std::string& target) {
  std::vector<std::string> matches;
  std::string dest;

  match("&#([0-9]{3});", target, matches);

  for (std::vector<std::string>::const_iterator it = matches.begin(); it != matches.end(); ++it) {
    if ((*it).compare("065") == 0) { dest.append("A"); }
    else if ((*it).compare("097") == 0) { dest.append("a"); }
    else if ((*it).compare("066") == 0) { dest.append("B"); }
    else if ((*it).compare("098") == 0) { dest.append("b"); }
    else if ((*it).compare("067") == 0) { dest.append("C"); }
    else if ((*it).compare("099") == 0) { dest.append("c"); }
    else if ((*it).compare("068") == 0) { dest.append("D"); }
    else if ((*it).compare("100") == 0) { dest.append("d"); }
    else if ((*it).compare("069") == 0) { dest.append("E"); }
    else if ((*it).compare("101") == 0) { dest.append("e"); }
    else if ((*it).compare("070") == 0) { dest.append("F"); }
    else if ((*it).compare("102") == 0) { dest.append("f"); }
    else if ((*it).compare("071") == 0) { dest.append("G"); }
    else if ((*it).compare("103") == 0) { dest.append("g"); }
    else if ((*it).compare("072") == 0) { dest.append("H"); }
    else if ((*it).compare("104") == 0) { dest.append("h"); }
    else if ((*it).compare("073") == 0) { dest.append("I"); }
    else if ((*it).compare("105") == 0) { dest.append("i"); }
    else if ((*it).compare("074") == 0) { dest.append("J"); }
    else if ((*it).compare("106") == 0) { dest.append("j"); }
    else if ((*it).compare("075") == 0) { dest.append("K"); }
    else if ((*it).compare("107") == 0) { dest.append("k"); }
    else if ((*it).compare("076") == 0) { dest.append("L"); }
    else if ((*it).compare("108") == 0) { dest.append("l"); }
    else if ((*it).compare("077") == 0) { dest.append("M"); }
    else if ((*it).compare("109") == 0) { dest.append("m"); }
    else if ((*it).compare("078") == 0) { dest.append("N"); }
    else if ((*it).compare("110") == 0) { dest.append("n"); }
    else if ((*it).compare("079") == 0) { dest.append("O"); }
    else if ((*it).compare("111") == 0) { dest.append("o"); }
    else if ((*it).compare("080") == 0) { dest.append("P"); }
    else if ((*it).compare("112") == 0) { dest.append("p"); }
    else if ((*it).compare("081") == 0) { dest.append("Q"); }
    else if ((*it).compare("113") == 0) { dest.append("q"); }
    else if ((*it).compare("082") == 0) { dest.append("R"); }
    else if ((*it).compare("114") == 0) { dest.append("r"); }
    else if ((*it).compare("083") == 0) { dest.append("S"); }
    else if ((*it).compare("115") == 0) { dest.append("s"); }
    else if ((*it).compare("084") == 0) { dest.append("T"); }
    else if ((*it).compare("116") == 0) { dest.append("t"); }
    else if ((*it).compare("085") == 0) { dest.append("U"); }
    else if ((*it).compare("117") == 0) { dest.append("u"); }
    else if ((*it).compare("086") == 0) { dest.append("V"); }
    else if ((*it).compare("118") == 0) { dest.append("v"); }
    else if ((*it).compare("087") == 0) { dest.append("W"); }
    else if ((*it).compare("119") == 0) { dest.append("w"); }
    else if ((*it).compare("088") == 0) { dest.append("X"); }
    else if ((*it).compare("120") == 0) { dest.append("x"); }
    else if ((*it).compare("089") == 0) { dest.append("Y"); }
    else if ((*it).compare("121") == 0) { dest.append("y"); }
    else if ((*it).compare("090") == 0) { dest.append("Z"); }
    else if ((*it).compare("122") == 0) { dest.append("z"); }
    else if ((*it).compare("048") == 0) { dest.append("0"); }
    else if ((*it).compare("049") == 0) { dest.append("1"); }
    else if ((*it).compare("050") == 0) { dest.append("2"); }
    else if ((*it).compare("051") == 0) { dest.append("3"); }
    else if ((*it).compare("052") == 0) { dest.append("4"); }
    else if ((*it).compare("053") == 0) { dest.append("5"); }
    else if ((*it).compare("054") == 0) { dest.append("6"); }
    else if ((*it).compare("055") == 0) { dest.append("7"); }
    else if ((*it).compare("056") == 0) { dest.append("8"); }
    else if ((*it).compare("057") == 0) { dest.append("9"); }
    else if ((*it).compare("095") == 0) { dest.append("_"); }
    else if ((*it).compare("045") == 0) { dest.append("-"); }
    else if ((*it).compare("064") == 0) { dest.append("@"); }
    else if ((*it).compare("046") == 0) { dest.append("."); }
  }

  return dest;
}

std::string Regex::slugify(const std::string& source) {
  std::string dest = trim(lower(source));
  Regex::replace("[^a-z0-9 ]+", "", dest);
  Regex::replace(" +", "-", dest);
  return dest;
}
