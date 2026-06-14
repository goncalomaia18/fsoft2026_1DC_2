#include <gtest/gtest.h>
#include "model/Client.h"
#include "model/ClientOrder.h"

// 1. Testar o Construtor por Omissão (Vazio)
TEST(ClientTest, DefaultConstructor) {
    // Arrange
    Client c;

    // Act
    std::string email = c.getEmail();
    std::string password = c.getPassword();
    int ordersSize = c.getOrders().size();

    // Assert
    EXPECT_EQ(email, "");
    EXPECT_EQ(password, "");
    EXPECT_EQ(ordersSize, 0);
}

// 2. Testar o Construtor Parametrizado
TEST(ClientTest, ParameterizedConstructor) {
    // Arrange
    std::string expectedEmail = "joao@email.com";
    std::string expectedPassword = "password123";

    // Act
    Client c(expectedEmail, expectedPassword);

    // Assert
    EXPECT_EQ(c.getEmail(), expectedEmail);
    EXPECT_EQ(c.getPassword(), expectedPassword);
    // Assumimos que o nome começa vazio, visto que não é passado no construtor
    EXPECT_EQ(c.getName(), "");
}

// 3. Testar a adição de encomendas (addOrder) e a leitura (getOrders)
TEST(ClientTest, AddAndGetOrders) {
    // Arrange
    Client c("maria@email.com", "pass456");

    // Como o ClientOrder precisa de itens e um total, vamos criar dados falsos
    std::vector<std::pair<Product, int>> emptyItems;
    float total = 10.0f;
    ClientOrder order(emptyItems, total); // Usamos o construtor correto

    // Act
    c.addOrder(order);

    // Assert
    EXPECT_EQ(c.getOrders().size(), 1);
}

// 4. Testar a versão const do getOrders
TEST(ClientTest, GetOrdersConst) {
    // Arrange
    Client c("rui@email.com", "pass789");
    const Client& constClient = c; // Criamos uma referência const para forçar a chamada do método const

    // Act
    const std::vector<ClientOrder>& orders = constClient.getOrders();

    // Assert
    EXPECT_EQ(orders.size(), 0);
}
