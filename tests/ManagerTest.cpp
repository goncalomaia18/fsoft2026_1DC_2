#include <gtest/gtest.h>
#include "model/Manager.h"

// 1. Testar o Construtor por Omissão (Vazio) e os Getters
TEST(ManagerTest, DefaultConstructor) {
    // Arrange
    Manager m;

    // Act
    std::string email = m.getEmail();
    std::string password = m.getPassword();

    // Assert
    // Assumimos que o construtor vazio inicializa as strings vazias
    EXPECT_EQ(email, "");
    EXPECT_EQ(password, "");
}

// 2. Testar o Construtor Parametrizado e os Getters
TEST(ManagerTest, ParameterizedConstructor) {
    // Arrange
    std::string expectedEmail = "admin@pescatudo.pt";
    std::string expectedPassword = "super_secret_admin";

    // Act
    Manager m(expectedEmail, expectedPassword);

    // Assert
    EXPECT_EQ(m.getEmail(), expectedEmail);
    EXPECT_EQ(m.getPassword(), expectedPassword);
}