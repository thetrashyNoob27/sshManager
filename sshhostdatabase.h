#ifndef SSHHOSTDATABASE_H
#define SSHHOSTDATABASE_H
#include <string>
#include <memory>
#include "sqlite3.h"

class sshHostDataBase
{
public:
  sshHostDataBase(std::string& sqlitePath);


private:
  //functions
  bool connectDatabase();
  bool initDatabase();
private:
  //variables
  const std::string dbPath;
  sqlite3* db=nullptr;



};

#endif // SSHHOSTDATABASE_H
