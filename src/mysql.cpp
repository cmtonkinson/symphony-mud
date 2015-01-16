
#include "mysql.h"

const char* Mysql::CONFIG_FILE = "config/mysql.conf";
MysqlCache Mysql::_cache;

//////////////////////////////////////////////////////////////////////////  MYSQL EXCEPTION  //////////////////////////////////////////////////////////////////////////
MysqlException::MysqlException(void) {
  return;
}

MysqlException::MysqlException(const std::string& file, const unsigned long& line, const std::string& extra) {
  char* foo = new char[extra.size()+100];
  sprintf(foo, "MySQL Exception %s line %lu: %s\n", file.c_str(), line, extra.c_str());
  setMessage(foo);
  return;
}

MysqlException::MysqlException(const MysqlException& ref) {
  setMessage(ref.getMessage());
  return;
}

MysqlException::~MysqlException(void) {
  return;
}

//////////////////////////////////////////////////////////////////////////  CACHE PIECE  //////////////////////////////////////////////////////////////////////////
CachePiece::CachePiece(void) {
  return;
}

CachePiece::CachePiece(const std::string& hostname, const std::string& namename, const std::string& password, const std::string& database) {
  setHostname(hostname);
  setUsername(namename);
  setPassword(password);
  setDatabase(database);
  return;
}

CachePiece::CachePiece(const CachePiece& ref) {
  setHostname(ref.getHostname());
  setUsername(ref.getUsername());
  setPassword(ref.getPassword());
  setDatabase(ref.getDatabase());
  return;
}

CachePiece::~CachePiece(void) {
  return;
}

//////////////////////////////////////////////////////////////////////////  MYSQL CACHE  //////////////////////////////////////////////////////////////////////////
MysqlCache::MysqlCache(void) {
  return;
}

MysqlCache::MysqlCache(const MysqlCache& ref) {
  return;
}

MysqlCache::~MysqlCache(void) {
  getCache().clear();
  return;
}

CachePiece* MysqlCache::get(const std::string& config_file) {
  std::map<std::string,CachePiece*>::iterator it;
  CachePiece* cp = NULL;
  std::ifstream read;
  std::string line, option, value, hostname, username, password, database;

  if ((it = getCache().find(config_file)) == getCache().end()) {
    // The file wasn't in the cache... let's open it, read it, and cache it...
    read.open(config_file.c_str());
    if (read.fail()) {
      throw MysqlException(__FILE__, __LINE__, "Failed to open configuration file.");
    } else {
      while (read.peek() != EOF) {
        getline(read, line);
        if (line[0] != '#' && line.find("=") != std::string::npos) {
          if (line.find("hostname") != std::string::npos) {
            hostname = Mysql::trim(line.substr(line.find_last_of('=') + 1));
          } else if (line.find("username") != std::string::npos) {
            username = Mysql::trim(line.substr(line.find_last_of('=') + 1));
          } else if (line.find("password") != std::string::npos) {
            password = Mysql::trim(line.substr(line.find_last_of('=') + 1));
          } else if (line.find("database") != std::string::npos) {
            database = Mysql::trim(line.substr(line.find_last_of('=') + 1));
          } else {
            throw MysqlException(__FILE__, __LINE__, "Unknown configuration option.");
          }
        }
      }
      if (hostname.empty()) {
        throw MysqlException(__FILE__, __LINE__, "Configuration error := Missing value for hostname.");
      } else if (username.empty()) {
        throw MysqlException(__FILE__, __LINE__, "Configuration error := Missing value for username.");
      } else if (password.empty()) {
        throw MysqlException(__FILE__, __LINE__, "Configuration error := Missing value for password.");
      } else if (database.empty()) {
        throw MysqlException(__FILE__, __LINE__, "Configuration error := Missing value for database.");
      }
      cp = new CachePiece(hostname, username, password, database);
      getCache().insert(std::make_pair(config_file, cp));
      return cp;
    }
  } else {
    // We already have that file's contents in cache...
    return it->second;
  }

}

//////////////////////////////////////////////////////////////////////////  MYSQL CLASS  //////////////////////////////////////////////////////////////////////////
// Constructor...
Mysql::Mysql(std::string config_file) {
  setCredentials(CREDENTIAL_CONFIG_FILE);
  if (!config_file.empty()) {
    setFilename(config_file);
  } else {
    setFilename(CONFIG_FILE);
  }
  init();
  connect();
  setResult(NULL);
  setRow(NULL);
  return;
}

// Initializing constructor...
Mysql::Mysql(const std::string& hostname, const std::string& username, const std::string& password, const std::string& database, bool autoconnect) {
  setCredentials(CREDENTIAL_CONSTRUCTOR);
  init();
  setResult(NULL);
  setRow(NULL);
  setHostname(hostname);
  setUsername(username);
  setPassword(password);
  setDatabase(database);
  if (autoconnect) {
    connect();
  }
  return;
}

// Destructor...
Mysql::~Mysql(void) {
  mysql_close(getConnection());
  return;
}

// Initialization method...
void Mysql::readConfiguration(void) {
  CachePiece* cp = getCache()->get(getFilename());

  setHostname(cp->getHostname());
  setUsername(cp->getUsername());
  setPassword(cp->getPassword());
  setDatabase(cp->getDatabase());

  return;
}

void Mysql::init(void) {
  setConnection(mysql_init(NULL));
  if (!getConnection()) {
    throw std::string("Could not initialize a new MySQL connection object.");
  }
/*  if (!mysql_set_character_set(getConnection(), "utf8")) {
    throw std::string("Could not change MySQL client character set to UTF-8.");
  }*/
  if (getCredentials() == CREDENTIAL_CONFIG_FILE) {
    readConfiguration();
  }
  return;
}

// Lower-level memory management stuff...
unsigned long Mysql::handleResults(void) {
  setResult(mysql_store_result(getConnection()));
  if (getResult() == NULL) {
    // We failed to retrieve the information from the query...
    return 0;
  } else {
    setNumRows(mysql_num_rows(getResult()));
    store();
    mysql_free_result(getResult());
    return getNumRows();
  }

  return -1;
}

// Data storage method...
void Mysql::store(void) {
  int num_fields = mysql_num_fields(getResult());
  ROW row;

  getData().clear();

  while (setRow(mysql_fetch_row(getResult()))) {
    for (int i=0; i < num_fields; i++) {
      if (getRow(i)) {
        row.insert(std::make_pair(std::string(getResult()->fields[i].name) , mysql_string(getRow(i))));
      }
    }
    getData().push_back(row);
    row.clear();
  }

  return;
}

// Connection method...
bool Mysql::connect(void) {
  if (mysql_real_connect(getConnection(), getHostname().c_str(), getUsername().c_str(), getPassword().c_str(), getDatabase().c_str(), 0, NULL, CLIENT_MULTI_STATEMENTS)) {
    return true;
  } else {
    throw MysqlException(__FILE__, __LINE__, mysql_error(getConnection()));
  }
}

// General query method...
unsigned long Mysql::query(const std::string& query) {
  unsigned long foo = 0;

  queryPrep();

  if (mysql_real_query(getConnection(), query.c_str(), query.size())) {
    // The query failed...
    return 0;
  } else {
    if (mysql_field_count(getConnection())) {
      return handleResults();
    } else {
      if ((foo = mysql_insert_id(getConnection()))) {
        setInsertID(foo);
        return foo;
      } else if ((foo = mysql_affected_rows(getConnection()))) {
        setAffectedRows(foo);
        return foo;
      } else {
        // Something funky happened....
        return 0;
      }
    }
  }

  return -1;
}

// Select method...
unsigned long Mysql::select(const std::string& query) {
  queryPrep();

  if (mysql_real_query(getConnection(), query.c_str(), query.size())) {
    // The query failed...
    return 0;
  } else {
    return handleResults();
  }

  return -1;
}

// Insert method...
unsigned long Mysql::insert(const std::string& query) {
  queryPrep();

  if (mysql_real_query(getConnection(), query.c_str(), query.size())) {
    // The query failed...
    return 0;
  } else {
    setInsertID(mysql_insert_id(getConnection()));
    return getInsertID();
  }

  return -1;
}

// Update method...
unsigned long Mysql::update(const std::string& query) {
  queryPrep();

  if (mysql_real_query(getConnection(), query.c_str(), query.size())) {
    // The query failed...
    return 0;
  } else {
    setAffectedRows(mysql_affected_rows(getConnection()));
    return getAffectedRows();
  }

  return -1;
}

// Remove method (called 'remove' because 'delete' is a reserved keyword)...
unsigned long Mysql::remove(const std::string& query) {
  queryPrep();

  if (mysql_real_query(getConnection(), query.c_str(), query.size())) {
    // The query failed...
    return 0;
  } else {
    setAffectedRows(mysql_affected_rows(getConnection()));
    return getAffectedRows();
  }

  return -1;
}

bool Mysql::moreRows(void) {
  return getData().size();
}

ROW Mysql::fetch(void) {
  ROW r;

  if (getData().size()) {
    r = getData().front();
    getData().pop_front();
  }

  return r;
}

std::string Mysql::addslashes(const std::string& str) {
  std::string dest;
  size_t pos = 0;
  std::string::const_iterator it;

  for (it = str.begin(), pos = 0; it != str.end(); it++, pos++) {
    if (*it == '\\') {
      // Prepend all backslashes with a backslash...
      dest.append("\\\\");
    } else if (*it == '\'') {
      // Prepend all single quotes with a single quote...
      dest.append("''");
    } else {
      dest.append(1, *it);
    }
  }

  return dest;
}

std::string Mysql::trim(const std::string& str) {
  std::string dest = str;

  /// Remove whitespace from the begining of the string...
  while (isspace(dest[0])) {
    dest.erase(dest.begin());
  }

  /// Remove whitespace from the end of the string...
  while (isspace(dest[dest.size()-1])) {
    dest.erase(dest.end()-1);
  }

  return dest;
}
