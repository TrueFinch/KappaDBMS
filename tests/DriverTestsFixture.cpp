//
// Created by truefinch on 16.11.18.
//

#include <vector>
#include <exception>
#include <memory>

#include <gtest/gtest.h>
#include <driver/driver.hpp>
#include <json.hpp>

namespace
{

using json = nlohmann::json;

class DriverTestsFixture : public ::testing::Test {
public:
  sql::Driver& driver = sql::Driver::Instance();

  virtual void SetUp() { }

  virtual void TearDown() { }

  static void SetUpTestCase()
  {
    try {
      sql::Driver::Instance().RunQuery("DROP TABLE products");
    } catch (std::logic_error& error) { }
  }

  static void TearDownTestCase()
  {
    try {
      sql::Driver::Instance().RunQuery("DROP TABLE products");
    } catch (std::logic_error& error) { }
  }
};

} // namespace

TEST_F(DriverTestsFixture, CREATE_TABLE_TEST)
{
  std::string result;
  std::vector<sql::Table> expected_result;
  sql::Table expected_table;
  json expected_json;

  expected_table = sql::Table(
      { "CREATE TABLE products (id INTEGER, count INTEGER, price DOUBLE, description TEXT)" },
      { {"result", cmd::LiteralType::BOOL} },
      { {std::make_shared<sql::BoolField>(true)} }
    );
  expected_result.push_back(expected_table);
  expected_json["code"] = 1;
  expected_json["result"] = expected_result;
  result = driver.RunQuery("CREATE TABLE products (id INTEGER, count INTEGER, price DOUBLE, description TEXT);");

  ASSERT_EQ(expected_json.dump(), result);
}

TEST_F(DriverTestsFixture, CREATE_TABLE_EXISTS)
{
  ASSERT_THROW(driver.RunQuery("CREATE TABLE products (id integer)"), std::logic_error);
}

TEST_F(DriverTestsFixture, INSERT_TEST_1)
{
  std::string result;
  std::vector<sql::Table> expected_result;
  sql::Table expected_table;
  json expected_json;

  expected_table = sql::Table(
      { "INSERT INTO products VALUES (1, 1, 12.24, 'Book')" },
      { {"result", cmd::LiteralType::BOOL} },
      { {std::make_shared<sql::BoolField>(true)} }
    );
  expected_result.push_back(expected_table);
  expected_json["code"] = 1;
  expected_json["result"] = expected_result;
  result = driver.RunQuery("INSERT INTO products VALUES (1, 1, 12.24, 'Book');");

  ASSERT_EQ(expected_json.dump(), result);
}

TEST_F(DriverTestsFixture, INSERT_TEST_2)
{
  std::string result;
  std::vector<sql::Table> expected_result;
  sql::Table expected_table;
  json expected_json;

  expected_table = sql::Table(
      { "INSERT INTO products VALUES (2, 2, 13.36, 'Axe')" },
      { {"result", cmd::LiteralType::BOOL} },
      { {std::make_shared<sql::BoolField>(true)} }
    );
  expected_result.push_back(expected_table);
  expected_json["code"] = 1;
  expected_json["result"] = expected_result;
  result = driver.RunQuery("INSERT INTO products VALUES (2, 2, 13.36, 'Axe');");

  ASSERT_EQ(expected_json.dump(), result);
}

TEST_F(DriverTestsFixture, INSERT_TEST_3)
{
  std::string result;
  std::vector<sql::Table> expected_result;
  sql::Table expected_table;
  json expected_json;

  expected_table = sql::Table(
      { "INSERT INTO products VALUES (3, 4, 228.228, 'Computer')" },
      { {"result", cmd::LiteralType::BOOL} },
      { {std::make_shared<sql::BoolField>(true)} }
    );
  expected_result.push_back(expected_table);
  expected_json["code"] = 1;
  expected_json["result"] = expected_result;
  result = driver.RunQuery("INSERT INTO products VALUES (3, 4, 228.228, 'Computer');");

  ASSERT_EQ(expected_json.dump(), result);
}

TEST_F (DriverTestsFixture, SELECT_ALL)
{
  std::string result;
  std::vector<sql::Table> expected_result;
  sql::Table expected_table;
  json expected_json;

  expected_table = sql::Table(
      { "SELECT * FROM products" },
      {
        { "id", cmd::LiteralType::INTEGER },
        { "count", cmd::LiteralType::INTEGER },
        { "price", cmd::LiteralType::DOUBLE },
        { "description", cmd::LiteralType::TEXT },
      },
      {
        {
          std::make_shared<sql::IntField>(1),
          std::make_shared<sql::IntField>(1),
          std::make_shared<sql::DoubleField>(12.24),
          std::make_shared<sql::TextField>("Book")
        },
        {
          std::make_shared<sql::IntField>(2),
          std::make_shared<sql::IntField>(2),
          std::make_shared<sql::DoubleField>(13.36),
          std::make_shared<sql::TextField>("Axe")
        },
        {
          std::make_shared<sql::IntField>(3),
          std::make_shared<sql::IntField>(4),
          std::make_shared<sql::DoubleField>(228.228),
          std::make_shared<sql::TextField>("Computer")
        },
      }
    );
  expected_result.push_back(expected_table);
  expected_json["code"] = 1;
  expected_json["result"] = expected_result;
  result = driver.RunQuery("SELECT * FROM products;");

  ASSERT_EQ(expected_json.dump(), result);
}

TEST_F (DriverTestsFixture, SELECT_ALL_WHERE)
{
  std::string result;
  std::vector<sql::Table> expected_result;
  sql::Table expected_table;
  json expected_json;

  expected_table = sql::Table(
      { "SELECT * FROM products WHERE id < 3 AND count > 1" },
      {
        { "id", cmd::LiteralType::INTEGER },
        { "count", cmd::LiteralType::INTEGER },
        { "price", cmd::LiteralType::DOUBLE },
        { "description", cmd::LiteralType::TEXT }
      },
      {
        {
          std::make_shared<sql::IntField>(2),
          std::make_shared<sql::IntField>(2),
          std::make_shared<sql::DoubleField>(13.36),
          std::make_shared<sql::TextField>("Axe")
        },
      }
    );
  expected_result.push_back(expected_table);
  expected_json["code"] = 1;
  expected_json["result"] = expected_result;
  result = driver.RunQuery("SELECT * FROM products WHERE id < 3 AND count > 1;");

  ASSERT_EQ(expected_json.dump(), result);
}

TEST_F (DriverTestsFixture, SELECT_LIST)
{
  std::string result;
  std::vector<sql::Table> expected_result;
  sql::Table expected_table;
  json expected_json;

  expected_table = sql::Table(
      { "SELECT count, id, description FROM products" },
      {
        { "count", cmd::LiteralType::INTEGER },
        { "id", cmd::LiteralType::INTEGER },
        { "description", cmd::LiteralType::TEXT }
      },
      {
        {
          std::make_shared<sql::IntField>(1),
          std::make_shared<sql::IntField>(1),
          std::make_shared<sql::TextField>("Book")
        },
        {
          std::make_shared<sql::IntField>(2),
          std::make_shared<sql::IntField>(2),
          std::make_shared<sql::TextField>("Axe")
        },
        {
          std::make_shared<sql::IntField>(4),
          std::make_shared<sql::IntField>(3),
          std::make_shared<sql::TextField>("Computer")
        },
      }
    );
  expected_result.push_back(expected_table);
  expected_json["code"] = 1;
  expected_json["result"] = expected_result;
  result = driver.RunQuery("SELECT count, id, description FROM products;");

  ASSERT_EQ(expected_json.dump(), result);
}

TEST_F (DriverTestsFixture, UPDATE_ALL)
{
  std::string result;
  std::vector<sql::Table> expected_result;
  sql::Table expected_table;
  json expected_json;

  driver.RunQuery("UPDATE products SET count = 0");

  expected_table = sql::Table(
      { "SELECT * FROM products WHERE count <> 0" },
      {
        { "id", cmd::LiteralType::INTEGER },
        { "count", cmd::LiteralType::INTEGER },
        { "price", cmd::LiteralType::DOUBLE },
        { "description", cmd::LiteralType::TEXT },
      },
      { }
    );
  expected_result.push_back(expected_table);
  expected_json["code"] = 1;
  expected_json["result"] = expected_result;
  result = driver.RunQuery("SELECT * FROM products WHERE count <> 0;");

  ASSERT_TRUE(json::parse(result)["result"][0]["records"].empty());
  ASSERT_EQ(expected_json.dump(), result);
}

TEST_F (DriverTestsFixture, UPDATE_COMPLEX)
{
  std::string result;
  std::vector<sql::Table> expected_result;
  sql::Table expected_table;
  json expected_json;

  driver.RunQuery("UPDATE products SET price = price * 10 + id * 12");

  expected_table = sql::Table(
      { "SELECT price FROM products" },
      {
        { "price", cmd::LiteralType::DOUBLE },
      },
      {
        {
          std::make_shared<sql::DoubleField>(12.24 * 10 + 12 * 1)
        },
        {
          std::make_shared<sql::DoubleField>(13.36 * 10 + 12 * 2)
        },
        {
          std::make_shared<sql::DoubleField>(228.228 * 10 + 12 * 3)
        },
      }
    );
  expected_result.push_back(expected_table);
  expected_json["code"] = 1;
  expected_json["result"] = expected_result;
  result = driver.RunQuery("SELECT price FROM products;");

  ASSERT_EQ(expected_json.dump(), result);
}

TEST_F (DriverTestsFixture, UPDATE_WHERE)
{
  std::string result;
  std::vector<sql::Table> expected_result;
  sql::Table expected_table;
  json expected_json;

  driver.RunQuery("UPDATE products SET count = 1 WHERE price > 1000");

  expected_table = sql::Table(
      { "SELECT count FROM products WHERE count > 0" },
      {
        { "count", cmd::LiteralType::INTEGER },
      },
      {
        {
          std::make_shared<sql::IntField>(1)
        },
      }
    );
  expected_result.push_back(expected_table);
  expected_json["code"] = 1;
  expected_json["result"] = expected_result;
  result = driver.RunQuery("SELECT count FROM products WHERE count > 0");

  ASSERT_EQ(json::parse(result)["result"][0]["records"].size(), 1);
  ASSERT_EQ(expected_json.dump(), result);
}

TEST_F (DriverTestsFixture, DELETE_WHERE)
{
  std::string result;
  std::vector<sql::Table> expected_result;
  sql::Table expected_table;
  json expected_json;

  driver.RunQuery("DELETE FROM products WHERE count < 1");

  expected_table = sql::Table(
      { "SELECT count FROM products" },
      {
        { "count", cmd::LiteralType::INTEGER },
      },
      {
        {
          std::make_shared<sql::IntField>(1)
        },
      }
    );
  expected_result.push_back(expected_table);
  expected_json["code"] = 1;
  expected_json["result"] = expected_result;
  result = driver.RunQuery("SELECT count FROM products");

  ASSERT_EQ(json::parse(result)["result"][0]["records"].size(), 1);
  ASSERT_EQ(expected_json.dump(), result);
}

TEST_F (DriverTestsFixture, CLEAR_TABLE)
{
  std::string result;
  std::vector<sql::Table> expected_result;
  sql::Table expected_table;
  json expected_json;

  driver.RunQuery("DELETE FROM products");

  expected_table = sql::Table(
      { "SELECT * FROM products" },
      {
        { "id", cmd::LiteralType::INTEGER },
        { "count", cmd::LiteralType::INTEGER },
        { "price", cmd::LiteralType::DOUBLE },
        { "description", cmd::LiteralType::TEXT },
      },
      { }
    );
  expected_result.push_back(expected_table);
  expected_json["code"] = 1;
  expected_json["result"] = expected_result;
  result = driver.RunQuery("SELECT * FROM products");

  ASSERT_TRUE(json::parse(result)["result"][0]["records"].empty());
  ASSERT_EQ(expected_json.dump(), result);
}

TEST_F(DriverTestsFixture, DROP_TABLE)
{
  driver.RunQuery("DROP TABLE products");
  ASSERT_THROW(driver.RunQuery("DROP TABLE products"), std::logic_error);
}

// TODO: tests for multiple queries
