#include <iostream>

#include "PlayDB.h"

using namespace playdb;

int main() {
  std::string sql =
      "select order_id from orders where cust_id = 'SDYT987645' and price > "
      "200";
  std::shared_ptr<PlayDB> dl = std::make_shared<PlayDB>();
  std::string dbName = dl->getDBName(sql);
  std::cout << "sql: " << sql << std::endl;
  std::cout << "db: " << dbName << std::endl;
}
