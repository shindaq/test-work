#include <gtest/gtest.h>
#include "components/stock_exchange/component.hpp"

TEST(TestStockWork, TestPartialDeal) {
  components::StockClient client1;
  std::string name1 = "PartialTestUser1";
  client1.AuthUser(name1);
  models::Requisition req{.volume = 15,
                          .exchange_rate = 62,
                          .operation_type = models::OperationTypes::kBuy};
  client1.AddRequisition(req);
  auto balance = client1.TakeUserInfo();
  ASSERT_EQ(0, balance.rub);  // обработка заявки без сделки
  ASSERT_EQ(0, balance.usd);

  components::StockClient client2;
  std::string name2 = "PartialTestUser2";
  client2.AuthUser(name2);
  req = {.volume = 20,
         .exchange_rate = 63,
         .operation_type = models::OperationTypes::kSell};
  client2.AddRequisition(req);

  balance = client1.TakeUserInfo();
  ASSERT_EQ(-930, balance.rub);  // 62*15
  ASSERT_EQ(15, balance.usd);

  balance = client2.TakeUserInfo();
  ASSERT_EQ(930, balance.rub);
  ASSERT_EQ(-15, balance.usd);

  // Заявка 2-го пользователя всё еще висит на 5$
  req = {.volume = 5,
         .exchange_rate = 61,
         .operation_type = models::OperationTypes::kBuy};
  client1.AddRequisition(req);
  balance = client1.TakeUserInfo();
  ASSERT_EQ(-1245, balance.rub);  // -930 - 5*63
  ASSERT_EQ(20, balance.usd);

  balance = client2.TakeUserInfo();
  ASSERT_EQ(+1245, balance.rub);  // -930 - 5*61
  ASSERT_EQ(-20, balance.usd);
}

// Тестирование на примере из Readme.md
TEST(TestStockWork, TestMatchingMultiple) {
  components::StockClient client1;
  std::string name1 = "User1";
  client1.AuthUser(name1);
  models::Requisition req{.volume = 10,
                          .exchange_rate = 62,
                          .operation_type = models::OperationTypes::kBuy};
  client1.AddRequisition(req);
  auto balance = client1.TakeUserInfo();
  ASSERT_EQ(0, balance.rub);  // обработка заявки без сделки
  ASSERT_EQ(0, balance.usd);

  components::StockClient client2;
  std::string name2 = "User2";
  client2.AuthUser(name2);
  req = {.volume = 20,
         .exchange_rate = 63,
         .operation_type = models::OperationTypes::kBuy};
  client2.AddRequisition(req);
  balance = client2.TakeUserInfo();
  ASSERT_EQ(0, balance.rub);
  ASSERT_EQ(0, balance.usd);

  components::StockClient client3;
  std::string name3 = "User3";
  client3.AuthUser(name3);
  req = {.volume = 50,
         .exchange_rate = 61,
         .operation_type = models::OperationTypes::kSell};
  client3.AddRequisition(req);

  balance = client1.TakeUserInfo();
  ASSERT_EQ(-620, balance.rub);
  ASSERT_EQ(10, balance.usd);

  balance = client2.TakeUserInfo();
  ASSERT_EQ(-1260, balance.rub);
  ASSERT_EQ(20, balance.usd);

  balance = client3.TakeUserInfo();
  ASSERT_EQ(1880, balance.rub);
  ASSERT_EQ(-30, balance.usd);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}