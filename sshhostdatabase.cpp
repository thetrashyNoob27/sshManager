#include "sshhostdatabase.h"

sshHostDataBase::sshHostDataBase(std::string &sqlitePath):dbPath(sqlitePath)
{

}

bool sshHostDataBase::connectDatabase()
{
  char* connectError=nullptr;
  auto openStatus=sqlite3_open(this->dbPath.c_str(),&(this->db));
  if(openStatus)
    {
      //open error
      return false;
    }
  return true;
}
