/*
 * $Id: mysql.h 408 2010-06-16 00:40:36Z cmtonkinson@gmail.com $
 *
 * This file is part of the Symphony project <http://code.google.com/p/symphonymud/>
 * Copyright 2005-2010 Chris Tonkinson <cmtonkinson@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Inclusion guards...
#ifndef H_MYSQL
#define H_MYSQL

// Includes...
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <mysql/mysql.h>
#include <string>

// Exception codes...
#define DB_EXCEPTION_CANT_OPEN_CONFIG   1
#define DB_EXCEPTION_CANT_CONNECT       2
#define DB_EXCEPTION_UNKNOWN_CONFIG     3
#define DB_EXCEPTION_NOT_CONNECTED      4
#define DB_EXCEPTION_EMPTY_CONFIG_VALUE 5
#define DB_EXCEPTION_FAILED_ASSERTION   6

// Other options...
#define	CONNECT_TIMEOUT	5			      	       // MySQL timeout (in seconds)
#define CONFIG_FILE		  "config/mysql.conf"	 // File which holds configuration options

// Credential classes...
#define MYSQL_CREDENTIAL_CONFIG_FILE  0
#define MYSQL_CREDENTIAL_CONSTRUCTOR  1

//////////////////////////////////////////////////////////////////////////  MYSQL STRING  //////////////////////////////////////////////////////////////////////////
class mysql_string: public std::string {
  public:
    mysql_string(void) { return; }
    mysql_string(const mysql_string& ref) { this->assign(ref.data()); }
    mysql_string(const char* cs) { this->assign(cs); return; }
    mysql_string(const std::string& s) { this->assign(s); return; }
    ~mysql_string(void) { return; }

    operator bool               (void) const { return (atof(this->c_str()) != 0 || atoi(this->c_str()) != 0); }
    operator signed char        (void) const { return this->c_str()[0]; }
    operator unsigned char      (void) const { return this->c_str()[0]; }
    operator signed short int   (void) const { return atol(this->c_str()); }
    operator unsigned short int (void) const { return atol(this->c_str()); }
    operator signed int         (void) const { return atoi(this->c_str()); }
    operator unsigned int       (void) const { return atoi(this->c_str()); }
    operator signed long int    (void) const { return atol(this->c_str()); }
    operator unsigned long int  (void) const { return atol(this->c_str()); }
    operator float              (void) const { return atof(this->c_str()); }
    operator double             (void) const { return atof(this->c_str()); }
    operator char               (void) const { return this->c_str()[0]; }
    operator const char*        (void) const { return this->c_str(); }
};

//////////////////////////////////////////////////////////////////////////  MYSQL ROW  //////////////////////////////////////////////////////////////////////////
template <class key_type, class val_type> class mysql_map: public std::map<key_type,val_type> {
  public:
    bool operator = (const mysql_map<key_type,val_type>& src) {
      this->std::map<key_type,val_type>::operator = (src);
      return !this->empty();
    }
};

typedef mysql_map<std::string,mysql_string> ROW;

//////////////////////////////////////////////////////////////////////////  MYSQL EXCEPTION  //////////////////////////////////////////////////////////////////////////
class MysqlException {
  public:
    MysqlException(void);
    MysqlException(const std::string& file, const unsigned long& line, const std::string& extra = "");
    MysqlException(const MysqlException& ref);
    ~MysqlException(void);

    std::string    getMessage(void) const { return _message; }

  private:
    std::string     _message;

    void  setMessage(const std::string& message) { _message = message; return; }
};

//////////////////////////////////////////////////////////////////////////  CACHE PIECE  //////////////////////////////////////////////////////////////////////////
class CachePiece {
  public:
    CachePiece(void);
    CachePiece(const std::string& host, const std::string& name, const std::string& password, const std::string& database);
    CachePiece(const CachePiece& ref);
    ~CachePiece(void);

    std::string getHostname(void) const { return _hostname; }
    std::string getUsername(void) const { return _username; }
    std::string getPassword(void) const { return _password; }
    std::string getDatabase(void) const { return _database; }

  private:
    std::string _hostname;
    std::string _username;
    std::string _password;
    std::string _database;

    void  setHostname(const std::string& hostname) { _hostname = hostname; return; }
    void  setUsername(const std::string& username) { _username = username; return; }
    void  setPassword(const std::string& password) { _password = password; return; }
    void  setDatabase(const std::string& database) { _database = database; return; }
};

//////////////////////////////////////////////////////////////////////////  MYSQL CACHE  //////////////////////////////////////////////////////////////////////////
class MysqlCache {
  public:
    MysqlCache(void);
    MysqlCache(const MysqlCache& ref);
    ~MysqlCache(void);

    CachePiece* get(const std::string& config_file);

  private:
    std::map<std::string,CachePiece*>   _cache;

    std::map<std::string,CachePiece*>&  getCache(void) { return _cache; }
};

//////////////////////////////////////////////////////////////////////////  MYSQL CLASS  //////////////////////////////////////////////////////////////////////////
class Mysql {
  public:
    Mysql(std::string config_file = "");
    Mysql(const std::string& hostname, const std::string& username, const std::string& password, const std::string& database, bool autoconnect = true);
    ~Mysql(void);

    void          setHostname(const std::string& hostname)  { _hostname = hostname; return; }
    std::string   getHostname(void) const                   { return _hostname; }
    void          setUsername(const std::string& username)  { _username = username; return; }
    std::string   getUsername(void) const                   { return _username; }
    void          setPassword(const std::string& password)  { _password = password; return; }
    std::string   getPassword(void) const                   { return _password; }
    void          setDatabase(const std::string& database)  { _database = database; return; }
    std::string   getDatabase(void) const                   { return _database; }
    std::string   getFilename(void) const                   { return _filename; }

    bool          connect(void);
    unsigned long query(const std::string& query);
    unsigned long select(const std::string& query);
    unsigned long insert(const std::string& query);
    unsigned long update(const std::string& query);
    unsigned long remove(const std::string& query);
    unsigned long getNumRows(void) const { return _numRows; }
    unsigned long getAffectedRows(void) const { return _affectedRows; }
    unsigned long getInsertID(void) const { return _insertID; }
    bool          moreRows(void);
    ROW           fetch(void);

    static std::string addslashes(const std::string& str);
    static std::string trim(const std::string& str);

  private:
    MYSQL*         _connection;    ///< A pointer to a MySQL C API connection object.
    MYSQL_RES*     _result;        ///< A pointer to a MySQL C API result object.
    MYSQL_ROW      _row;           ///< A MySQL C API row object.
    std::list<ROW> _data;          ///< Local STL storage for returned result sets.
    unsigned long  _numRows;       ///< The number of rows returned by the most recent select query.
    unsigned long  _affectedRows;  ///< The number of rows affected by the most recent insert, update, or delete query.
    unsigned long  _insertID;      ///< The primary key of the most recently inserted record.
    std::string    _hostname;      ///< The database host for the connection.
    std::string    _username;      ///< The username for the database connection.
    std::string    _password;      ///< The password for the database connection.
    std::string    _database;      ///< The default MySQL database to use.
    unsigned short _credentials;   ///< Where to get database credentials from.
    std::string    _filename;      ///< Configuration file to use, if any.

    static MysqlCache   _cache;

    MysqlCache*     getCache(void)                                      { return &_cache; }
    void            setConnection(MYSQL* connection)                    { _connection = connection; return; }
    MYSQL*          getConnection(void)                                 { return _connection; }
    void            setResult(MYSQL_RES* result)                        { _result = result; return; }
    MYSQL_RES*      getResult(void)                                     { return _result; }
    MYSQL_ROW       setRow(MYSQL_ROW row)                               { return (_row = row); }
    MYSQL_ROW       getRow(void)                                        { return _row; }
    char*           getRow(int i)                                       { return _row[i]; }
    std::list<ROW>& getData(void)                                       { return _data; }
    void            setNumRows(const unsigned long& numRows)            { _numRows = numRows; return; }
    void            setAffectedRows(const unsigned long& affectedRows)  { _affectedRows = affectedRows; return; }
    void            setInsertID(const unsigned long& insertID)          { _insertID = insertID; return; }
    void            setCredentials(const unsigned short& credentials)   { _credentials = credentials; return; }
    unsigned short  getCredentials(void) const                          { return _credentials; }
    void            setFilename(const std::string& filename)            { _filename = filename; return; }
    void            readConfiguration(void);
    void            init(void);
    void            queryPrep(void)                                     { setNumRows(0); setAffectedRows(0); setInsertID(0); return; }
    unsigned long   handleResults(void);
    void            store(void);
};

#endif // #ifndef _H__MYSQL
